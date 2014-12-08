#include "rfm01.h"
#include "Energia.h"

void nIRQISR();
	
//uint8_t _pinSOMI;
//uint8_t _pinSIMO;
//uint8_t _pinSerialClock;

uint8_t _pinNIRQ;
uint8_t _pinChipSelect;
uint8_t _RFAddress;

/* 
* Protocol
* ========
* byte 0	message length including byte 0
* byte 1	address
* byte 2	data 0
* byte 3	data 1
* ...		...
* byte n-1	data n-3
* byte n	checksum
*
* checksum = message length + address + data0 + data n-3
* 
* example
* =======
* byte 0	0x09 -> message length = 8
* byte 1	0x01 -> address
* byte 2	0x12 -> data 0
* byte 3	0xFF -> data 1
* byte 4	0x2A -> data 2
* byte 5	0x32 -> data 3
* byte 6	0x74 -> data 4
* byte 7	0xEB -> checksum
* 
*/


// Booster Pack Pins FR5969
//  7 - P2.2 for SPI_CLK mode
// 15 - P1.6 for SPI_SIMO mode
// 14 - P1.7 for SPI_SOMI mode
//  5 - P2.5 output pin for SPI_CS
// 18 - P3.0 nIRQ for sending data


//uint8_t _MessageLength = MESSAGELENGTH;		//default value for expected message length
						// a message consists of several bytes
volatile int RxFlag = LOW; 	//flag that indicates if nIRQ went low
int PacketCounter=0; 		// counts the number of bytes that are received
    
 
/*
* variables for empty constructors
*/    
static const uint8_t P_CS   = 4;
static const uint8_t P_NIRQ = 18;
static const uint8_t RFAddress = 0x02;

/*
* empty constructor
*/
RFM01::RFM01()
{
	RFM01(RFAddress, P_CS, P_NIRQ);
}
/*
* constructor with 1 variables
* RFAddress is predefined
*/
RFM01::RFM01(uint8_t Address)
{
	RFM01(Address, P_CS, P_NIRQ);
}

/*
* constructor with 3 variables
* all are given by user
*/
RFM01::RFM01( uint8_t Address, uint8_t pinChipSelect,uint8_t pinNIRQ)
{
	_pinChipSelect = pinChipSelect;
	_pinNIRQ = pinNIRQ;
	_RFAddress = Address;
}

/*
* Setup Routine
* =============
* 1. set chip select to output high
* 2. set nIRQ as input
* 3. run function to set up RX
* 4. create ISR on nIRQ falling edge
* 5. blink red LED 
*/
void RFM01::begin()
{
	digitalWrite(_pinChipSelect, HIGH);
	pinMode(_pinChipSelect, OUTPUT);
	pinMode(P_NIRQ, INPUT);	

	configureDeviceSettings();		
	
	attachInterrupt(_pinNIRQ, nIRQISR, FALLING); 
	
	// blink red led 10 ms to indicate setup ready
	pinMode(RED_LED, OUTPUT);
	digitalWrite(RED_LED, HIGH);
	delay(10);
	digitalWrite(RED_LED, LOW);
	
}

/* 
 * Write RFM01 Register
 * ====================
 * input: 	HighByte	upper byte of 16bit register
 * 		LowByte		lower byte of 16bit register
 * 
 * output:	n/a
 *
 * 1. set CS low
 * 2. write high byte
 * 3. write low byte
 * 4. set CS high
 *
 */
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
	writeRegister(0xC0,0xC2);	// enable RX	
	writeRegister(0xC0,0xC3);	// enable RX
	
	
}

/* 
 * receive_with_protocol
 * =====================
 * input:	*rxData		pointer to receive array
 * output:	_MsgCorrect	0 if not correct
 * 				1 if correct
 *
 */
uint8_t RFM01::receive_with_protocol(uint8_t *rxData, uint8_t MsgLngth){
	
	uint8_t _temp;		// temp variable to store result
	uint8_t _MsgCorrect=0;	// stays 0 if message is not correct
	
	/*
	 * 1. read 2 status bytes, but don't evaluate
	 * 2. read data in third byte and store in local variable
	 * 3. store local varialbe to rxData array (pointer)
	 * increment PacketCounter
	 * set ISR flag low
	 *
	 */ 
	if(RxFlag) {
		digitalWrite(_pinChipSelect, LOW);
		SPI.transfer(0x00);
		SPI.transfer(0x00);
		_temp = SPI.transfer(0x00);
		rxData[PacketCounter] = _temp;
		digitalWrite(_pinChipSelect, HIGH);
	 
		PacketCounter++;  // increase counter for received packets
		RxFlag = LOW;	// reset ISR flag
	}
	
	/*
	 * 1. if expected message length is reached clear FIFO
	 * 2. check if message is correct
	 * 3. reset packet counter
	 */
	if(PacketCounter==MsgLngth){
   
		writeRegister(0xCE,0x84);// FIFO sync word
		writeRegister(0xCE,0x87);// FIFO fill and enable
		
	//	if(CheckMessage(rxData) == 0)
			_MsgCorrect = 1;
		
		PacketCounter=0;// reset PacketCounter
	}
	
	// return value if complete Message has been received
	return _MsgCorrect;
}
/*
* CheckMessage
* ============
* input:	*rxData		pointer to rx data array
* output:	_ErrorCounter	0 = message correct
* 				>=1 message incorrect
*
* checks if received message is valid
*  - first byte cannot be zero
*  - checks for correct address  ->  0 < rxData[1] <= 0xFE
*  - calculate checksum (to be done)
*
*/
uint8_t RFM01::CheckMessage(uint8_t *rxData)
{
	uint8_t _ErrorCounter=0;
	
	// check byte 0
	if(rxData[0]==0)	
		_ErrorCounter+=1;	
	// check byte 1
	if(!((rxData[1]==_RFAddress) || (rxData[1]==0xFF)))
		_ErrorCounter+=1;
	//if(TestChkSum(rxData) != 0)
	//	_ErrorCounter+=1;
	// if _ErrorCounter is = 0, message is ok
	return _ErrorCounter;	
}

/*
 * TestChkSum
 * ==========
 * input:	*rxData		pointer to rx data array
 * output:	_Error		0 = chksum correct
 * 				1 = chksum incorrect
 *
 */
uint8_t RFM01::TestChkSum(uint8_t *rxData)
{
	uint8_t i;
	uint8_t rxChkSum;
	

	for(i=0;i<MESSAGELENGTH-1;i++){
		rxChkSum += rxData[i];
	}
	
	if(rxChkSum == rxData[sizeof(rxData)-1])
		return 0;
	else
		return 1;
}
/*
* nIRQ ISR
* ========
*
* if nIRQ = low ==> byte is received)
* set RxFlag = high
*/
void nIRQISR()
{
	RxFlag = HIGH; 					
}
