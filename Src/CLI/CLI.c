/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "C_defs.h"
#include "STDC.h"
#include "NVM.h"
#include "NRF24.h"
#include "HAL_BRD.h"
#include "COMPILER_defs.h"

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
    "\r\nToo many arguments\r\n",
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
	 {"led",   		  &led_handler,         HELP_LED,               SUPPORTED_FOR_ALL_MODES, ENABLE, 2, LED_CMD_PARAM_LIST  },
	 {"wladd", 		  &wl_add_handler,      HELP_WL_ADD, 			SUPPORTED_FOR_ALL_MODES, ENABLE, 1, WL_ADD_CMD_PARAM_LIST  },
	 {"wlremove",     &wl_remove_handler,	HELP_WL_REM,			SUPPORTED_FOR_ALL_MODES, ENABLE, 1, WL_REMOVE_CMD_PARAM_LIST  },
	 {"wldisplay",    &wl_display_handler,	HELP_WL_DISPLAY,	    SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST  },
	 {"test",    	  &test_handler,	    HELP_TEST,	            SUPPORTED_FOR_ALL_MODES, ENABLE, 0, NULL_PARAM_LIST  },
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
				error = CLI_ERROR_INVALID_ARGS_MORE;
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

	if( aArgCount > CLI_CMD_LINE_ARGS_MAX )
	{
		error = CLI_ERROR_INVALID_ARGS_MORE;
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

CLI_error_et readnvm_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	NVM_generic_data_blk_st* nvm_p = &(NVM_info_s.NVM_generic_data_blk_s);

	STDC_memset(output_string, 0x00, sizeof(output_string));

	sprintf( output_string, "/****** NVM data ******/\r\nChecksum:\t%X\r\nVersion:\t%d\r\nWrite count:\t%d",
	NVM_info_s.checksum,
	(int)NVM_info_s.version,
	(int)NVM_info_s.write_count );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

	STDC_memset(output_string, 0x00, sizeof(output_string));
	sprintf( output_string, "%02X",
	nvm_p->device_id );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();

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

	STDC_memset( output_string, 0x00, sizeof( output_string ) );

	CLI_send_newline();
	sprintf( output_string, "/****** NVM data ******/\r\nchksum:\t\t0x%02X\r\nVers:\t\t%d\r\nwrites:\t\t%d\r\n",
																									   NVM_info_s.checksum,
																								  (int)NVM_info_s.version,
																								  (int)NVM_info_s.write_count );

	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	sprintf( output_string, "Device ID:\t0x%02X\r\n",
	(int)NVM_info_s.NVM_generic_data_blk_s.device_id );
	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	STDC_memset( output_string, 0x00, sizeof( output_string ) );
	sprintf( output_string, "Stored CLI commands:\r\n" );
	CLI_send_newline();
	CLI_send_data( output_string, strlen(output_string));

	for( i = 0u; i < CLI_MAX_COMMAND_HISTORY; i++ )
	{
		STDC_memset( output_string, 0x00, sizeof( output_string ) );
		sprintf( output_string, "%2d: %s\r\n", i + 1, NVM_info_s.NVM_generic_data_blk_s.cmd_list[i].cmd );
		CLI_send_data( output_string, strlen(output_string));
	}

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

	u8_t num_node = 0u;
	u16_t node_ids[RF_MGR_RF_DATA_HANDLER_SIZE];

	CLI_send_newline();

	RF_MGR_get_all_decoded_IDs( node_ids );

	for( num_node = 0; num_node < RF_MGR_RF_DATA_HANDLER_SIZE; num_node++ )
	{
		if( node_ids[num_node] != 0u )
		{
			sprintf( output_string, "Node ID %02d:\t0x%04X\r\n", num_node, node_ids[num_node] );
			CLI_send_data( output_string, strlen(output_string));
			STDC_memset( output_string, 0x20, sizeof( output_string ) );
		}
		else
		{
			break;
		}
	}
	sprintf( output_string, "Num Free Nodes:\t%02d\r\n", ( RF_MGR_RF_DATA_HANDLER_SIZE - num_node ) );
	CLI_send_data( output_string, strlen(output_string));
	CLI_send_newline();
	STDC_memset( output_string, 0x20, sizeof( output_string ) );

	return( error );
}


CLI_error_et remove_wl_node_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	false_true_et node_located = FALSE;
	u16_t id = 0u;

	CLI_send_newline();

	/* Lets read all the nodes first to ensure that the one being deleted is actually on the list */
	u8_t num_node = 0u;
	u16_t node_ids[RF_MGR_RF_DATA_HANDLER_SIZE];

	RF_MGR_get_all_decoded_IDs( node_ids );

	/* What ID is the user trying to remove */
	id = strtoul( aArgVector[1], NULL, 16 );

	/* Is that node actually on the list */
	for( num_node = 0; num_node < RF_MGR_RF_DATA_HANDLER_SIZE; num_node++ )
	{
		if( node_ids[num_node] == id )
		{
			node_located = TRUE;
			break;
		}
	}

	if( node_located == TRUE )
	{
		RF_MGR_remove_wl_node( 0 );
		sprintf( output_string, "Node 0x%04X has been located in the current list and will be deleted...", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
		STDC_memset( output_string, 0x20, sizeof( output_string ) );
	}
	else
	{
		sprintf( output_string, "Node 0x%04X has not been located in the current list!!!,\r\nHere is the current list of nodes..", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
		STDC_memset( output_string, 0x20, sizeof( output_string ) );

		/* Print the current list to the user so that they can see them and delete the correct node */
		for( num_node = 0; num_node < RF_MGR_RF_DATA_HANDLER_SIZE; num_node++ )
		{
			if( node_ids[num_node] != 0u )
			{
				sprintf( output_string, "Node ID %02d:\t0x%04X\r\n", num_node, node_ids[num_node] );
				CLI_send_data( output_string, strlen(output_string));
				CLI_send_newline();
				STDC_memset( output_string, 0x20, sizeof( output_string ) );
			}
			else
			{
				break;
			}
		}
	}

	return( error );
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
		sprintf( output_string, "Failed to add Node ID 0x%02X", id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}


	return( error );
}



CLI_error_et wl_remove_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	u8_t pos = 0u;
	pass_fail_et result;

	CLI_send_newline();

	/* What LED is the user trying to control */
	pos = strtoul( aArgVector[1], NULL, 10 );

	result = RF_MGR_remove_wl_node( pos );

	if( result == PASS )
	{
		sprintf( output_string, "Node at position %d successfully removed", pos );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}
	else
	{
		sprintf( output_string, "Failed to remove node at position %d", pos );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
	}

	return( error );
}


CLI_error_et wl_display_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	u8_t i = 0u;
	RF_MGR_whitelist_st *data_p;

	data_p = RF_MGR_get_whitelist_addres();

	CLI_send_newline();

	for( i = 0; i < RF_MGR_RF_DATA_HANDLER_SIZE ; i++ )
	{
		sprintf( output_string, "Node %02d ID:\t0x%04X", i, data_p->id );
		CLI_send_data( output_string, strlen(output_string));
		CLI_send_newline();
		data_p ++;
	}

	return( error );
}



CLI_error_et test_handler( u8_t aArgCount, char *aArgVector[] )
{
	CLI_error_et error = CLI_ERROR_NONE;
	char output_string[200];
	u8_t i = 0u;

	while(1);

	return( error );
}




///* STM32 specific includes */
//#include "C_defs.h"
//#include "COMPILER_defs.h"
//#include "STDC.h"
//#include "NVM.h"
//#include "HAL_UART.h"
//#include "CLI.h"
//
//
///* Module Identification for STDC_assert functionality */
//
//STATIC const u8_t SERIAL_help_promt_s[] =
//		"\r\nValid Commands\r\n"
//		"-----------------------\r\n"
//		"help:   help menu displayed\r\n"
//		"reset:  resets the core\r\n"
//		"stats:  displays stats\r\n"
//		"batt:   returns the supply voltage\r\n"
//		"temp:   returns the internal temperature\r\n"
//		"debug:  prints all debug information \r\n"
//		"led:    < 0 or 1 > 0 - LED OFF, 1 - LED ON\r\n"
//		"ver:    returns the SW version\r\n"
//		"clocks: returns the current clock speed\r\n"
//		"rf:     rf conf - Sets up the RF config ( 0..5)\r\n"
//		"        rf tx - Transmitts a packet of RF\r\n"
//		"        rf read - Reads all RF registers\r\n"
//		"        rf fifo - Reads the RF fifo\r\n"
//		"        rf power - Sets the RF power level ( 0..31)\r\n"
//		"        rf test - Runs the RF self test routine\r\n"
//		"rand:   returns a random number generated by srand\r\n"
//		"db:     < 0..3 > < 0 or 1 > ( eg db 1 1 turns DB 1 ON )\r\n"
//		"sleep:  <1..15000> sets the sleep time in secs\r\n"
//		"stream: <0 1> Turns streaming mode on and off\r\n"
//		"nvm:	 prints the current NVM data\r\n\r\n";
//
//
//STATIC const u8_t SERIAL_welcome_message_s[] =
//		"\r\n\r\nHello.\r\nDebug Console is ready, Type 'help' for a list of commands\r\n"
//		"All commands are case 'insensitive'\r\n"
//		"/*********************************************************\r\n\r\n";
//
//STATIC const u8_t SERIAL_buffer_reset_message_s[] =
//		"\r\nERROR!... The cmd line has been reset!!\r\n\r\n";
//
//
//STATIC const u8_t SERIAL_invalid_cmd_message_s[] =
//		"\r\nERROR! that is not a valid command!\r\nPlease try again or type 'help' for help menu..\r\n\r\n";
//
//STATIC const u8_t SERIAL_newline_s[] =
//		"\r\n\r\n";
//
//
//
//STATIC false_true_et SERIAL_cr_received_s;
//STATIC char SERIAL_rx_buf_idx_s;
//STATIC char SERIAL_rx_buf_char_s;
//STATIC char SERIAL_rx_buf_s[RX_BUF_SIZE] = {'\0'};
//STATIC char SERIAL_tx_buf_s[TX_BUF_SIZE] = {'\0'};
//STATIC false_true_et SERIAL_stream_mode_s;
//STATIC false_true_et SERIAL_stream_triggered_s;
//
//STATIC u32_t SERIAL_max_missed_frames_s;
//
//
//
//
//void CLI_init( void )
//{
//	MODE_MGR_tick_timer_msecs_s = MODE_MGR_TICK_RATE_MSECS;
//	MODE_MGR_mode_s = MODE_MGR_MODE_NORMAL;
//	MODE_MGR_system_init_s = FALSE;
//	MODE_MGR_selector_switch_state_s = 0u;
//	MODE_MGR_debounce_ctr_s = 0xFF;
//}
//
//
//
//
//
//void SERIAL_msg_handler( void )
//{
//	if (SERIAL_cr_received_s == TRUE)
//	{
//		// Reset CR Flag
//		SERIAL_cr_received_s = FALSE;
//
//		if( strncmp(strlwr(SERIAL_rx_buf_s), "help\r", 5) == 0)
//		{
//			SERIAL_Send_data( SERIAL_help_promt_s );
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "led" ) != 0)
//		{
//			char *result = strstr(SERIAL_rx_buf_s, "led") + 4;
//			if( ( *result == '0' ) || ( strstr(result, "off") != 0 ) )
//			{
//				GPIO_SetBits(GPIOC, GPIO_Pin_13);
//				sprintf( SERIAL_tx_buf_s, "\r\nLED turned off\r\n\r\n" );
//				SERIAL_Send_data(SERIAL_tx_buf_s);
//			}
//			else if( ( * result == '1' ) || ( strstr(result, "on") != 0 ) )
//			{
//				GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//				sprintf( SERIAL_tx_buf_s, "\r\nLED turned on\r\n\r\n" );
//				SERIAL_Send_data(SERIAL_tx_buf_s);
//			}
//			else
//			{
//				SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
//			}
//		}
//		else if( strncmp(strlwr(SERIAL_rx_buf_s), "batt\r", 5) == 0)
//		{
//			u16_t battery_voltage = 3000u;
//
//			sprintf( SERIAL_tx_buf_s, "\r\nSupply voltage is %dmV\r\n\r\n", battery_voltage );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//		}
//		else if( strncmp(strlwr(SERIAL_rx_buf_s), "reset\r", 6) == 0)
//		{
//			SERIAL_Send_data("\r\nResetting Core....\r\n\r\n");
//			NVIC_SystemReset();
//		}
//		else if( strncmp(strlwr(SERIAL_rx_buf_s), "ver\r", 4) == 0)
//		{
//			char version_num[5];
//
//			//HAL_BRD_get_SW_version_num( version_num );
//
//			sprintf( SERIAL_tx_buf_s, "\r\nSW version is %d.%d.%d\r\n", version_num[0], version_num[1], version_num[2] );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//
//			//HAL_BRD_get_HW_version_num( version_num );
//
//			sprintf( SERIAL_tx_buf_s, "HW version is %d.%d\r\n\r\n", version_num[0], version_num[1] );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "stream" ) != 0)
//		{
//			char *result = strstr(SERIAL_rx_buf_s, "stream") + 7;
//			if( ( *result == '0' ) || ( strstr((char*)result, "off") != 0 ) )
//			{
//				SERIAL_stream_mode_s = FALSE;
//				SERIAL_Send_data("\r\nStream mode turned off\r\n\r\n");
//			}
//			else if( ( * result == '1' ) || ( strstr(result, "on") != 0 ) )
//			{
//				SERIAL_stream_mode_s = TRUE;
//				SERIAL_Send_data("\r\nStream mode turned on\r\n\r\n");
//			}
//			else
//			{
//				SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
//			}
//		}
//		else if( strncmp(strlwr(SERIAL_rx_buf_s), "clocks\r", 7) == 0)
//		{
//			RCC_ClocksTypeDef RCC_Clocks;
//			RCC_GetClocksFreq (&RCC_Clocks);
//
//			sprintf( SERIAL_tx_buf_s, "SYSCLK:\t%dHz\r\nHCLK:\t%dHz\r\nPCLK1:\t%dHz\r\nPCLK2:\t%dHz\r\nADCCLK:\t%dHz\r\n",
//					(int)RCC_Clocks.SYSCLK_Frequency, (int)RCC_Clocks.HCLK_Frequency, (int)RCC_Clocks.PCLK1_Frequency,
//					(int)RCC_Clocks.PCLK2_Frequency, (int)RCC_Clocks.ADCCLK_Frequency );
//
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "db" ) != 0)
//		{
//			char *result = strstr(SERIAL_rx_buf_s, "db") + 3;
//			char *result1 = strstr(SERIAL_rx_buf_s, "db") + 5;
//
//			switch( *result )
//			{
//				case '0':
//					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
//					{
//					}
//					else
//					{
//					}
//					break;
//
//				case '1':
//					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
//					{
//					}
//					else
//					{
//					}
//					break;
//
//				case '2':
//					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
//					{
//					}
//					else
//					{
//					}
//					break;
//
//				case '3':
//					if( ( *result1 == '0' ) || ( strstr(result1, "off") != 0 ) )
//					{
//					}
//					else
//					{
//					}
//					break;
//
//				default:
//					SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
//					break;
//			}
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "sleep" ) != 0)
//		{
//			u32_t val;
//
//			char *result = strstr(SERIAL_rx_buf_s, "sleep") + 6;
//
//			val = atoi(result);
//
//			if( val > 15300 )
//			{
//				val = 15300;
//				sprintf( SERIAL_tx_buf_s, "Selection too high, Max = 15300, using Max instead :)\r\n\r\n" );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//			}
//			else
//			{
//				sprintf( SERIAL_tx_buf_s, "Sleep time has been set to %d secs\r\n\r\n", (int)val );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				RTC_set_wakeup_time( val );
//
//				/* change the value and write to NVM */
//				NVM_info_s.NVM_generic_data_blk_s.sleep_time = val;
//				NVM_request_flush();
//			}
//
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "rf" ) != 0)
//		{
//			u8_t val;
//
//			char *sub_string = strstr(SERIAL_rx_buf_s, "rf") + 3;
//
//			if( ( strstr(sub_string, "conf") != 0 ) )
//			{
//				sub_string = strstr(SERIAL_rx_buf_s, "rf") + 8;
//
//				val = atoi(sub_string);
//
//				if( val >= RFM69_CFG_MAX )
//				{
//					sprintf( SERIAL_tx_buf_s, "Selection not valid.. config can be between 0..%d\r\n", RFM69_CFG_MAX );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//				}
//				else
//				{
//					NVM_info_s.NVM_generic_data_blk_s.rf_config = (RFM69_static_configuration_et)val;
//					NVM_request_flush();
//
//					sprintf( SERIAL_tx_buf_s, "\r\nRf config %d selected\r\n", val );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//
//					/* reset the RF chip so it can use the new settings */
//					RFM69_reset();
//				}
//			}
//			else if( ( strstr(sub_string, "tx") != 0 ) )
//			{
//				*sub_string = strstr(SERIAL_rx_buf_s, "tx") + 3;
//			}
//			else if( ( strstr(sub_string, "read") != 0 ) )
//			{
//				u8_t read_data[84];
//				u8_t i;
//
//				/* read as many registers as we can in one go */
//				RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGOPMODE, read_data, ( sizeof( read_data ) - 5 ) );
//
//				RFM69_read_registers( READ_FROM_CHIP, REGTESTLNA, &read_data[80], 1 );
//
//				RFM69_read_registers( READ_FROM_CHIP, REGTESTPA1, &read_data[81], 1 );
//
//				RFM69_read_registers( READ_FROM_CHIP, REGTESTPA2, &read_data[82], 1 );
//
//				RFM69_read_registers( READ_FROM_CHIP, REGTESTDAGC, &read_data[83], 1 );
//
//				RFM69_read_registers( READ_FROM_CHIP, REGTESTAFC, &read_data[84], 1 );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nReading back all RFM69 registers...\r\n" );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				for( i = 0u; i < ( sizeof( read_data ) - 5 ); i++ )
//				{
//					sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", i+1, read_data[i] );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//				}
//
//				/* Now print all the registers that are not sequential */
//				sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", REGTESTLNA, read_data[80] );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", REGTESTPA1, read_data[81] );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", REGTESTPA2, read_data[82] );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", REGTESTDAGC, read_data[83] );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is 0x%02X", REGTESTAFC, read_data[84] );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				sprintf( SERIAL_tx_buf_s, "\r\n\r\n" );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//			}
//			else if( ( strstr(sub_string, "fifo") != 0 ) )
//			{
//				u8_t rfm69_fifo_regs[ RFM69_MAX_DATA_LEN ];
//				u8_t i;
//
//				RFM69_read_FIFO_register( rfm69_fifo_regs );
//
//				for( i = 0u; i < RFM69_MAX_DATA_LEN; i++ )
//				{
//					sprintf( SERIAL_tx_buf_s, "\r\nFifo Reg %02d is %02X", i, rfm69_fifo_regs[i] );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//				}
//
//				sprintf( SERIAL_tx_buf_s, "\r\n\r\n" );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//			}
//			else if( ( strstr(sub_string, "power") != 0 ) )
//			{
//				sub_string = strstr(SERIAL_rx_buf_s, "rf") + 9;
//
//				val = atoi(sub_string);
//
//				if( val > RFM69_MAX_TX_POWER_LEVEL )
//				{
//					sprintf( SERIAL_tx_buf_s, "\r\nRF TX power selection %d is not valid\r\nAuto adjusting the RF TX power selection...\r\n", val );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//
//					/* Default them to config */
//					val = RFM69_MAX_TX_POWER_LEVEL;
//				}
//
//				sprintf( SERIAL_tx_buf_s, "\r\nRF TX power %d selected\r\n", val );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				RFM69_set_PA_level( val );
//
//				NVM_info_s.NVM_generic_data_blk_s.tx_power_level = val;
//				NVM_request_flush();
//
//				sprintf( SERIAL_tx_buf_s, "\r\nActual RF TX power read back : 0x%02X \r\n", val );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//			}
//			else if (( strstr(sub_string, "id") != 0 ) )
//			{
//				sub_string = strstr(SERIAL_rx_buf_s, "rf") + 6;
//
//				val = atoi(sub_string);
//				NVM_info_s.NVM_generic_data_blk_s.node_id = val;
//				NVM_request_flush();
//
//				sprintf( SERIAL_tx_buf_s, "\r\nRF node id set to %d \r\n", val );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//
//				//RFM69_set_own_node_address( val );
//			}
//			else if( ( strstr(sub_string, "test") != 0 ) )
//			{
//				u8_t i;
//
//				if( RFM69_read_version_num() != RFM69_VERSION )
//				{
//				   STDC_basic_assert();
//
//				   sprintf( SERIAL_tx_buf_s, "\r\nReading back the RFM69 version number failed....fuck!! :( \r\n" );
//				   SERIAL_Send_data( SERIAL_tx_buf_s );
//				}
//				else
//				{
//					sprintf( SERIAL_tx_buf_s, "\r\nVersion number check passed\r\nWriting down the RFM69 Register config....\r\n" );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//					STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//					/* Fire down a config of registers */
//					RFM69_set_configuration( NVM_info_s.NVM_generic_data_blk_s.rf_config );
//
//					u32_t freq;
//					freq = RFM69_read_rf_carrier_freq();
//					sprintf( SERIAL_tx_buf_s, "\r\nRF carrier frequency is %dHz\r\n", (int)freq );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//					STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//					sprintf( SERIAL_tx_buf_s, "\r\nReading back the RFM69 Register config....\r\n" );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//					STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//					u8_t read_data[79];
//					RFM69_register_data_st write_data[79];
//
//					RFM69_read_registers( READ_FROM_CHIP_BURST_MODE, REGOPMODE, read_data, sizeof( read_data ) );
//					RFM69_get_configuration( RFM69_433_DEFAULT_CONFIG, write_data );
//
//					for( i = 0u; i < 79; i++ )
//					{
//						sprintf( SERIAL_tx_buf_s, "\r\nReg %02d is write:%02X read:%02X", write_data[i].RFM69_register, write_data[i].register_data, read_data[i] );
//						SERIAL_Send_data( SERIAL_tx_buf_s );
//					}
//
//					sprintf( SERIAL_tx_buf_s, "\r\n\r\n" );
//					SERIAL_Send_data( SERIAL_tx_buf_s );
//				}
//			}
//			else if(( strstr(sub_string, "stats") != 0 ) )
//			{
//				u32_t cnt;
//				u32_t time;
//				f32_t percent;
//				s8_t rssi;
//
//				cnt = RFM69_get_received_packet_cnt();
//				time = HAL_TIM_get_time();
//				rssi = RFM69_get_last_received_RSSI();
//				percent = ( ( (f32_t)cnt / (f32_t)time ) * 100.0f );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nCurrent received packet count:\t\t\t%d", cnt );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nSeconds elapsed since first frame received:\t%d", time );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nNo of missed frames:\t\t\t\t%d", ( time - cnt ) );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nLast received frame RSSI:\t\t\t%d", rssi );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nPacket reception status:\t\t\t%d%%\r\n", (u8_t)percent );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//				sprintf( SERIAL_tx_buf_s, "\r\nRun time in secs since power-up:\t\t\t%d%\r\n", time );
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//			}
//			else if(( strstr(sub_string, "list nodes") != 0 ) )
//            {
//                u8_t num_node = 0u;
//                u16_t node_ids[RF_MGR_RF_DATA_HANDLER_SIZE];
//
//                RF_MGR_get_all_decoded_IDs( node_ids );
//
//                for( num_node = 0; num_node < RF_MGR_RF_DATA_HANDLER_SIZE; num_node++ )
//                {
//                    if( node_ids[num_node] != 0u )
//                    {
//                        sprintf( SERIAL_tx_buf_s, "Node ID %02d:\t0x%04X\r\n", num_node, node_ids[num_node] );
//                        SERIAL_Send_data( SERIAL_tx_buf_s );
//                        STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//                    }
//                    else
//                    {
//                        break;
//                    }
//                }
//                sprintf( SERIAL_tx_buf_s, "Num Free Nodes:\t%02d\r\n", ( RF_MGR_RF_DATA_HANDLER_SIZE - num_node ) );
//                SERIAL_Send_data( SERIAL_tx_buf_s );
//                STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//            }
//			else if(( strstr(sub_string, "remove node") != 0 ) )
//			{
//				RF_MGR_remove_node( 0 );
//			}
//			else
//			{
//				sprintf( SERIAL_tx_buf_s, "Selection not valid please select \"conf\" or \"tx\"\r\n");
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				sprintf( SERIAL_tx_buf_s, "eg conf 0 = loads rf config 0, or others if applicable\r\n");
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//				sprintf( SERIAL_tx_buf_s, "eg rf tx = triggers a packet of rf to be sent\r\n");
//				SERIAL_Send_data( SERIAL_tx_buf_s );
//			}
//		}
//		else if( strncmp(strlwr(SERIAL_rx_buf_s), "rand\r", 5) == 0)
//		{
//			u16_t random_number;
//
//			/* Set the seed */
//			srand( get_counter() );
//
//			/* Grab the now "random :)" number */
//			random_number = rand()%100;
//
//			while( random_number == 0 )
//			{
//				random_number = rand()%100;
//			}
//
//			sprintf( SERIAL_tx_buf_s, "Random number:\t%d\r\n\r\n", (int)random_number );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "time" ) != 0)
//		{
//			u8_t hours;
//			u8_t mins;
//			u8_t secs;
//
//			char *sub_string = strstr(SERIAL_rx_buf_s, "time") + 5;
//			hours = (u8_t)atoi(sub_string);
//
//			*sub_string = strstr(SERIAL_rx_buf_s, "time") + 8;
//			mins = (u8_t)atoi(sub_string);
//
//			*sub_string = strstr(SERIAL_rx_buf_s, "time") + 11;
//			secs = (u8_t)atoi(sub_string);
//
//			sprintf( SERIAL_tx_buf_s, "\r\nTime has been set to\r\nHours:\t%d\r\nMins:\t%d\r\n\r\nSecs:\t%d", hours, mins, secs );
//		}
//		else if( strstr(strlwr(SERIAL_rx_buf_s), "nvm" ) != 0)
//		{
//			sprintf( SERIAL_tx_buf_s, "chksum:\t0x%02X\r\nVers:\t%d\r\nwrites:\t%d\r\nSleep time:\t%d\r\n", NVM_info_s.checksum, NVM_info_s.version,
//																										  NVM_info_s.write_count,
//																										  NVM_info_s.NVM_generic_data_blk_s.sleep_time );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//		}
//		else if ( strstr(strlwr(SERIAL_rx_buf_s), "test" ) != 0)
//		{
//
//		}
//		else
//		{
//			SERIAL_Send_data( SERIAL_invalid_cmd_message_s );
//		}
//
//		SERIAL_clear_RXBuffer();
//		SERIAL_clear_TXBuffer();
//	}
//	else if( SERIAL_stream_mode_s == TRUE )
//	{
//		if( SERIAL_stream_triggered_s == TRUE )
//		{
//			SERIAL_stream_triggered_s = FALSE;
//
//			u32_t cnt;
//			u32_t time;
//			f32_t percent;
//			s8_t  rssi;
//			u32_t frame_timestamp;
//
//			cnt = RFM69_get_received_packet_cnt();
//			time = HAL_TIM_get_time();
//			rssi = RFM69_get_last_received_RSSI();
//			percent = ( ( (f32_t)cnt / (f32_t)time ) * 100.0f );
//			frame_timestamp = RFM69_get_receieved_packet_timestamp();
//
//			sprintf( SERIAL_tx_buf_s, "\r\nCurrent received packet count:\t\t\t%d", cnt );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//			sprintf( SERIAL_tx_buf_s, "\r\nSeconds elapsed since last frame received:\t%d", time - frame_timestamp );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//			sprintf( SERIAL_tx_buf_s, "\r\nNo of missed frames:\t\t\t\t%d", ( time - cnt ) );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//			if( (time - frame_timestamp ) > SERIAL_max_missed_frames_s )
//            {
//                SERIAL_max_missed_frames_s = ( time - cnt );
//            }
//            sprintf( SERIAL_tx_buf_s, "\r\nMax missed frames in a row:\t\t\t%d", SERIAL_max_missed_frames_s );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//			sprintf( SERIAL_tx_buf_s, "\r\nLast received frame RSSI:\t\t\t%d", rssi );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//			sprintf( SERIAL_tx_buf_s, "\r\nPacket reception status:\t\t\t%d%%\r\n", (u8_t)percent );
//			SERIAL_Send_data( SERIAL_tx_buf_s );
//			STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//
//			sprintf( SERIAL_tx_buf_s, "\r\nRun time in secs since power-up:\t\t%d%\r\n", time );
//            SERIAL_Send_data( SERIAL_tx_buf_s );
//            STDC_memset( SERIAL_tx_buf_s, 0x20, sizeof( SERIAL_tx_buf_s ) );
//		}
//	}
//}
