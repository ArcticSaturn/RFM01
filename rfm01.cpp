

#include "rfm01.h"
#include "Energia.h"

void nIRQISR();
	
uint8_t _pinNIRQ;
uint8_t _pinSOMI;
uint8_t _pinSIMO;
uint8_t _pinChipSelect;
uint8_t _pinSerialClock;

uint8_t _MessageLength = MESSAGELENGTH;		//default value for expected message length
											// a message consists of several bytes

// Booster Pack Pins FR5969
    //  7 - P2.2 for SPI_CLK mode
    // 15 - P1.6 for SPI_SIMO mode
	// 14 - P1.7 for SPI_SOMI mode
	//  5 - P2.5 output pin for SPI_CS
    // 18 - P3.0 nIRQ for sending data
    

static const uint8_t P_CS   = 4;
static const uint8_t P_NIRQ = 18;

volatile int flag = HIGH; 	//flag that indicates if nIRQ went low
int PacketCounter=0; 		// counts the number of bytes that are received

// empty constructor
RFM01::RFM01() {
	RFM01(P_CS, P_NIRQ);
}

// constructor with variables
RFM01::RFM01(uint8_t pinChipSelect,uint8_t pinNIRQ){
	_pinChipSelect = pinChipSelect;
	_pinNIRQ = pinNIRQ;
}



// setup routine
void RFM01::begin() {
	digitalWrite(_pinChipSelect, HIGH);		// set chip select high
	pinMode(_pinChipSelect, OUTPUT);		// define chip select as output
    pinMode(P_NIRQ, INPUT);					// set nIRQ as input
	
	configureDeviceSettings();				// configure RFM01 to correct settings			
	attachInterrupt(_pinNIRQ, nIRQISR, FALLING); // activate interrupt on falling edge of nIRQ = byte received
	
	pinMode(RED_LED, OUTPUT);				// set pin with red led as output
	digitalWrite(RED_LED, HIGH);			// blink red led 50 ms to indicate setup ready
	delay(50);
	digitalWrite(RED_LED, LOW);
}

// write configuration to RFM01 register
void RFM01::writeRegister(uint8_t HighByte, uint8_t LowByte) {
	digitalWrite(_pinChipSelect,LOW);
	SPI.transfer(HighByte);
	SPI.transfer(LowByte);
	digitalWrite(_pinChipSelect,HIGH);
}


// 
void RFM01::configureDeviceSettings() {
	writeRegister(0x00,0x00);	// 
	writeRegister(0x91,0x88);	// 868MHz Band +/- 134kHz bandwidth, 12.5pF
	writeRegister(0xA6,0x86);	// 868.35 MHz
	writeRegister(0xC8,0x47);	// 4.8kbps
	writeRegister(0xC6,0x9B);	// AFC control register
	writeRegister(0xC4,0x2A);	// Clock recovery manual control,Digital filter,DQD=4
	writeRegister(0xC2,0xE0);	// //output 1.66MHz
	writeRegister(0xC0,0x42);	// Clock recovery lock
	writeRegister(0xE0,0x00);	// wake up timer
	writeRegister(0xCC,0x00);	// low duty cycle command
	
	writeRegister(0xCE,0x84);	// FIFO sync word
	writeRegister(0xCE,0x87);	// FIFO fill and enable
	writeRegister(0xC0,0x82);	// enable RX
	writeRegister(0xC0,0x83);	// enable RX
	
}



uint8_t RFM01::receive(uint8_t *rxData){
	int dummy;
	uint8_t _result;					// temporary variable to store result
	uint8_t _MessageReceived=0;			// stays 0, as long as message is not complete
	
	if(flag) {
		digitalWrite(4, LOW); 			// CS LOW
		SPI.transfer(0x00);  			// read high status byte, but don't evaluate
		SPI.transfer(0x00);  			// read low status byte, but don't evaluate
		_result = SPI.transfer(0x00); 	// store received packet into into local variable
		rxData[PacketCounter] = _result; 	// store received packet into array
		digitalWrite(4, HIGH); 			// CS HIGH
	 
		PacketCounter++;  				// increase counter for received packets
		flag = LOW;						// reset ISR flag
	}
	if(PacketCounter==_MessageLength){
   
		writeRegister(0xCE,0x84);		// FIFO sync word
		writeRegister(0xCE,0x87);		// FIFO fill and enable
		_MessageReceived=1;				// complete message has been received
		PacketCounter=0;				// reset PacketCounter
	}

	return _MessageReceived;			// return value if complete Message has been received
}


void nIRQISR()
{
	flag = HIGH; 						// if nIRQ = low (byte received) set flag = high
}


