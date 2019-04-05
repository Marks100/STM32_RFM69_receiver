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
#ifndef RF_MGR_H
#define RF_MGR_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

#define RF_MGR_RF_DATA_HANDLER_SIZE  40u
#define EARLY_PROTOTYPE_SED          1u


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/

typedef struct
{
    u8_t  sensor_type;
    u16_t node_id;
    u8_t  payload[32];
    u32_t packet_counter;
    false_true_et updated;
} data_packet_st;


typedef struct
{
    data_packet_st data_packet_s[RF_MGR_RF_DATA_HANDLER_SIZE];
} RF_MGR_rf_data_store_st;


typedef struct
{
	u16_t node_id;
	u8_t  packet_type;
	u8_t  mode_type;
	u16_t packet_ctr;
	u8_t  status;
	s16_t  temperature;
	u16_t pressure;
	u16_t tx_interval_secs;
} RF_MGR_sed_data_st;


typedef struct
{
	u16_t id[RF_MGR_RF_DATA_HANDLER_SIZE];
	disable_enable_et state;
} RF_MGR_whitelist_st;

/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void 					 RF_MGR_init( void );
void                 	 RF_MGR_packet_received_event( u8_t* rf_data, u8_t rf_data_size );
void                 	 RF_MGR_tick( void );
void                 	 RF_MGR_analyse_received_packets( void );
void                 	 RF_MGR_handle_early_prototype_sed( u16_t sensor_id, u8_t* data_p, u32_t packet_count );
RF_MGR_rf_data_store_st* RF_MGR_get_all_decoded_IDs_address( void );
pass_fail_et 		 	 RF_MGR_remove_wl_node( u16_t node_id );
pass_fail_et 		 	 RF_MGR_add_wl_node( u16_t node_id );
void 				 	 RF_MGR_display_sed_data( void );
RF_MGR_whitelist_st* 	 RF_MGR_get_whitelist_address( void );
false_true_et 		 	 RF_MGR_check_ID_in_whitelist( u16_t id );
void 				 	 RF_MGR_set_whitelist_state( disable_enable_et state );


#endif /* RF_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
