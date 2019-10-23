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

#define QUEUE_SIZE                      5u
#define RF_WATCHDOG_TIMEOUT             ( 60u * 60u )    //1hr
#define PAYLOAD_START_POS				2u


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
    u16_t master_id;
    u8_t  payload[32];
    u32_t packet_counter;
    false_true_et updated;
} RF_MGR_data_packet_st;


typedef struct
{
	RF_MGR_data_packet_st data_packet_s[QUEUE_SIZE];
} RF_MGR_rf_data_store_st;




typedef struct
{
	u16_t master_id;
	u8_t  action_req;
	u8_t  mode_req;
	u16_t packet_ctr;
} RF_MGR_controller_data_st;




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
void 					 RF_MGR_analyse_fault_conditions( void );



#endif /* RF_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/
