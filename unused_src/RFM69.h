/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/LF_TOOL_GEN2/trunk/Src/HAL/RF/RF.h $
*
*   \brief      Public header file for RF module.
*/
/* COPYRIGHT NOTICE
* ==================================================================================================
*
* The contents of this document are protected under copyright and contain commercially and/or
* technically confidential information. The content of this document must not be used other than
* for the purpose for which it was provided nor may it be disclosed or copied (by the authorised
* recipient or otherwise) without the prior written consent of an authorised officer of Schrader
* Electronics Ltd.
*
*         (C) $Date:: 2014#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/
#ifndef RFM69_H
#define RFM69_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#define RFM69_VERSION            0x24
#define RFM69_WRITE_ACCESS       0x80
#define RFM69_MAX_TX_POWER_LEVEL 31u
#define ENCRYPT_KEY_SIZE         16u
#define FREQ_STEP                61.03515625f
#define RFM69_MAX_DATA_LEN       66u
#define RFM69_MAX_PAYLOAD_LEN    64u  //! We will stick to this as this is the maximum number of bytes that can use the AES128 encryption scheme ( 66byte FIFO )- 2 CRC bytes */

#define RFM69_TIMEOUT            400u
#define NODE_OWN_ADDRESS	     0x01

#define BIT_MASK_1_BIT 1
#define BIT_MASK_2_BIT 3
#define BIT_MASK_3_BIT 7
#define BIT_MASK_4_BIT 15

/* BIT Pneumonics ( shift patterns ) */
#define SLEEP                0x00
#define STDBY                0x04
#define FS                   0x08
#define TX                   0x0C
#define RX                   0x10
#define LISTEN_ABORT         0x20
#define LISTENON             0x40
#define SEQUENCER_OFF        0x80
#define PACKET_FORMAT        0x80
#define CLK_OUT_OFF          0x07
#define TEMP_MEASURE_START   0x08
#define TEMP_MEASURE_RUNNING 0x04
#define MODULATION_TYPE      0x18
#define DATA_PROCESS_MODE    0x60
#define LISTEN_IDLE_RES      0xC0
#define LISTEN_RX_RES        0x30
#define PA0_ON               0x80
#define PA1_ON               0x40
#define PA2_ON               0x20
#define RSSI_START           0x01
#define AES_ON               0x01
#define DIO0                 0xC0
#define DIO1                 0x30
#define DIO2                 0x0C
#define DIO3                 0x03
#define DIO4                 0xC0
#define DIO5                 0x30


#define COURSE_TEMP_COEF    -90 // puts the temperature reading in the ballpark, user can fine tune the returned value


//******************************************************
// RF69/SX1231 bit control definition
//******************************************************
// RegOpMode
#define RF_OPMODE_SEQUENCER_OFF      			0x80
#define RF_OPMODE_SEQUENCER_ON       			0x00  // Default

#define RF_OPMODE_LISTEN_ON          			0x40
#define RF_OPMODE_LISTEN_OFF         			0x00  // Default

#define RF_OPMODE_LISTENABORT        			0x20

#define RF_OPMODE_SLEEP              			0x00
#define RF_OPMODE_STANDBY            			0x04  // Default
#define RF_OPMODE_SYNTHESIZER        			0x08
#define RF_OPMODE_TRANSMITTER        			0x0C
#define RF_OPMODE_RECEIVER           			0x10

// RegDataModul
#define RF_DATAMODUL_DATAMODE_PACKET            0x00  // Default
#define RF_DATAMODUL_DATAMODE_CONTINUOUS        0x40
#define RF_DATAMODUL_DATAMODE_CONTINUOUSNOBSYNC 0x60

#define RF_DATAMODUL_MODULATIONTYPE_FSK         0x00  // Default
#define RF_DATAMODUL_MODULATIONTYPE_OOK         0x08

#define RF_DATAMODUL_MODULATIONSHAPING_00       0x00  // Default
#define RF_DATAMODUL_MODULATIONSHAPING_01       0x01
#define RF_DATAMODUL_MODULATIONSHAPING_10       0x02
#define RF_DATAMODUL_MODULATIONSHAPING_11       0x03

// RegBitRate (bits/sec) example bit rates
#define RF_BITRATEMSB_1200            			0x68
#define RF_BITRATELSB_1200            			0x2B
#define RF_BITRATEMSB_2400            			0x34
#define RF_BITRATELSB_2400            			0x15
#define RF_BITRATEMSB_4800            			0x1A  // Default
#define RF_BITRATELSB_4800            			0x0B  // Default
#define RF_BITRATEMSB_9600            			0x0D
#define RF_BITRATELSB_9600            			0x05
#define RF_BITRATEMSB_19200           			0x06
#define RF_BITRATELSB_19200           			0x83
#define RF_BITRATEMSB_38400           			0x03
#define RF_BITRATELSB_38400           			0x41

#define RF_BITRATEMSB_38323           			0x03
#define RF_BITRATELSB_38323           			0x43

#define RF_BITRATEMSB_34482           			0x03
#define RF_BITRATELSB_34482           			0xA0

#define RF_BITRATEMSB_76800           			0x01
#define RF_BITRATELSB_76800           			0xA1
#define RF_BITRATEMSB_153600          			0x00
#define RF_BITRATELSB_153600          			0xD0
#define RF_BITRATEMSB_57600           			0x02
#define RF_BITRATELSB_57600           			0x2C
#define RF_BITRATEMSB_115200          			0x01
#define RF_BITRATELSB_115200          			0x16
#define RF_BITRATEMSB_12500           			0x0A
#define RF_BITRATELSB_12500           			0x00
#define RF_BITRATEMSB_25000           			0x05
#define RF_BITRATELSB_25000           			0x00
#define RF_BITRATEMSB_50000           			0x02
#define RF_BITRATELSB_50000           			0x80
#define RF_BITRATEMSB_100000          			0x01
#define RF_BITRATELSB_100000          			0x40
#define RF_BITRATEMSB_150000          			0x00
#define RF_BITRATELSB_150000          			0xD5
#define RF_BITRATEMSB_200000          			0x00
#define RF_BITRATELSB_200000          			0xA0
#define RF_BITRATEMSB_250000          			0x00
#define RF_BITRATELSB_250000          			0x80
#define RF_BITRATEMSB_300000          			0x00
#define RF_BITRATELSB_300000          			0x6B
#define RF_BITRATEMSB_32768           			0x03
#define RF_BITRATELSB_32768           			0xD1
// custom bit rates
#define RF_BITRATEMSB_55555           			0x02
#define RF_BITRATELSB_55555           			0x40
#define RF_BITRATEMSB_200KBPS         			0x00
#define RF_BITRATELSB_200KBPS         			0xa0

// RegFdev - frequency deviation (Hz)
#define RF_FDEVMSB_2000            				0x00
#define RF_FDEVLSB_2000            				0x21
#define RF_FDEVMSB_5000            				0x00  // Default
#define RF_FDEVLSB_5000            				0x52  // Default
#define RF_FDEVMSB_7500            				0x00
#define RF_FDEVLSB_7500            				0x7B
#define RF_FDEVMSB_10000           				0x00
#define RF_FDEVLSB_10000           				0xA4
#define RF_FDEVMSB_15000           				0x00
#define RF_FDEVLSB_15000           				0xF6
#define RF_FDEVMSB_20000           				0x01
#define RF_FDEVLSB_20000           				0x48
#define RF_FDEVMSB_25000           				0x01
#define RF_FDEVLSB_25000           				0x9A
#define RF_FDEVMSB_30000           				0x01
#define RF_FDEVLSB_30000           				0xEC
#define RF_FDEVMSB_35000           				0x02
#define RF_FDEVLSB_35000           				0x3D
#define RF_FDEVMSB_40000           				0x02
#define RF_FDEVLSB_40000           				0x8F
#define RF_FDEVMSB_45000           				0x02
#define RF_FDEVLSB_45000           				0xE1
#define RF_FDEVMSB_50000           				0x03
#define RF_FDEVLSB_50000           				0x33
#define RF_FDEVMSB_55000           				0x03
#define RF_FDEVLSB_55000           				0x85
#define RF_FDEVMSB_60000           				0x03
#define RF_FDEVLSB_60000           				0xD7
#define RF_FDEVMSB_65000           				0x04
#define RF_FDEVLSB_65000           				0x29
#define RF_FDEVMSB_70000           				0x04
#define RF_FDEVLSB_70000           				0x7B
#define RF_FDEVMSB_75000           				0x04
#define RF_FDEVLSB_75000           				0xCD
#define RF_FDEVMSB_80000           				0x05
#define RF_FDEVLSB_80000           				0x1F
#define RF_FDEVMSB_85000           				0x05
#define RF_FDEVLSB_85000           				0x71
#define RF_FDEVMSB_90000           				0x05
#define RF_FDEVLSB_90000           				0xC3
#define RF_FDEVMSB_95000           				0x06
#define RF_FDEVLSB_95000           				0x14
#define RF_FDEVMSB_100000          				0x06
#define RF_FDEVLSB_100000          				0x66
#define RF_FDEVMSB_110000          				0x07
#define RF_FDEVLSB_110000          				0x0A
#define RF_FDEVMSB_120000          				0x07
#define RF_FDEVLSB_120000          				0xAE
#define RF_FDEVMSB_130000          				0x08
#define RF_FDEVLSB_130000          				0x52
#define RF_FDEVMSB_140000          				0x08
#define RF_FDEVLSB_140000          				0xF6
#define RF_FDEVMSB_150000          				0x09
#define RF_FDEVLSB_150000          				0x9A
#define RF_FDEVMSB_160000          				0x0A
#define RF_FDEVLSB_160000          				0x3D
#define RF_FDEVMSB_170000          				0x0A
#define RF_FDEVLSB_170000          				0xE1
#define RF_FDEVMSB_180000          				0x0B
#define RF_FDEVLSB_180000          				0x85
#define RF_FDEVMSB_190000          				0x0C
#define RF_FDEVLSB_190000          				0x29
#define RF_FDEVMSB_200000          				0x0C
#define RF_FDEVLSB_200000          				0xCD
#define RF_FDEVMSB_210000          				0x0D
#define RF_FDEVLSB_210000          				0x71
#define RF_FDEVMSB_220000          				0x0E
#define RF_FDEVLSB_220000          				0x14
#define RF_FDEVMSB_230000          				0x0E
#define RF_FDEVLSB_230000          				0xB8
#define RF_FDEVMSB_240000          				0x0F
#define RF_FDEVLSB_240000          				0x5C
#define RF_FDEVMSB_250000          				0x10
#define RF_FDEVLSB_250000          				0x00
#define RF_FDEVMSB_260000          				0x10
#define RF_FDEVLSB_260000          				0xA4
#define RF_FDEVMSB_270000          				0x11
#define RF_FDEVLSB_270000          				0x48
#define RF_FDEVMSB_280000          				0x11
#define RF_FDEVLSB_280000          				0xEC
#define RF_FDEVMSB_290000          				0x12
#define RF_FDEVLSB_290000          				0x8F
#define RF_FDEVMSB_300000          				0x13
#define RF_FDEVLSB_300000          				0x33

// RegFrf (MHz) - carrier frequency
// 315Mhz band
#define RF_FRFMSB_314            				0x4E
#define RF_FRFMID_314            				0x80
#define RF_FRFLSB_314            				0x00
#define RF_FRFMSB_315            				0x4E
#define RF_FRFMID_315            				0xC0
#define RF_FRFLSB_315            				0x00
#define RF_FRFMSB_316            				0x4F
#define RF_FRFMID_316            				0x00
#define RF_FRFLSB_316            				0x00
// 433mhz band
#define RF_FRFMSB_433            				0x6C
#define RF_FRFMID_433            				0x40
#define RF_FRFLSB_433            				0x00
#define RF_FRFMSB_434            				0x6C
#define RF_FRFMID_434            				0x80
#define RF_FRFLSB_434            				0x00
#define RF_FRFMSB_435            				0x6C
#define RF_FRFMID_435            				0xC0
#define RF_FRFLSB_435            				0x00
// 868Mhz band
#define RF_FRFMSB_863            				0xD7
#define RF_FRFMID_863            				0xC0
#define RF_FRFLSB_863            				0x00
#define RF_FRFMSB_864            				0xD8
#define RF_FRFMID_864            				0x00
#define RF_FRFLSB_864            				0x00
#define RF_FRFMSB_865            				0xD8
#define RF_FRFMID_865            				0x40
#define RF_FRFLSB_865            				0x00
#define RF_FRFMSB_866            				0xD8
#define RF_FRFMID_866            				0x80
#define RF_FRFLSB_866            				0x00
#define RF_FRFMSB_867            				0xD8
#define RF_FRFMID_867            				0xC0
#define RF_FRFLSB_867            				0x00
#define RF_FRFMSB_868            				0xD9
#define RF_FRFMID_868            				0x00
#define RF_FRFLSB_868            				0x00
#define RF_FRFMSB_869            				0xD9
#define RF_FRFMID_869            				0x40
#define RF_FRFLSB_869            				0x00
#define RF_FRFMSB_870            				0xD9
#define RF_FRFMID_870            				0x80
#define RF_FRFLSB_870            				0x00
// 915Mhz band
#define RF_FRFMSB_902            				0xE1
#define RF_FRFMID_902            				0x80
#define RF_FRFLSB_902            				0x00
#define RF_FRFMSB_903            				0xE1
#define RF_FRFMID_903            				0xC0
#define RF_FRFLSB_903            				0x00
#define RF_FRFMSB_904            				0xE2
#define RF_FRFMID_904            				0x00
#define RF_FRFLSB_904            				0x00
#define RF_FRFMSB_905            				0xE2
#define RF_FRFMID_905            				0x40
#define RF_FRFLSB_905            				0x00
#define RF_FRFMSB_906            				0xE2
#define RF_FRFMID_906            				0x80
#define RF_FRFLSB_906            				0x00
#define RF_FRFMSB_907            				0xE2
#define RF_FRFMID_907            				0xC0
#define RF_FRFLSB_907            				0x00
#define RF_FRFMSB_908            				0xE3
#define RF_FRFMID_908            				0x00
#define RF_FRFLSB_908            				0x00
#define RF_FRFMSB_909            				0xE3
#define RF_FRFMID_909            				0x40
#define RF_FRFLSB_909            				0x00
#define RF_FRFMSB_910            				0xE3
#define RF_FRFMID_910            				0x80
#define RF_FRFLSB_910            				0x00
#define RF_FRFMSB_911            				0xE3
#define RF_FRFMID_911            				0xC0
#define RF_FRFLSB_911            				0x00
#define RF_FRFMSB_912            				0xE4
#define RF_FRFMID_912            				0x00
#define RF_FRFLSB_912            				0x00
#define RF_FRFMSB_913            				0xE4
#define RF_FRFMID_913            				0x40
#define RF_FRFLSB_913            				0x00
#define RF_FRFMSB_914            				0xE4
#define RF_FRFMID_914            				0x80
#define RF_FRFLSB_914            				0x00
#define RF_FRFMSB_915            				0xE4  // Default
#define RF_FRFMID_915            				0xC0  // Default
#define RF_FRFLSB_915            				0x00  // Default
#define RF_FRFMSB_916            				0xE5
#define RF_FRFMID_916            				0x00
#define RF_FRFLSB_916            				0x00
#define RF_FRFMSB_917            				0xE5
#define RF_FRFMID_917            				0x40
#define RF_FRFLSB_917            				0x00
#define RF_FRFMSB_918            				0xE5
#define RF_FRFMID_918            				0x80
#define RF_FRFLSB_918            				0x00
#define RF_FRFMSB_919            				0xE5
#define RF_FRFMID_919            				0xC0
#define RF_FRFLSB_919            				0x00
#define RF_FRFMSB_920            				0xE6
#define RF_FRFMID_920            				0x00
#define RF_FRFLSB_920            				0x00
#define RF_FRFMSB_921            				0xE6
#define RF_FRFMID_921            				0x40
#define RF_FRFLSB_921            				0x00
#define RF_FRFMSB_922            				0xE6
#define RF_FRFMID_922            				0x80
#define RF_FRFLSB_922            				0x00
#define RF_FRFMSB_923            				0xE6
#define RF_FRFMID_923            				0xC0
#define RF_FRFLSB_923            				0x00
#define RF_FRFMSB_924            				0xE7
#define RF_FRFMID_924            				0x00
#define RF_FRFLSB_924            				0x00
#define RF_FRFMSB_925            				0xE7
#define RF_FRFMID_925            				0x40
#define RF_FRFLSB_925            				0x00
#define RF_FRFMSB_926            				0xE7
#define RF_FRFMID_926            				0x80
#define RF_FRFLSB_926            				0x00
#define RF_FRFMSB_927            				0xE7
#define RF_FRFMID_927            				0xC0
#define RF_FRFLSB_927            				0x00
#define RF_FRFMSB_928            				0xE8
#define RF_FRFMID_928            				0x00
#define RF_FRFLSB_928            				0x00

// RegOsc1
#define RF_OSC1_RCCAL_START      				0x80
#define RF_OSC1_RCCAL_DONE       				0x40

// RegAfcCtrl
#define RF_AFCCTRL_LOWBETA_OFF   				0x00  // Default
#define RF_AFCCTRL_LOWBETA_ON    				0x20

// RegLowBat
#define RF_LOWBAT_MONITOR        				0x10
#define RF_LOWBAT_ON             				0x08
#define RF_LOWBAT_OFF            				0x00  // Default

#define RF_LOWBAT_TRIM_1695      				0x00
#define RF_LOWBAT_TRIM_1764      				0x01
#define RF_LOWBAT_TRIM_1835      				0x02  // Default
#define RF_LOWBAT_TRIM_1905      				0x03
#define RF_LOWBAT_TRIM_1976      				0x04
#define RF_LOWBAT_TRIM_2045      				0x05
#define RF_LOWBAT_TRIM_2116      				0x06
#define RF_LOWBAT_TRIM_2185      				0x07

// RegListen1
#define RF_LISTEN1_RESOL_64      				0x50
#define RF_LISTEN1_RESOL_4100    				0xA0  // Default
#define RF_LISTEN1_RESOL_262000  				0xF0

#define RF_LISTEN1_RESOL_IDLE_64     			0x40
#define RF_LISTEN1_RESOL_IDLE_4100   			0x80  // Default
#define RF_LISTEN1_RESOL_IDLE_262000 			0xC0

#define RF_LISTEN1_RESOL_RX_64       			0x10
#define RF_LISTEN1_RESOL_RX_4100     			0x20  // Default
#define RF_LISTEN1_RESOL_RX_262000   			0x30

#define RF_LISTEN1_CRITERIA_RSSI          		0x00  // Default
#define RF_LISTEN1_CRITERIA_RSSIANDSYNC   		0x08

#define RF_LISTEN1_END_00                 		0x00
#define RF_LISTEN1_END_01                 		0x02  // Default
#define RF_LISTEN1_END_10                 		0x04

// RegListen2
#define RF_LISTEN2_COEFIDLE_VALUE        		0xF5 // Default

// RegListen3
#define RF_LISTEN3_COEFRX_VALUE          		0x20 // Default

// RegVersion
#define RF_VERSION_VER       					0x24  // Default

// RegPaLevel
#define RF_PALEVEL_PA0_ON    					0x80  // Default
#define RF_PALEVEL_PA0_OFF   					0x00
#define RF_PALEVEL_PA1_ON    					0x40
#define RF_PALEVEL_PA1_OFF   					0x00  // Default
#define RF_PALEVEL_PA2_ON    					0x20
#define RF_PALEVEL_PA2_OFF   					0x00  // Default

#define RF_PALEVEL_OUTPUTPOWER_00000      		0x00
#define RF_PALEVEL_OUTPUTPOWER_00001      		0x01
#define RF_PALEVEL_OUTPUTPOWER_00010      		0x02
#define RF_PALEVEL_OUTPUTPOWER_00011      		0x03
#define RF_PALEVEL_OUTPUTPOWER_00100      		0x04
#define RF_PALEVEL_OUTPUTPOWER_00101      		0x05
#define RF_PALEVEL_OUTPUTPOWER_00110      		0x06
#define RF_PALEVEL_OUTPUTPOWER_00111      		0x07
#define RF_PALEVEL_OUTPUTPOWER_01000      		0x08
#define RF_PALEVEL_OUTPUTPOWER_01001      		0x09
#define RF_PALEVEL_OUTPUTPOWER_01010      		0x0A
#define RF_PALEVEL_OUTPUTPOWER_01011      		0x0B
#define RF_PALEVEL_OUTPUTPOWER_01100      		0x0C
#define RF_PALEVEL_OUTPUTPOWER_01101      		0x0D
#define RF_PALEVEL_OUTPUTPOWER_01110      		0x0E
#define RF_PALEVEL_OUTPUTPOWER_01111      		0x0F
#define RF_PALEVEL_OUTPUTPOWER_10000      		0x10
#define RF_PALEVEL_OUTPUTPOWER_10001      		0x11
#define RF_PALEVEL_OUTPUTPOWER_10010      		0x12
#define RF_PALEVEL_OUTPUTPOWER_10011      		0x13
#define RF_PALEVEL_OUTPUTPOWER_10100      		0x14
#define RF_PALEVEL_OUTPUTPOWER_10101      		0x15
#define RF_PALEVEL_OUTPUTPOWER_10110      		0x16
#define RF_PALEVEL_OUTPUTPOWER_10111      		0x17
#define RF_PALEVEL_OUTPUTPOWER_11000      		0x18
#define RF_PALEVEL_OUTPUTPOWER_11001      		0x19
#define RF_PALEVEL_OUTPUTPOWER_11010      		0x1A
#define RF_PALEVEL_OUTPUTPOWER_11011      		0x1B
#define RF_PALEVEL_OUTPUTPOWER_11100      		0x1C
#define RF_PALEVEL_OUTPUTPOWER_11101      		0x1D
#define RF_PALEVEL_OUTPUTPOWER_11110      		0x1E
#define RF_PALEVEL_OUTPUTPOWER_11111      		0x1F  // Default

// RegPaRamp
#define RF_PARAMP_3400           				0x00
#define RF_PARAMP_2000           				0x01
#define RF_PARAMP_1000           				0x02
#define RF_PARAMP_500            				0x03
#define RF_PARAMP_250            				0x04
#define RF_PARAMP_125            				0x05
#define RF_PARAMP_100            				0x06
#define RF_PARAMP_62             				0x07
#define RF_PARAMP_50             				0x08
#define RF_PARAMP_40             				0x09  // Default
#define RF_PARAMP_31             				0x0A
#define RF_PARAMP_25             				0x0B
#define RF_PARAMP_20             				0x0C
#define RF_PARAMP_15             				0x0D
#define RF_PARAMP_12             				0x0E
#define RF_PARAMP_10             				0x0F


// RegOcp
#define RF_OCP_OFF                				0x0F
#define RF_OCP_ON                 				0x1A  // Default

#define RF_OCP_TRIM_45            				0x00
#define RF_OCP_TRIM_50            				0x01
#define RF_OCP_TRIM_55            				0x02
#define RF_OCP_TRIM_60            				0x03
#define RF_OCP_TRIM_65            				0x04
#define RF_OCP_TRIM_70            				0x05
#define RF_OCP_TRIM_75            				0x06
#define RF_OCP_TRIM_80            				0x07
#define RF_OCP_TRIM_85            				0x08
#define RF_OCP_TRIM_90            				0x09
#define RF_OCP_TRIM_95            				0x0A  // Default
#define RF_OCP_TRIM_100           				0x0B
#define RF_OCP_TRIM_105           				0x0C
#define RF_OCP_TRIM_110           				0x0D
#define RF_OCP_TRIM_115           				0x0E
#define RF_OCP_TRIM_120           				0x0F

// RegAgcRef - not present on RFM69/SX1231
#define RF_AGCREF_AUTO_ON         				0x40  // Default
#define RF_AGCREF_AUTO_OFF        				0x00

#define RF_AGCREF_LEVEL_MINUS80   				0x00  // Default
#define RF_AGCREF_LEVEL_MINUS81   				0x01
#define RF_AGCREF_LEVEL_MINUS82   				0x02
#define RF_AGCREF_LEVEL_MINUS83   				0x03
#define RF_AGCREF_LEVEL_MINUS84   				0x04
#define RF_AGCREF_LEVEL_MINUS85   				0x05
#define RF_AGCREF_LEVEL_MINUS86   				0x06
#define RF_AGCREF_LEVEL_MINUS87   				0x07
#define RF_AGCREF_LEVEL_MINUS88   				0x08
#define RF_AGCREF_LEVEL_MINUS89   				0x09
#define RF_AGCREF_LEVEL_MINUS90   				0x0A
#define RF_AGCREF_LEVEL_MINUS91   				0x0B
#define RF_AGCREF_LEVEL_MINUS92   				0x0C
#define RF_AGCREF_LEVEL_MINUS93   				0x0D
#define RF_AGCREF_LEVEL_MINUS94   				0x0E
#define RF_AGCREF_LEVEL_MINUS95   				0x0F
#define RF_AGCREF_LEVEL_MINUS96   				0x10
#define RF_AGCREF_LEVEL_MINUS97   				0x11
#define RF_AGCREF_LEVEL_MINUS98   				0x12
#define RF_AGCREF_LEVEL_MINUS99   				0x13
#define RF_AGCREF_LEVEL_MINUS100  				0x14
#define RF_AGCREF_LEVEL_MINUS101  				0x15
#define RF_AGCREF_LEVEL_MINUS102  				0x16
#define RF_AGCREF_LEVEL_MINUS103  				0x17
#define RF_AGCREF_LEVEL_MINUS104  				0x18
#define RF_AGCREF_LEVEL_MINUS105  				0x19
#define RF_AGCREF_LEVEL_MINUS106  				0x1A
#define RF_AGCREF_LEVEL_MINUS107  				0x1B
#define RF_AGCREF_LEVEL_MINUS108  				0x1C
#define RF_AGCREF_LEVEL_MINUS109  				0x1D
#define RF_AGCREF_LEVEL_MINUS110  				0x1E
#define RF_AGCREF_LEVEL_MINUS111  				0x1F
#define RF_AGCREF_LEVEL_MINUS112  				0x20
#define RF_AGCREF_LEVEL_MINUS113  				0x21
#define RF_AGCREF_LEVEL_MINUS114  				0x22
#define RF_AGCREF_LEVEL_MINUS115  				0x23
#define RF_AGCREF_LEVEL_MINUS116  				0x24
#define RF_AGCREF_LEVEL_MINUS117  				0x25
#define RF_AGCREF_LEVEL_MINUS118  				0x26
#define RF_AGCREF_LEVEL_MINUS119  				0x27
#define RF_AGCREF_LEVEL_MINUS120  				0x28
#define RF_AGCREF_LEVEL_MINUS121  				0x29
#define RF_AGCREF_LEVEL_MINUS122  				0x2A
#define RF_AGCREF_LEVEL_MINUS123  				0x2B
#define RF_AGCREF_LEVEL_MINUS124  				0x2C
#define RF_AGCREF_LEVEL_MINUS125  				0x2D
#define RF_AGCREF_LEVEL_MINUS126  				0x2E
#define RF_AGCREF_LEVEL_MINUS127  				0x2F
#define RF_AGCREF_LEVEL_MINUS128  				0x30
#define RF_AGCREF_LEVEL_MINUS129  				0x31
#define RF_AGCREF_LEVEL_MINUS130  				0x32
#define RF_AGCREF_LEVEL_MINUS131  				0x33
#define RF_AGCREF_LEVEL_MINUS132  				0x34
#define RF_AGCREF_LEVEL_MINUS133  				0x35
#define RF_AGCREF_LEVEL_MINUS134  				0x36
#define RF_AGCREF_LEVEL_MINUS135  				0x37
#define RF_AGCREF_LEVEL_MINUS136  				0x38
#define RF_AGCREF_LEVEL_MINUS137  				0x39
#define RF_AGCREF_LEVEL_MINUS138  				0x3A
#define RF_AGCREF_LEVEL_MINUS139  				0x3B
#define RF_AGCREF_LEVEL_MINUS140  				0x3C
#define RF_AGCREF_LEVEL_MINUS141  				0x3D
#define RF_AGCREF_LEVEL_MINUS142  				0x3E
#define RF_AGCREF_LEVEL_MINUS143  				0x3F

// RegAgcThresh1 - not present on RFM69/SX1231
#define RF_AGCTHRESH1_SNRMARGIN_000   			0x00
#define RF_AGCTHRESH1_SNRMARGIN_001   			0x20
#define RF_AGCTHRESH1_SNRMARGIN_010   			0x40
#define RF_AGCTHRESH1_SNRMARGIN_011   			0x60
#define RF_AGCTHRESH1_SNRMARGIN_100   			0x80
#define RF_AGCTHRESH1_SNRMARGIN_101   			0xA0  // Default
#define RF_AGCTHRESH1_SNRMARGIN_110   			0xC0
#define RF_AGCTHRESH1_SNRMARGIN_111   			0xE0

#define RF_AGCTHRESH1_STEP1_0         			0x00
#define RF_AGCTHRESH1_STEP1_1         			0x01
#define RF_AGCTHRESH1_STEP1_2         			0x02
#define RF_AGCTHRESH1_STEP1_3         			0x03
#define RF_AGCTHRESH1_STEP1_4         			0x04
#define RF_AGCTHRESH1_STEP1_5         			0x05
#define RF_AGCTHRESH1_STEP1_6         			0x06
#define RF_AGCTHRESH1_STEP1_7         			0x07
#define RF_AGCTHRESH1_STEP1_8         			0x08
#define RF_AGCTHRESH1_STEP1_9         			0x09
#define RF_AGCTHRESH1_STEP1_10        			0x0A
#define RF_AGCTHRESH1_STEP1_11        			0x0B
#define RF_AGCTHRESH1_STEP1_12        			0x0C
#define RF_AGCTHRESH1_STEP1_13        			0x0D
#define RF_AGCTHRESH1_STEP1_14        			0x0E
#define RF_AGCTHRESH1_STEP1_15        			0x0F
#define RF_AGCTHRESH1_STEP1_16        			0x10  // Default
#define RF_AGCTHRESH1_STEP1_17        			0x11
#define RF_AGCTHRESH1_STEP1_18        			0x12
#define RF_AGCTHRESH1_STEP1_19        			0x13
#define RF_AGCTHRESH1_STEP1_20        			0x14
#define RF_AGCTHRESH1_STEP1_21        			0x15
#define RF_AGCTHRESH1_STEP1_22        			0x16
#define RF_AGCTHRESH1_STEP1_23        			0x17
#define RF_AGCTHRESH1_STEP1_24        			0x18
#define RF_AGCTHRESH1_STEP1_25        			0x19
#define RF_AGCTHRESH1_STEP1_26        			0x1A
#define RF_AGCTHRESH1_STEP1_27        			0x1B
#define RF_AGCTHRESH1_STEP1_28        			0x1C
#define RF_AGCTHRESH1_STEP1_29        			0x1D
#define RF_AGCTHRESH1_STEP1_30        			0x1E
#define RF_AGCTHRESH1_STEP1_31        			0x1F

// RegAgcThresh2 - not present on RFM6			9/SX1231
#define RF_AGCTHRESH2_STEP2_0         			0x00
#define RF_AGCTHRESH2_STEP2_1         			0x10
#define RF_AGCTHRESH2_STEP2_2         			0x20
#define RF_AGCTHRESH2_STEP2_3         			0x30  // XXX wrong -- Default
#define RF_AGCTHRESH2_STEP2_4         			0x40
#define RF_AGCTHRESH2_STEP2_5         			0x50
#define RF_AGCTHRESH2_STEP2_6         			0x60
#define RF_AGCTHRESH2_STEP2_7         			0x70  // default
#define RF_AGCTHRESH2_STEP2_8         			0x80
#define RF_AGCTHRESH2_STEP2_9         			0x90
#define RF_AGCTHRESH2_STEP2_10        			0xA0
#define RF_AGCTHRESH2_STEP2_11        			0xB0
#define RF_AGCTHRESH2_STEP2_12        			0xC0
#define RF_AGCTHRESH2_STEP2_13        			0xD0
#define RF_AGCTHRESH2_STEP2_14        			0xE0
#define RF_AGCTHRESH2_STEP2_15        			0xF0

#define RF_AGCTHRESH2_STEP3_0         			0x00
#define RF_AGCTHRESH2_STEP3_1         			0x01
#define RF_AGCTHRESH2_STEP3_2         			0x02
#define RF_AGCTHRESH2_STEP3_3         			0x03
#define RF_AGCTHRESH2_STEP3_4         			0x04
#define RF_AGCTHRESH2_STEP3_5         			0x05
#define RF_AGCTHRESH2_STEP3_6         			0x06
#define RF_AGCTHRESH2_STEP3_7         			0x07
#define RF_AGCTHRESH2_STEP3_8         			0x08
#define RF_AGCTHRESH2_STEP3_9         			0x09
#define RF_AGCTHRESH2_STEP3_10        			0x0A
#define RF_AGCTHRESH2_STEP3_11        			0x0B  // Default
#define RF_AGCTHRESH2_STEP3_12        			0x0C
#define RF_AGCTHRESH2_STEP3_13        			0x0D
#define RF_AGCTHRESH2_STEP3_14        			0x0E
#define RF_AGCTHRESH2_STEP3_15        			0x0F

// RegAgcThresh3 - not present on RFM69/SX1231
#define RF_AGCTHRESH3_STEP4_0         			0x00
#define RF_AGCTHRESH3_STEP4_1         			0x10
#define RF_AGCTHRESH3_STEP4_2         			0x20
#define RF_AGCTHRESH3_STEP4_3         			0x30
#define RF_AGCTHRESH3_STEP4_4         			0x40
#define RF_AGCTHRESH3_STEP4_5         			0x50
#define RF_AGCTHRESH3_STEP4_6         			0x60
#define RF_AGCTHRESH3_STEP4_7         			0x70
#define RF_AGCTHRESH3_STEP4_8         			0x80
#define RF_AGCTHRESH3_STEP4_9         			0x90  // Default
#define RF_AGCTHRESH3_STEP4_10        			0xA0
#define RF_AGCTHRESH3_STEP4_11        			0xB0
#define RF_AGCTHRESH3_STEP4_12        			0xC0
#define RF_AGCTHRESH3_STEP4_13        			0xD0
#define RF_AGCTHRESH3_STEP4_14        			0xE0
#define RF_AGCTHRESH3_STEP4_15        			0xF0

#define RF_AGCTHRESH3_STEP5_0         			0x00
#define RF_AGCTHRESH3_STEP5_1         			0x01
#define RF_AGCTHRESH3_STEP5_2         			0x02
#define RF_AGCTHRESH3_STEP5_3         			0x03
#define RF_AGCTHRESH3_STEP5_4         			0x04
#define RF_AGCTHRESH3_STEP5_5         			0x05
#define RF_AGCTHRESH3_STEP5_6         			0x06
#define RF_AGCTHRESH3_STEP5_7         			0x07
#define RF_AGCTHRES33_STEP5_8         			0x08
#define RF_AGCTHRESH3_STEP5_9         			0x09
#define RF_AGCTHRESH3_STEP5_10        			0x0A
#define RF_AGCTHRESH3_STEP5_11        			0x0B  // Default
#define RF_AGCTHRESH3_STEP5_12        			0x0C
#define RF_AGCTHRESH3_STEP5_13        			0x0D
#define RF_AGCTHRESH3_STEP5_14        			0x0E
#define RF_AGCTHRESH3_STEP5_15        			0x0F

// RegLna
#define RF_LNA_ZIN_50                 			0x00  // Reset value
#define RF_LNA_ZIN_200                			0x80  // Recommended default

#define RF_LNA_LOWPOWER_OFF           			0x00  // Default
#define RF_LNA_LOWPOWER_ON            			0x40

#define RF_LNA_CURRENTGAIN            			0x08

#define RF_LNA_GAINSELECT_AUTO        			0x00  // Default
#define RF_LNA_GAINSELECT_MAX         			0x01
#define RF_LNA_GAINSELECT_MAXMINUS6   			0x02
#define RF_LNA_GAINSELECT_MAXMINUS12  			0x03
#define RF_LNA_GAINSELECT_MAXMINUS24  			0x04
#define RF_LNA_GAINSELECT_MAXMINUS36  			0x05
#define RF_LNA_GAINSELECT_MAXMINUS48  			0x06

// RegRxBw
#define RF_RXBW_DCCFREQ_000           			0x00
#define RF_RXBW_DCCFREQ_001           			0x20
#define RF_RXBW_DCCFREQ_010           			0x40  // Recommended default
#define RF_RXBW_DCCFREQ_011           			0x60
#define RF_RXBW_DCCFREQ_100           			0x80  // Reset value
#define RF_RXBW_DCCFREQ_101           			0xA0
#define RF_RXBW_DCCFREQ_110           			0xC0
#define RF_RXBW_DCCFREQ_111           			0xE0

#define RF_RXBW_MANT_16               			0x00  // Reset value
#define RF_RXBW_MANT_20               			0x08
#define RF_RXBW_MANT_24               			0x10  // Recommended default

#define RF_RXBW_EXP_0                 			0x00
#define RF_RXBW_EXP_1                 			0x01
#define RF_RXBW_EXP_2                 			0x02
#define RF_RXBW_EXP_3                 			0x03
#define RF_RXBW_EXP_4                 			0x04
#define RF_RXBW_EXP_5                 			0x05  // Recommended default
#define RF_RXBW_EXP_6                 			0x06  // Reset value
#define RF_RXBW_EXP_7                 			0x07

// RegAfcBw
#define RF_AFCBW_DCCFREQAFC_000       			0x00
#define RF_AFCBW_DCCFREQAFC_001       			0x20
#define RF_AFCBW_DCCFREQAFC_010       			0x40
#define RF_AFCBW_DCCFREQAFC_011       			0x60
#define RF_AFCBW_DCCFREQAFC_100       			0x80  // Default
#define RF_AFCBW_DCCFREQAFC_101       			0xA0
#define RF_AFCBW_DCCFREQAFC_110       			0xC0
#define RF_AFCBW_DCCFREQAFC_111       			0xE0

#define RF_AFCBW_MANTAFC_16           			0x00
#define RF_AFCBW_MANTAFC_20           			0x08  // Default
#define RF_AFCBW_MANTAFC_24           			0x10

#define RF_AFCBW_EXPAFC_0             			0x00
#define RF_AFCBW_EXPAFC_1             			0x01
#define RF_AFCBW_EXPAFC_2             			0x02  // Reset value
#define RF_AFCBW_EXPAFC_3             			0x03  // Recommended default
#define RF_AFCBW_EXPAFC_4             			0x04
#define RF_AFCBW_EXPAFC_5             			0x05
#define RF_AFCBW_EXPAFC_6             			0x06
#define RF_AFCBW_EXPAFC_7             			0x07

// RegOokPeak
#define RF_OOKPEAK_THRESHTYPE_FIXED       		0x00
#define RF_OOKPEAK_THRESHTYPE_PEAK        		0x40  // Default
#define RF_OOKPEAK_THRESHTYPE_AVERAGE     		0x80

#define RF_OOKPEAK_PEAKTHRESHSTEP_000     		0x00  // Default
#define RF_OOKPEAK_PEAKTHRESHSTEP_001     		0x08
#define RF_OOKPEAK_PEAKTHRESHSTEP_010     		0x10
#define RF_OOKPEAK_PEAKTHRESHSTEP_011     		0x18
#define RF_OOKPEAK_PEAKTHRESHSTEP_100     		0x20
#define RF_OOKPEAK_PEAKTHRESHSTEP_101     		0x28
#define RF_OOKPEAK_PEAKTHRESHSTEP_110     		0x30
#define RF_OOKPEAK_PEAKTHRESHSTEP_111     		0x38

#define RF_OOKPEAK_PEAKTHRESHDEC_000      		0x00  // Default
#define RF_OOKPEAK_PEAKTHRESHDEC_001      		0x01
#define RF_OOKPEAK_PEAKTHRESHDEC_010      		0x02
#define RF_OOKPEAK_PEAKTHRESHDEC_011      		0x03
#define RF_OOKPEAK_PEAKTHRESHDEC_100      		0x04
#define RF_OOKPEAK_PEAKTHRESHDEC_101      		0x05
#define RF_OOKPEAK_PEAKTHRESHDEC_110      		0x06
#define RF_OOKPEAK_PEAKTHRESHDEC_111      		0x07

// RegOokAvg
#define RF_OOKAVG_AVERAGETHRESHFILT_00    		0x00
#define RF_OOKAVG_AVERAGETHRESHFILT_01    		0x40
#define RF_OOKAVG_AVERAGETHRESHFILT_10    		0x80  // Default
#define RF_OOKAVG_AVERAGETHRESHFILT_11    		0xC0

// RegOokFix
#define RF_OOKFIX_FIXEDTHRESH_VALUE       		0x06  // Default

// RegAfcFei
#define RF_AFCFEI_FEI_DONE                		0x40
#define RF_AFCFEI_FEI_START               		0x20
#define RF_AFCFEI_AFC_DONE                		0x10
#define RF_AFCFEI_AFCAUTOCLEAR_ON         		0x08
#define RF_AFCFEI_AFCAUTOCLEAR_OFF        		0x00  // Default

#define RF_AFCFEI_AFCAUTO_ON              		0x04
#define RF_AFCFEI_AFCAUTO_OFF             		0x00  // Default

#define RF_AFCFEI_AFC_CLEAR               		0x02
#define RF_AFCFEI_AFC_START               		0x01

// RegRssiConfig
#define RF_RSSI_FASTRX_ON                 		0x08  // not present on RFM69/SX1231
#define RF_RSSI_FASTRX_OFF                		0x00  // Default

#define RF_RSSI_DONE                      		0x02
#define RF_RSSI_START                     		0x01

// RegDioMapping1
#define RF_DIOMAPPING1_DIO0_00            		0x00  // Default
#define RF_DIOMAPPING1_DIO0_01            		0x40
#define RF_DIOMAPPING1_DIO0_10            		0x80
#define RF_DIOMAPPING1_DIO0_11            		0xC0

#define RF_DIOMAPPING1_DIO1_00            		0x00  // Default
#define RF_DIOMAPPING1_DIO1_01            		0x10
#define RF_DIOMAPPING1_DIO1_10            		0x20
#define RF_DIOMAPPING1_DIO1_11            		0x30

#define RF_DIOMAPPING1_DIO2_00            		0x00  // Default
#define RF_DIOMAPPING1_DIO2_01            		0x04
#define RF_DIOMAPPING1_DIO2_10            		0x08
#define RF_DIOMAPPING1_DIO2_11            		0x0C

#define RF_DIOMAPPING1_DIO3_00            		0x00  // Default
#define RF_DIOMAPPING1_DIO3_01            		0x01
#define RF_DIOMAPPING1_DIO3_10            		0x02
#define RF_DIOMAPPING1_DIO3_11            		0x03

// RegDioMapping2
#define RF_DIOMAPPING2_DIO4_00            		0x00  // Default
#define RF_DIOMAPPING2_DIO4_01            		0x40
#define RF_DIOMAPPING2_DIO4_10            		0x80
#define RF_DIOMAPPING2_DIO4_11            		0xC0

#define RF_DIOMAPPING2_DIO5_00            		0x00  // Default
#define RF_DIOMAPPING2_DIO5_01            		0x10
#define RF_DIOMAPPING2_DIO5_10            		0x20
#define RF_DIOMAPPING2_DIO5_11            		0x30

#define RF_DIOMAPPING2_CLKOUT_32MHZ       		0x00
#define RF_DIOMAPPING2_CLKOUT_16MHZ       		0x01
#define RF_DIOMAPPING2_CLKOUT_8MHZ        		0x02
#define RF_DIOMAPPING2_CLKOUT_4MHZ        		0x03
#define RF_DIOMAPPING2_CLKOUT_2MHZ        		0x04
#define RF_DIOMAPPING2_CLKOUT_1MHZ        		0x05  // Reset value
#define RF_DIOMAPPING2_CLKOUT_RC          		0x06
#define RF_DIOMAPPING2_CLKOUT_OFF         		0x07  // Recommended default

// RegIrqFlags1
#define RF_IRQFLAGS1_MODEREADY           		0x80
#define RF_IRQFLAGS1_RXREADY             		0x40
#define RF_IRQFLAGS1_TXREADY             		0x20
#define RF_IRQFLAGS1_PLLLOCK             		0x10
#define RF_IRQFLAGS1_RSSI                		0x08
#define RF_IRQFLAGS1_TIMEOUT             		0x04
#define RF_IRQFLAGS1_AUTOMODE            		0x02
#define RF_IRQFLAGS1_SYNCADDRESSMATCH    		0x01

// RegIrqFlags2
#define RF_IRQFLAGS2_FIFOFULL            		0x80
#define RF_IRQFLAGS2_FIFONOTEMPTY        		0x40
#define RF_IRQFLAGS2_FIFOLEVEL           		0x20
#define RF_IRQFLAGS2_FIFOOVERRUN         		0x10
#define RF_IRQFLAGS2_PACKETSENT          		0x08
#define RF_IRQFLAGS2_PAYLOADREADY        		0x04
#define RF_IRQFLAGS2_CRCOK               		0x02
#define RF_IRQFLAGS2_LOWBAT              		0x01  // not present on RFM69/SX1231

// RegRssiThresh
#define RF_RSSITHRESH_VALUE               		0xE4  // Default

// RegRxTimeout1
#define RF_RXTIMEOUT1_RXSTART_VALUE       		0x00  // Default

// RegRxTimeout2
#define RF_RXTIMEOUT2_RSSITHRESH_VALUE    		0x00  // Default

// RegPreamble
#define RF_PREAMBLESIZE_MSB_VALUE         		0x00  // Default
#define RF_PREAMBLESIZE_LSB_VALUE         		0x03  // Default

// RegSyncConfig
#define RF_SYNC_ON               				0x80  // Default
#define RF_SYNC_OFF              				0x00

#define RF_SYNC_FIFOFILL_AUTO    				0x00  // Default -- when sync interrupt occurs
#define RF_SYNC_FIFOFILL_MANUAL  				0x40

#define RF_SYNC_SIZE_1           				0x00
#define RF_SYNC_SIZE_2           				0x08
#define RF_SYNC_SIZE_3           				0x10
#define RF_SYNC_SIZE_4           				0x18  // Default
#define RF_SYNC_SIZE_5           				0x20
#define RF_SYNC_SIZE_6           				0x28
#define RF_SYNC_SIZE_7           				0x30
#define RF_SYNC_SIZE_8           				0x38

#define RF_SYNC_TOL_0            				0x00  // Default
#define RF_SYNC_TOL_1            				0x01
#define RF_SYNC_TOL_2            				0x02
#define RF_SYNC_TOL_3            				0x03
#define RF_SYNC_TOL_4            				0x04
#define RF_SYNC_TOL_5            				0x05
#define RF_SYNC_TOL_6            				0x06
#define RF_SYNC_TOL_7            				0x07

// RegSyncValue1-8
#define RF_SYNC_BYTE1_VALUE      				0x00  // Default
#define RF_SYNC_BYTE2_VALUE      				0x00  // Default
#define RF_SYNC_BYTE3_VALUE      				0x00  // Default
#define RF_SYNC_BYTE4_VALUE      				0x00  // Default
#define RF_SYNC_BYTE5_VALUE      				0x00  // Default
#define RF_SYNC_BYTE6_VALUE      				0x00  // Default
#define RF_SYNC_BYTE7_VALUE      				0x00  // Default
#define RF_SYNC_BYTE8_VALUE      				0x00  // Default

// RegPacketConfig1
#define RF_PACKET1_FORMAT_FIXED      			0x00  // Default
#define RF_PACKET1_FORMAT_VARIABLE   			0x80

#define RF_PACKET1_DCFREE_OFF        			0x00  // Default
#define RF_PACKET1_DCFREE_MANCHESTER 			0x20
#define RF_PACKET1_DCFREE_WHITENING  			0x40

#define RF_PACKET1_CRC_ON            			0x10  // Default
#define RF_PACKET1_CRC_OFF           			0x00

#define RF_PACKET1_CRCAUTOCLEAR_ON   			0x00  // Default
#define RF_PACKET1_CRCAUTOCLEAR_OFF  			0x08

#define RF_PACKET1_ADRSFILTERING_OFF            0x00  // Default
#define RF_PACKET1_ADRSFILTERING_NODE           0x02
#define RF_PACKET1_ADRSFILTERING_NODEBROADCAST  0x04

// RegPayloadLength
#define RF_PAYLOADLENGTH_VALUE          		0x40  // Default

// RegBroadcastAdrs
#define RF_BROADCASTADDRESS_VALUE       		0x00

// RegAutoModes
#define RF_AUTOMODES_ENTER_OFF                	0x00  // Default
#define RF_AUTOMODES_ENTER_FIFONOTEMPTY       	0x20
#define RF_AUTOMODES_ENTER_FIFOLEVEL          	0x40
#define RF_AUTOMODES_ENTER_CRCOK              	0x60
#define RF_AUTOMODES_ENTER_PAYLOADREADY       	0x80
#define RF_AUTOMODES_ENTER_SYNCADRSMATCH      	0xA0
#define RF_AUTOMODES_ENTER_PACKETSENT         	0xC0
#define RF_AUTOMODES_ENTER_FIFOEMPTY          	0xE0

#define RF_AUTOMODES_EXIT_OFF                 	0x00  // Default
#define RF_AUTOMODES_EXIT_FIFOEMPTY           	0x04
#define RF_AUTOMODES_EXIT_FIFOLEVEL           	0x08
#define RF_AUTOMODES_EXIT_CRCOK               	0x0C
#define RF_AUTOMODES_EXIT_PAYLOADREADY        	0x10
#define RF_AUTOMODES_EXIT_SYNCADRSMATCH       	0x14
#define RF_AUTOMODES_EXIT_PACKETSENT          	0x18
#define RF_AUTOMODES_EXIT_RXTIMEOUT           	0x1C
#define RF_AUTOMODES_INTERMEDIATE_SLEEP       	0x00  // Default
#define RF_AUTOMODES_INTERMEDIATE_STANDBY     	0x01
#define RF_AUTOMODES_INTERMEDIATE_RECEIVER    	0x02
#define RF_AUTOMODES_INTERMEDIATE_TRANSMITTER 	0x03


// RegFifoThresh
#define RF_FIFOTHRESH_TXSTART_FIFOTHRESH      	0x00  // Reset value
#define RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY    	0x80  // Recommended default
#define RF_FIFOTHRESH_VALUE                   	0x0F  // Default


// RegPacketConfig2
#define RF_PACKET2_RXRESTARTDELAY_1BIT        	0x00  // Default
#define RF_PACKET2_RXRESTARTDELAY_2BITS       	0x10
#define RF_PACKET2_RXRESTARTDELAY_4BITS       	0x20
#define RF_PACKET2_RXRESTARTDELAY_8BITS       	0x30
#define RF_PACKET2_RXRESTARTDELAY_16BITS      	0x40
#define RF_PACKET2_RXRESTARTDELAY_32BITS      	0x50
#define RF_PACKET2_RXRESTARTDELAY_64BITS      	0x60
#define RF_PACKET2_RXRESTARTDELAY_128BITS     	0x70
#define RF_PACKET2_RXRESTARTDELAY_256BITS     	0x80
#define RF_PACKET2_RXRESTARTDELAY_512BITS     	0x90
#define RF_PACKET2_RXRESTARTDELAY_1024BITS    	0xA0
#define RF_PACKET2_RXRESTARTDELAY_2048BITS    	0xB0
#define RF_PACKET2_RXRESTARTDELAY_NONE        	0xC0
#define RF_PACKET2_RXRESTART                  	0x04

#define RF_PACKET2_AUTORXRESTART_ON          	0x02  // Default
#define RF_PACKET2_AUTORXRESTART_OFF         	0x00
#define RF_PACKET2_AES_ON                    	0x01
#define RF_PACKET2_AES_OFF                   	0x00  // Default



// RegAesKey1-16
#define RF_AESKEY1_VALUE           				0x00  // Default
#define RF_AESKEY2_VALUE           				0x00  // Default
#define RF_AESKEY3_VALUE           				0x00  // Default
#define RF_AESKEY4_VALUE           				0x00  // Default
#define RF_AESKEY5_VALUE           				0x00  // Default
#define RF_AESKEY6_VALUE           				0x00  // Default
#define RF_AESKEY7_VALUE           				0x00  // Default
#define RF_AESKEY8_VALUE           				0x00  // Default
#define RF_AESKEY9_VALUE           				0x00  // Default
#define RF_AESKEY10_VALUE          				0x00  // Default
#define RF_AESKEY11_VALUE          				0x00  // Default
#define RF_AESKEY12_VALUE          				0x00  // Default
#define RF_AESKEY13_VALUE          				0x00  // Default
#define RF_AESKEY14_VALUE          				0x00  // Default
#define RF_AESKEY15_VALUE          				0x00  // Default
#define RF_AESKEY16_VALUE          				0x00  // Default

// RegTemp1
#define RF_TEMP1_MEAS_START         			0x08
#define RF_TEMP1_MEAS_RUNNING       			0x04

// RegTestLna
#define RF_TESTLNA_NORMAL           			0x1B
#define RF_TESTLNA_HIGH_SENSITIVITY 			0x2D

// RegTestDagc
#define RF_DAGC_NORMAL              			0x00  // Reset value
#define RF_DAGC_IMPROVED_LOWBETA1   			0x20
#define RF_DAGC_IMPROVED_LOWBETA0   			0x30  // Recommended default


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None */


typedef enum
{
    REGFIFO          = 0x00,
    REGOPMODE        = 0x01,
    REGDATAMODUL     = 0x02,
    REGBITRATEMSB    = 0x03,
    REGBITRATELSB    = 0x04,
    REGFDEVMSB       = 0x05,
    REGFDEVLSB       = 0x06,
    REGFRFMSB        = 0x07,
    REGFRFMID        = 0x08,
    REGFRFLSB        = 0x09,
    REGOSC1          = 0x0A,
    REGAFCCTRL       = 0x0B,
    RESERVED0C       = 0x0C,
    REGLISTEN1       = 0x0D,
    REGLISTEN2       = 0x0E,
    REGLISTEN3       = 0x0F,
    REGVERSION       = 0x10,
    REGPALEVEL       = 0x11,
    REGPARAMP        = 0x12,
    REGOCP           = 0x13,
    RESERVED14       = 0x14,
    RESERVED15       = 0x15,
    RESERVED16       = 0x16,
    RESERVED17       = 0x17,
    REGLNA           = 0x18,
    REGRXBW          = 0x19,
    REGAFCBW         = 0x1A,
    REGOOKPEAK       = 0x1B,
    REGOOKAVG        = 0x1C,
    REGOOKFIX        = 0x1D,
    REGAFCFEI        = 0x1E,
    REGAFCMSB        = 0x1F,
    REGAFCLSB        = 0x20,
    REGFEIMSB        = 0x21,
    REGFEILSB        = 0x22,
    REGRSSICONFIG    = 0x23,
    REGRSSIVALUE     = 0x24,
    REGDIOMAPPING1   = 0x25,
    REGDIOMAPPING2   = 0x26,
    REGIRQFLAGS1     = 0x27,
    REGIRQFLAGS2     = 0x28,
    REGRSSITHRESH    = 0x29,
    REGRXTIMEOUT1    = 0x2A,
    REGRXTIMEOUT2    = 0x2B,
    REGPREAMBLEMSB   = 0x2C,
    REGPREAMBLELSB   = 0x2D,
    REGSYNCCONFIG    = 0x2E,
    REGSYNCVALU1     = 0x2F,
    REGSYNCVALU2     = 0x30,
    REGSYNCVALU3     = 0x31,
    REGSYNCVALU4     = 0x32,
    REGSYNCVALU5     = 0x33,
    REGSYNCVALU6     = 0x34,
    REGSYNCVALU7     = 0x35,
    REGSYNCVALU8     = 0x36,
    REGPACKETCONFIG1 = 0x37,
    REGPAYLOADLENGTH = 0x38,
    REGNODEADRS      = 0x39,
    REGBROADCASTADRS = 0x3A,
    REGAUTOMODES     = 0x3B,
    REGFIFOTHRESH    = 0x3C,
    REGPACKETCONFIG2 = 0x3D,
    REGAESKEY1       = 0x3E,
    REGAESKEY2       = 0x3F,
    REGAESKEY3       = 0x40,
    REGAESKEY4       = 0x41,
    REGAESKEY5       = 0x42,
    REGAESKEY6       = 0x43,
    REGAESKEY7       = 0x44,
    REGAESKEY8       = 0x45,
    REGAESKEY9       = 0x46,
    REGAESKEY10      = 0x47,
    REGAESKEY11      = 0x48,
    REGAESKEY12      = 0x49,
    REGAESKEY13      = 0x4A,
    REGAESKEY14      = 0x4B,
    REGAESKEY15      = 0x4C,
    REGAESKEY16      = 0x4D,
    REGTEMP1         = 0x4E,
    REGTEMP2         = 0x4F,
    REGTESTLNA       = 0x58,
    REGTESTPA1       = 0x5A,
    REGTESTPA2       = 0x5C,
    REGTESTDAGC      = 0x6F,
    REGTESTAFC       = 0x71,
    REG_MAX
} RFM69_registers_et;


typedef enum
{
    RFM69_MGR_INITIALISING = 0u,
    RFM69_MGR_SETUP_TX_MODE,
    RFM69_MGR_TX_MODE,
    RFM69_MGR_SETUP_RX_MODE,
    RFM69_MGR_RX_MODE,
    RFM69_MGR_SLEEP_MODE,
    RFM69_MGR_IDLE,
    RFM69_MGR_RF_RESET,
} RFM69_state_et;



typedef enum
{
    RFM69_433_DEFAULT_CONFIG,
    RFM69_433_ONLINE_CONFIG,
    RFM69_CFG_MAX
} RFM69_static_configuration_et;


typedef enum
{
    RFM69_SLEEP_MODE = 0u,
    RFM69_STANDBY_MODE,
    RFM69_FS_MODE,
    RFM69_TRANSMIT_MODE,
    RFM69_RECEIVE_MODE,
    RFM69_LISTEN_MODE
} RFM69_operating_modes_et;


typedef enum
{
    RFM69_FIXED_PACKET_MODE = 0u,
    RFM69_VARIABLE_PACKET_MODE,
} RFM69_packet_modes_et;


typedef enum
{
    RFM69_FSK = 0u,
    RFM69_OOK,
    RFM69_RESERVED
} RFM69_modulation_type_et;

typedef enum
{
    RFM69_PACKET_MODE = 0u,
    RFM69_CONT_WITH_BIT_SYNC = 2u,
    RFM69_CONT_WITHOUT_BIT_SYNC = 3u,
    RFM69_MODE_MAX
} RFM69_data_mode_et;


//! RFM69 instructions
typedef enum
{
    WRITE_TO_CHIP_BURST_MODE    = 0x01,     //! Writes data to a number of consecutive registers
    WRITE_TO_CHIP               = 0x02,     //! Writes 1 byte of data to a specific register
    READ_FROM_CHIP              = 0x03,     //! Reads 1 byte of data from a specific register
    READ_FIFO_FROM_CHIP         = 0x04,     //! Reads data from the FIFO buffer
    READ_FROM_CHIP_BURST_MODE   = 0x05,     //! Reads data from a number of consecutive registers
    WRITE_TO_CHIP_BURST_MODE_CONF   = 0x06,     //! writes data from a config structure
    RFM69_INSTRUCTION_MAX                   //! RFM69_INSTRUCTION_MAX
} RFM69_instruction_et;

typedef enum
{
    RFM_69_64us = 1u,
    RFM_69_4100us,
    RFM_69_26200us
} RFM69_listen_time_et;

typedef enum
{
    RFM69_LISTEN_IDLE = 0u,
    RFM69_LISTEN_RX
} RFM69_listen_state_et;

typedef enum
{
    RFM69_1200bps = 0u,
    RFM69_2400bps,
    RFM69_4800bps,
    RFM69_9600bps,
    RFM69_19200bps,
    RFM69_38400bps,
    RFM69_76800bps,
    RFM69_153600bps
} RFM69_predefined_bitrates_et;


typedef enum
{
    RFM69_DIO_MODE_0 = 0u,
    RFM69_DIO_MODE_1,
    RFM69_DIO_MODE_2,
    RFM69_DIO_MODE_3,
} RFM69_DIO_map_mode_et;

typedef struct
{
   RFM69_registers_et RFM69_register;
   u8_t register_data;
} RFM69_register_data_st;


typedef struct
{
    RFM69_register_data_st* buffer_p; //! pointer to configuration
    u16_t length;                      //! length of configuration
} RFM69_static_configuration_st;


typedef struct
{
	u8_t tx_rd_data[32];
} RFM69_data_packet_st;


/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */




/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void          RFM69_init( void );
void          RFM69_tick( void );
void          RFM69_setup_receive_mode( void );
pass_fail_et  RFM69_read_registers( RFM69_instruction_et instruction, RFM69_registers_et address, u8_t* read_data, u8_t num_bytes );
pass_fail_et  RFM69_write_registers( RFM69_instruction_et instruction, RFM69_registers_et address, u8_t* write_data, u8_t num_bytes );
pass_fail_et  RFM69_set_configuration( RFM69_static_configuration_et config );
void 		  RFM69_get_configuration( RFM69_static_configuration_et config, RFM69_register_data_st* data_p );
void          RFM69_set_reset_pin_state( low_high_et state );
void 		  RFM69_set_enable_pin_state( low_high_et state );

/* Writes */
false_true_et RFM69_set_clock_out( disable_enable_et state );
u8_t 		  RFM69_set_PA_level( u8_t level );
false_true_et RFM69_set_operating_mode( RFM69_operating_modes_et operating_mode );
false_true_et RFM69_set_packet_mode( RFM69_packet_modes_et operating_mode );
false_true_et RFM69_set_rx_payload_length( u8_t num_bytes );
false_true_et RFM69_set_modulation_type( RFM69_modulation_type_et type );
false_true_et RFM69_set_data_mode( RFM69_data_mode_et mode );
false_true_et RFM69_set_bit_rate( RFM69_predefined_bitrates_et bit_rate );
false_true_et RFM69_set_listen_time_resolution( RFM69_listen_state_et state, RFM69_listen_time_et listen_time_resolution );
false_true_et RFM69_set_listen_time( RFM69_listen_state_et state, u16_t time );
false_true_et RFM69_set_own_node_address( u8_t address );
false_true_et RFM69_set_own_network_id( u8_t network_id );
false_true_et RFM69_trigger_RSSi_measurement( void );
false_true_et RFM69_set_encryption_key( u8_t* key, false_true_et state );
false_true_et RFM69_set_DIO_mapping( u8_t pin, RFM69_DIO_map_mode_et mode );
false_true_et RFM69_write_to_FIFO( u8_t* buffer, u8_t len );
false_true_et RFM69_send_frame( u8_t* buffer, u8_t len, u8_t rx_node_address );
u16_t RFM69_read_RSSI( void );
void  RFM69_update_packet_sent( false_true_et state );
void  RFM69_update_packet_received( false_true_et state );
u32_t RFM69_get_received_packet_cnt( void );
void  RFM69_update_packet_reception_percent( u16_t secs );
f32_t RFM69_get_packet_reception_percent( void );
s8_t  RFM69_get_last_received_RSSI( void );
void  RFM69_timestamp_receieved_packet( void );
u32_t RFM69_get_receieved_packet_timestamp( void );
void RFM69_receive_frame( void );


/* Reads */
u8_t RFM69_read_version_num( void );
pass_fail_et RFM69_read_reserved_registers( void );
u8_t RFM69_read_temp( void );
u8_t RFM69_read_current_LNA_gain( void );
u32_t RFM69_read_rf_carrier_freq( void );
false_true_et RFM69_read_FIFO_register( u8_t* data_p );


#endif /* RFM69_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
