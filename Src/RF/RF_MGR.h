/*! \file
*
*               $Author: mstewart $
*
*   \brief      Public header file for RF module.
*/
#ifndef RF_MGR_H
#define RF_MGR_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

#define RF_MGR_RF_DATA_HANDLER_SIZE  40u
#define EARLY_PROTOTYPE_SED          1u
#define EARLY_PROTOTYPE_CONTROLLER   2u

#define RF_WATCHDOG_TIMEOUT			 ( 60u * 60u )	//1hr


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
typedef enum
{
	RF_MGR_MISSING_SENSOR = 0u

} RF_MGR_generic_dtc_et;

typedef enum 
{
	RF_MGR_TX = 0u,
	RF_MGR_RX
} RF_MGR_rf_state_et;

typedef struct
{
    u8_t  sensor_type;
    u16_t node_id;
    u8_t  payload[32];
    u32_t packet_counter;
    false_true_et updated;
    u16_t watchdog_timeout;
    u16_t generic_dtc_mask;
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
	s16_t temperature;
	u16_t pressure;
	u16_t tx_interval_secs;
} RF_MGR_sed_data_st;


typedef struct
{
	u16_t node_id;
	u8_t  packet_type;
	u8_t  mode_type;
	u16_t packet_ctr;
	u8_t  status;
} RF_MGR_controller_data_st;


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
void                     RF_MGR_set_state( RF_MGR_rf_state_et state );
RF_MGR_rf_state_et       RF_MGR_get_state( void );
void                 	 RF_MGR_analyse_received_packets( void );
void                 	 RF_MGR_handle_early_prototype_sed( u16_t sensor_id, u8_t* data_p, u32_t packet_count );
RF_MGR_rf_data_store_st* RF_MGR_get_all_decoded_IDs_address( void );
pass_fail_et 		 	 RF_MGR_remove_wl_node( u16_t node_id );
pass_fail_et 		 	 RF_MGR_add_wl_node( u16_t node_id );
void 				 	 RF_MGR_display_sed_data( void );
RF_MGR_whitelist_st* 	 RF_MGR_get_whitelist_address( void );
false_true_et 		 	 RF_MGR_check_ID_in_whitelist( u16_t id );
void 				 	 RF_MGR_set_whitelist_state( disable_enable_et state );
void 					 RF_MGR_analyse_fault_conditions( void );
void				     RF_MGR_set_dtc_state( u16_t node_index, RF_MGR_generic_dtc_et dtc, pass_fail_et state );
void				     RF_MGR_set_dbg_output_state( disable_enable_et state );
disable_enable_et 		 RF_MGR_get_dbg_output_state( void );
disable_enable_et 		 RF_MGR_toggle_dbg_output_state( void );

void 					 RF_MGR_display_controller_data( void );
void 					 RF_MGR_handle_early_prototype_controller( u16_t sensor_id, u8_t* data_p, u32_t packet_count );


#endif /* RF_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
