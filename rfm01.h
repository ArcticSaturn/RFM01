

#ifndef RFM01_h
#define RFM01_h

#include <SPI.h>
#include "rfm01_defines.h"

// defaults
#define MESSAGELENGTH	10	// expect 10 bytes to receive



class RFM01 {
public:
	RFM01();		// empty constructor
	RFM01(uint8_t pinChipSelect, uint8_t pinNIRQ);  // constructor with variables
	void begin();  // setup RFM01
	uint8_t receive(uint8_t *data);  // receive bytes
	void SetMessageLength(uint8_t MessageLength);			// set length of expected message



//private:
	void writeRegister(uint8_t HighByte, uint8_t LowByte);
	void configureDeviceSettings();
	
	

};

#endif
