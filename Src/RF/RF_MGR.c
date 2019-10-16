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
#include "CLI.h"

#include "AIRCON.h"
#include "NRF24.h"
#include "RF_MGR.h"


STATIC RF_MGR_rf_data_store_st RF_MGR_rf_data_store_s;
STATIC RF_MGR_sed_data_st	   RF_MGR_sed_data_s;
STATIC RF_MGR_sed_data_st	   RF_MGR_controller_data_s;

STATIC RF_MGR_whitelist_st	   RF_MGR_whitelist_s;
STATIC disable_enable_et	   RF_MGR_dbg_out_s;


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
	u8_t i ;

    STDC_memset( &RF_MGR_rf_data_store_s, 0x00, sizeof( RF_MGR_rf_data_store_s ) );
    STDC_memset( &RF_MGR_sed_data_s, 0x00, sizeof( RF_MGR_sed_data_s ) );

    /* Copy the whitelist from NVM */
    for( i = 0; i < RF_MGR_RF_DATA_HANDLER_SIZE; i++ )
    {
    	STDC_memcpy( &RF_MGR_whitelist_s.id[i], &NVM_info_s.NVM_generic_data_blk_s.rf_whitelist[i], sizeof( RF_MGR_whitelist_s.id )/RF_MGR_RF_DATA_HANDLER_SIZE );

    	RF_MGR_rf_data_store_s.data_packet_s[i].watchdog_timeout = RF_WATCHDOG_TIMEOUT;
    }

    RF_MGR_whitelist_s.state = NVM_info_s.NVM_generic_data_blk_s.whitelist_state;
	RF_MGR_dbg_out_s = NVM_info_s.NVM_generic_data_blk_s.rf_dbg_out;

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
    RF_MGR_analyse_received_packets();
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
    u8_t sensor_type;
    u16_t sensor_id;
    u8_t node_index;

    /* Run through the RF data structure and action any packet that needs attention */
    for( node_index = 0u; node_index < RF_MGR_RF_DATA_HANDLER_SIZE; node_index++ )
    {
        if( RF_MGR_rf_data_store_s.data_packet_s[node_index].updated == TRUE )
        {
            RF_MGR_rf_data_store_s.data_packet_s[node_index].updated = FALSE;

            /* Reset the watchdog timeout for that node ID */
            RF_MGR_rf_data_store_s.data_packet_s[node_index].watchdog_timeout = RF_WATCHDOG_TIMEOUT;

            sensor_type = RF_MGR_rf_data_store_s.data_packet_s[node_index].sensor_type;
            sensor_id = RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id;

            switch( sensor_type )
            {
                case EARLY_PROTOTYPE_SED:
                {
                    RF_MGR_handle_early_prototype_sed( sensor_id, (u8_t*)&RF_MGR_rf_data_store_s.data_packet_s[node_index].payload,
                                                       RF_MGR_rf_data_store_s.data_packet_s[node_index ].packet_counter );
                }
                break;

                case EARLY_PROTOTYPE_CONTROLLER:
                {

                }
                break;

                default:
                	//assert(0);
                    break;
            }
        }
    }
}


/*!
****************************************************************************************************
*
*   \brief         Analyse the fault conditions
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_analyse_fault_conditions( void )
{
    /* Handle different sensor types */
	//false_true_et fault_present = FALSE;
    u8_t node_index;

    /* Run through the RF data structure and decrement the watchdog times */
    for( node_index = 0u; node_index < RF_MGR_RF_DATA_HANDLER_SIZE; node_index++ )
    {
        if( RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id != 0u )
        {
            /* Decrement the watchdog timer for that node ID */
        	if( RF_MGR_rf_data_store_s.data_packet_s[node_index].watchdog_timeout > 0u )
        	{
        		RF_MGR_rf_data_store_s.data_packet_s[node_index].watchdog_timeout --;
        	}
        }
    }


    /* Run through the RF data structure and check if the watchdog has timed out */
	for( node_index = 0u; node_index < RF_MGR_RF_DATA_HANDLER_SIZE; node_index++ )
	{
		if( RF_MGR_rf_data_store_s.data_packet_s[node_index].watchdog_timeout == 0u )
		{
			/* We have a problem here */
			RF_MGR_set_dtc_state( node_index, RF_MGR_MISSING_SENSOR, FAIL );
		}
		else
		{
			/* We dont have a problem, OR we had a problem that has now cleared */
			RF_MGR_set_dtc_state( node_index, RF_MGR_MISSING_SENSOR, PASS );
		}
	}
}




/*!
****************************************************************************************************
*
*   \brief         Handle a specific type of sensor
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_handle_early_prototype_sed( u16_t sensor_id, u8_t* data_p, u32_t packet_count )
{
	/* byte 0 - Sensor type - common
	 * byte 1 Node ID MSB   - common
	 * byte 2 Node ID LSB   - common
	 * byte 3 packet type   - 1st SED
	 * byte 4 mode type     - 1st SED
	 * byte 5 status        - 1st SED
	 * byte 6 temp 		    - 1st SED ( degree c )
	 * byte 7 temp		    - 1st SED ( degree c remainder )
	 */

    RF_MGR_sed_data_s.node_id     = sensor_id;
    RF_MGR_sed_data_s.packet_type = data_p[0];
    RF_MGR_sed_data_s.mode_type   = data_p[1];
    RF_MGR_sed_data_s.status      = data_p[2];
    RF_MGR_sed_data_s.temperature = STDC_make_16_bit( data_p[3], data_p[4] );
    RF_MGR_sed_data_s.packet_ctr  = packet_count;
    RF_MGR_sed_data_s.tx_interval_secs = STDC_make_16_bit( data_p[5], data_p[6] );

    AIRCON_set_oat( RF_MGR_sed_data_s.temperature / 10.0, RF_MGR_sed_data_s.node_id );

	if( RF_MGR_get_dbg_output_state() == ENABLE_ )
	{
		RF_MGR_display_sed_data();
	}
}






/*!
****************************************************************************************************
*
*   \brief         Handle a specific type of sensor
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_handle_early_prototype_controller( u16_t sensor_id, u8_t* data_p, u32_t packet_count )
{
	RF_MGR_controller_data_s.node_id     = sensor_id;
	RF_MGR_controller_data_s.packet_type = data_p[0];
	RF_MGR_controller_data_s.mode_type   = data_p[1];
	RF_MGR_controller_data_s.status      = data_p[2];

	if( RF_MGR_get_dbg_output_state() == ENABLE_ )
	{
    	RF_MGR_display_controller_data();
	}
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
    u8_t sensor_type;
    u8_t node_index;
    u16_t node_id;
    false_true_et id_allowed = TRUE;

    sensor_type = rf_data[0];
    node_id = STDC_make_16_bit( rf_data[1], rf_data[2]);

    if( RF_MGR_whitelist_s.state == ENABLE_ )
    {
    	if( RF_MGR_check_ID_in_whitelist( node_id ) == FALSE )
    	{
    		id_allowed = FALSE;
    	}
    }

    if( id_allowed == TRUE )
    {
		/* Check is there free space by looking for an id of 0 in the rf data struct */
		for( node_index = 0u; node_index < RF_MGR_RF_DATA_HANDLER_SIZE; node_index++ )
		{
			if( ( ( RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id == 0u ) ||
				  ( RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id == node_id ) ) )
			{
				/* Check for valid sensor type */
				if( sensor_type != 0u )
				{
					RF_MGR_rf_data_store_s.data_packet_s[node_index].sensor_type = rf_data[0];
					RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id     = node_id;

					/* Copy in the payload */
					STDC_memcpy( RF_MGR_rf_data_store_s.data_packet_s[node_index].payload, &rf_data[3], rf_data_size - 3 );

					/* increment the packet counter for the individual node id */
					if( RF_MGR_rf_data_store_s.data_packet_s[node_index].packet_counter < U16_T_MAX )
					{
						RF_MGR_rf_data_store_s.data_packet_s[node_index].packet_counter++;
					}
					else
					{
						RF_MGR_rf_data_store_s.data_packet_s[node_index].packet_counter = 0u;
					}

					/* Updated frame received */
					RF_MGR_rf_data_store_s.data_packet_s[node_index].updated = TRUE;
				}
				/* Break out as we have found a free space and no need to keep searching */
				break;
			}
		}
    }
}





/*!
****************************************************************************************************
*
*   \brief         Displays the received sensor data
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_display_sed_data( void )
{
	u8_t display_data[200];

	const char* sensor_type[6] =
	{
		"1",
		"2",
		"3",
		"4",
		"5",
		"1st Gen Sleepy Sensor Device"
	};

	const char* mode_type[2] =
	{
		"NORMAL MODE",
		"DEBUG MODE",
	};

	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	CLI_send_newline();
	sprintf( display_data, "**------------------------------------------------**");
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	CLI_send_newline();
	sprintf( display_data, "Sensor ID:\t0x%04X\r\n",
			RF_MGR_sed_data_s.node_id );
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	sprintf( display_data, "Sensor Type: \t%s\r\n", sensor_type[ RF_MGR_sed_data_s.packet_type ]);
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	sprintf( display_data, "Mode Type:\t%s\r\n", mode_type[ RF_MGR_sed_data_s.mode_type ]);
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	sprintf( display_data, "Status 1:\t0x%02X\r\nStatus 2:\t0x%02X\r\n", 0, 0 );
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	/* The temperature needs to be divided by 10 */
	s16_t temp_whole = ( RF_MGR_sed_data_s.temperature/10 );
	u8_t remainder = ( abs( RF_MGR_sed_data_s.temperature ) - ( abs(temp_whole) * 10 ) );

	sprintf( display_data, "Packet ctr:\t%d\r\nTemperature:\t%d.%d degree c\r\nTX rate secs:\t%d",
			 RF_MGR_sed_data_s.packet_ctr, temp_whole, remainder, RF_MGR_sed_data_s.tx_interval_secs  );
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );
	CLI_send_newline();
	CLI_send_newline();
}



/*!
****************************************************************************************************
*
*   \brief         Displays the received sensor data
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_display_controller_data( void )
{
}



/*!
****************************************************************************************************
*
*   \brief         Removes a specific node ID from the whitelist
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
pass_fail_et RF_MGR_remove_wl_node( u16_t node_id )
{
	pass_fail_et returnType = FAIL;
	false_true_et id_found = FALSE;
	u8_t pos;

	/* Check if the ID is actually in the list */
	for( pos = 0; pos < RF_MGR_RF_DATA_HANDLER_SIZE; pos++ )
	{
		if( node_id == RF_MGR_whitelist_s.id[pos] )
		{
			if( node_id != 0x0000 )
			{
				/* We have found the pos of the ID in question */
				id_found = TRUE;
				break;
			}
		}
	}

	if( id_found == TRUE )
	{
		STDC_memset( &RF_MGR_whitelist_s.id[pos], 0x00, ( sizeof( RF_MGR_whitelist_s.id )/ RF_MGR_RF_DATA_HANDLER_SIZE ) );

		/* Now that we have removed that ID and its 0, we want to shift the IDs that are above it
		 * in the array down to fill the empty spot */

		for( pos = pos; pos < RF_MGR_RF_DATA_HANDLER_SIZE; pos++ )
		{
			if( RF_MGR_whitelist_s.id[pos + 1] != 0u )
			{
				STDC_memcpy( &RF_MGR_whitelist_s.id[pos],
							&RF_MGR_whitelist_s.id[pos + 1], sizeof( RF_MGR_whitelist_s.id[pos] ) );
			}
			else
			{
				STDC_memset( &RF_MGR_whitelist_s.id[pos], 0x00, sizeof( RF_MGR_whitelist_s.id[pos] ) );
				break;
			}
		}
		returnType = PASS;
	}

	return ( returnType );
}



/*!
****************************************************************************************************
*
*   \brief         Adds a specific ID to the whitelist
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
pass_fail_et RF_MGR_add_wl_node( u16_t id )
{
	pass_fail_et returnType = FAIL;
	u8_t pos = 0u;

	/* first of all check that the pos is valid */
	for( pos = 0u; pos < RF_MGR_RF_DATA_HANDLER_SIZE; pos++ )
	{
		if( RF_MGR_whitelist_s.id[pos] == id )
		{
			returnType = FAIL;
			break;
		}
		if( RF_MGR_whitelist_s.id[pos] == 0u )
		{
			/* We have found a free position */
			RF_MGR_whitelist_s.id[pos] = id;
			returnType = PASS;
			break;
		}
	}
	return ( returnType );
}



/*!
****************************************************************************************************
*
*   \brief         Check that an ID is actually in the Whitelist
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
false_true_et RF_MGR_check_ID_in_whitelist( u16_t id )
{
	u8_t i;
	false_true_et id_found = FALSE;

	for( i = 0; i < RF_MGR_RF_DATA_HANDLER_SIZE; i++ )
	{
		if( id == RF_MGR_whitelist_s.id[i] )
		{
			id_found = TRUE;
			break;
		}
	}
	return( id_found );
}



RF_MGR_rf_data_store_st* RF_MGR_get_all_decoded_IDs_address( void )
{
	return( &RF_MGR_rf_data_store_s );
}


/*!
****************************************************************************************************
*
*   \brief         Gets the address of the whitelist
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
RF_MGR_whitelist_st* RF_MGR_get_whitelist_address( void )
{
	return( &RF_MGR_whitelist_s );
}


/*!
****************************************************************************************************
*
*   \brief         Turn the whitelist on and off
*
*   \author        MS
*
*   \return        none
*
*   \note
*
***************************************************************************************************/
void RF_MGR_set_whitelist_state( disable_enable_et state )
{
	RF_MGR_whitelist_s.state = state;
}

disable_enable_et RF_MGR_toggle_dbg_output_state( void )
{
	if( RF_MGR_get_dbg_output_state() == ENABLE_ )
	{
		RF_MGR_set_dbg_output_state( DISABLE_ );
	}
	else
	{
		RF_MGR_set_dbg_output_state( ENABLE_ );
	}
	return( RF_MGR_dbg_out_s );
}


void RF_MGR_set_dbg_output_state( disable_enable_et state )
{
	RF_MGR_dbg_out_s = state;
	NVM_info_s.NVM_generic_data_blk_s.rf_dbg_out = RF_MGR_dbg_out_s;
}

disable_enable_et RF_MGR_get_dbg_output_state( void )
{
	return ( RF_MGR_dbg_out_s );
}



void RF_MGR_set_dtc_state( u16_t node_index, RF_MGR_generic_dtc_et dtc, pass_fail_et state )
{
	if( state == PASS )
	{
		RF_MGR_rf_data_store_s.data_packet_s[node_index].generic_dtc_mask &= ~( 1u << dtc );
	}
	else
	{
		RF_MGR_rf_data_store_s.data_packet_s[node_index].generic_dtc_mask |= ( 1u << dtc );
	}
}


/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/


/****************************** END OF FILE *******************************************************/
