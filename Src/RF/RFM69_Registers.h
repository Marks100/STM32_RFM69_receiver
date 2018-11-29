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
/* Default register set 433Mhz ( 19200bps ) */
const RFM69_register_data_st RFM69_default_register_set[] =
{
	/*! Common Configuration Registers */
    { REGOPMODE        ,0x04 },         
    { REGDATAMODUL     ,0x00 },         
    { REGBITRATEMSB    ,RF_BITRATEMSB_19200 },
    { REGBITRATELSB    ,RF_BITRATELSB_19200 },
    { REGFDEVMSB       ,0x00 },         
    { REGFDEVLSB       ,0x52 },         
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
	/*! Transmitter Registers */       
    { REGPALEVEL       ,0x9F },         
    { REGPARAMP        ,0x09 },         
    { REGOCP           ,0x1A }, 
	/*! Receiver Registers */        
	{ RESERVED14       ,0x40 },         
	{ RESERVED15       ,0xB0 },         
	{ RESERVED16       ,0x7B },         
	{ RESERVED17       ,0x9B },         
    { REGLNA           ,0x88 },         
    { REGRXBW          ,0x55 },         
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
	/*! IRQ and Pin Mapping Registers */         
    { REGDIOMAPPING1   ,0x00 },         
    { REGDIOMAPPING2   ,0x07 },         
    { REGIRQFLAGS1     ,0x80 },         
    { REGIRQFLAGS2     ,0x00 },         
    { REGRSSITHRESH    ,0xE4 },         
    { REGRXTIMEOUT1    ,0x00 },         
    { REGRXTIMEOUT2    ,0x00 }, 
	/*!Packet Engine Registers*/        
    { REGPREAMBLEMSB   ,0x00 },         
    { REGPREAMBLELSB   ,0x03 },         
    { REGSYNCCONFIG    ,0x98 },         
    { REGSYNCVALU1     ,0x01 },         
    { REGSYNCVALU2     ,0x01 },         
    { REGSYNCVALU3     ,0x01 },         
    { REGSYNCVALU4     ,0x01 },         
    { REGSYNCVALU5     ,0x01 },         
    { REGSYNCVALU6     ,0x01 },         
    { REGSYNCVALU7     ,0x01 },         
    { REGSYNCVALU8     ,0x01 },         
    { REGPACKETCONFIG1 ,0x10 },         
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
    /*! Temperature Sensor Registers */
    { REGTEMP1         ,0x01 },
	{ REGTEMP2         ,0x00 },
	/*! Test Registers */
	{ REGTESTLNA       ,0x1B },
	{ REGTESTPA1       ,0x55 },
	{ REGTESTPA2       ,0x70 },
	{ REGTESTDAGC      ,0x30 },
	{ REGTESTAFC       ,0x00 },
};



/* Default register set 433Mhz ( 19200bps ) */
const RFM69_register_data_st RFM69_online_register_set[] =
{
    /*! Common Configuration Registers */
    { REGOPMODE        ,RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
    { REGDATAMODUL     ,RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, //no shaping
    { REGBITRATEMSB    ,RF_BITRATEMSB_55555 },
    { REGBITRATELSB    ,RF_BITRATELSB_55555 },
    { REGFDEVMSB       ,RF_FDEVMSB_50000}, //default:5khz, (FDEV + BitRate/2 <= 500Khz)
    { REGFDEVLSB       ,RF_FDEVLSB_50000 },
    { REGFRFMSB        ,RF_FRFMSB_433 },
    { REGFRFMID        ,RF_FRFMID_433 },
    { REGFRFLSB        ,RF_FRFLSB_433 },
    { REGOSC1          ,0x41 },
    { REGAFCCTRL       ,0x00 },
	{ RESERVED0C       ,0x02 },
    { REGLISTEN1       ,0xA2 },
    { REGLISTEN2       ,0xF5 },
    { REGLISTEN3       ,0x20 },
	{ REGVERSION       ,0x24 },
	/*! Transmitter Registers */
    { REGPALEVEL       ,0x9F },
    { REGPARAMP        ,0x09 },
    { REGOCP           ,0x1A },
    /*! Receiver Registers */
	{ RESERVED14       ,0x40 },
	{ RESERVED15       ,0xB0 },
	{ RESERVED16       ,0x7B },
	{ RESERVED17       ,0x9B },
    { REGLNA           ,0x88 },
    { REGRXBW          ,RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, //(BitRate < 2 * RxBw)
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
	/*! IRQ and Pin Mapping Registers */
    { REGDIOMAPPING1   ,0x00 },
    { REGDIOMAPPING2   ,0x07 },
    { REGIRQFLAGS1     ,0x80 },
    { REGIRQFLAGS2     ,0x00 },
    { REGRSSITHRESH    ,220 }, //must be set to dBm = (-Sensitivity / 2) - default is 0xE4=228 so -114dBm
    { REGRXTIMEOUT1    ,0x00 },
    { REGRXTIMEOUT2    ,0x00 },
    /*!Packet Engine Registers*/
    { REGPREAMBLEMSB   ,0x00 },
    { REGPREAMBLELSB   ,0x03 },
    { REGSYNCCONFIG    ,RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_1 | RF_SYNC_TOL_0 },

    { REGSYNCVALU1     ,0x01 },
    { REGSYNCVALU2     ,0x01 },
    { REGSYNCVALU3     ,0x01 },
    { REGSYNCVALU4     ,0x01 },
    { REGSYNCVALU5     ,0x01 },
    { REGSYNCVALU6     ,0x01 },
    { REGSYNCVALU7     ,0x01 },
    { REGSYNCVALU8     ,0x01 },
    { REGPACKETCONFIG1 ,/*RF_PACKET1_FORMAT_VARIABLE |*/ RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_OFF | RF_PACKET1_ADRSFILTERING_OFF },
    { REGPAYLOADLENGTH ,10u },
    { REGNODEADRS      ,0x01 },
    { REGBROADCASTADRS ,0x01 },
    { REGAUTOMODES     ,0x00 },
    { REGFIFOTHRESH    ,RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, //TX on FIFO not empty
    { REGPACKETCONFIG2 ,RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, //RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
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
    /*! Temperature Sensor Registers */
    { REGTEMP1         ,0x01 },
	{ REGTEMP2         ,0x00 },
	/*! Test Registers */
	{ REGTESTLNA       ,0x1B },
	{ REGTESTPA1       ,0x55 },
	{ REGTESTPA2       ,0x70 },
	{ REGTESTDAGC      ,RF_DAGC_IMPROVED_LOWBETA0 },
	{ REGTESTAFC       ,0x00 },
};




#define DEFAULT_CONFIGURATION_SIZE      ( 79u )

const RFM69_static_configuration_st RFM69_config_c[ 2 ] =
{
    { (RFM69_register_data_st * )RFM69_default_register_set, DEFAULT_CONFIGURATION_SIZE },
    { (RFM69_register_data_st * )RFM69_online_register_set,  DEFAULT_CONFIGURATION_SIZE },
};


#endif /* RFM69_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
