#ifndef C_DEFS_H
#define C_DEFS_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "COMPILER_defs.h"

/*******************************************************************************
**                              Definitions                                   **
*******************************************************************************/

// Min and Max values for standard data types
// Useful for purposes of checking for underflows / overflows without using magic numbers
// Please use decimal values as casting of signed values can be problematic otherwise!
#define U8_T_MAX    ( 255u )            // 0xFF
#define U16_T_MAX   ( 65535u )          // 0xFFFF
#define U32_T_MAX   ( 4294967295ul )    // 0xFFFFFFFF

#define S8_T_MIN    ( -128 )            // 0x80
#define S8_T_MAX    ( 127  )            // 0x7F

#define S16_T_MIN   ( -32768 )          // -32768
#define S16_T_MAX   ( 32767 )           // +32767

#define S32_T_MIN   ( -2147483648 )     // 0x80000000
#define S32_T_MAX   ( 2147483647  )     // 0x7FFFFFFF

//! Macro to convert between bit numbers and mask bits
#define BIT2MASK( bit )  ( 0x01u << ( bit ) )   /* PRQA S 3429,3453 */ // Fn like macro being used for optimisation

//! should these be UL??? 8bit, 16bit, 32bit????
//! Mask Bits
#define MASK_BIT_0  BIT2MASK( 0 )
#define MASK_BIT_1  BIT2MASK( 1 )
#define MASK_BIT_2  BIT2MASK( 2 )
#define MASK_BIT_3  BIT2MASK( 3 )
#define MASK_BIT_4  BIT2MASK( 4 )
#define MASK_BIT_5  BIT2MASK( 5 )
#define MASK_BIT_6  BIT2MASK( 6 )
#define MASK_BIT_7  BIT2MASK( 7 )

#define MASK_BIT_8  BIT2MASK( 8 )
#define MASK_BIT_9  BIT2MASK( 9 )
#define MASK_BIT_10 BIT2MASK( 10 )
#define MASK_BIT_11 BIT2MASK( 11 )
#define MASK_BIT_12 BIT2MASK( 12 )
#define MASK_BIT_13 BIT2MASK( 13 )
#define MASK_BIT_14 BIT2MASK( 14 )
#define MASK_BIT_15 BIT2MASK( 15 )

#define MASK_BIT_16 BIT2MASK( 16 )
#define MASK_BIT_17 BIT2MASK( 17 )
#define MASK_BIT_18 BIT2MASK( 18 )
#define MASK_BIT_19 BIT2MASK( 19 )
#define MASK_BIT_20 BIT2MASK( 20 )
#define MASK_BIT_21 BIT2MASK( 21 )
#define MASK_BIT_22 BIT2MASK( 22 )
#define MASK_BIT_23 BIT2MASK( 23 )
#define MASK_BIT_24 BIT2MASK( 24 )
#define MASK_BIT_25 BIT2MASK( 25 )
#define MASK_BIT_26 BIT2MASK( 26 )
#define MASK_BIT_27 BIT2MASK( 27 )
#define MASK_BIT_28 BIT2MASK( 28 )
#define MASK_BIT_29 BIT2MASK( 29 )
#define MASK_BIT_30 BIT2MASK( 30 )
#define MASK_BIT_31 BIT2MASK( 31 )

//! Bits
#define BIT_0   ( 0 )
#define BIT_1   ( 1 )
#define BIT_2   ( 2 )
#define BIT_3   ( 3 )
#define BIT_4   ( 4 )
#define BIT_5   ( 5 )
#define BIT_6   ( 6 )
#define BIT_7   ( 7 )

#define BIT_8   ( 8 )
#define BIT_9   ( 9 )
#define BIT_10  ( 10 )
#define BIT_11  ( 11 )
#define BIT_12  ( 12 )
#define BIT_13  ( 13 )
#define BIT_14  ( 14 )
#define BIT_15  ( 15 )

#define BIT_16  ( 16 )
#define BIT_17  ( 17 )
#define BIT_18  ( 18 )
#define BIT_19  ( 19 )
#define BIT_20  ( 20 )
#define BIT_21  ( 21 )
#define BIT_22  ( 22 )
#define BIT_23  ( 23 )
#define BIT_24  ( 24 )
#define BIT_25  ( 25 )
#define BIT_26  ( 26 )
#define BIT_27  ( 27 )
#define BIT_28  ( 28 )
#define BIT_29  ( 29 )
#define BIT_30  ( 30 )
#define BIT_31  ( 31 )


//! null pointer
#define NULL_P ( 0u )




/*******************************************************************************
**                       Data Types                                           **
*******************************************************************************/
//! False/True enumeration
typedef enum
{
    FALSE = 0,
    TRUE  = 1,
    FALSE_TRUE_MAX = 2
}false_true_et;

//! False/True enumeration (bool)
typedef false_true_et bool_et;

//! Disable/Enable enumeration
typedef enum
{
    DISABLE_ = 0,
    ENABLE_ = 1,
    DISABLE_ENABLE_MAX = 2
}disable_enable_et;

//! Pass/Fail enumeration
typedef enum
{
    PASS = 0,
    FAIL = 1,
    PASS_FAIL_MAX = 2
}pass_fail_et;

//! Off/On enumeration
typedef enum
{
    OFF = 0,
    ON = 1,
    OFF_ON_MAX = 2
}off_on_et;

//! Low/High enumeration
typedef enum
{
    LOW = 0,
    HIGH = 1,
    LOW_HIGH_MAX = 2
}low_high_et;

//! OK/NOK enumeration
typedef enum
{
    OK = 0,
    NOK = 1,
    OK_NOK_MAX = 2
}ok_nok_et;

typedef enum
{
    LOW_TO_HIGH = 0,
    HIGH_TO_LOW
} interrupt_edge_et;

typedef ok_nok_et rtn_et;

//! Software Release Type
//! Release type can be used to further help to identify the intention of the release.
typedef enum
{
    RELEASE_CANDIDATE = 0, //! RC / Release Candidate (Software testing complete, no show stoppers found, potentially good as release to customer)
    SPECIAL_RELEASE   = 1, //! SR / Special Release (Hacked software to provide a particular functionality, not intended for customer use, completely unsupported)
    ALPHA_RELEASE     = 2, //! A / Alpha release (Majority of Software NOT tested)
    BETA_RELEASE      = 3, //! B / Beta release (Majority of Software tested, some known bugs)
    RELEASE_MAX       = 4
}sw_release_type_et;

//!  version number format
typedef struct
{
    char major_milestone;                   //! Major milestone version Holds at milestone functionality
    char patch;                             //! Bug fix version or small functional updates - Updated for bug fixes to milestone functionality
    char verification;                      //! Verification version Updated to track verification Releases
    sw_release_type_et release_type;    //! Release type for extra info about what the release is intended for

}sw_version_format_st;

//!  version number format
typedef struct
{
	char major_milestone;                   //! Major milestone version Holds at milestone functionality
	char patch;                             //! Bug fix version or small functional updates - Updated for bug fixes to milestone functionality
	char verification;                      //! Verification version Updated to track verification Releases
}hw_version_format_st;

#endif // #ifndef C_DEFS_H
