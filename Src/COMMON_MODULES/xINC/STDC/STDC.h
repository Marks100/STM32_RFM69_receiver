/*! \file
*               $Revision: 14981 $
*
*               $Author: ireid $
*
*               $Date: 2013-09-16 08:36:20 +0100 (Mon, 16 Sep 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/LIBS/trunk/STDC/STDC.h $
*
*   \brief      public header file for STDC.c
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
#ifndef STDC_H
#define STDC_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"
#include "STDC_config.h"

#if( STDC_AUTOGEN_ASSERTS_ENABLED == 1 )
  #include "STDC_gen_asserts.h"
#endif



/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
//! Structure to capture the date
typedef struct
{
    u8_t day;   //! 0 to 31
    u8_t month; //! 1 to 12
    u16_t year; //! 0AD to 65,536AD
}STDC_date_st;

//! Structure to capture the time
typedef struct
{
    u8_t hour;  //! 0 to 23
    u8_t mins;  //! 0 to 59
    u8_t secs;  //! 0 to 59
}STDC_time_st;

//! \note The following enum values are used externally e.g. by CANoe Configurations
//        Do not change the numbering of existing modules (Even if new Module Ids are no longer in alphabetical order!)



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/
#if( STDC_CFG_SELMATH_BACKWARDS_COMPATIBILITY == 1u )
// Fn like macro being used for optimisation
/* PRQA S 3429,3453 EOF */

#include "SELMATH.h"

#define STDC_divide_u8_with_rounding         SELMATH_divide_u8_with_rounding
#define STDC_divide_u16_with_rounding        SELMATH_divide_u16_with_rounding
#define STDC_divide_u32_with_rounding        SELMATH_divide_u32_with_rounding
#define STDC_sqrt                            SELMATH_sqrt
#define STDC_abs                             SELMATH_abs
#define STDC_abs_word                        SELMATH_abs_word
#define STDC_max_u8_t                        SELMATH_max_u8_t
#define STDC_min_u8_t                        SELMATH_min_u8_t

#endif // #if( STDC_CFG_SELMATH_BACKWARDS_COMPATIBILITY == 1u )



#define STDC_bit(x) ( (u8_t)1 << (x) )
#define STDC_bit_set(VAR,Place)  ( (VAR) |= (u8_t) ( (u8_t)1 << (Place) ) )
#define STDC_bit_clr(VAR,Place)  ( (VAR) &= (u8_t) ( (u8_t)( (u8_t)1 << (Place) )^255) )
#define STDC_bit_tog(VAR,Place)  ( (VAR) ^= (u8_t) ( (u8_t)1 << (Place) ) )
#define STDC_bit_val(VAR,Place)  ( (VAR) &  (u8_t) ( (u8_t)1 << (Place) ) )
#define STDC_bit_val_word(VAR,Place)  ( (VAR) &  (u16_t) ( (u16_t)1 << (Place) ) )

/*!
****************************************************************************************************
*
*   \brief          Macro to make 2 separate 8bit values become a 16bit value
*
*   \author         SS
*
*   \param          MSB - u8_t value that is to become the MSB
*   \param          LSB - u8_t value that is to become the LSB
*
*   \return         u16_t
*
***************************************************************************************************/
#define STDC_make_16_bit(MSB,LSB)           ((u16_t)((((u16_t)(MSB))<<8)|    \
                                            ((u16_t)(LSB))))

/*!
****************************************************************************************************
*
*   \brief          Macro to make 4 separate 8bit values become a 32bit value
*
*   \author         SS
*
*   \param          MMSB - u8_t value that is to become the Most Most Sig Bit (MMSB)
*   \param          MSB - u8_t value that is to become the MSB
*   \param          LSB - u8_t value that is to become the LSB
*   \param          LLSB - u8_t value that is to become the Least Least Sig Bit (LLSB)
*
*   \return         u32_t
*
***************************************************************************************************/
#define STDC_make_32_bit(MMSB,MSB,LSB,LLSB) ( ( u32_t )( ( ( ( u32_t )( MMSB ) )<<24 )| \
                                            ( ( ( u32_t )( MSB ) )<<16 )|           \
                                            ( ( ( u32_t )( LSB ) )<<8 ) |           \
                                            ( ( u32_t )( LLSB ) ) ) )

/*!
****************************************************************************************************
*
*   \brief          Sets bit number 'Place' in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to modify. Place - bit number to set
*
*   \return         None
*
***************************************************************************************************/
#define STDC_set_bit_num(VAR,Place)  ( (VAR) |= (u8_t)  ( (u8_t)1 << (Place) ) )

#define STDC_set_bit_num_word(VAR,Place) ( (VAR) |= (u16_t) ( (u16_t)1 << (Place) ) )

#define STDC_set_bit_num_long(VAR,Place) ( (VAR) |= (u32_t) ( (u32_t)1 << (Place) ) )



/*!
****************************************************************************************************
*
*   \brief          Clears bit number 'Place' in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to modify. Place - bit number to clear
*
*   \return         None
*
***************************************************************************************************/
#define STDC_clr_bit_num(VAR,Place)  ( (VAR) &= (u8_t) ( (u8_t)( (u8_t)1 << (Place) )^255) )

#define STDC_clr_bit_num_word(VAR,Place)  ( (VAR) &= (u16_t) ( (u16_t)( (u16_t)1 << (Place) )^0xFFFFu) )

#define STDC_clr_bit_num_long(VAR,Place)  ( (VAR) &= (u32_t) ( (u32_t)( (u32_t)1 << (Place) )^0xFFFFFFFFu) )



/*!
****************************************************************************************************
*
*   \brief          Toggles bit number 'Place' in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to modify. Place - bit number to toggle
*
*   \return         None
*
***************************************************************************************************/
#define STDC_tog_bit_num(VAR,Place)  ( (VAR) ^= (u8_t) ( (u8_t)1 << (Place) ) )


/*!
****************************************************************************************************
*
*   \brief          Checks of bit number 'Place' in variable 'VAR' is set
*
*   \author         Unknown
*
*   \param          VAR - variable to check. Place - bit number to check
*
*   \return         Returns zero if not set, returns 'Place' if it is set
*
***************************************************************************************************/
#define STDC_sample_bit_num(VAR,Place)          ( ( u8_t )( ( VAR ) & ( ( u8_t )1 << ( Place ) ) ) )

#define STDC_sample_bit_num_word(VAR,Place)     ( ( u16_t )( ( VAR ) & ( ( u16_t )1 << ( Place ) ) ) )

#define STDC_sample_bit_num_long(VAR,Place)     ( ( u32_t )( ( VAR ) & ( ( u32_t )1 << ( Place ) ) ) )



/*!
****************************************************************************************************
*
*   \brief          Sets bit numbers (as set in Mask) in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to set
*   \param        . MASK - bit mask to use
*
*   \return         None
*
***************************************************************************************************/
#define STDC_set_bit_mask(VAR,Mask)         ( (VAR) |= (Mask) )
#define STDC_set_word_bit_mask(VAR,Mask)    ( (VAR) |= ( (u16_t)(Mask) ) )


/*!
****************************************************************************************************
*
*   \brief          Clears bit numbers (as set in Mask) in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to clear
*   \param        . MASK - bit mask to use
*
*   \return         None
*
***************************************************************************************************/
#define STDC_clr_bit_mask(VAR,Mask)         ( (VAR) &= ( (u8_t)~(Mask) ) )
#define STDC_clr_word_bit_mask(VAR,Mask)    ( (VAR) &= ( (u16_t)~(Mask) ) )


/*!
****************************************************************************************************
*
*   \brief          Toggles bit numbers (as set in Mask) in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to toggle
*   \param        . MASK - bit mask to use
*
*   \return         None
*
***************************************************************************************************/
#define STDC_tog_bit_mask(VAR,Mask)  ( (VAR) ^= (Mask) )

/*
*******************************************************************************
*
*   Macro Name      : STDC_sample_bit_mask
*   Author          :
*   Description     : Clears bit numbers (as set in Mask) in variable 'VAR'
*
*   Inputs          : VAR - variable to check. Place - bit number to check
*   Outputs         : Clears bits in VAR as defined in Mask
*
*******************************************************************************
*/
/*!
****************************************************************************************************
*
*   \brief          Samples bit numbers (as set in Mask) in variable 'VAR'
*
*   \author         Unknown
*
*   \param          VAR - variable to toggle
*   \param        . MASK - bit mask to use
*
*   \return         None
*
***************************************************************************************************/
#define STDC_sample_bit_mask(VAR,Mask)  ( (VAR) & (Mask) )


/*!
*******************************************************************************
*
*   \brief          Polls a register for a particular value, for a specified amount of time
*
*   \note           Should be used for polling any hardware registers instead of using dodgy while(XXX)loop
*
*   \author         IR
*
*   \param          reg - register to poll
*   \param          compare_val - value to be checked for in register
*   \param          time - length of time to poll register (microseconds)
*   \param          temp_u8_t - temporary u8_t variable to use for looping
*   \param          pass_fail_result - SEL_pass_fail_et variable to place pass / fail result of poll
*
*   \return         None
*
*   \note           Although no check for STDC_POLL_REGISTER_NUM_US_PER_LOOP
*                   being defined is performed, the preprocessor or compiler
*                   will flag an error in the loop test statement
*
*   \note           SIT: TPMECUSWCM-93
*                   The body of a MACRO statement must be within a do-while-zero construct.
*                   This is the only permitted use of a complete macro statement as defined by MISRA-C:2004 rule 19.4.
*
*******************************************************************************
*/
#define STDC_poll_register( reg, compare_val, time, temp_u8_t, pass_fail_result )               \
do{                                                                                             \
    (pass_fail_result) = SEL_FAIL;                                                                \
    for( (temp_u8_t) = 0; (temp_u8_t) < ((time) / STDC_POLL_REGISTER_NUM_US_PER_LOOP); (temp_u8_t)++ )  \
    {                                                                                           \
        if( (reg) == (compare_val) )                                                                \
        {                                                                                       \
            (pass_fail_result) = SEL_PASS;                                                        \
            break;                                                                              \
        }                                                                                       \
    }                                                                                           \
} while(0)


#if( STDC_AUTOGEN_ASSERTS_ENABLED == 1 )

    /* Nothing to do here for STDC_assert macros */
    /* It's all done in the auto-generated file included at top */
    #define STDC_module_id_et  STDC_auto_ids_et

#elif( STDC_BASIC_ASSERT_ENABLED == 1 )

    #define STDC_module_id_et  STDC_common_module_id_et

    #define STDC_assert( true_false_flag )              \
    do{                                                 \
        if( true_false_flag )                           \
        {                                               \
            STDC_basic_assert();                        \
        }                                               \
    } while(0)

    #define STDC_assert_with_para( true_false_flag, para ) \
    do{                                                    \
        if( true_false_flag )                              \
        {                                                  \
            STDC_basic_assert();                           \
        }                                                  \
    } while(0)

#elif( STDC_FANCY_ASSERT_ENABLED == 1 )

    #define STDC_module_id_et  STDC_common_module_id_et

    #define STDC_assert( true_false_flag )                  \
    do{                                                     \
        if( true_false_flag )                               \
        {                                                   \
            STDC_fancy_assert( STDC_MODULE_ID, __LINE__ );  \
        }                                                   \
    } while(0)


    #define STDC_assert_with_para( true_false_flag, para )                  \
    do{                                                                     \
        if( true_false_flag )                                               \
        {                                                                   \
            STDC_fancy_assert_with_para( STDC_MODULE_ID, __LINE__ , para ); \
        }                                                                   \
    } while(0)

#elif( STDC_NO_ASSERTS_ENABLED == 1 )

    #define STDC_module_id_et

    // define STDC_assert as blank... so that no checks are performed
    #define STDC_assert( true_false_flag )
    #define STDC_assert_with_para( true_false_flag, para )

#endif // #if( STDC_BASIC_ASSERT_ENABLED == 1 )



/* The following macro can be used to perform compile time checks - like sizeof etc */
#define CCASSERT(predicate) _x_CCASSERT_LINE_CAT( predicate, __LINE__ )

/* PRQA S 0342 2 */ // MISRA-C:2004 Rule 19.13 -> Intended assert functionality
#define _x_CCASSERT_LINE_CAT(predicate, line)                                   \
extern char constraint_violated_on_line_##line[1-2*((predicate)!=0)]



/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None */


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
  void STDC_copy_8bit_to_buffer_msb_first( void* buffer_p, u8_t var_8bit);
  void STDC_copy_16bit_to_buffer_msb_first( void* buffer_p, u16_t var_16bit );
  void STDC_copy_32bit_to_buffer_msb_first( void* buffer_p, u32_t var_32bit );
  u16_t STDC_copy_buffer_msb_first_to_16bit( void* buffer_p );
  u32_t STDC_copy_buffer_msb_first_to_32bit( void* buffer_p );
  void STDC_swap_buffer_byte_order( void* buffer_p, u8_t buffer_length );
  void STDC_left_shift_buffer( void* buffer_p, u8_t buffer_length, u8_t shift_count );
  void STDC_right_shift_buffer( void* buffer_p, u8_t buffer_length, u8_t shift_count );
  u8_t  STDC_pack_byte(u8_t b1, u8_t b2);
  u8_t  STDC_bit_count(u8_t abyte);
  u8_t  STDC_bit_count_word(u16_t aword);
  u8_t  STDC_bit_num_to_mask(u8_t bitnum);
  u8_t  STDC_bit_byte_num(u8_t bitnum);
  u8_t  STDC_bit_bit_num(u8_t bitnum);
  void  STDC_u8_t_2_ascii( u8_t byte, u8_t * dest );
  void  STDC_u16_t_2_ascii( u16_t byte, u8_t * dest );
  void  STDC_u32_t_2_ascii( u32_t byte, u8_t * dest );
  void  STDC_buffer_2_ascii( u8_t* dest_p, u8_t* source_p, u8_t num_bytes );
  void  STDC_packed_serial_num_2_ascii( void *source , u8_t *dest);
  void  STDC_val_2_ascii( u32_t val, u8_t* buffer, u8_t num_chars, u8_t base );
  u8_t  STDC_bcd_2_u8_t( u8_t bcd );
  u8_t  STDC_u8_t_2_bcd( u8_t byte );
  u16_t STDC_num_days_between_dates( STDC_date_st first_date, STDC_date_st second_date );
  #ifndef STDC_memset
    void STDC_memset( void* address, u8_t fill_byte, u16_t number_of_bytes);
  #endif // #ifndef STDC_memset

  #ifndef STDC_memcpy
    void STDC_memcpy( void* dest_address, const void* source_address, u16_t number_of_bytes);
  #endif // STDC_memcpy

  false_true_et STDC_memcompare(void* source_addr1, void* source_addr2, u32_t number_of_bytes);
  false_true_et STDC_memcompare_value( void* source_addr, u8_t data_pattern, u32_t number_of_bytes );
  void STDC_sort_ascending( u16_t* unsorted_element_buffer, u8_t* sorted_indices_buffer, u8_t element_count );
  void STDC_sort_decending( u16_t* unsorted_element_buffer, u8_t* sorted_indices_buffer, u8_t element_count );

  // Assert Fns
  void STDC_basic_assert( void );
  void STDC_get_assert_info( u8_t* dest_buffer_p );
  void STDC_get_mux_data( u8_t mux_id, u8_t dest_buffer_p[] );



#endif /* Multiple inclusion guard */
/****************************** END OF FILE *******************************************************/
