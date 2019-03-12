#ifndef CLI_H
#define CLI_H




#define CLI_MAX_INPUT_CHARS           60u
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
#define NULL_PARAM_LIST               //{0}
#define IGNORE_RANGE_CHECK            ((u32_t)~0)

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
    u8_t cmd[CLI_MAX_INPUT_CHARS];
}CLI_cmd_st;


typedef struct
{
    u8_t index_to_next_cmd;
    CLI_cmd_st cmd_list[CLI_MAX_COMMAND_HISTORY];
}CLI_cmd_history_st;




void CLI_init( void );
void CLI_message_handler( void );
void CLI_send_newline( void );
void CLI_print_welcome_message( void );

STATIC false_true_et CLI_is_hex_char(char c);
STATIC false_true_et CLI_is_dec_char(char c);
STATIC false_true_et CLI_carriage_return_check( void );
STATIC void          CLI_clear_rx_buffer( void );
STATIC void          CLI_print_prompt( false_true_et newline );
STATIC false_true_et CLI_is_space_or_newLine( char c );
STATIC unsigned long long CLI_str_to_hex( const char* str );

void CLI_handle_received_char( u8_t received_char );



#endif
