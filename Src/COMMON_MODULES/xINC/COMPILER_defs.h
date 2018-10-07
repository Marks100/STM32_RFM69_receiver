#ifndef COMPILER_DEFS_H
#define COMPILER_DEFS_H


/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "PROJ_config.h"
#include "COMPILER_config.h"



/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

    /* TYPEDEFS */
#if ( defined STM32 )
    typedef unsigned char   u8_t;       //!< unsigned 8bit type
    typedef unsigned short  u16_t;      //!< unsigned 16bit type
    typedef unsigned long   u32_t;      //!< unsigned 32bit type
    typedef signed char     s8_t;       //!< signed 8bit type
    typedef signed short    s16_t;      //!< signed 16bit type
    typedef signed long     s32_t;      //!< signed 32bit type
    typedef float			f32_t;		//!< float 32bit type

    /* Compiler specific for assembly */
    #define ASM(x)    _asm(x)

    /* STATIC keyword - can be undefed for unit testing */
    #ifndef STATIC
        #if ( defined UNIT_TEST )
            #define STATIC
        #else
            #define STATIC static
        #endif
    #endif // #ifndef STATIC

    #define AUTOMATIC
    #define INLINE @inline

#else
	#error "NO ENVIRONMENT CONFIGURED"

	typedef unsigned char   u8_t;       //!< unsigned 8bit type
    typedef unsigned short  u16_t;      //!< unsigned 16bit type
    typedef unsigned long   u32_t;      //!< unsigned 32bit type
    typedef signed char     s8_t;       //!< signed 8bit type
    typedef signed short    s16_t;      //!< signed 16bit type
    typedef signed long     s32_t;      //!< signed 32bit type

    /* Compiler specific for assembly */
    #define ASM(x)    _asm(x)

    /* STATIC keyword - can be undefed for unit testing */
    #ifndef STATIC
        #if ( defined UNIT_TEST )
            #define STATIC
        #else
            #define STATIC static
        #endif
    #endif // #ifndef STATIC

    #define AUTOMATIC
    #define INLINE @inline
#endif

#endif


/****************************** END OF FILE *******************************************************/
