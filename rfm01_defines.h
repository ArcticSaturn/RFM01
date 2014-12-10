/*
 * defines for RFM01
 * based on define-file from joeder for RFM02_Transmitter
 *
 * Author: ArcticSaturn
 * Date: December 2014
 *
 *  
 *
 *
 */


/*    Control Register                                                        */
/******************************************************************************/
/*    RF-Config-Register  Initvalue: 8080hex -> For 430MHz-Band , b1(0) b0(1) */
/*                                                                            */
/*    Bits:  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0    /
/*            1   0   0  b1  b0  eb  et  ex  x3  x2  x1  x0  i2  i1  i0  dc   */
/*                                                                            */
/*    default 1   0   0   0   1   0   0   1   0   0   1   1   1   0   1   0   */
/*                                                                            */
/*                                                                            */
/*    Frequency Deviation [kHz]:  m2  m1  m0   value                          */
/*                                 0   0   0    30    *                       */
/*                                 0   0   1    60                            */
/*                                 0   1   0    90                            */
/*                                 0   1   1   120                            */
/*                                 1   0   0   150                            */
/*                                 1   0   1   180                            */
/*                                 1   1   0   210                            */
/*                                                                            */
/*    Modulation Polarity:         ms  value                                  */
/*                                  0   +180°  *                              */
/*                                  1   -180°                                 */
/*                                                                            */
/*    Crystal Load Capacity:       x3  x2  x1  x0   value                     */
/*                                  0   0   0   0   8.5 pF                    */
/*                                  0   0   0   1   9.0 pF                    */
/*                                  0   0   1   0   9.5 pF                    */
/*                                  0   0   1   1   10.0 pF                   */
/*                                  0   1   0   0   10.5 pF                   */
/*                                  0   1   0   1   11.0 pF                   */
/*                                  0   1   1   0   11.5 pF                   */
/*                                  0   1   1   1   12.0 pF                   */
/*                                  1   0   0   0   12.5 pF                   */
/*                                  1   0   0   1   13.0 pF                   */
/*                                  1   0   1   0   13.5 pF                   */
/*                                  1   0   1   1   14.0 pF                   */
/*                                  1   1   0   0   14.5 pF                   */
/*                                  1   1   0   1   15.0 pF                   */
/*                                  1   1   1   0   15.5 pF                   */
/*                                  1   1   1   1   16.0 pF                   */
/*                                                                            */
/*    Frequency of Pin CLK:        d2  d1  d0   value                         */
/*                                  0   0   0   1    MHz   *                  */
/*                                  0   0   1   1.25 MHz                      */
/*                                  0   1   0   1.66 MHz                      */
/*                                  0   1   1   2.0  MHz                      */
/*                                  1   0   0   2.5  MHz                      */
/*                                  1   0   1   3.33 MHz                      */
/*                                  1   1   0   5    MHz                      */
/*                                  1   1   1   10   MHz                      */
/*                                                                            */
/*    If the CLK-Pin is not usesd to clock an external mikrocontroller        */
/*    set the dc-Bit in the Power Management Register.                        */
/*                                                                            */
/*    Band Selection:                   b1  b0  value                         */
/*                                      0   0   315 MHz                       */
/*                                      0   1   433 MHz   *                   */
/*                                      1   0   868 MHz                       */
/*                                      1   1   915 MHz                       */
/*    *: default value                                                        */
/******************************************************************************/
// set:  433MHz, Deviation 90kHz default: CLK 1MHz, Cap 12.0pF, Modularity +180°
//#define mrc_QuickSet_RFM02_TX()     CONFIG_CMD|=FREQ_DEV_90kHz+BAND_SELECT_433MHz

#define CONFIG_CMD		(0x8000)	// reset value for Config command = 0x8080

/* Frequency Deviation Control Bits */
#define CLKDSBL			(0x0001)	// Clock output disable
#define BNDWDTH0		(0x0002)	// Baseband Bandwidth Bit: 0
#define BNDWDTH1		(0x0004)	// Baseband Bandwidth Bit: 1
#define BNDWDTH2		(0x0008)	// Baseband Bandwidth Bit: 2
#define LOADCAP0		(0x0010)	// Crystal Load Cap Bit: 0
#define LOADCAP1		(0x0020)	// Crystal Load Cap Bit: 1
#define LOADCAP2		(0x0040)	// Crystal Load Cap Bit: 2
#define LOADCAP3		(0x0080)	// Crystal Load Cap Bit: 3
#define CRYSTLACT		(0x0100)	// Crystal active during sleep mode
#define WAKETIME		(0x0200)	// Wake-up timer enable
#define LOWBAT			(0x0400)	// Low Battery enable
#define FREQBAND0		(0x0800)	// Frequency Band Bit: 0
#define FREQBAND1		(0x1000)	// Frequency Band Bit: 1

/*    Base Bandwidth:  i2  i1  i0   value                          */
#define BW_400kHz		(2*0x0001u)	// bandwidth 400 kHz
#define BW_340kHz		(4*0x0001u)	// bandwidth 340 kHz
#define BW_270kHz		(6*0x0001u)	// bandwidth 270 kHz
#define BW_200kHz		(8*0x0001u)	// bandwidth 200 kHz
#define BW_134kHz		(10*0x0001u)	// bandwidth 134 kHz
#define BW_60kHz		(12*0x0001u)	// bandwidth 60 kHz


/*    Crystal Load Capacity:       x3  x2  x1  x0   value                     */
#define CAP_08_5PF  		0x0000		// 8,5pF
#define CAP_09_0PF  		0x0010		// ...
#define CAP_09_5PF  		0x0020
#define CAP_10_0PF  		0x0030
#define CAP_10_5PF		0x0040
#define CAP_11_0PF		0x0050
#define CAP_11_5PF		0x0060
#define CAP_12_0PF		0x0070
#define CAP_12_5PF		0x0080
#define CAP_13_0PF		0x0090
#define CAP_13_5PF		0x00A0
#define CAP_14_0PF		0x00B0
#define CAP_14_5PF		0x00C0
#define CAP_15_0PF		0x00D0
#define CAP_15_5PF		0x00E0
#define CAP_16_0PF		0x00F0

/*    Band Selection:               b1  b0    value ...                        */
#define BAND_315MHz		0x0000
#define BAND_433MHz		0x0800
#define BAND_868MHz		0x1000
#define BAND_915MHz		0x1800




/******************************************************************************/
/*    RF-Frequency-Adjust-Register  Initvalue:  A7D0hex                       */
/*                                                                            */
/*    Bits:  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   */
/*            1   0   1   0  f11 f10 f9  f8  f7  f6  f5  f4  f3  f2  f1  f0   */
/*                                                                            */
/*              C1   C2                                                       */
/*     315band: 1    31                                                       */
/*     433band: 1    43                                                       */
/*     868band: 2    43                                                       */
/*     915band: 3    30                                                       */
/*                                                                            */
/*     value ranges from 0x0060..0x0F3F                                       */
/*                                                                            */
/*     f = carrier frequency                                                  */
/*                                                                            */
/*     f = 10MHz * C1 * (C2 + F/4000)      default: 435 MHz                   */
/*                                                                            */
/*                                                                            */
/******************************************************************************/
#define FREQ_SET_CMD    0xA000

// Value for frequency must be calculated for the define
#define F_430_2400_MHZ      0x0060  // min. value
#define F_435_0000_MHZ      0x07D0   // default
#define F_439_7575_MHZ      0x0F3F   // max value

#define F_860_4800_MHZ      0x0060  // min. value
#define F_868_0000_MHZ      0x0640  // 868.00MHz value
#define F_868_3500_MHZ      0x0686  // 868.35MHz value
#define F_879_5150_MHZ      0x0F3F  // max. value


/******************************************************************************/
/*    Baudrate-Adjust-Register  Initvalue:   C800hex                          */
/*                                                                            */
/*    Bits:  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0   */
/*            1   1   0   0   1   0   0   0  r7  r6  r5  r4  r3  r2  r1  r0   */
/*                                                                            */
/*     Baud Rate = 10MHz/29/(R+1)                                             */
/*                                                                            */
/*     R ranges from 0..255                                                   */
/*                                                                            */
/*     --> Baud Rate can be set from 1,35 kBit/s  ..  344,8 kBit/s            */
/*                                                                            */
/*     Common values range from 9600 Bit/s .. 115.000 Bit/s                   */
/******************************************************************************/
#define BAUD_CMD           0xC800

// Value for baud rate must be calculated for the define
// Value for 38.400 bps  is 7.97988  --> 8

#define BR_115_0_BPS         0x0002
#define BR_38_4_BPS          0x0008
#define BR_19_2_BPS          0x0011
#define BR_09_6_BPS          0x0023  
#define BR_04_8_BPS          0x0047

/*
*    Receiver Setting Command  Initvalue:  C0C1h                             
*                                                                            
*    Bits:  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0  
*            1   1   0   0   0   0   0   0  d1  d0  g1  g0  r2  r1  r0  en  
*                                                                            
* to enable RX, write the "en" in a single command. don't configure other bits
*    
*     d1    d0   VDI output
*     0     0   Digital RSSI OUT (DRSSI)
*     0     1   Data Quality Detector Output (DQD)
*     1     0   Clock recovery lock
*     1     1   DRSSI && DQD
*
*    g1    g0   GLNA (dB relative to max. G)
*     0     0   0
*     0     1   -14
*     1     0   -6
*     1     1   -20
*
*    r2    r1    r0   GLNA (dB relative to max. G)
*     0     0    0    -103
*     0     0    1     -97
*     0     1    0     -91
*     0     1    1     -85
*     1     0    0     -79
*     1     0    1     -73
*     1     1    0     reserved
*     1     1    1     reserved

*/

#define RX_CMD		0xC000

#define	RX_EN		0x0001
#define RSSI_BIT0	0x0002		// RSSI setting bit: 0
#define RSSI_BIT1	0x0004		// RSSI setting bit: 1
#define RSSI_BIT2	0x0008		// RSSI setting bit: 2
#define VDIOUT_BIT0	0x0010		// VDI output bit: 0
#define VDIOUT_BIT1	0x0020		// VDI output bit: 0
#define GLNA_BIT0	0x0040		// VDI output bit: 0
#define GLNA_BIT1	0x0080		// VDI output bit: 0

#define GLNA_0dB	(0*0x0010u)	// 0dB
#define GLNA_14dB	(1*0x0010u)	// -14dBm
#define GLNA_6dB	(2*0x0010u)	// -6dBm
#define GLNA_20dB	(3*0x0010u)	// -20dBm

#define VDI_DRSSI	(0*0x0040u)	// Digital RSSI Out (DRSSI)
#define VDI_DQD		(1*0x0040u)	// Data Quality Detector Output (DQD)
#define VDI_CLK		(2*0x0040u)	// Clock recovery lock
#define VDI_DRSSIDQD	(3*0x0040u)	// DRSSI && DQD

#define RSSI_103dBm	(0*0x0001u)	// -103dBm
#define RSSI_97dBm	(1*0x0001u)	// -97dBm
#define RSSI_91dBm	(2*0x0001u)	// -91dBm
#define RSSI_85dBm	(3*0x0001u)	// -85dBm
#define RSSI_79dBm	(4*0x0001u)	// -79dBm
#define RSSI_73dBm	(5*0x0001u)	// -73dBm

