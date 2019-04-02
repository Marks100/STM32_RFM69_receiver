#ifndef CLI_H
#define CLI_H

#include "C_defs.h"


#define CLI_MAX_INPUT_CHARS           40u
#define CLI_CMD_LINE_ARGS_MAX         6
#define CLI_MAX_COMMAND_HISTORY       6
#define CLI_BACKSPACE                 0x08
#define CLI_CR                        0x0D
#define CLI_SPACE                     0x20
#define CLI_DEL                       0x7F

#define UP_ARROW                      0x41
#define DOWN_ARROW                    0x42
#define LEFT_ARROW					  0x43
#define RIGHT_ARROW					  0x44
#define ERASE_CURRENT_LINE            "\r                                                                        \r\b\b\b"
#define IGNORE_RANGE_CHECK            ((u32_t)~0)

#define RED			"\x1b[31m"
#define GREEN		"\x1b[32m"
#define BLUE		"\x1b[34m"
#define YELLOW		"\x1b[33m"
#define CYAN		"\x1b[36m"
#define MAGENTA		"\x1b[35m"
#define WHITE		"\x1b[37m"

#define SUPPORTED_FOR_ALL_MODES                      0xFFFF

#define HELP_HELP                "help:              Prints the help menu\r\n"
#define HELP_VER                 "ver:               Returns the SW and HW versions of the WES\r\n"
#define HELP_SET_MODE            "setmode:           Sets the operating mode of the device\r\n"
#define HELP_RESET               "reset:             Resets CPU\r\n"
#define HELP_NVM                 "readnvm:           Returns the current NVM info\r\n"
#define HELP_SET_ID              "setid:             Sets the ID of the device\r\n"
#define HELP_CLOCKS				 "clocks:            Reads the values of the internal clocks\r\n"
#define HELP_TEMP				 "temp:              Reads the internal temperature value of the device\r\n"
#define HELP_BATT				 "batt:              Reads the internal battery/supply voltoge of the device\r\n"
#define HELP_LISTNODES			 "listnodes:         Lists all of the connected sensor node IDs\r\n"
#define HELP_REMOVENODE          "removenode:        Removes a specific node from the list of connected nodes\r\n"
#define HELP_LED                 "led:               <0..3> <0..1> Controls the leds\r\n"
#define HELP_WL_ADD              "wladd:             Adds the ID of a node to the RF whitelist\r\n"
#define HELP_WL_REM              "wlremove:          Removes the selected pos from the RF whitelist\r\n"
#define HELP_WL_DISPLAY          "wldisplay:         Displays the entire RF whitelist\r\n"

#define NULL_PARAM_LIST { { 0, (u32_t)0x0000, (u32_t)0x0000 } }

#define SET_ID_CMD_PARAM_LIST { { 4, (u32_t)0x0001, (u32_t)0xFFFF } }

#define LED_CMD_PARAM_LIST { \
		{ 1, (u32_t)LED_0,(u32_t)(LED_MAX - 1) }, \
		{ 1, (u32_t)OFF,(u32_t)ON } }

#define WL_ADD_CMD_PARAM_LIST { { 4, (u32_t)1,(u32_t)(0xFFFF) } }

#define WL_REMOVE_CMD_PARAM_LIST { { 2,(u32_t)0x0000,(u32_t)RF_MGR_RF_DATA_HANDLER_SIZE } }

typedef enum
{
    CLI_ERROR_NONE = 0,
    CLI_ERROR_EMPTY,
    CLI_ERROR_FAILED,
    CLI_ERROR_INVALID_ARGS_MORE,
    CLI_ERROR_INVALID_ARGS,
    CLI_ERROR_NOT_SUPPORTED,
    CLI_ERROR_PROHIBITED,
    CLI_ERROR_NOT_FOUND,
    CLI_ERROR_PARAM_OUT_OF_RANGE,
    CLI_ERROR_INVALID_HEX_PARAM,
    CLI_ERROR_INVALID_DEC_PARAM,
    CLI_ERROR_INVALID_HEX_PARAM_LEN,
    CLI_ERROR_INVALID_DEC_PARAM_LEN,
    CLI_ALREADY_SET,
    CLI_ERROR_INVALID_STR_PARAM,
    CLI_ERROR_MAX
} CLI_error_et;

typedef struct
{
    u8_t cmd[CLI_MAX_INPUT_CHARS + 1u];
}CLI_cmd_st;


typedef struct
{
    u8_t index_to_next_cmd;
    CLI_cmd_st cmd_list[CLI_MAX_COMMAND_HISTORY];
}CLI_cmd_history_st;

typedef struct
{
    u8_t  num_chars;
    u32_t llimit;
    u32_t ulimit;
} CLI_Param_st;

typedef struct
{
    const char *command_name;
    CLI_error_et (*command_handler)(u8_t aArgCount, char *aArgVector[] );
    const char *helpinfo;
    u16_t availbility;  			// bit map setting of all modes in which this cmd would be supported
    disable_enable_et public;       // if this is TRUE it means this command is exposed to customers otherwise it is for developers
    u8_t num_params;
    CLI_Param_st param_list[CLI_CMD_LINE_ARGS_MAX];
} CLI_Command_st;



void CLI_init( void );
void CLI_message_handler( void );
void CLI_send_newline( void );
void CLI_print_welcome_message( void );
void CLI_handle_received_char( u8_t received_char );
void CLI_send_data( char* data, u16_t data_size );


void 		  CLI_handle_serial_command ( void );
false_true_et CLI_is_hex_char(char c);
false_true_et CLI_is_dec_char(char c);
false_true_et CLI_carriage_return_check( void );
void          CLI_clear_rx_buffer( void );
void          CLI_print_prompt( false_true_et newline );
false_true_et CLI_is_space_or_newLine( char c );
unsigned long long CLI_str_to_hex( const char* str );

CLI_error_et CLI_parse_cmd( char* message_string, u8_t* calc_argumen_count, char *argument_vector[], u8_t max_num_args );
CLI_error_et CLI_process_cmd( u8_t  aArgCount, char *aArgVector[] );
CLI_error_et CLI_validate_arguments( u8_t  aArgCount, char *aArgVector[], u8_t command_index );
void 		 CLI_store_history( void );



/* CLI handler functios */
CLI_error_et help_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et ver_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et setmode_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et reset_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et readnvm_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et setid_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et nvm_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et clocks_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et temp_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et batt_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et listnodes_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et removenode_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et led_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et wl_add_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et wl_remove_handler( u8_t aArgCount, char *aArgVector[] );
CLI_error_et wl_display_handler( u8_t aArgCount, char *aArgVector[] );


#endif
