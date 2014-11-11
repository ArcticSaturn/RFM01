

#include "rfm01.h"
#include "Energia.h"
void ISR();
	
uint8_t _pinNIRQ;
uint8_t _pinSOMI;
uint8_t _pinSIMO;
uint8_t _pinChipSelect;
uint8_t _pinSerialClock;

// Booster Pack Pins FR5969
    //  7 - P2.2 for SPI_CLK mode
    // 15 - P1.6 for SPI_SIMO mode
	// 14 - P1.7 for SPI_SOMI mode
	//  5 - P2.5 output pin for SPI_CS
    // 18 - P3.0 nIRQ for sending data
    // Set display's VCC and DISP pins to high


static const uint8_t P_CS   = 4;
static const uint8_t P_NIRQ = 18;

volatile int flag = HIGH;
int result[20];
int PacketCounter=0; 

// empty constructor
RFM01::RFM01() {
	RFM01(P_CS, P_NIRQ);
}

// constructor with variables
RFM01::RFM01(uint8_t pinChipSelect,uint8_t pinNIRQ){
	_pinChipSelect = pinChipSelect;
	_pinNIRQ = pinNIRQ;
}


/*
RFM01::RFM01(uint8_t deviceAddress) {
	RFM01(deviceAddress, CHANNEL);
}
RFM01::RFM01(uint8_t deviceAddress, uint8_t channel) {
	_deviceAddress = deviceAddress;
	_channel = channel;
	_gdo0 = GDO0;	
}

*/

void RFM01::begin() {
	digitalWrite(_pinChipSelect, HIGH);
	pinMode(_pinChipSelect, OUTPUT);
    
	pinMode(P_NIRQ, INPUT);
	
	pinMode(25, OUTPUT);
	pinMode(26, OUTPUT);
	digitalWrite(26, LOW);
	digitalWrite(25, HIGH);
	delay(50);
	digitalWrite(25, LOW);
	
	attachInterrupt(_pinNIRQ, ISR, FALLING); // Interrupt is fired whenever button is pressed

	configureDeviceSettings();
	//execStrobeCommand(RFM02_CMD_SRX);
}

void RFM01::writeRegister(uint8_t HighByte, uint8_t LowByte) {
	digitalWrite(_pinChipSelect,LOW);
	SPI.transfer(HighByte);
	SPI.transfer(LowByte);
	digitalWrite(_pinChipSelect,HIGH);
}



void RFM01::configureDeviceSettings() {
	writeRegister(0x00,0x00);	// 
	writeRegister(0x91,0x88);	// 868MHz Band +/- 134kHz bandwidth, 12.5pF
	writeRegister(0xA6,0x86);	// 868.35 MHz
	//writeRegister(0xD0,0x40);	// RATE/2
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
uint8_t RFM01::receive(){
	int dummy;
	int result;
	uint8_t _PacketReceived=0;
	if(flag) {
		digitalWrite(4, LOW); // CS LOW
		//result[PacketCounter] = SPI.transfer(0x00);  // high statusbyte 
		//result[PacketCounter] = SPI.transfer(0x00);  // low status byte
		SPI.transfer(0x00);  // read high status byte, but don't evaluate
		SPI.transfer(0x00);  // read low status byte, but don't evaluate
		//result[PacketCounter] = SPI.transfer(0x00);  // result
		Serial.println(SPI.transfer(0x00));  // result
		digitalWrite(4, HIGH); // CS HIGH
	 
		PacketCounter++;  
		flag = LOW;
	}
	if(PacketCounter==PACKETLENGTH){
   
		writeRegister(0xCE,0x84);	// FIFO sync word
		writeRegister(0xCE,0x87);	// FIFO fill and enable
		Serial.println(PacketCounter);
		//Serial.println(result[0]);
		//Serial.println(result[1]);
		//Serial.println(result[2]);
		//Serial.println(result[3]);
		//Serial.println(result[4]);
		//Serial.println(result[5]);
		//Serial.println(result[6]
		//Serial.println(result[7]
		//Serial.println(result[8]);
		digitalWrite(26, HIGH);
		delay(30);
		digitalWrite(26, LOW);
		_PacketReceived=1;
		PacketCounter=0;
		
 // count=0;  
 }
return _PacketReceived;
 }


void ISR()
{
	flag = HIGH; 
}


