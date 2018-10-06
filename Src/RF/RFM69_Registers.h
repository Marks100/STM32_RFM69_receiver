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
#ifndef RFM69_REGISTERS_H
#define RFM69_REGISTERS_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"
#include "RFM69.h"



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/


/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* Default register set 915Mhz */
const RFM69_register_data_st RFM69_default_register_set[] =
{
    { REGOPMODE        ,0x04 },         
    { REGDATAMODUL     ,0x00 },         
    { REGBITRATEMSB    ,RF_BITRATEMSB_1200 },
    { REGBITRATELSB    ,RF_BITRATELSB_1200 },
    { REGFDEVMSB       ,0x01 },         
    { REGFDEVLSB       ,0x48 },         
    { REGFRFMSB        ,RF_FRFMSB_433 },         
    { REGFRFMID        ,RF_FRFMID_433 },         
    { REGFRFLSB        ,RF_FRFLSB_433 },         
    { REGOSC1          ,0x41 },         
    { REGAFCCTRL       ,0x00 },         
	{ RESERVED0C       ,0x0C },         
    { REGLISTEN1       ,0x92 },         
    { REGLISTEN2       ,0xF5 },         
    { REGLISTEN3       ,0x20 },         
	{ REGVERSION       ,0x24 },         
    { REGPALEVEL       ,0x9F },         
    { REGPARAMP        ,0x09 },         
    { REGOCP           ,0x1A },         
	{ RESERVED14       ,0x40 },         
	{ RESERVED15       ,0xB0 },         
	{ RESERVED16       ,0x7B },         
	{ RESERVED17       ,0x9B },         
    { REGLNA           ,0x88 },         
    { REGRXBW          ,0x4C },         
    { REGAFCBW         ,0x8B },         
    { REGOOKPEAK       ,0x40 },         
    { REGOOKAVG        ,0x80 },         
    { REGOOKFIX        ,0x06 },         
    { REGAFCFEI        ,0x10 },         
	{ REGAFCMSB		   ,0x00 },         
	{ REGAFCLSB		   ,0x00 },         
	{ REGFEIMSB        ,0x00 },         
	{ REGFEILSB        ,0x00 },         
    { REGRSSICONFIG    ,0x02 },         
	{ REGRSSIVALUE     ,0xFF },         
    { REGDIOMAPPING1   ,0x00 },         
    { REGDIOMAPPING2   ,0x07 },         
    { REGIRQFLAGS1     ,0x80 },         
    { REGIRQFLAGS2     ,0x00 },         
    { REGRSSITHRESH    ,0xE4 },         
    { REGRXTIMEOUT1    ,0x00 },         
    { REGRXTIMEOUT2    ,0x00 },         
    { REGPREAMBLEMSB   ,0x00 },         
    { REGPREAMBLELSB   ,0x03 },         
    { REGSYNCCONFIG    ,0x88 },         
    { REGSYNCVALU1     ,0x41 },         
    { REGSYNCVALU2     ,0x48 },         
    { REGSYNCVALU3     ,0x01 },         
    { REGSYNCVALU4     ,0x01 },         
    { REGSYNCVALU5     ,0x01 },         
    { REGSYNCVALU6     ,0x01 },         
    { REGSYNCVALU7     ,0x01 },         
    { REGSYNCVALU8     ,0x01 },         
    { REGPACKETCONFIG1 ,0xD0 },         
    { REGPAYLOADLENGTH ,0x40 },         
    { REGNODEADRS      ,0x00 },         
    { REGBROADCASTADRS ,0x00 },         
    { REGAUTOMODES     ,0x00 },         
    { REGFIFOTHRESH    ,0x8F },         
    { REGPACKETCONFIG2 ,0x02 },         
    { REGAESKEY1       ,0x00 },         
    { REGAESKEY2       ,0x00 },         
    { REGAESKEY3       ,0x00 },         
    { REGAESKEY4       ,0x00 },         
    { REGAESKEY5       ,0x00 },         
    { REGAESKEY6       ,0x00 },         
    { REGAESKEY7       ,0x00 },         
    { REGAESKEY8       ,0x00 },         
    { REGAESKEY9       ,0x00 },         
    { REGAESKEY10      ,0x00 },         
    { REGAESKEY11      ,0x00 },         
    { REGAESKEY12      ,0x00 },
    { REGAESKEY13      ,0x00 },
    { REGAESKEY14      ,0x00 },
    { REGAESKEY15      ,0x00 },
    { REGAESKEY16      ,0x00 },
    { REGTEMP1         ,0x01 },
	{ REGTEMP2         ,0x00 },
};



/* 433Mhz OOK register set */
const RFM69_register_data_st RFM69_433Mhz_OOK_set[] =
{
    { REGOPMODE        ,0x04 },			//Operating mode 				1
    { REGDATAMODUL     ,0x08 },         //Packet Mode | OOK
    { REGBITRATEMSB    ,0x1A },         //4.8Kbps Bitrate
    { REGBITRATELSB    ,0x0B },         //4.8Kbps Bitrate
    { REGFDEVMSB       ,0x00 },
    { REGFDEVLSB       ,0x52 },
    { REGFRFMSB        ,0x6C },         //433Mhz
    { REGFRFMID        ,0x40 },         //433Mhz
    { REGFRFLSB        ,0x00 },         //433Mhz
    { REGOSC1          ,0x41 },
    { REGAFCCTRL       ,0x00 },
    { RESERVED0C	   ,0x00 },
    { REGLISTEN1       ,0xA0 },
    { REGLISTEN2       ,0xF5 },
    { REGLISTEN3       ,0x20 },
	{ REGVERSION       ,0x24 },
	{ REGPALEVEL       ,0x85 },			//Medium output power on PA0
    { REGPARAMP        ,0x09 },
    { REGOCP           ,0x1A },         //OCP
	{ RESERVED14       ,0x40 }, 
	{ RESERVED15       ,0xB0 }, 
	{ RESERVED16       ,0x7B }, 
	{ RESERVED17       ,0x9B }, 
    { REGLNA           ,0x88 },
    { REGRXBW          ,0x42 },
    { REGAFCBW         ,0x8B },
    { REGOOKPEAK       ,0x40 },
    { REGOOKAVG        ,0x80 },
    { REGOOKFIX        ,0x06 },
    { REGAFCFEI        ,0x10 },
	{ REGAFCMSB		   ,0x00 },
	{ REGAFCLSB		   ,0x00 },
	{ REGFEIMSB        ,0x00 },
	{ REGFEILSB        ,0x00 },
    { REGRSSICONFIG    ,0x02 },
	{ REGRSSIVALUE     ,0xFF }, 
    { REGDIOMAPPING1   ,0x00 },
    { REGDIOMAPPING2   ,0x07 },
    { REGIRQFLAGS1     ,0x80 },
    { REGIRQFLAGS2     ,0x00 },
    { REGRSSITHRESH    ,0xDC },
    { REGRXTIMEOUT1    ,0x00 },
    { REGRXTIMEOUT2    ,0x00 },
    { REGPREAMBLEMSB   ,0x00 },
    { REGPREAMBLELSB   ,0x03 },
    { REGSYNCCONFIG    ,0x90 },
    { REGSYNCVALU1     ,0xAA },			//Keep this as the default sync value meaning 0xAA must be present on all nodes and receiver
    { REGSYNCVALU2     ,0xAA },			// Will be replaced by network ID ( default to 0xAA )
    { REGSYNCVALU3     ,0x01 },
    { REGSYNCVALU4     ,0x01 },
    { REGSYNCVALU5     ,0x01 },
    { REGSYNCVALU6     ,0x01 },
    { REGSYNCVALU7     ,0x01 },
    { REGSYNCVALU8     ,0x01 },
    { REGPACKETCONFIG1 ,0x90 },         //variable length packets | CRC OFF | NO Address filtering
    { REGPAYLOADLENGTH ,0x42 },
    { REGNODEADRS      ,0x00 },
    { REGBROADCASTADRS ,0x00 },
    { REGAUTOMODES     ,0x00 },
    { REGFIFOTHRESH    ,0x8F },
    { REGPACKETCONFIG2 ,0x03 },         //AES ON
    { REGAESKEY1       ,0x00 },
    { REGAESKEY2       ,0x00 },
    { REGAESKEY3       ,0x00 },
    { REGAESKEY4       ,0x00 },
    { REGAESKEY5       ,0x00 },
    { REGAESKEY6       ,0x00 },
    { REGAESKEY7       ,0x00 },
    { REGAESKEY8       ,0x00 },
    { REGAESKEY9       ,0x00 },
    { REGAESKEY10      ,0x00 },
    { REGAESKEY11      ,0x00 },
    { REGAESKEY12      ,0x00 },
    { REGAESKEY13      ,0x00 },
    { REGAESKEY14      ,0x00 },
    { REGAESKEY15      ,0x00 },
    { REGAESKEY16      ,0x00 },
    { REGTEMP1         ,0x01 },
	{ REGTEMP2         ,0x00 },
};



/* Test Register set */
const RFM69_register_data_st RFM69_test_register_set[] =
{
    { REGOPMODE        ,RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },	
    { REGDATAMODUL     ,RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 },
    { REGBITRATEMSB    ,RF_BITRATEMSB_55555 }, // default: 4.8 KBPS
    { REGBITRATELSB    ,RF_BITRATELSB_55555 },
    { REGFDEVMSB       ,RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
    { REGFDEVLSB       ,RF_FDEVLSB_50000},
    { REGFRFMSB        ,RF_FRFMSB_433 },
    { REGFRFMID        ,RF_FRFMID_433 },
    { REGFRFLSB        ,RF_FRFLSB_433 },
    { REGOSC1          ,0x41 },
    { REGAFCCTRL       ,0x00 },
	{ RESERVED0C       ,0x02 },
    { REGLISTEN1       ,0x92 },
    { REGLISTEN2       ,0xF5 },
    { REGLISTEN3       ,0x20 },
	{ REGVERSION       ,0x24 },
    { REGPALEVEL       ,RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111 },
    { REGPARAMP        ,0x09 },
    { REGOCP           ,RF_OCP_ON | RF_OCP_TRIM_95 }, // over current protection (default is 95mA)
	{ RESERVED14       ,0x40 },
	{ RESERVED15       ,0xB0 },
	{ RESERVED16       ,0x7B },
	{ RESERVED17       ,0x9B },
    { REGLNA           ,0x88 },
    { REGRXBW          ,RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
    { REGAFCBW         ,0x8B },
    { REGOOKPEAK       ,0x40 },
    { REGOOKAVG        ,0x80 },
    { REGOOKFIX        ,0x06 },
    { REGAFCFEI        ,0x10 },
	{ REGAFCMSB		   ,0x00 },
	{ REGAFCLSB		   ,0x00 },
	{ REGFEIMSB        ,0x00 },
	{ REGFEILSB        ,0x00 },
    { REGRSSICONFIG    ,0x02 },
	{ REGRSSIVALUE     ,0xFF },
    { REGDIOMAPPING1   ,RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
    { REGDIOMAPPING2   ,RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
    { REGIRQFLAGS1     ,0x80 },
    { REGIRQFLAGS2     ,RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
    { REGRSSITHRESH    ,220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
    { REGRXTIMEOUT1    ,0x00 },
    { REGRXTIMEOUT2    ,0x00 },
    { REGPREAMBLEMSB   ,0x00 },
    { REGPREAMBLELSB   ,0x03 },
    { REGSYNCCONFIG    ,RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
    { REGSYNCVALU1     ,0x2D },
    { REGSYNCVALU2     ,0xAA },
    { REGSYNCVALU3     ,0x01 },
    { REGSYNCVALU4     ,0x01 },
    { REGSYNCVALU5     ,0x01 },
    { REGSYNCVALU6     ,0x01 },
    { REGSYNCVALU7     ,0x01 },
    { REGSYNCVALU8     ,0x01 },
    { REGPACKETCONFIG1 ,RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
    { REGPAYLOADLENGTH ,66 }, // in variable length mode: the max frame size, not used in TX
    { REGNODEADRS      ,0x00 },
    { REGBROADCASTADRS ,0x00 },
    { REGAUTOMODES     ,0x00 },
    { REGFIFOTHRESH    ,RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
    { REGPACKETCONFIG2 ,RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
    { REGAESKEY1       ,0x00 },
    { REGAESKEY2       ,0x00 },
    { REGAESKEY3       ,0x00 },
    { REGAESKEY4       ,0x00 },
    { REGAESKEY5       ,0x00 },
    { REGAESKEY6       ,0x00 },
    { REGAESKEY7       ,0x00 },
    { REGAESKEY8       ,0x00 },
    { REGAESKEY9       ,0x00 },
    { REGAESKEY10      ,0x00 },
    { REGAESKEY11      ,0x00 },
    { REGAESKEY12      ,0x00 },
    { REGAESKEY13      ,0x00 },
    { REGAESKEY14      ,0x00 },
    { REGAESKEY15      ,0x00 },
    { REGAESKEY16      ,0x00 },
    { REGTEMP1         ,0x01 },
	{ REGTEMP2         ,0x00 },
};



#define DEFAULT_CONFIGURATION_SIZE      ( sizeof( RFM69_default_register_set ) / sizeof( RFM69_register_data_st ) )
#define RFM69_433Mhz_CONFIGURATION_SIZE ( sizeof( RFM69_433Mhz_OOK_set ) / sizeof( RFM69_register_data_st ) )
#define RFM69_TEST_CONFIGURATION_SIZE   ( sizeof( RFM69_test_register_set ) / sizeof( RFM69_register_data_st ) )

const RFM69_static_configuration_st RFM69_config_c[ 3 ] =
{
    { (RFM69_register_data_st * )RFM69_default_register_set, DEFAULT_CONFIGURATION_SIZE },
    { (RFM69_register_data_st * )RFM69_433Mhz_OOK_set,       RFM69_433Mhz_CONFIGURATION_SIZE },
	{ (RFM69_register_data_st * )RFM69_test_register_set,    RFM69_TEST_CONFIGURATION_SIZE },
};


#endif /* RFM69_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
