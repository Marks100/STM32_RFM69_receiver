/*! \file
*               $Revision: 14979 $
*
*               $Author: ireid $
*
*               $Date: 2013-09-16 08:24:38 +0100 (Mon, 16 Sep 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/INC/trunk/TPMS_std.h $
*
*   \brief      Standard include file for TPMS
*   \note       Ensure PROJ_CONFIG.h contains:
*               (1) the number of wheels e.g. VEHICLE_TYPE_BIKE, VEHICLE_TYPE_CAR, VEHICLE_TYPE_GENERIC_6_WHEELER
*               (2) the type of sensor e.g. TX_TYPE_SEL_WAL, TX_TYPE_HYUNDAI
*               (3) the TX count resolution e.g. TX_RESOLUTION_50_MBAR
*
*/
/* COPYRIGHT NOTICE
* ==================================================================================================
*
* The contents of this document are protected under copyright and contain commercially and/or
* technically confidential information. The content of this document must not be used other than
* for the purpose for which it was provided nor may it be disclosed or copied (by the authorised
* recipient or otherwise) without the prior written consent of an authorised officer of Schrader
* Electronics Ltd.
*
*         (C) $Date:: 2013#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/
#ifndef TPMS_STD_H
#define TPMS_STD_H


/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "PROJ_config.h"
#include "TPMS_types.h"


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/*****************/
/* TPMS specific */
/*****************/

// This seems like it needs to be here as we have RF decode V2
// This is used in TPMS_APP.c when RF blocks are received
// Could we review the usage of this and tidy appropriately
typedef enum
{
    RF_SOURCE_LOCAL,
    RF_SOURCE_REMOTE_1,
    RF_SOURCE_REMOTE_2,
    RF_SOURCE_REMOTE_3,
    RF_SOURCE_REMOTE_4,
    RF_SOURCE_MAX
}TPMS_std_rf_source_et;

//! Axle Positions
typedef enum
{
    //! Front Axle
    AXLE_FRONT   = 0,
    //! Rear Axle
    AXLE_REAR    = 1,
    //! Unknown Axle
    AXLE_UNKNOWN = 2,
    AXLE_MAX     = 3
}axle_position_et;


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

/* PRQA S 3453 EOF */ // MISRA-C:2004 Rule 19.7 -> RunTime Optimisation

// Vehicle Types
#if( VEHICLE_TYPE_CAR )

    //! Number of wheels on the vehicle
    #define RTPMS_NUM_WHEELS    ( 4u )

    //! Number of axles on the vehicle
    #define RTPMS_NUM_AXLES     ( 2u )

    //! Wheel positions, always numbered from LF clockwise
    typedef enum
    {
        POS_LF          = 0,    //! Left Front wheel position
        POS_RF          = 1,    //! Right Front wheel position
        POS_RR          = 2,    //! Right Rear wheel position
        POS_LR          = 3,    //! Left Rear wheel position
        POS_SPARE       = 4,    //! Spare wheel position
        POS_UNALLOCATED = 5,    //! Not allocated wheel position
        POS_MAX         = 6
    }wheel_position_et;

    //! Side Positions
    typedef enum
    {
        SIDE_LEFT    = 0,       //! Left Side
        SIDE_RIGHT   = 1,       //! Right Side
        SIDE_UNKNOWN = 2,       //! Unknown Side
        SIDE_MAX     = 3
    }side_position_et;

/*!
****************************************************************************************************
*
*   \brief          Remaps wheel position to axle position
*
*   \author         IR
*
*   \param          wheel_position - wheel position to be mapped to axle position
*   \note           wheel_position must be < POS_SPARE
*
*   \return         axle_position - axle position that wheel maps onto
*
***************************************************************************************************/
#define map_wheel_position_to_axle( wheel_position )    (axle_position_et)( ( ( wheel_position ) < POS_SPARE ) ? ( ( wheel_position ) / 2u ) : AXLE_UNKNOWN )

/*!
****************************************************************************************************
*
*   \brief          Remaps axle position to wheel position for a given side
*
*   \author         IR
*
*   \param          axle - axle of wheel position to mapped ( must be < AXLE_UNKNOWN )
*   \param          side - side of wheel position to mapped ( must be < SIDE_UNKNOWN )
*
*   \return         wheel_position - wheel position that wheel maps onto
*
***************************************************************************************************/
#define map_axle_to_wheel_position( axle, side )        (wheel_position_et)( ( ( ( axle ) < AXLE_UNKNOWN ) && ( ( side ) < SIDE_UNKNOWN ) ) ? ( ( ( axle ) * 2 ) + ( side ) ) : POS_UNALLOCATED )

#elif( VEHICLE_TYPE_BIKE )

    // Wheel positions, always numbered from LF clockwise
    typedef enum
    {
        POS_FRONT       = 0,
        POS_REAR        = 1,
        POS_UNALLOCATED = 2,
        POS_MAX         = 3
    }wheel_position_et;

    //! Number of wheels on the vehicle
    #define RTPMS_NUM_WHEELS    ( 2u )

    //! Number of axles on the vehicle
    #define RTPMS_NUM_AXLES     ( 2u )

#elif( VEHICLE_TYPE_GENERIC_6_WHEELER )
    //! Number of wheels on the vehicle
    #define RTPMS_NUM_WHEELS    ( 6u )

    typedef enum
    {
        POS_ONE         = 0,
        POS_TWO         = 1,
        POS_THREE       = 2,
        POS_FOUR        = 3,
        POS_FIVE        = 4,
        POS_SIX         = 5,
        POS_UNALLOCATED = 6,
        POS_MAX         = 7
    }wheel_position_et;
    //! Number of axles on the vehicle
    #define RTPMS_NUM_AXLES     ( 6u )
#else
    #error "VEHICLE_TYPE_XXX.. not defined in PROJ_config.h"
#endif // #if( VEHICLE_TYPE_CAR )

// Tx Types
#if( TX_TYPE_SEL_WAL )

    // Tx Temperature offset & resolution
    #define TX_TEMPERATURE_OFFSET       ( 50 )
    #define TX_TEMPERATURE_RESOLUTUION  ( 1 )

    // Function codes used by the transmitters
    // least significant 3 bits
    typedef enum
    {
        FCODE_LEARN                 = 0x00,
        FCODE_EXITING_WAL           = 0x01,
        FCODE_ENTERING_OFF_MODE     = 0x02,
        FCODE_NORMAL_WAL_REMEASURE  = 0x03,
        FCODE_STATIONARY_REMEASURE  = 0x04,
        FCODE_RIGHT                 = 0x05,
        FCODE_LEFT                  = 0x06,
        FCODE_NORMAL_UNKNOWN        = 0x07,
        FCODE_MAX                   = 0x08
    }tx_function_codes_et;

    #define MODE_FUNCTION_CODE_MASK     ( 0x07 )

    #define TX_WAKE_SPEED_KPH           ( 30u )

    // define the data for the rest of the Function codes for a WAL protocol
    // most significant 5 bits
    #define LOW_BATTERY_MASK            ( 0x08 )
    #define BROKEN_SHOCK_SENSOR_MASK    ( 0x10 )
    #define WAL_MODE_MASK               ( 0x20 )
    #define TOO_SLOW_MASK               ( 0x40 )
    #define LONG_TERM_STATIONARY_MASK   ( 0x80 )
    #define LOW_CONF_MASK               ( 0x40 )

#elif( TX_TYPE_HYUNDAI )

    // Tx Temperature offset & resolution
    #define TX_TEMPERATURE_OFFSET       ( 50u )   //TODO: CHECK THIS
    #define TX_TEMPERATURE_RESOLUTUION  ( 1u )

    // Function codes used by the transmitters
    // least significant 3 bits
    typedef enum
    {
        FCODE_EXIT_OFF              = 0x00,     // Exit Off Mode
        FCODE_REMEASURE             = 0x01,     // Alert/Delta P Mode
        FCODE_TEST                  = 0x02,     // Test Mode
        FCODE_ENTERING_OFF_MODE     = 0x03,     // Off Mode
        FCODE_LEFT                  = 0x04,     // Left
        FCODE_RIGHT                 = 0x05,     // Right
        FCODE_NORMAL_UNKNOWN        = 0x06,     // Normal/Unknown
        FCODE_MAX                   = 0x08
    }tx_function_codes_et;

    #define STATUS_CODE_MASK                ( 0x07 )
    #define MODE_FUNCTION_CODE_MASK         ( 0x07 )
    #define TX_WAKE_SPEED_KPH               ( 30u )

    // define the data for the rest of the Function codes for a WAL protocol
    // most significant 5 bits
    #define LOW_BATTERY_MASK                ( 0x80 )
    #define BROKEN_SHOCK_SENSOR_MASK        ( 0x40 )
    #define LF_RESPONSE_MASK                ( 0x20 )
    #define WAL_MODE_MASK                   ( 0x10 )
    #define ROLLING_MODE_MASK               ( 0x08 )

    #define SEL_WAL_FCODE_RIGHT             ( 0x25 )
    #define SEL_WAL_FCODE_LEFT              ( 0x26 )
    #define SEL_WAL_UNKNOWN                 ( 0x27 )
    #define SEL_WAL_NORMAL                  ( 0x07 )
    #define SEL_WAL_NORMAL_WAL_REMEASURE    ( 0x03 )
    #define SEL_WAL_STATIONARY_REMEASURE    ( 0x04 )
    #define SEL_WAL_LEARN                   ( 0x01 )

#elif( TX_TYPE_OTR_EZ_SENSOR )

    // Tx Temperature offset & resolution
    #define TX_TEMPERATURE_OFFSET           ( 50 )
    #define TX_TEMPERATURE_RESOLUTUION      ( 1 )

    // Function codes used by the transmitters
    // least significant 3 bits
    typedef enum
    {
        FCODE_LEARN                 = 0x00,
        FCODE_LOW_BATTERY           = 0x01,
        FCODE_ENTERING_OFF_MODE     = 0x02,
        FCODE_REMEASURE             = 0x03,
        //FCODE_RESERVED            = 0x04,
        FCODE_ENTERING_FREQ_TX_MODE = 0x05,
        FCODE_EXITING_OFF_MODE      = 0x06,
        FCODE_NORMAL_MODE           = 0x07,
        FCODE_MAX                   = 0x08
    }tx_function_codes_et;

    #define MODE_FUNCTION_CODE_MASK         ( 0x07 )

    #define TX_WAKE_SPEED_KPH               ( 30u )

#elif( TX_TYPE_SEL_GEN_2 || TX_TYPE_SEL_GEN_3 || TX_TYPE_SEL_GEN_5 )

    #if( TX_TYPE_SEL_GEN_3 || TX_TYPE_SEL_GEN_5 )
        // Tx Temperature offset & resolution
        #define TX_TEMPERATURE_OFFSET       ( 50 )
        #define TX_TEMPERATURE_RESOLUTUION  ( 1 )
    #endif

    #define TX_WAKE_SPEED_KPH               ( 30u )

    // Function codes used by the transmitters
    // least significant 3 bits
    typedef enum
    {
        FCODE_LEARN                 = 0x00,
        FCODE_LOW_BATTERY           = 0x01,
        FCODE_ENTERING_OFF_MODE     = 0x02,
        FCODE_REMEASURE             = 0x03,
        FCODE_LINE_1                = 0x04,
        FCODE_WAKE                  = 0x05,
        FCODE_LINE_2                = 0x06,
        FCODE_NORMAL_PRESSURE       = 0x07,
        FCODE_MAX                   = 0x08
    }tx_function_codes_et;

    #define MODE_FUNCTION_CODE_MASK     0x07

#elif( TX_TYPE_GM_COMM_ARCH )
/*!
****************************************************************************************************
*
*   \brief          GM Comm Architecture
*
*   \author         PW
*
*   \note           Sensor specification is found in various places, note exceptions list is important
*                   [1] https://selacvs01.schrader.local:8443/svn/ECU_Software/GM/OPEL/Corsa/trunk/DOCS/1_Ref_Docs/1-2_Customer_Docs/1-2-3_Misc/CTS1 6 Exceptions 071219.pdf
*                   [2] https://selacvs01.schrader.local:8443/svn/ECU_Software/GM/OPEL/Corsa/trunk/DOCS/1_Ref_Docs/1-2_Customer_Docs/1-2-3_Misc/Schrader Global A MY09 Sensor Spec v02
*                   [3] https://selacvs01.schrader.local:8443/svn/ECU_Software/GM/OPEL/Corsa/trunk/DOCS/1_Ref_Docs/1-2_Customer_Docs/1-2-3_Misc/CTS_Global_A_Tire_Pressure_Sensor_V4_0_Final
*
***************************************************************************************************/
    // Tx Temperature offset & resolution
    #define TX_TEMPERATURE_OFFSET           ( 50 )      //! Temperature Offset
    #define TX_TEMPERATURE_RESOLUTUION      ( 1 )       //! Temperature Resolution
    #define TX_WAKE_SPEED_KPH               ( 30u )     //! Wake speed for sensor  (Ref [2], says 20kph; Ref [3], says 30kph)

    // least significant 4 bits
    typedef enum
    {
        FCODE_LEARN                 = 0x00,
        FCODE_UNDEFINED_0           = 0x01,
        FCODE_REMEASURE             = 0x02,
        FCODE_ROLLING               = 0x03,
        FCODE_ENTERING_OFF_MODE     = 0x04,
        FCODE_EXITING_OFF_MODE      = 0x05,
        FCODE_PRESSURE_TRIGGERED    = 0x06,
        //FCODE_RESERVED           = 0x07,
        //FCODE_RESERVED           = 0x08,
        //FCODE_RESERVED           = 0x09,
        //FCODE_RESERVED           = 0x0A,
        //FCODE_RESERVED           = 0x0B,
        //FCODE_RESERVED           = 0x0C,
        //FCODE_RESERVED           = 0x0D,
        //FCODE_RESERVED           = 0x0E,
        //FCODE_RESERVED           = 0x0F,
        FCODE_MAX                   = 0x10
    }TPMS_tx_function_codes_et;                        //! Ref [3], Page 22.

    #define MODE_FUNCTION_CODE_MASK     ( 0x0F )

    // most significant 4 bits
    #define L_R_POSITION_MSB            ( 0x80 )       //! Ref [3], Page 21
    #define TRANSMITTER_FAULT           ( 0x40 )
    #define LOW_BATTERY_MASK            ( 0x20 )
    #define L_R_POSITION_LSB            ( 0x10 )

    // Position     b7  b4
    // Unknown      0   0
    // Right (CW)   0   1
    // Left (CCW)   1   0
    // Not used     1   1

    #define FCODE_RIGHT             ( 0x05 )
    #define FCODE_LEFT              ( 0x06 )
    #define FCODE_NORMAL_UNKNOWN    ( 0x07 )

    #define SEL_WAL_FCODE_RIGHT     ( 0x25 )
    #define SEL_WAL_FCODE_LEFT      ( 0x26 )
    #define SEL_WAL_UNKNOWN         ( 0x27 )
    #define SEL_WAL_NORMAL          ( 0x07 )

    #define WAL_MODE_BIT_MASK       ( 0x01 )
    #define WAL_MODE_MASK           ( 0x20 )

// Function code remaps for purposes of compatibly
#define FCODE_NORMAL_WAL_REMEASURE  FCODE_REMEASURE
#define FCODE_STATIONARY_REMEASURE  FCODE_REMEASURE
#define BROKEN_SHOCK_SENSOR_MASK    ( 0x00 )

#elif( TX_TYPE_DCX_COMM_ARCH )

    // Nothing to define yet!

#elif( TX_TYPE_RENAULT )

    // Nothing to define yet!

#elif( TX_TYPE_CHRYSLER_PAL )

    // Tx Temperature offset & resolution
    #define TX_TEMPERATURE_OFFSET       ( 50 )
    #define TX_TEMPERATURE_RESOLUTUION  ( 1 )

    // Function codes used by the transmitters
    // least significant 3 bits
    typedef enum
    {
        FCODE_LEARN                 = 0x00,
        FCODE_NOT_USED              = 0x01,
        FCODE_ENTERING_OFF_MODE     = 0x02,
        FCODE_NORMAL_PAL_REMEASURE  = 0x03,
        FCODE_STATIONARY_REMEASURE  = 0x04,
        FCODE_PAL_SYNCHED           = 0x05,
        FCODE_PAL_NO_SYNC           = 0x06,
        FCODE_NORMAL_UNKNOWN        = 0x07,
        FCODE_MAX                   = 0x08
    }tx_function_codes_et;

    #define MODE_FUNCTION_CODE_MASK     ( 0x07 )

    #define TX_WAKE_SPEED_KPH           ( 20u )

    // define the data for the rest of the Function codes for a PAL protocol
    // most significant 5 bits
    #define LOW_BATTERY_MASK            ( 0x08 )
    #define BROKEN_SHOCK_SENSOR_MASK    ( 0x10 )
    #define SPARE_MASK                  ( 0x20 )
    #define TOO_SLOW_MASK               ( 0x40 )
    #define PAL_MODE_MASK               ( 0x80 )
    #define LOW_CONF_MASK               ( 0x40 )

#elif( TX_TYPE_AK_RDK_PAL )

    // Tx Temperature offset & resolution

    //! \todo confirm the following settings
    #define TX_TEMPERATURE_OFFSET       ( 52 )
    #define TX_TEMPERATURE_RESOLUTUION  ( 1 )
    #define TX_WAKE_SPEED_KPH           ( 20u )

    typedef enum
    {
        FCODE_LEARN                 = 0x00,
        FCODE_NORMAL_DRIVE          = 0x01,
        FCODE_ENTERING_OFF_MODE     = 0x02,
        FCODE_DRIVE_REMEASURE       = 0x03,
        FCODE_STATIONARY_REMEASURE  = 0x04,
        FCODE_PAL_OUT_OF_RANGE      = 0x05,
        FCODE_PAL_SYNC              = 0x06,
        FCODE_PAL_NO_SYNC           = 0x07,
        FCODE_MAX                   = 0x08
    }tx_function_codes_et;

    #define MODE_FUNCTION_CODE_MASK     ( 0x07 )

    // define the data for the rest of the Function codes for a PAL protocol
    // most significant 5 bits
    #define LOW_BATTERY_MASK            ( 0x08 )
    #define BATTERY_LOAD_TEST_COUNT     ( 0x30 )

#elif( TX_TYPE_AK_RDK_SEL_VCC_PAL )

    // Tx Temperature offset & resolution

    //! \todo confirm the following settings
    #define TX_TEMPERATURE_OFFSET       ( 52 )
    #define TX_TEMPERATURE_RESOLUTUION  ( 1 )
    #define TX_WAKE_SPEED_KPH           ( 20u )

    typedef enum
    {
       OFF              = 0x0, // Off
       STATIONARY       = 0x1, // Stationary
       WAKE             = 0x2, // Wake
       PAL              = 0x3, // PAL
       NORMAL_DRIVE     = 0x4, // Normal (Drive)
       SERVICE          = 0x5, // Service
       RESERVED1        = 0x6, // Reserved
       RESERVED2        = 0x7  // Reserved
    }sensor_mode_et;

#else
    #error "TX_TYPE_XXX.. not defined in PROJ_config.h"
#endif // #if TX_TYPE_SEL_WAL


/*!
****************************************************************************************************
*
*   \brief          Pressure in the database is defined in Tx counts which may
*                   be in units e.g. 25 or maybe a decimal value e.g. 27.5
*
*   \author         PW
***************************************************************************************************/
// now define the Transmitter resolution
#ifdef TX_RESOLUTION_50_MBAR
    #define TX_PRESSURE_RESOLUTION_DIVIDEND   ( 50u )   //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
    #define TX_PRESSURE_RESOLUTION_DIVISOR    ( 1u )    //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
#elif TX_RESOLUTION_25_MBAR
    #define TX_PRESSURE_RESOLUTION_DIVIDEND   ( 25u )   //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
    #define TX_PRESSURE_RESOLUTION_DIVISOR    ( 1u )    //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
#elif TX_RESOLUTION_22_MBAR
    #define TX_PRESSURE_RESOLUTION_DIVIDEND   ( 22u )   //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
    #define TX_PRESSURE_RESOLUTION_DIVISOR    ( 1u )    //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
#elif TX_RESOLUTION_20_MBAR
    #define TX_PRESSURE_RESOLUTION_DIVIDEND   ( 20u )   //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
    #define TX_PRESSURE_RESOLUTION_DIVISOR    ( 1u )    //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
#elif TX_RESOLUTION_17_MBAR
    #define TX_PRESSURE_RESOLUTION_DIVIDEND   ( 17u )   //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
    #define TX_PRESSURE_RESOLUTION_DIVISOR    ( 1u )    //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
#elif TX_RESOLUTION_27_5_MBAR
    #define TX_PRESSURE_RESOLUTION_DIVIDEND   ( 55u )   //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
    #define TX_PRESSURE_RESOLUTION_DIVISOR    ( 2u )    //! TX_RESOLUTION = TX_PRESSURE_RESOLUTION_DIVIDEND/TX_PRESSURE_RESOLUTION_DIVISOR
#else
    #error  "No pressure resolution of Transmitter defined in PROJ_config.h"
#endif

/*!
****************************************************************************************************
*
*   \brief          macro for converting pressure into TX counts
*
*   \author         PW
*
*   \param          pressure - units are TX counts according to the
*                     resolution defined above.
*
*   \return         pressure - units are millibar (positive)
*
*   \note           beware of any rounding errors that are not acceptable.
*                   TPMS_TX2MBAR(MBAR(pressure)) does not always return the same input
*                   e.g. if resolution is 25 then
*                   TPMS_TX2MBAR(MBAR(2500)) = 2500
*                   TPMS_TX2MBAR(MBAR(2495)) = 2475
*
***************************************************************************************************/
#define MBAR(pressure)         ( (u32_t)( ((pressure) * (TX_PRESSURE_RESOLUTION_DIVISOR)) / (TX_PRESSURE_RESOLUTION_DIVIDEND) ))

/*!
****************************************************************************************************
*
*   \brief          macro for converting TX counts into pressure
*
*   \author         PW
*
*   \param          pressure - units are millibar.
*
*   \return         pressure - units are TX counts according to the
*                     resolution defined above.
*
*   \note           beware of any rounding errors that are not acceptable.
*                   TPMS_TX2MBAR(MBAR(pressure)) does not always return the same input
*                   e.g. if resolution is 25 then
*                   TPMS_TX2MBAR(MBAR(2500)) = 2500
*                   TPMS_TX2MBAR(MBAR(2495)) = 2475
***************************************************************************************************/
#define TPMS_TX2MBAR(pressure) (SELMATH_divide_u16_with_rounding(((pressure) * TX_PRESSURE_RESOLUTION_DIVIDEND ), TX_PRESSURE_RESOLUTION_DIVISOR))

/*!
****************************************************************************************************
*
*   \brief          (temporary) macro used to give pressure resolution
*
*   \author         PW
*
*   \note           Use sparingly as this macro is about to be made obsolete.
*                   It does not work for non integer values
*
***************************************************************************************************/
#define TX_PRESSURE_RESOLUTION ( (u16_t) (TX_PRESSURE_RESOLUTION_DIVIDEND / TX_PRESSURE_RESOLUTION_DIVISOR))

// removed macros for kPa and PSI, since want to use one common unit throughout software

/*!
****************************************************************************************************
*
*   \brief          macro used to convert from temperature values to counts
*
*   \author         PW
*
*   \param          temp_degc - temperature in degree C
*
*   \return         temperature counts - temp_degC plus its offset
*
*   \note           only valid if temperature >= -50degC && <= 215degC
*
***************************************************************************************************/
#define CONVERT_DEGC_TX(temp_degc)       (u8_t) (   (s16_t)(temp_degc) + TX_TEMPERATURE_OFFSET )

/*!
****************************************************************************************************
*
*   \brief          macro used to convert from counts to temperature values
*
*   \author         PW
*
*   \param          temp_tx - temperature counts
*
*   \return         temperature in degree C  - temp_degC minus its offset
*
*   \note           only valid if temperature >= 0 && <=255
*
***************************************************************************************************/
#define CONVERT_TX_DEGC(temp_tx)         (s16_t)(   (s16_t)(temp_tx) - TX_TEMPERATURE_OFFSET )

/*!
****************************************************************************************************
*
*   \brief          macro used to convert from temperature Kelvin values to counts
*
*   \author         PW
*
*   \param          temp_kelvin - temperature in Kelvin
*
*   \return         temperature counts - temp_kelvin plus its offset  minus 273
*
*   \note           only valid if temperature >= 0 && <=255
*
***************************************************************************************************/
#define CONVERT_KELVIN_TX(temp_kelvin)   (u8_t) (   ( (s16_t)(temp_kelvin) + TX_TEMPERATURE_OFFSET ) - 273 )

/*!
****************************************************************************************************
*
*   \brief          macro used to convert from counts to temperature values
*
*   \author         PW
*
*   \param          temp_tx - temperature counts
*
*   \return         temperature in Kelvin  - kelvin minus its offset plus 273
*
*   \note           only valid if temperature >= 0 && <=255
*
***************************************************************************************************/
#define CONVERT_TX_KELVIN(temp_tx)       (u16_t)(   ( (s16_t)(temp_tx) - TX_TEMPERATURE_OFFSET ) + 273 )

/*!
****************************************************************************************************
*
*   \brief          macro used to convert from degree C to Kelvin
*
*   \author         PW
*
*   \param          temp_degc - temperature in degree C (may be negative)
*
*   \return         temperature in Kelvin  - add 273 to degree C  (always position)
*
***************************************************************************************************/
#define CONVERT_DEGC_KELVIN(temp_degc)   (u16_t)( (s16_t)(temp_degc)   + 273 )

/*!
****************************************************************************************************
*
*   \brief          macro used to convert from kelvin to degree C
*
*   \author         PW
*
*   \param          temp_kelvin - temperature in kelvin
*
*   \return         temperature in degree ( may result in negative number)
*
*
***************************************************************************************************/
#define CONVERT_KELVIN_DEGC(temp_kelvin) (s16_t)( (s16_t)(temp_kelvin) - 273 )




/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None allowed */




/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
/* None allowed */



#endif /* TPMS_STD_H */
/****************************** END OF FILE *******************************************************/
