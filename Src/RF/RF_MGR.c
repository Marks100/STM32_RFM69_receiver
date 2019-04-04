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

#include "C_defs.h"
#include "STDC.h"
#include "COMPILER_defs.h"
#include "HAL_BRD.h"
#include "HAL_SPI.h"
#include "NVM.h"
#include "CLI.h"
#include "main.h"
#include "NRF24.h"
#include "RF_MGR.h"


STATIC RF_MGR_rf_data_store_st RF_MGR_rf_data_store_s;
STATIC RF_MGR_sed_data_st	   RF_MGR_sed_data_s;
STATIC RF_MGR_whitelist_st	   RF_MGR_whitelist_s;

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
    	STDC_memcpy( &RF_MGR_whitelist_s.id[i], &NVM_info_s.NVM_generic_data_blk_s.rf_whitelist[i], sizeof( RF_MGR_whitelist_s.id ) );
    }

    RF_MGR_whitelist_s.state = NVM_info_s.NVM_generic_data_blk_s.whitelist_state;

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

            sensor_type = RF_MGR_rf_data_store_s.data_packet_s[node_index].sensor_type;
            sensor_id = RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id;

            switch( sensor_type )
            {
                case EARLY_PROTOTYPE_SED:
                    RF_MGR_handle_early_prototype_sed( sensor_id, (u8_t*)&RF_MGR_rf_data_store_s.data_packet_s[node_index].payload,
                                                       RF_MGR_rf_data_store_s.data_packet_s[node_index ].packet_counter );
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

    RF_MGR_sed_data_s.node_id     = sensor_id;

    RF_MGR_sed_data_s.packet_type = data_p[0];
    RF_MGR_sed_data_s.mode_type   = data_p[1];
    RF_MGR_sed_data_s.status      = data_p[2];
    RF_MGR_sed_data_s.temperature  = ( data_p[3] << 8u );
    RF_MGR_sed_data_s.temperature |= ( data_p[4] );
    RF_MGR_sed_data_s.packet_ctr  = packet_count;
    RF_MGR_sed_data_s.pressure    = data_p[6];

    RF_MGR_display_sed_data();
}


/* byte 0 - Sensor type - common
 * byte 1 Node ID MSB   - common
 * byte 2 Node ID LSB   - common
 * byte 3 packet type   - 1st SED
 * byte 4 mode type     - 1st SED
 * byte 5 status        - 1st SED
 * byte 6 temp 		    - 1st SED ( degree c )
 * byte 7 temp		    - 1st SED ( degree c remainder )
 */


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
    node_id = ( rf_data[1] << 8u | rf_data[2] ) ;

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
					RF_MGR_rf_data_store_s.data_packet_s[node_index].node_id     = ( rf_data[1] << 8u | rf_data[2] ) ;

					/* Copy in the payload */
					STDC_memcpy( RF_MGR_rf_data_store_s.data_packet_s[node_index].payload, &rf_data[3], rf_data_size - 3 );

					/* increment the packet counter for the individual node id */
					RF_MGR_rf_data_store_s.data_packet_s[node_index].packet_counter++;

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

	STDC_memset( display_data, 0x20, sizeof( display_data ) );

	CLI_send_newline();
	sprintf( display_data, "**------------------------------------------------**");
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	CLI_send_newline();
	sprintf( display_data, "Sensor ID:\t0x%04X\r\n",
			RF_MGR_sed_data_s.node_id );
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	switch( RF_MGR_sed_data_s.packet_type )
	{
		case 0:
			sprintf( display_data, "Sensor Type: \t1\r\n");
			break;

		case 1:
			sprintf( display_data, "Sensor Type: \t2\r\n");
			break;

		case 2:
			sprintf( display_data, "Sensor Type: \t3\r\n");
			break;

		case 3:
			sprintf( display_data, "Sensor Type: \t4\r\n");
			break;

		case 4:
			sprintf( display_data, "Sensor Type: \t5\r\n");
			break;

		default:
			sprintf( display_data, "Sensor Type: \t1st Gen Sleepy Sensor Device\r\n");
			break;
	}

	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	switch( RF_MGR_sed_data_s.mode_type )
	{
		case 0:
			sprintf( display_data, "Mode Type:\tNORMAL MODE\r\n");
			break;

		case 1:
			sprintf( display_data, "Mode Type:\tDEBUG MODE\r\n");
			break;

		default:
			sprintf( display_data, "Mode Type:\tNORMAL MODE\r\n");
			break;
	}
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	sprintf( display_data, "Status 1:\t0x%02X\r\nStatus 2:\t0x%02X\r\n", 0, 0 );
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );

	/* The temperature needs to be divided by 10 */
	s16_t temp_whole = ( RF_MGR_sed_data_s.temperature/10 );
	u8_t remainder = ( abs( RF_MGR_sed_data_s.temperature ) - ( abs(temp_whole) * 10 ) );


	sprintf( display_data, "Packet ctr:\t%d\r\nTemperature:\t%d.%d degree c\r\n",
			 RF_MGR_sed_data_s.packet_ctr, temp_whole, remainder );
	CLI_send_data( display_data, strlen( display_data ) );
	STDC_memset( display_data, 0x00, sizeof( display_data ) );
	CLI_send_newline();
	CLI_send_newline();
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
			/* We have found the pos of the ID in question */
			id_found = TRUE;
			break;
		}
	}

	if( id_found == TRUE )
	{
		STDC_memset( &RF_MGR_whitelist_s.id[pos], 0x00, sizeof( RF_MGR_whitelist_s.id ) );

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

/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/


/****************************** END OF FILE *******************************************************/
