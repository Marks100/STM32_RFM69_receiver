
#ifndef HAL_UART_H
#define HAL_UART_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "COMPILER_defs.h"

/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#define RX_BUF_SIZE 80
#define TX_BUF_SIZE 100


/***************************************************************************************************
**                              Constants                                                         **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */



/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void SERIAL_init( void );
void SERIAL_Send_data(const char *pucBuffer);
void SERIAL_clear_RXBuffer(void);
void SERIAL_clear_TXBuffer(void);
void SERIAL_msg_handler( void );
void SERIAL_trigger_stream_output( void );



#endif /* HAL_UART_PUB_H multiple inclusion guard */

/****************************** END OF FILE *******************************************************/