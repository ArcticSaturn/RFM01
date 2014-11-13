
#ifndef MESSAGELENGTH
#define MESSAGELENGTH	10// expect 10 bytes to receive
#endif

#ifndef RFM01_h
#define RFM01_h
// defaults

#include <SPI.h>
#include "rfm01_defines.h"






class RFM01 {
public:
	RFM01();		// empty constructor
	RFM01(uint8_t pinChipSelect, uint8_t pinNIRQ);  // constructor with variables
	void begin();  // setup RFM01
	uint8_t receive(uint8_t *rxData);  // receive bytes



//private:
	void writeRegister(uint8_t HighByte, uint8_t LowByte);
	void configureDeviceSettings();
	
	

};

#endif
