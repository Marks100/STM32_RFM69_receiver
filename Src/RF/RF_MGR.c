/*! \file
*               Author: mstewart
*   \brief      RF_MGR module
*/
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/

#ifndef UNIT_TEST
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"

#include "STDC.h"
#include "HAL_BRD.h"
#include "NVM.h"
#include "NRF24.h"
#include "RF_MGR.h"


STATIC RF_MGR_rf_data_store_st RF_MGR_rf_data_store_s;
STATIC RF_MGR_rf_state_et      RF_MGR_rf_state_s;
STATIC u16_t                   RF_MGR_tick_ctr_s;


/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/
/*!
****************************************************************************************************
*
*   \brief         Module (re-)initialisation function
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_init( void )
{

	RF_MGR_rf_state_s = RF_MGR_RX;
	RF_MGR_tick_ctr_s = 0u;

	STDC_memset( &RF_MGR_rf_data_store_s, 0x00, sizeof( RF_MGR_rf_data_store_st ) );

    /* Initialise the NRF24 specific variables */
    NRF24_init();
}



/*!
****************************************************************************************************
*
*   \brief         Module tick
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_tick( void )
{
	switch( RF_MGR_rf_state_s )
	{
		case RF_MGR_TX:
			RF_MGR_setup_tx_event();
			RF_MGR_set_state(RF_MGR_RX);
		break;

		case RF_MGR_RX:
			RF_MGR_analyse_received_packets();
		break;

		default:
		break;
	}

	if( RF_MGR_tick_ctr_s < 10 )
	{
		RF_MGR_tick_ctr_s ++;
	}
	else
	{
		RF_MGR_tick_ctr_s = 0u;
		RF_MGR_set_state(RF_MGR_TX);
	}
}



void RF_MGR_set_state( RF_MGR_rf_state_et state )
{
	RF_MGR_rf_state_s = state;

	if( RF_MGR_get_state() == RF_MGR_TX )
	{
		NRF24_set_state( NRF24_SETUP_TX );
	}
	else
	{
		NRF24_set_state( NRF24_SETUP_RX );
	}
}



RF_MGR_rf_state_et RF_MGR_get_state( void )
{
	return( RF_MGR_rf_state_s );
}



/*!
****************************************************************************************************
*
*   \brief         Analyse the received RF packets
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_analyse_received_packets( void )
{
    /* Handle different sensor types */
    u8_t action_req;
    u8_t q_index;
    u8_t mode_req;

    /* Run through the RF data structure and action any packet that needs attention */
    for( q_index = 0u; q_index < QUEUE_SIZE; q_index++ )
    {
        if( RF_MGR_rf_data_store_s.data_packet_s[q_index].updated == TRUE )
        {
            RF_MGR_rf_data_store_s.data_packet_s[q_index].updated = FALSE;

            action_req = RF_MGR_rf_data_store_s.data_packet_s[q_index].payload[0];
            mode_req = RF_MGR_rf_data_store_s.data_packet_s[q_index].payload[1];

            switch( mode_req )
            {
				case 1 ... 5:
					switch( action_req )
					{
						case 1 ... 5:
						 HAL_BRD_TOGGLE_RELAY();
						break;

						default:
							break;
					}
				break;

				default:
					break;
            }
        }
    }
}





/*!
****************************************************************************************************
*
*   \brief         This sets up the next tx event
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_setup_tx_event( void )
{
    
}









/*!
****************************************************************************************************
*
*   \brief         This gets called when the underlying RF driver receives a valid packet
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_packet_received_event( u8_t* rf_data, u8_t rf_data_size )
{
    u8_t  q_index;
    u16_t master_id;

	master_id = STDC_make_16_bit( rf_data[0], rf_data[1] );

	/* Check is there free space by looking for an id of 0 in the rf data struct */
	for( q_index = 0u; q_index < QUEUE_SIZE; q_index++ )
	{
		if( RF_MGR_rf_data_store_s.data_packet_s[q_index].updated == FALSE )
		{
			if( ( ( RF_MGR_rf_data_store_s.data_packet_s[q_index].master_id == 0u ) ||
				( RF_MGR_rf_data_store_s.data_packet_s[q_index].master_id == master_id ) ) )
			{
					RF_MGR_rf_data_store_s.data_packet_s[q_index].master_id = master_id;

					/* Copy in the payload */
					STDC_memcpy( RF_MGR_rf_data_store_s.data_packet_s[q_index].payload, &rf_data[PAYLOAD_START_POS],
							     rf_data_size - PAYLOAD_START_POS );

					/* increment the packet counter for the individual node id */
					if( RF_MGR_rf_data_store_s.data_packet_s[q_index].packet_counter < U16_T_MAX )
					{
						RF_MGR_rf_data_store_s.data_packet_s[q_index].packet_counter++;
					}
					else
					{
						RF_MGR_rf_data_store_s.data_packet_s[q_index].packet_counter = 0u;
					}

					/* Updated frame received */
					RF_MGR_rf_data_store_s.data_packet_s[q_index].updated = TRUE;
			}
			break;
		}
	}
}







void RF_MGR_set_dtc_state( u16_t node_index, RF_MGR_generic_dtc_et dtc, pass_fail_et state )
{
	if( state == PASS )
	{
		//RF_MGR_rf_data_store_s.data_packet_s[node_index].generic_dtc_mask &= ~( 1u << dtc );
	}
	else
	{
		//RF_MGR_rf_data_store_s.data_packet_s[node_index].generic_dtc_mask |= ( 1u << dtc );
	}
}


/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/


/****************************** END OF FILE *******************************************************/
