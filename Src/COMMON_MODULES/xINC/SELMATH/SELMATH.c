/*! \file
*               $Revision: 16068 $
*
*               $Author: ireid $
*
*               $Date: 2013-11-18 08:44:26 +0000 (Mon, 18 Nov 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/LIBS/trunk/SELMATH/SELMATH.c $
*
*   \brief      Schrader mathematics module
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
/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "SELMATH.h"
#include "STDC.h"
#include "TPMS_types.h"
#include "SELMATH_pri.h"

/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
/* define Module Id for purposes of assert functionality */
#define STDC_MODULE_ID   STDC_MOD_SELMATH



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/
/*!
****************************************************************************************************
*
*   \brief      absolute difference between two values
*
*   \author     IR
*
*   \param      x - number 1, y - number 2
*
*   \return     difference
*
***************************************************************************************************/
u8_t SELMATH_abs( u8_t x, u8_t y )
{
    u8_t ret_val;

    if ( x > y )
    {
        ret_val = ( u8_t )( x - y );
    }
    else
    {
        ret_val = ( u8_t )( y - x );
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          absolute difference between two word values
*
*   \author         IR
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         difference
*
***************************************************************************************************/
u16_t SELMATH_abs_word( u16_t x, u16_t y )
{
    u16_t ret_val;

    if ( x > y )
    {
        ret_val = ( x - y );
    }
    else
    {
        ret_val = ( y - x );
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          absolute difference between two long values
*
*   \author         JD
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         ret_val - difference
*
***************************************************************************************************/
u32_t SELMATH_abs_long( u32_t x, u32_t y )
{
    u32_t ret_val;

    if ( x > y )
    {
        ret_val = ( x - y );
    }
    else
    {
        ret_val = ( y - x );
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          returns larger of two u8_t's
*
*   \author         IR
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         Maximum of two numbers
*
***************************************************************************************************/
u8_t SELMATH_max_u8_t( u8_t x, u8_t y )
{
    u8_t ret_val;

    if ( x > y )
    {
        ret_val = x;
    }
    else
    {
        ret_val = y;
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          returns larger of two u16_t's
*
*   \author         IR
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         Maximum of two numbers
*
***************************************************************************************************/
u16_t SELMATH_max_u16_t( u16_t x, u16_t y )
{
    u16_t ret_val;

    if ( x > y )
    {
        ret_val = x;
    }
    else
    {
        ret_val = y;
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          returns larger of two u32_t's
*
*   \author         DL
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         Maximum of two numbers
*
***************************************************************************************************/
u32_t SELMATH_max_u32_t( u32_t x, u32_t y )
{
    u32_t ret_val;

    if ( x > y )
    {
        ret_val = x;
    }
    else
    {
        ret_val = y;
    }

    return( ret_val );
}


/*!
****************************************************************************************************
*
*   \brief          returns smaller of two u8_t's
*
*   \author         IR
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         Minimum of two numbers
*
***************************************************************************************************/
u8_t SELMATH_min_u8_t( u8_t x, u8_t y )
{
    u8_t ret_val;

    if ( x < y )
    {
        ret_val = x;
    }
    else
    {
        ret_val = y;
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          returns smaller of two u16_t's
*
*   \author         IR
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         Minimum of two numbers
*
***************************************************************************************************/
u16_t SELMATH_min_u16_t( u16_t x, u16_t y )
{
    u16_t ret_val;

    if ( x < y )
    {
        ret_val = x;
    }
    else
    {
        ret_val = y;
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          returns smaller of two u32_t's
*
*   \author         IR
*
*   \param          x - number 1
*   \param          y - number 2
*
*   \return         Minimum of two numbers
*
***************************************************************************************************/
u32_t SELMATH_min_u32_t( u32_t x, u32_t y )
{
    u32_t ret_val;

    if ( x < y )
    {
        ret_val = x;
    }
    else
    {
        ret_val = y;
    }

    return( ret_val );
}

/*!
****************************************************************************************************
*
*   \brief          Function to provide the square root of a number
*
*   \author         IR
*
*   \param          The number to sqaure root
*
*   \return         quotient - the result of division with rounding
*
***************************************************************************************************/
u8_t  SELMATH_sqrt( u8_t number_to_square_root )
{
    u8_t index;

    // divide number_to_square_root by 32 to get index into square_root_table
    index = ( u8_t )( number_to_square_root >> 5u );

    return( square_root_table_s[index] );
}


/*!
****************************************************************************************************
*
*   \brief          Divides the dividend by the divisor, and rounds the result to the nearest integer (unsigned 8bit version)
*
*   \author         IR
*
*   \param          dividend - the number to be divided
*   \param          divisor - the divisor
*
*   \return         quotient - the result of division with rounding
*   \return         full scale - on attempt to divide by zero
*
***************************************************************************************************/
u8_t SELMATH_divide_u8_with_rounding( u8_t dividend, u8_t divisor )
{
    u8_t modified_dividend, result_with_rounding;

    // check for divide by zero
    if( divisor == 0 )
    {
        //STDC_assert( 1 );

        // set to full scale to indicate an error
        result_with_rounding = U8_T_MAX;
    }
    else
    {
        // Add half of divisor onto dividend, so that final result already is rounded up or down
        modified_dividend = (u8_t)( dividend + ( divisor / 2u ) );
        result_with_rounding = (u8_t)( modified_dividend / divisor );
    }

    return( result_with_rounding );
}

/*!
****************************************************************************************************
*
*   \brief          Divides the dividend by the divisor, and rounds the result to the nearest integer (unsigned 16bit version)
*
*   \author         IR
*
*   \param          dividend - the number to be divided
*   \param          divisor - the divisor
*
*   \return         quotient - the result of division with rounding
*   \return         full scale - on attempt to divide by zero
*
***************************************************************************************************/
u16_t SELMATH_divide_u16_with_rounding( u16_t dividend, u16_t divisor )
{
    u16_t modified_dividend, result_with_rounding;

    // check for divide by zero
    if( divisor == 0 )
    {
        //STDC_assert( 1 );

        // set to full scale to indicate an error
        result_with_rounding = U16_T_MAX;
    }
    else
    {
        // Add half of divisor onto dividend, so that final result already is rounded up or down
        modified_dividend = dividend + ( divisor / 2u );
        result_with_rounding = modified_dividend / divisor;
    }

    return( result_with_rounding );
}

/*!
****************************************************************************************************
*
*   \brief          Divides the dividend by the divisor, and rounds the result to the nearest integer (unsigned 32bit version)
*
*   \author         IR
*
*   \param          dividend - the number to be divided
*   \param          divisor - the divisor
*
*   \return         quotient - the result of division with rounding
*   \return         full scale - on attempt to divide by zero
*
***************************************************************************************************/
u32_t SELMATH_divide_u32_with_rounding( u32_t dividend, u32_t divisor )
{
    u32_t result_with_rounding;

    // check for divide by zero
    if( divisor == 0 )
    {
        //STDC_assert( 1 );

        // set to full scale to indicate an error
        result_with_rounding = U32_T_MAX;
    }
    else
    {
#if( STDC_EASY_TO_UNDERSTAND_ROUNDING == 1u )
        u32_t result_without_rounding, remainder;

        result_without_rounding = dividend / divisor;

        // check if remainder was > than half of divisor
        remainder = dividend % divisor;
        if( remainder > ( divisor / 2ul ) )
        {
            // round up to next integer
            result_with_rounding = result_without_rounding + 1ul;
        }
        else
        {
            // leave result as is
            result_with_rounding = result_without_rounding;
        }
#else // Superduper rounding method, equivilent to above simple to understand method, however a lot more effiecent in size and speed
        u32_t modified_dividend;

        // Add half of divisor onto dividend, so that final result already is rounded up or down
        modified_dividend = dividend + ( divisor / 2ul );
        result_with_rounding = modified_dividend / divisor;

#endif // #if( STDC_EASY_TO_UNDERSTAND_ROUNDING == 1u )

    }

    return( result_with_rounding );
}

/*!
****************************************************************************************************
*
*   \brief          ANSI C Linear congruential generator
*
*   \note           See http://en.wikipedia.org/wiki/Linear_congruential_generator
*
*   \author         IR
*
*   \param          current_value - Current value in LCG sequence
*
*   \return         next_value - Next value in LCG sequence
*
***************************************************************************************************/
u32_t SELMATH_lcg( u32_t current_value )
{
    u32_t next_value;

    next_value = current_value * LCG_MULTIPLIER;
    next_value += LCG_INCREMENT;

    return( next_value );
}

/*!
****************************************************************************************************
*
*   \brief          generates a seed, from some initial seeds
*
*   \author         IR
*
*   \param          initial_seeds[SELMATH_NUM_INDIVIDUAL_SEEDS] - Array of initial seeds
*   \param          generated_seeds[SELMATH_NUM_INDIVIDUAL_SEEDS] - Array of generated seeds
*
*   \return         None
*
***************************************************************************************************/
void SELMATH_generate_seed( SELMATH_seed_t initial_seeds[SELMATH_NUM_INDIVIDUAL_SEEDS], SELMATH_seed_t generated_seeds[SELMATH_NUM_INDIVIDUAL_SEEDS] )
{
    u8_t i;
    false_true_et seed_valid;

    // Set generated seed to all 0x00's to indicate that seed is invalid
    STDC_memset( &generated_seeds[0], 0x00, SELMATH_SEED_SIZE );

    // Validate input paras
    seed_valid = TRUE;
    for( i = 0; i < SELMATH_NUM_INDIVIDUAL_SEEDS; i++ )
    {
        if( initial_seeds[ i ] == 0 )
        {
            // initial seed is invalid
            //STDC_assert( 1 );
            seed_valid = FALSE;
        }
    }

    if( seed_valid == FALSE )
    {
        // invalid initial seeds
        //STDC_assert( 1 );
    }
    else
    {
        u8_t j, attempts;

        // Attempt SELMATH_GENERATE_SEED_MAX_ATTEMPTS times
        for( attempts = 0; attempts < SELMATH_GENERATE_SEED_MAX_ATTEMPTS; attempts++ )
        {
            // work through SELMATH_GENERATE_SEED_NUM_LCG_STEPS steps of the linear congruential generator
            for( i = 0; i < SELMATH_GENERATE_SEED_NUM_LCG_STEPS; i++ )
            {
                for( j = 0; j < SELMATH_NUM_INDIVIDUAL_SEEDS; j++ )
                {
                    initial_seeds[ j ] = SELMATH_lcg( initial_seeds[ j ] );
                }
            }

            // check if generated seeds are valid (all 0x00's or 0xFF's are invalid
            seed_valid = TRUE;
            for( j = 0; j < SELMATH_NUM_INDIVIDUAL_SEEDS; j++ )
            {
                if( ( initial_seeds[ j ] == 0 ) || ( initial_seeds[ j ] == 0xFFFFFFFF ) )
                {
                    seed_valid = FALSE;
                    break;
                }
            }

            // check if generated seeds are valid (all 0x00's or 0xFF's are invalid
            if( seed_valid == TRUE )
            {
                // copy generated key
                for( j = 0; j < SELMATH_NUM_INDIVIDUAL_SEEDS; j++ )
                {
                    generated_seeds[ j ] = initial_seeds[ j ];
                }

                // exit attempt loop
                break;
            }
            else
            {
                // invalid generated key, continue with next attempt, if all attempts fail, then generated_seed[] will return default value
            }
        }
    }

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Calculate a key from a number of given seeds
*
*   \author         IR
*
*   \param          seeds[SELMATH_NUM_INDIVIDUAL_SEEDS] - Array of seeds that key is to be generated from
*   \param          constant
*
*   \return         calculated_key - the calculated key
*
***************************************************************************************************/
SELMATH_key_t SELMATH_calculate_key( SELMATH_seed_t seeds[SELMATH_NUM_INDIVIDUAL_SEEDS], u32_t constant )
{
    SELMATH_key_t calculated_key;
    u8_t i;
    false_true_et seed_valid;

    // Validate input paras
    seed_valid = FALSE;
    for( i = 0; i < SELMATH_NUM_INDIVIDUAL_SEEDS; i++ )
    {
        if( ( seeds[ i ] != 0 ) || ( seeds[ i ] != INVALID_SEED_MAX ) )
        {
            // initial seed is valid
            seed_valid = TRUE;
        }
    }

    // validate seed
    if( seed_valid == FALSE )
    {
        // invalid key being used
        //STDC_assert( 1 );
        calculated_key = 0;
    }
    else
    {
        // seed is valid... calculate next LCG step for each of the seeds
        for( i = 0; i < SELMATH_NUM_INDIVIDUAL_SEEDS; i++ )
        {
            seeds[ i ] = SELMATH_lcg( seeds[ i ] );
        }

        // calculated key is XOR of seeds and constant
        calculated_key = constant;
        for( i = 0; i < SELMATH_NUM_INDIVIDUAL_SEEDS; i++ )
        {
            calculated_key ^= seeds[ i ];
        }
    }

    return( calculated_key );
}

/*!
****************************************************************************************************
*
*   \brief          Calculates a temperature compensated pressure assuming the ideal gas law.
*                   At this temperature what will this current pressure be
*
*                   This function converts a raw pressure from atmospheric to absolute pressure.
*
*   \author         JD / IR
*
*   \param          raw_pressure in mbar
*   \param          raw_temperature in degC
*
*   \return         temperature compensated pressure in mbar
*
***************************************************************************************************/
mbar_u16_t SELMATH_calculate_tc_pressure( mbar_u16_t raw_pressure, degrees_c_s16_t raw_temperature, degrees_c_s16_t ref_temperature )
{
    degrees_k_u16_t  Temperature_nom_kelvin;
    degrees_k_u16_t  Rx_temperature_kelvin;      // Rx temperature in Kelvin
    mbar_u32_t       Rx_pressure_adjusted;
    mbar_u32_t       TC_pressure;                // pressure at nomimal temperature

    // Validate input paras
    if( ( raw_pressure == MBAR_U16_INVALID ) || ( raw_temperature == DEGREES_C_S16_INVALID ) || (ref_temperature == DEGREES_C_S16_INVALID )  )
    {
        // unable to calculate temperature compensated pressure
        TC_pressure = MBAR_U16_INVALID;
    }
    else
    {
        // need to adjust rx_pressure to compensate for atmospheric pressure
        Rx_pressure_adjusted = (mbar_u32_t)raw_pressure + DEFAULT_ATMOSPHERIC_PRESSURE_MBAR;

        // need to convert rx_temperature to kelvin
        Rx_temperature_kelvin = (u16_t)( raw_temperature + 273 );

        // convert reference temperature to kelvin
        Temperature_nom_kelvin = (u16_t)( ref_temperature + 273 );

        // Pressure nom = (Pressure rx * Temperature nom) / Temperature rx
        /* To ensure we have good accuracy i.e. 2 decimal places. We will multiply  */
        /* the pressure up by 100 and add 1/2 millibar i.e. 50.  Then divide by 100 */
        /* again.  This will allow truncation to the nearest millibar as defined in */
        /* the specification.                                                       */
        TC_pressure = ( u32_t )( ( u32_t )( Rx_pressure_adjusted * 100ul ) * ( u32_t )Temperature_nom_kelvin ) / ( u32_t )Rx_temperature_kelvin;
        TC_pressure = ( TC_pressure + 50ul )/100ul;

        // subtract back atmospheric pressure 1 bar provided an underflow isn't going to occur
        if( TC_pressure >= DEFAULT_ATMOSPHERIC_PRESSURE_MBAR )
        {
            TC_pressure -= DEFAULT_ATMOSPHERIC_PRESSURE_MBAR;
        }
        else
        {
            TC_pressure = 0u;
        }

        // check for overflow
        if( TC_pressure > MBAR_U16_INVALID )
        {
            TC_pressure = MBAR_U16_INVALID;
        }
    }

    return( (mbar_u16_t)TC_pressure );
}


/*!
****************************************************************************************************
*
*   \brief          Calculates an atmospheric compensated pressure for the given wheel pressure
*
*   \author         Pwalker
*
*   \param          wheel pressure in mbar
*   \param          atmospheric pressure in mbar
*
*   \return         Atmospheric compensated pressure in mbar
*
***************************************************************************************************/
mbar_u16_t SELMATH_apply_atmospheric_compensation( mbar_u16_t pressure, mbar_u16_t atmospheric_pressure )
{
	mbar_u16_t compensated_pressure;
    mbar_u32_t calculated_pressure;

    /* check for valid values */
    if( ( pressure == MBAR_U16_INVALID ) || ( atmospheric_pressure == MBAR_U16_INVALID ) )
    {
        compensated_pressure = pressure;
    }
	else
	{
        calculated_pressure = pressure + DEFAULT_ATMOSPHERIC_PRESSURE_MBAR;

        /* Is our atmospheric_pressure less than our calculated pressure so far? */
        if( calculated_pressure >= ( mbar_u32_t ) atmospheric_pressure )
        {
            calculated_pressure = calculated_pressure - ( mbar_u32_t ) atmospheric_pressure;
        }
        else
        {
            /*
                Atmospheric pressure is greater than our wheel unit + default! we are at the dead sea
                with a flat tyre, set pressure to zero
            */
            calculated_pressure = 0u;
        }

        /* check for overflow */
        if( calculated_pressure > MBAR_U16_INVALID )
        {
            compensated_pressure = ( mbar_u16_t ) MBAR_U16_INVALID;
        }
        else
        {
            compensated_pressure = ( mbar_u16_t ) calculated_pressure;
        }
	}

    return( compensated_pressure );
}

/*!
****************************************************************************************************
*
*   \brief          Calculates the percentage of 1st para over 2nd para.
*   \note           Rounds up or down to the nearest %
*
*   \author         IR
*
*   \param          para1 - parameter 1 (restricted to a u16)
*   \param          para2 - parameter 2
*
*   \return         Percentage of para1 over para 2
*
***************************************************************************************************/
u8_t SELMATH_calculate_percent( u16_t para1, u32_t para2 )
{
    u8_t percent;
    u32_t temp;

    // check for divide by zero
    if( ( para1 > 0 ) && ( para2 == 0 ) )
    {
        //STDC_assert( 1 );
        percent = 1;
    }
    // check for 0/0
    else if( ( para1 == 0 )  && ( para2 == 0 ) )
    {
        // This is debatable! in most cases I think the customer will argue that it's 0%
        percent = 0;
    }
    else
    {
        // lets calculate
        temp = (u32_t)para1 * 100ul;
        temp = SELMATH_divide_u32_with_rounding( temp, para2 );

        if( temp > U8_T_MAX )
        {
            percent = 1;
        }
        else
        {
            percent = (u8_t)temp;
        }
    }

    return( percent );
}

/*!
****************************************************************************************************
*
*   \brief          Calculates a 1st order filter value.  Similar to a moving average
*
*   \author         IR
*
*   \param          last_filtered_value - last average
*   \param          new_value - new value
*   \param          new_value_weighting - weighting of
*
*   \return         new filtered value
*
***************************************************************************************************/
u8_t SELMATH_1st_order_u8_filter( u8_t last_filtered_value, u8_t new_value, u8_t new_value_weighting )
{
    u16_t new_filtered_value;

    new_filtered_value = (u16_t)new_value * (u16_t)new_value_weighting;
    new_filtered_value += (u16_t)last_filtered_value * (u16_t)( 100 - new_value_weighting );

    new_filtered_value = SELMATH_divide_u16_with_rounding( new_filtered_value, 100u );

    return( (u8_t)new_filtered_value );
}

/*!
****************************************************************************************************
*
*   \brief          Calculates a 1st order filter value.  Similar to a moving average
*
*   \author         IR
*
*   \param          last_filtered_value - last average
*   \param          new_value - new value
*   \param          new_value_weighting - weighting of
*
*   \return         new filtered value
*
***************************************************************************************************/
u16_t SELMATH_1st_order_u16_filter( u16_t last_filtered_value, u16_t new_value, u8_t new_value_weighting )
{
    u32_t new_filtered_value;

    new_filtered_value = (u32_t)new_value * (u32_t)new_value_weighting;
    new_filtered_value += (u32_t)last_filtered_value * (u32_t)( 100 - new_value_weighting );
    new_filtered_value = SELMATH_divide_u32_with_rounding( new_filtered_value, 100ul );

    return( (u16_t)new_filtered_value );
}




/*!
****************************************************************************************************
*
*   \brief          Maps a value from one range to another
*
*   \author         MS
*
*   \param          value - value to be mapped
*   \param          input_min - input min range of the value to be scaled from
*   \param          input_max - input max range of the value to be scaled from
*   \param          input_min - output min range of the value to be scaled to
*   \param          input_max - output max range of the value to be scaled to
*
*   \return         new scaled value
*
***************************************************************************************************/
u32_t SELMATH_map_ranges( u32_t value, u32_t input_min, u32_t input_max, u32_t output_min, u32_t output_max )
{
   u32_t result;

   if( ( input_max == 0u ) || ( output_max == 0u ) )
   {
       /*! The maximums cant be zero */
       result = 0xFFFFFFFF;
   }
   else if( input_max - input_min == 0u )
   {
       /*! Check for divide by zero */
       result = 0xFFFFFFFF;
   }
   else
   {
       result = ( ( value - input_min) * (output_max - output_min ) ) / (input_max - input_min) + output_min;
   }

   return ( result );
}


/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/
/* None */


/****************************** END OF FILE *******************************************************/
