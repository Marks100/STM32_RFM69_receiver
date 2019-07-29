#ifndef NVM_H
#define NVM_H

#include "C_defs.h"
#include "CLI.h"
#include "RF_MGR.h"
#include "NRF24.h"
#include "AIRCON.h"
#include "RFM69.h"

/***************************************************************************************************
**                              Defines                                                          **
***************************************************************************************************/
#define NVM_FLASH_PTR_START_ADDR     ((u32_t)0x801FC00)
#define NVM_FLASH_PTR_END_ADDR		 ((u32_t)0x801FFFF)

#define MAX_NUM_FLASH_BYTES       256u
#define CRC_BYTE                  255u


/***************************************************************************************************
**                              Structure Types                                                  **
***************************************************************************************************/

typedef enum NVM_module_state
{
    NVM_STATE_INITIALISING         = 0,
    NVM_STATE_INITIALISED          = 1,
    NVM_STATE_ERROR                = 2

} NVM_module_state_et;

typedef enum
{
    NO_EE_ERROR = 0,
    SEGMENT_TOO_SMALL,
    MISMATCHED_CRC,
    BYTE_ALLIGNMENT_FAIL,
}ee_error_code_et;


/**************************************************************************************************/
typedef enum
{
    NVM_VERS_GENERIC_DATA         = 8,
} NVM_blk_version_et;
/***************************************************************************************************
This is the version number of the NVM block, This needs to be incremented when changing the layout
the structure of the block or when adding new bytes in
***************************************************************************************************/


typedef struct
{
    u32_t  sleep_time;
    u8_t   tx_power_level;
    u16_t  device_id;
    RFM69_static_configuration_et rf_config;
    NRF24_state_et nrf_startup_tx_rx_mode;
    CLI_cmd_st cmd_list[CLI_MAX_COMMAND_HISTORY];
    u16_t rf_whitelist[RF_MGR_RF_DATA_HANDLER_SIZE];
    disable_enable_et whitelist_state;
    AIRCON_mode_et aircon_mode;
    disable_enable_et aircon_state;
    float cool_max_temp;
    float cool_min_temp;
    float heat_max_temp;
    float heat_min_temp;
    float auto_target_temp;

} NVM_generic_data_blk_st;


#pragma pack (1)
typedef struct
{
    u8_t checksum;
    u8_t version;
    u32_t write_count;
    NVM_generic_data_blk_st NVM_generic_data_blk_s;
} NVM_info_st;








/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
extern NVM_info_st NVM_info_s;


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
void NVM_init(void);
void NVM_request_flush(void);
NVM_module_state_et NVM_get_status( void );
void NVM_set_override_state( void );
void NVM_tick( void );
ok_nok_et NVM_check_blk_crc_and_version( void );
false_true_et NVM_populate_blk_crc_and_version( void );
false_true_et NVM_write_segment ( void );


#endif
