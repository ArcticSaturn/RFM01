
#ifndef MESSAGELENGTH
#define MESSAGELENGTH	10	// expect 10 bytes to receive
#endif

#ifndef RF_ADDRESS
#define RF_ADDRESS	0x01	// 0x01 standard receive address
#endif


#ifndef RFM01_h
#define RFM01_h
// defaults

#include <SPI.h>
#include "rfm01_defines.h"


class RFM01 {
public:
	
	// empty constructor
	RFM01();
	// constructor with variables
	RFM01(uint8_t Address);
	// constructor with variables
	RFM01(uint8_t Address, uint8_t pinChipSelect, uint8_t pinNIRQ );
	
	
	// setup RFM01
	void begin();
	// receive bytes
	uint8_t receive(uint8_t *rxData);  
	uint8_t receive_with_protocol(uint8_t *rxData);  



//private:
	void writeRegister(uint8_t HighByte, uint8_t LowByte);
	void configureDeviceSettings();
	uint8_t CheckMessage(uint8_t *rxData);
	

};

#endif
