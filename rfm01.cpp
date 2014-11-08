

#include "rfm01.h"


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
	
	pinMode(26, OUTPUT);
	digitalWrite(26, HIGH);
	delay(2000);
	digitalWrite(26, LOW);
	
	
	//digitalWrite(SS,HIGH);
	//pinMode(SS,OUTPUT);

	//resetDevice();
	
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
	/*	
	
	writeRegister(0x00,0x00);	// 
	writeRegister(0x91,0x8C);	// 868MHz Band +/- 134kHz bandwidth, 12.5pF
	writeRegister(0xA6,0x86);	// 868.35 MHz
	//writeRegister(0xD0,0x40);	// RATE/2
	writeRegister(0xC8,0x47);	// 4.8kbps
	writeRegister(0xC6,0x9B);	// AFC control register
	writeRegister(0xC4,0x2A);	// Clock recovery manual control,Digital filter,DQD=4
	writeRegister(0xC2,0x40);	// //output 1.66MHz
	writeRegister(0xC0,0x42);	// Clock recovery lock
	writeRegister(0xE0,0x00);	// wake up timer
	writeRegister(0xCC,0x00);	// low duty cycle command
	
	writeRegister(0xCE,0x8C);	// FIFO sync word
	writeRegister(0xCE,0x8F);	// FIFO fill and enable
	writeRegister(0xC0,0x81);	// enable RX
	*/
}
void RFM01::receive() {
	int dummy;
	int result;
	while(!(digitalRead(_pinNIRQ)))	// wait for the 0-1-edge of nIRQ, reading in the data
	{
	
	dummy = SPI.transfer(0x00);  // dummy byte
	dummy = SPI.transfer(0x00);  // dummy byte
	result = SPI.transfer(0x00);  // result
    }
}




// Data via FSK
/******************************************************************************/
/*         Sending data via the FSK-Pin as Input-Pin                          */
/*                                                                            */                
/*         After the PowerAmplifier has turned on ( ea=1 ) from rfm02-module  */
/*         comes a clock corresponding to the data-rate set before on nIRQ.   */
/*         The data to be transmitted is bitwise set on the FSK-Pin of the    */
/*         module, after the falling edge of nIRQ. With the following edge    */
/*         of nIRQ this bit is read in and sent out.                          */
/*         nSEL must be high, SCK low, both all the time                      */
/*                                                                            */                
/*                                                                            */                
/* TESTED: 28.09.2014 with Deviation +/- 90kHz and 435.000 MHz                */                
/*                  up to 115.000BPS                                          */                
/*                                                                            */                
/* Support & Copyright: tigarus.programming@web.de                            */                
/******************************************************************************/
/*
void RFM02::RFM02_TX_DataByte_FSK(uint8_t DataByte){
uint8_t i=8;
// PowerAmplifier is here already enabled, impulses on nIRQ corresponding to the 
// set data-rate, nSEL is high, SCK is low

      while(i){            // do 8 times..., (any number of bit's will do, also 9 or 121)
        i=i-1;
		digitalWrite(_pinFSK, LOW); //OUT_PORT_REG &= ~FSK; // first set Bitx as '0'
        if( DataByte & BIT7 )   // if not '0' write over with '1' 
			//OUT_PORT_REG |= FSK;  // ...write '1' if most significant bit is '1'
			digitalWrite(_pinFSK, HIGH); // ...write '1' if most significant bit is '1'
        //while(!(IN_PORT_REG & nIRQ));  // wait for the 0-1-edge of nIRQ, reading in the data
        //while(IN_PORT_REG & nIRQ);    // wait for 1-0-edge to send the last bit
		while(!(digitalRead(_pinNIRQ))); // wait for the 0-1-edge of nIRQ, reading in the data
		while((digitalRead(_pinNIRQ)));  // wait for the 0-1-edge of nIRQ, reading in the data
		
        DataByte <<= 1;         // shift DataByte one bit left to write the next bit
      }
}
*/
/*
void SendDataSPI(uint8_t data){
	digitalWrite(_pinChipSelect,LOW);
	SPI.transfer(0xC6);
	
	 while(i){  // any no of bit's will do, also 9 or 121
        i=i-1;
          
        SDI_low();                  // already set SDI-Port '0' again for next loop or
        if( data & BIT7 )          // now nIRQ is just been low, if MSB is '1'
            SDI_high();               // set SDI-Port '1' 

        while(!(IN_PORT_REG & nIRQ));  // wait for the next high-pulse, data is read by 
        while(IN_PORT_REG & nIRQ);     // now, wait for the falling edge

        data <<= 1;          // shift left ( next bit as most significant bit ...)

      } // end while(...)
   digitalWrite(_pinChipSelect,HIGH);
*/
