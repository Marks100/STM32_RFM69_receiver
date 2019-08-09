/*! \file
*               $Revision: 16923 $
*
*               $Author: mstewart $
*
*               $Date: 2014-01-16 15:40:40 +0000 (Thu, 16 Jan 2014) $
*
*               $HeadURL:
*
*   \brief      RF module
*/
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/

#include "stm32f10x_usart.h"
#include "misc.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "STDC.h"
#include "NVM.h"
#include "NRF24.h"
#include "HAL_BRD.h"


#include "HAL_UART.h"
#include "CLI.h"

extern NVM_info_st        NVM_info_s;

/* Debug display strings */
const char CLI_welcome_message_s[] =
        "\r\n\n\rHello...\r\nCLI Console is now open, Type 'help' for list of commands";

const char CLI_closing_serial_port_s[] =
        "\r\nClosing serial port due to inactivity to save power, please press the \"Enter\" key to re-open the debug prompt";

const char CLI_newline[] =
        "\r\n";

const char* CLI_err_string[ CLI_ERROR_MAX ] =
{
    "\r",
    "\r",
    "\r\nOperation failed\r\n",
    "\r\nInvalid number of arguments\r\n",
    "\r\nInvalid arguments\r\n",
    "\r\nCommand not supported\r\n",
    "\r\nCommand prohibited in this mode\r\n",
    "\r\nCommand not found\r\n",
    "\r\nOut of range\r\n",
    "\r\nInvalid Hex param\r\n",
    "\r\nInvalid Dec param\r\n",
    "\r\nInvalid hex param length\r\n",
    "\r\nInvalid Dec param length\r\n",
    "\r\nValue is already set\r\n",
    "\r\nInvalid string input\r\n"
};

STATIC u8_t               CLI_byte_index_s;
STATIC u8_t               CLI_rx_command_len;
STATIC false_true_et      CLI_cr_received_s;
STATIC char               CLI_msg_read_s[CLI_MAX_INPUT_CHARS + 1];
STATIC CLI_cmd_history_st CLI_cmd_history_s;


STATIC const CLI_Command_st CLI_commands[] =
{
	 {"help",		  &help_handler,		HELP_HELP, 				SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"ver",		  &ver_handler,			HELP_VER , 				SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"setmode",	  &setmode_handler,	    HELP_SET_MODE, 			SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"reset",		  &reset_handler,	    HELP_RESET, 			SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"setid",	      &setid_handler,	    HELP_SET_ID, 	        SUPPORTED_FOR_ALL_MODES, ENABLE, 1, SET_ID_CMD_PARAM_LIST },
	 {"nvm",		  &nvm_handler,			HELP_NVM,				SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"clocks",		  &clocks_handler,		HELP_CLOCKS,			SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"temp",		  &temp_handler,		HELP_TEMP,			    SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"batt",		  &batt_handler,		HELP_BATT,			    SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"listnodes",    &listnodes_handler,	HELP_LISTNODES,		    SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"nodestatus",   &nodestat_handler,	HELP_NODESTATS,		    SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {"led",   		  &led_handler,         HELP_LED,               SUPPORTED_FOR_ALL_MODES, ENABLE, 2, LED_CMD_PARAM_LIST  },
	 {"wladd", 		  &wl_add_handler,      HELP_WL_ADD, 			SUPPORTED_FOR_ALL_MODES, ENABLE, 1, WL_ADD_CMD_PARAM_LIST  },
	 {"wlremove",     &wl_remove_handler,	HELP_WL_REM,			SUPPORTED_FOR_ALL_MODES, ENABLE, 1, WL_REMOVE_CMD_PARAM_LIST },
	 {"wldisplay",    &wl_display_handler,	HELP_WL_DISPLAY,	    SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST  },
	 {"test",    	  &test_handler,	    HELP_TEST,	            SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST  },
	 {"wlstate",      &wl_state_handler,	HELP_WL_STATE,	        SUPPORTED_FOR_ALL_MODES, ENABLE, 1, WL_STATE_CMD_PARAM_LIST },
	 {"savenvm",      &savenvm_handler,	    HELP_SAVEMEVM,	        SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST  },
	 {"acstate",      &ac_state_handler,	HELP_AC_STATE,	        SUPPORTED_FOR_ALL_MODES, ENABLE, 1, AC_STATE_CMD_PARAM_LIST },
	 {"acmode",       &ac_mode_handler,	    HELP_AC_MODE,	        SUPPORTED_FOR_ALL_MODES, ENABLE, 1, AC_MODE_CMD_PARAM_LIST },
	 {"`",			  &rf_dbg_out_handler,  HELP_RF_DEBUG, 			SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST },
	 {NULL,			  NULL,					NULL,					SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST  }
};


void CLI_init( void )
{
	u8_t i = 0u;

    CLI_byte_index_s = 0u;
    CLI_rx_command_len = 0u;
    CLI_cr_received_s = 0u;

    STDC_memset( CLI_msg_read_s, 0x20, sizeof( CLI_msg_read_s ) );

    /* Reload the command history :) */
    for( i = 0u; i < CLI_MAX_COMMAND_HISTORY; i++ )
	{
		STDC_memcpy( &CLI_cmd_history_s.cmd_list[i], &NVM_info_s.NVM_generic_data_blk_s.cmd_list[i], sizeof( CLI_cmd_history_s.cmd_list ) );
	}

    /* Init the HAL UART driver */
    HAL_UART_init();

    /* Print a welcome message */
    CLI_print_welcome_message();

    CLI_print_prompt( TRUE );
}


void CLI_message_handler( void )
{
    if( CLI_carriage_return_check() == TRUE )
    {
        /* Reset the carriage return flag */
        CLI_cr_received_s = FALSE;

        /* handle the actual received command */
        CLI_handle_serial_command();

        /* Clear out the buffers */
        CLI_clear_rx_buffer();

        CLI_print_prompt( TRUE );
    }
}


void CLI_handle_serial_command ( void )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char *argVector[ CLI_CMD_LINE_ARGS_MAX ];
	u8_t argument_count = 0u;

	char  output_string[100];
	STDC_memset( output_string, 0, sizeof( output_string ) );

	error = CLI_parse_cmd( CLI_msg_read_s, &argument_count, argVector, CLI_CMD_LINE_ARGS_MAX );

	if( error == CLI_ERROR_NONE )
	{
		error = CLI_process_cmd( argument_count, argVector );
	}

	strcpy( output_string, CLI_err_string[error] );

	CLI_send_data( output_string, strlen(output_string) );
}

/*!
****************************************************************************************************
*
*   \brief         Check each charachter received
*
*   \author        MS
*
*   \return        void
*
***************************************************************************************************/

void CLI_handle_received_char( u8_t received_char )
{
    u8_t useable_commands = 0u;
    false_true_et command_present = FALSE;
    u8_t i = 0;

    if( ( ( received_char >= CLI_SPACE ) && ( received_char <= CLI_DEL ) ) || ( received_char <= CLI_CR ) )
    {
		/* count the number of non 0 commands */
		for( i = 0u; i < CLI_MAX_COMMAND_HISTORY; i++ )
		{
			if( CLI_cmd_history_s.cmd_list[i].cmd[0] != 0x00 )
			{
				useable_commands += 1u;
			}
		}

        if ( received_char == '\r')
        {
            CLI_rx_command_len = CLI_byte_index_s;
            CLI_byte_index_s = 0u;

            /* We have received a Carriage Return, so lets set this flag to let the system know a
            command has been received */
            CLI_cr_received_s = TRUE;
            CLI_send_data( "\n", sizeof(received_char) );

            if( CLI_rx_command_len != 0u )
            {
            	/* Dont add the new command if its already on the current list */
				for( i = 0u; i < useable_commands; i++ )
				{
					if( STDC_memcompare( CLI_cmd_history_s.cmd_list[i].cmd, CLI_msg_read_s, CLI_rx_command_len ) )
					{
						command_present = TRUE;
						break;
					}
				}

				if( command_present == FALSE )
				{
	            	if( useable_commands != CLI_MAX_COMMAND_HISTORY)
					{
						 /* Setup the index for the first command entered by the user, as the history is not full */
						 CLI_cmd_history_s.index_to_next_cmd = ( useable_commands  % CLI_MAX_COMMAND_HISTORY );
					}

					STDC_memset( &CLI_cmd_history_s.cmd_list[CLI_cmd_history_s.index_to_next_cmd].cmd, 0x00, sizeof( CLI_cmd_history_s.cmd_list[CLI_cmd_history_s.index_to_next_cmd].cmd ) );

					STDC_memcpy( &CLI_cmd_history_s.cmd_list[CLI_cmd_history_s.index_to_next_cmd].cmd, CLI_msg_read_s, CLI_rx_command_len );

					CLI_cmd_history_s.index_to_next_cmd = ( CLI_cmd_history_s.index_to_next_cmd + 1u )%CLI_MAX_COMMAND_HISTORY;
				}
            }

            CLI_store_history();
        }
        else if( received_char == CLI_DEL )
        {
            CLI_byte_index_s = 0u;
            STDC_memset( CLI_msg_read_s, 0x00, sizeof( CLI_msg_read_s ) );
            CLI_msg_read_s[ CLI_byte_index_s ] = 0u;
            CLI_send_data( (u8_t*)ERASE_CURRENT_LINE, strlen(ERASE_CURRENT_LINE));
            CLI_print_prompt( FALSE );
        }
        else if ( received_char == CLI_BACKSPACE )
        {
        	if( CLI_byte_index_s > 0u )
        	{
				CLI_byte_index_s--;
				CLI_msg_read_s[ CLI_byte_index_s ] = 0x00;
				CLI_send_data( "\b ", strlen("\b ") );
				CLI_send_data( &received_char, 1u );
        	}

            return;
        }
        else if( ( received_char == UP_ARROW || received_char == DOWN_ARROW ) && ( CLI_msg_read_s[ CLI_byte_index_s - 1u ] == '[' ) )
        {
            //received 'A' and may be we had received a '[' previously which means a up arrow has been hit.
            //load the previous command and print it and wait for the carriage return
            CLI_msg_read_s[ CLI_byte_index_s++ ] = received_char;

            /* If there are no useable commands then dont allow the user to scroll up or down */
            if( useable_commands != 0u )
            {
				if( received_char == UP_ARROW )
				{
					if( CLI_cmd_history_s.index_to_next_cmd == 0u )
					{
						CLI_cmd_history_s.index_to_next_cmd = ( useable_commands - 1u );
					}
					else
					{
						CLI_cmd_history_s.index_to_next_cmd -= 1u;
					}
				}
				else
				{
					CLI_cmd_history_s.index_to_next_cmd = ( ( CLI_cmd_history_s.index_to_next_cmd + 1u ) % useable_commands );
				}

				CLI_store_history();

				STDC_memset( CLI_msg_read_s, 0x00, sizeof( CLI_msg_read_s ) );
				strcpy( CLI_msg_read_s, CLI_cmd_history_s.cmd_list[ CLI_cmd_history_s.index_to_next_cmd ].cmd);

				CLI_byte_index_s = strlen( CLI_cmd_history_s.cmd_list[ CLI_cmd_history_s.index_to_next_cmd ].cmd );

				CLI_send_data( (u8_t*)ERASE_CURRENT_LINE, strlen(ERASE_CURRENT_LINE) );

				CLI_print_prompt( FALSE );

				CLI_send_data( CLI_msg_read_s, strlen( CLI_msg_read_s ) );
            }
            return;
        }
        else if( ( received_char == LEFT_ARROW || received_char == RIGHT_ARROW ) && ( CLI_msg_read_s[ CLI_byte_index_s - 1u ] == '[' ) )
        {
        	/* handle the left and right arrow keys aswell, but for now just ignore them */
        	return;
        }
        else if( received_char == '[' )
        {
            CLI_msg_read_s[ CLI_byte_index_s++ ] = received_char;
            return;
        }
        else
        {
            if( CLI_byte_index_s < CLI_MAX_INPUT_CHARS )
            {
            	/* Record every byte that gets received */
            	CLI_msg_read_s[ CLI_byte_index_s++ ] = received_char;
            	CLI_send_data( &received_char, sizeof(received_char) );
            }
        }
    }
}

void CLI_clear_rx_buffer( void )
{
     STDC_memset( CLI_msg_read_s, 0x00, sizeof( CLI_msg_read_s ) );
     CLI_byte_index_s = 0u;
     CLI_rx_command_len = 0u;
}


false_true_et CLI_carriage_return_check( void )
{
    false_true_et return_type = FALSE;

    if( CLI_cr_received_s == TRUE )
    {
        return_type = TRUE;
    }
    return ( return_type );
}



false_true_et CLI_is_space_or_newLine( char c )
{
    false_true_et return_type = FALSE;

    if( (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n') )
    {
        return_type = TRUE;
    }
    return ( return_type );
}


false_true_et CLI_is_hex_char( char c )
{
    false_true_et return_type = FALSE;

    if( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') )
    {
        return_type = TRUE;
    }

    return ( return_type );
}

false_true_et CLI_is_dec_char( char c )
{
    false_true_et return_type = FALSE;

    if( c >= '0' && c <= '9' )
    {
        return_type = TRUE;
    }
    return ( return_type );
}


unsigned long long CLI_str_to_hex( const char* str )
{
    return (unsigned long long) strtoull(str, 0, 16);
}

void CLI_send_newline( void )
{
    CLI_send_data( (char*)CLI_newline, sizeof( CLI_newline ) );
}

void CLI_print_welcome_message( void )
{
    CLI_send_data( (char*)CLI_welcome_message_s, sizeof( CLI_welcome_message_s ) );
}

void CLI_print_prompt( false_true_et newline )
{
    char prompt_string[100u];

    STDC_memset( prompt_string, 0x20, sizeof( prompt_string ) );

    if( newline == TRUE )
    {
    	CLI_send_newline();
    	CLI_send_newline();
    }
    sprintf(  prompt_string, "CLI PROMPT>" );

    CLI_send_data( (u8_t*)prompt_string, strlen( prompt_string ) );
}


void CLI_send_data( char* data, u16_t data_size )
{
    HAL_UART_send_data( data, data_size );
}


CLI_error_et CLI_parse_cmd( char* message_string, u8_t* argumen_count, char *argument_vector[], u8_t max_num_args )
{
    CLI_error_et error = CLI_ERROR_NONE;
    char* cmd;
    u8_t calc_argumen_count = 0u;
    u8_t i = 0u;

    cmd = message_string;

    // Search for the first charachter */
    for( i = 0u; i < strlen( message_string ) - 1; i++ )
    {
    	if( CLI_is_space_or_newLine( *(cmd+i) ) != TRUE )
    	{
    		break;
    	}
    }

    /* We have pointed the command to the start of the string ignoring any leading spaces or tabs
     * and because we have found that this charachter was not a space we can immediately move to the next charachter*/

    argument_vector[calc_argumen_count++] = cmd;
    cmd += ( i + 1u );

	for (; *cmd; cmd++)
	{
		if ( CLI_is_space_or_newLine(*cmd) == TRUE )
		{
			*cmd = '\0';
		}
		else if ( *(cmd - 1u) == '\0')
		{
			if( ( calc_argumen_count ) >= max_num_args )
			{
				error = CLI_ERROR_INVALID_ARGS_NUM;
				break;
			}
			argument_vector[calc_argumen_count++] = cmd;
		}
	}

	*argumen_count = calc_argumen_count;

    return error;
}


CLI_error_et CLI_validate_arguments( u8_t aArgCount, char *aArgVector[], u8_t command_index )
{
	CLI_error_et error = CLI_ERROR_NONE;
	u8_t i = 0u;
	u8_t len = 0u;
	u32_t min_arg_value;
	u32_t max_arg_value;

	(void)min_arg_value;
	(void)max_arg_value;

	if( ( aArgCount > CLI_CMD_LINE_ARGS_MAX ) || ( aArgCount < CLI_commands[command_index].num_params ) )
	{
		error = CLI_ERROR_INVALID_ARGS_NUM;
	}

	if( error == CLI_ERROR_NONE)
	{
		/* Valid number of args, now check the length of each args */
		for( i = 0u; i < aArgCount; i++ )
		{
			len = strlen( aArgVector[i+1] );
			if( len > CLI_commands[command_index].param_list[i].num_chars )
			{
				error = CLI_ERROR_INVALID_ARGS;
				break;
			}
		}

	}
	return ( error );
}



CLI_error_et CLI_process_cmd( u8_t aArgCount, char *aArgVector[] )
{
    CLI_error_et error = CLI_ERROR_NONE;
    const CLI_Command_st *cmd = NULL;
    u8_t command_index = 0u;

    for ( cmd = CLI_commands; cmd->command_name != NULL; cmd++ )
    {
        if ( (strcmp( aArgVector[0], cmd->command_name ) == 0u ) )
        {
			//the command can be executed. Now get into validating the parameters and their values provided by the user
        	error = CLI_validate_arguments( aArgCount - 1, aArgVector, command_index );
//			}
//			else
//			{
//				error = CLI_ERROR_PROHIBITED;
//			}
			// there is a matching entry in the table. just come out of the loop!
			break;
    	}
        command_index += 1u;
    }

    if( error == CLI_ERROR_NONE )
    {
		if(cmd->command_name == NULL)
		{
			// looks like we have looped through the entire table and we did not get see the command.
			error = CLI_ERROR_NOT_FOUND;
		}
    }

    if( error == CLI_ERROR_NONE  )
    {
        error = cmd->command_handler((aArgCount-1), aArgVector );
    }

    return error;
}



void CLI_store_history( void )
{
	u8_t i = 0u;

	for( i = 0u; i < CLI_MAX_COMMAND_HISTORY; i++ )
	{
		STDC_memcpy( &NVM_info_s.NVM_generic_data_blk_s.cmd_list[i], &CLI_cmd_history_s.cmd_list[i], sizeof( CLI_cmd_st ) );
	}
}


CLI_error_et help_handler( u8_t aArgCount, char *aArgVector[] )
{
	char  output_string[250] = { 0 };
	const CLI_Command_st *cmd = NULL;
	CLI_error_et error = CLI_ERROR_NONE;

	CLI_send_newline();

	for ( cmd = CLI_commands; cmd->command_name != NULL; cmd++)
	{
		STDC_memset( output_string, 0x20, sizeof( output_string ) );

		if( strcmp( cmd->command_name,"mode") == 0 )
		{
			sprintf(output_string, cmd->helpinfo);
			CLI_send_data( output_string, strlen(output_string));

			STDC_memset( output_string, 0, sizeof( output_string ) );
			CLI_send_newline();
		}
		else
		{
			sprintf(output_string, "\r\t%s", cmd->helpinfo);
			CLI_send_data( output_string, strlen(output_string));
		}
	}
	CLI_send_newline();

	return ( error );
}

CLI_error_et setmode_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;

	return( error );

}

CLI_error_et reset_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];

	STDC_memset(output_string, 0x00, sizeof(output_string));

	sprintf( output_string, "\r\nSaving NVM contents....\r\nResetting Core..." );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	NVM_request_flush();

	NVIC_SystemReset();

	return( error );
}


CLI_error_et ver_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	char version_num[SW_VERSION_NUM_SIZE];

	STDC_memset( output_string, 0x20, sizeof( output_string ) );

	HAL_BRD_get_SW_version_num( (u8_t*)version_num );

	CLI_send_newline();
	sprintf( output_string, "SW version is %d.%d.%d", version_num[0], version_num[1], version_num[2] );
	CLI_send_data( output_string, strlen(output_string));

	HAL_BRD_get_HW_version_num( (u8_t*)version_num );

	CLI_send_newline();
	sprintf( output_string, "HW version is %d", version_num[0] );
	CLI_send_data( output_string, strlen(output_string));

	return( error );
}



CLI_error_et setid_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	u16_t id = 0;

	id = strtoul( aArgVector[1], NULL, 16 );
	NVM_info_s.NVM_generic_data_blk_s.device_id = id;

	sprintf( output_string, "Devide ID has been set to 0x%04X", id );

	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	NVM_request_flush();

	return( error );
}



CLI_error_et nvm_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	u8_t i = 0u;
	u16_t* data_p;
	u16_t temp_val;
	u16_t divisor;
	u8_t divident;
	float* temp_data_p;
	u8_t* strings[4] =
	{
		"cool mode max temp",
		"cool mode min temp",
		"heat mode max temp",
		"heat mode min temp"
	};


	STDC_memset( output_string, 0x00, sizeof( output_string ) );

	CLI_send_newline();
	CLI_send_newline();
	sprintf( output_string, "/****** NVM data ******/\r\nchksum:\t\t0x%02X\r\nVers:\t\t%d\r\nwrites:\t\t%d",
																									   NVM_info_s.checksum,
																								  (int)NVM_info_s.version,
																								  (int)NVM_info_s.write_count );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();
	CLI_send_newline();

	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	sprintf( output_string, "Device ID:\t\t0x%02X\r\n\rWhitelist state:\t%d",
	(int)NVM_info_s.NVM_generic_data_blk_s.device_id, (int)NVM_info_s.NVM_generic_data_blk_s.whitelist_state );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();
	CLI_send_newline();


	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	sprintf( output_string, "/*** Stored CLI commands ***/" );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	for( i = 0u; i < CLI_MAX_COMMAND_HISTORY; i++ )
	{
		STDC_memset( output_string, 0x00, sizeof( output_string ) );
		sprintf( output_string, "%2d: %s\r\n", i + 1, NVM_info_s.NVM_generic_data_blk_s.cmd_list[i].cmd );
		CLI_send_data( output_string, strlen(output_string));
	}

	CLI_send_newline();

	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	sprintf( output_string, "/*** Whitelist ID's ***/" );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	data_p = NVM_info_s.NVM_generic_data_blk_s.rf_whitelist;

	for( i = 0u; i < RF_MGR_RF_DATA_HANDLER_SIZE/4u; i++ )
	{
		STDC_memset( output_string, 0x00, sizeof( output_string ) );
		sprintf( output_string, " %2d ID:0x%04X, %2d ID:0x%04X, %2d ID:0x%04X, %2d ID:0x%04X",
				(i*4), *(data_p + (i*4)), ((i*4) + 1), *(data_p + (i*4) + 1), ((i*4) + 2), *(data_p + (i*4) + 2), ((i*4) + 3), *(data_p + (i*4) + 3) );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}
	CLI_send_newline();

	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	sprintf( output_string, "/*** AIRCON ***/" );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	/* setup the pointer */
	temp_data_p = &NVM_info_s.NVM_generic_data_blk_s.cool_max_temp;

	for( i = 0; i < 4; i++ )
	{
		temp_val = ( ( (*temp_data_p) * 10u ) );
		divisor = ( temp_val / 10u );
		divident = ( temp_val - ( divisor ) * 10 );

		temp_data_p++;

		STDC_memset( output_string, 0x00, sizeof( output_string ) );
		sprintf( output_string, "%s:\t%d.%d",
				strings[i],
				(divisor),
				(divident) );

		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();

	}

	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	CLI_send_newline();

	sprintf( output_string, "state:\t%d\r\nmode:\t%d",
			NVM_info_s.NVM_generic_data_blk_s.aircon_state,
			NVM_info_s.NVM_generic_data_blk_s.aircon_mode );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();


	return( error );
}


CLI_error_et clocks_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);

	sprintf( output_string, "SYSCLK:\t%dHz\r\nHCLK:\t%dHz\r\nPCLK1:\t%dHz\r\nPCLK2:\t%dHz\r\nADCCLK:\t%dHz\r\n",
			(int)RCC_Clocks.SYSCLK_Frequency, (int)RCC_Clocks.HCLK_Frequency, (int)RCC_Clocks.PCLK1_Frequency,
			(int)RCC_Clocks.PCLK2_Frequency, (int)RCC_Clocks.ADCCLK_Frequency );

	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	return( error );
}


CLI_error_et temp_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];

	sprintf( output_string, "The internal device temperature is not yet supported!!!" );

	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	return( error );
}

CLI_error_et batt_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];

	sprintf( output_string, "The internal battery/supply voltage is not yet supported!!!" );

	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	return( error );
}


CLI_error_et listnodes_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];

	u8_t node_idx = 0u;
	u8_t num_nodes = 0u;

	RF_MGR_rf_data_store_st* node_id_p;

	CLI_send_newline();

	node_id_p = RF_MGR_get_all_decoded_IDs_address();

	for( node_idx = 0; node_idx < RF_MGR_RF_DATA_HANDLER_SIZE; node_idx++ )
	{
		if( node_id_p->data_packet_s[node_idx].node_id != 0x0000 )
		{
			num_nodes ++;
		}
		sprintf( output_string, "Node ID %02d:\t0x%04X\r\n", node_idx, node_id_p->data_packet_s[node_idx].node_id );
		CLI_send_data( output_string, strlen(output_string));
		STDC_memset( output_string, 0x20, sizeof( output_string ) );
	}

	sprintf( output_string, "Num Free Nodes:\t%02d\r\n", ( RF_MGR_RF_DATA_HANDLER_SIZE - num_nodes ) );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();
	STDC_memset( output_string, 0x20, sizeof( output_string ) );

	return( error );
}

CLI_error_et nodestat_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
}



CLI_error_et led_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	HAL_BRD_led_et led = 0u;
	off_on_et state = OFF;

	(void)output_string;

	CLI_send_newline();

	/* What LED is the user trying to control */
	led = strtoul( aArgVector[1], NULL, 16 );
	state = (off_on_et)strtoul( aArgVector[2], NULL, 16 );

	if( led == 0x0a )
	{
		HAL_BRD_set_LED_state( LED_0, state );
		HAL_BRD_set_LED_state( LED_1, state );
		HAL_BRD_set_LED_state( LED_2, state );
		HAL_BRD_set_LED_state( LED_3, state );
	}
	else
	{
		HAL_BRD_set_LED_state( led, state );
	}

	return( error );
}


CLI_error_et wl_add_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	pass_fail_et result;
	u16_t id;

	/* What ID are we trying to add */
	id = strtoul( aArgVector[1], NULL, 16 );

	CLI_send_newline();

	result = RF_MGR_add_wl_node( id );

	if( result == PASS )
	{
		sprintf( output_string, "Node ID 0x%04X successfully added", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}
	else
	{
		sprintf( output_string, "Failed to add Node ID 0x%04X", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}


	return( error );
}



CLI_error_et wl_remove_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	pass_fail_et status = FAIL;
	u16_t id = 0u;

	/* What ID is the user trying to remove */
	id = strtoul( aArgVector[1], NULL, 16 );

	/* This function should handle everything */
	status = RF_MGR_remove_wl_node( id );

	CLI_send_newline();

	if( status == PASS )
	{
		sprintf( output_string, "Node 0x%04X was located in the current whitelist and has now been removed...", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
		STDC_memset( output_string, 0x20, sizeof( output_string ) );
	}
	else
	{
		sprintf( output_string, "Node 0x%04X has not been located in the whitelist!!!", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
		STDC_memset( output_string, 0x20, sizeof( output_string ) );
	}

	return( error );
}


CLI_error_et wl_display_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	u8_t i = 0u;
	RF_MGR_whitelist_st *data_p;

	data_p = RF_MGR_get_whitelist_address();
	CLI_send_newline();

	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	sprintf( output_string, "/*** Whitelist ID's ***/" );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	for( i = 0; i < RF_MGR_RF_DATA_HANDLER_SIZE/4u; i++ )
	{
		STDC_memset( output_string, 0x00, sizeof( output_string ) );
		sprintf( output_string, " %2d ID:0x%04X, %2d ID:0x%04X, %2d ID:0x%04X, %2d ID:0x%04X",
				(i*4), data_p->id[(i*4)], ((i*4) + 1), data_p->id[(i*4)+1], ((i*4) + 2),  data_p->id[(i*4)+2], ((i*4) + 3),  data_p->id[(i*4)+3] );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}

	return( error );
}


CLI_error_et wl_state_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	disable_enable_et state;

	state = strtoul( aArgVector[1], NULL, 10 );

	CLI_send_newline();

	NVM_info_s.NVM_generic_data_blk_s.whitelist_state = state;
	RF_MGR_set_whitelist_state( state );

	NVM_request_flush();

	sprintf( output_string, "New whitelist state - %d", state );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();
	
	return( error );
}


CLI_error_et ac_state_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	disable_enable_et state;

	state = strtoul( aArgVector[1], NULL, 10 );

	CLI_send_newline();

	NVM_info_s.NVM_generic_data_blk_s.aircon_state = state;
	AIRCON_set_state( state );

	NVM_request_flush();

	sprintf( output_string, "New aircon state - %d", state );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	return( error );
}

CLI_error_et ac_mode_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	AIRCON_mode_et mode;

	mode = strtoul( aArgVector[1], NULL, 10 );

	CLI_send_newline();

	NVM_info_s.NVM_generic_data_blk_s.aircon_mode = mode;
	AIRCON_set_mode( mode );

	NVM_request_flush();

	sprintf( output_string, "New aircon mode - %d", mode );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	return( error );
}
	

CLI_error_et savenvm_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];

	/* Force a write to nvm */
	NVM_set_override_state();
	NVM_request_flush();

	CLI_send_newline();

	sprintf( output_string, "NVM flush has been completed" );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	return( error );
}


CLI_error_et rf_dbg_out_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	disable_enable_et state;

	CLI_send_newline();

	/* This is a toggle, function, it can turn the RF debug on/off */
	state = RF_MGR_toggle_dbg_output_state();
	NVM_request_flush();
	sprintf( output_string, "RF debug output state - %d", state );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();
}



CLI_error_et test_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;

	while(1);

	return( error );
}
