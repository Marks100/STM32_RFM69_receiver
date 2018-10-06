/*! \file
*               $Revision: 14487 $
*
*               $Author: pewalker $
*
*               $Date: 2013-08-12 16:33:40 +0100 (Mon, 12 Aug 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/INC/trunk/COMPILER_check.h $
*
*   \brief      Compiler Check Header File - Checks that correct Compiler, Compiler Version and Processor Core have been selected for the given project
*/
/* COPYRIGHT NOTICE
* =================
*
* The contents of this document are protected under copyright and contain
* commercially and/or technically confidential information. The content of
* this document must not be used other than for the purpose for which it was
* provided nor may it be disclosed or copied (by the authorised recipient or
* otherwise) without the prior written consent of an authorised officer of
* Schrader Electronics Ltd.
*
*         (C) $Date:: 2013#$ Schrader Electronics Ltd.
*
*******************************************************************************
*/

#ifndef COMPILER_CHECK_H
#define COMPILER_CHECK_H

#if ( MUX_GENERATOR_RUN == 1 )
    // No compiler checks required if this is a Mux Generator Run

#elif ( SEL_COMPILER_QAC == 1 )
    // No compiler checks required if this is a QAC Analysis

#elif( SEL_COMPILER_DOXYGEN == 1 )
    //! \todo Add checks for this compiler

#else

    // Check that all EXPECTED macros have been defined
    #ifndef EXPECTED_COMPILER_VERSION
        #error "EXPECTED_COMPILER_VERSION not defined - Please add to PROJ_config.h"
    #endif // #ifndef EXPECTED_COMPILER_VERSION

    /* Checks for S12 Cosmic Compiler  */
    #if ( defined SEL_COMP_COSMIC_S12 )

    // __TRGT__ expands to a text string containing the target processor name.
    // __VERS__ expands to a text string containing the compiler version

        #if( __VERS__ != EXPECTED_COMPILER_VERSION )
            #error "Incorrect compiler version - Check that compiler being used matches expected compiler version"
        #endif // #if( __TRGT__ != EXPECTED_COMPILER_VERSION )

    /* Checks for STM8 Cosmic Compiler  */
    #elif ( defined SEL_COMP_COSMIC_STM8 )
        //! \todo Add checks for this compiler

    /* Checks for Freescale's Metrowerks / code warrior compiler  */
    #elif ( defined SEL_COMP_METROWERKS ) || ( defined SEL_COMP_CODEWARRIOR )
        //! \todo Add checks for this compiler

    // __MWERKS__  Defined as the version number of the Metrowerks C/C++ compiler if you are using the CodeWarrior CW7 that was released in 1995. For example, with
    // the Metrowerks C/C++ compiler version 2.2, the value of __MWERKS__ is 0x2200. This macro is defined as 1 if the compiler was issued before the CodeWarrior
    // CW7 that was released in 1995.

        #if( __MWERKS__ != EXPECTED_COMPILER_VERSION )
            #error "Incorrect metrowerks / Code Warrior Compiler version being used - Please Check!"
        #endif // #if( __MWERKS__ != EXPECTED_COMPILER_VERSION )

    /* Checks for IAR compiler  */
    #elif ( defined SEL_COMPILER_IAR )

    // __CORE__ Identifies the processor core in use
    // __BUILD_NUMBER__ Identifies the build number of the compiler currently in use
    // __SUBVERSION__ Identifies the version letter of the version number

        #if( __VER__ != EXPECTED_COMPILER_VERSION )
            #error "Incorrect IAR Compiler version being used - Please Check!"
        #endif // #if( __VER__ != EXPECTED_COMPILER_VERSION )

    /* Checks for Microchip C30 compiler  */
    #elif ( defined SEL_COMPILER_MPLAB_C30 )
        //! \todo Add checks for this compiler

    /* Checks for GCC compiler  */
    #elif ( defined SEL_COMPILER_GCC )

    // use the predefined macros to calculate a single number, then compare that against a threshold:
    #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

        #if( GCC_VERSION != EXPECTED_COMPILER_VERSION )
            #error "Incorrect GCC Compiler version being used - Please Check!"
        #endif // #if( GCC_VERSION != EXPECTED_COMPILER_VERSION )

    /* Checks for Wind River Diab compiler  */
    #elif ( defined SEL_COMPILER_WINDRIVER_DIAB )

    // _DIAB_TOOL Indicates the Wind River Diab Compiler is being used.
    // __VERSION_NUMBER__ The version number of the compiler and tools, represented as an integer.

        #if( ( __VERSION_NUMBER__ != EXPECTED_COMPILER_VERSION ) )
            #error "Incorrect Wind River Diab Compiler version being used - Please Check!"
        #endif // #if( ( _DIAB_TOOL == 1 ) && ( __VERSION_NUMBER__ == 5900 ) )

    /* Checks for MS VC++  */
    #elif ( defined SEL_COMPILER_MS_VCPLUSPLUS )

    // _MSC_VER Evaluates to the major and minor number components of the compiler's version number. The major number is the first component of the period-delimited version number and the minor number is the second component.

        // check that correct compiler version is being used
        #if( _MSC_VER != EXPECTED_COMPILER_VERSION )
            #error "Incorrect MS VC++ Compiler version being used - Please Check!"
        #endif // #if( _MSC_VER != EXPECTED_COMPILER_VERSION )

    #elif ( defined SEL_RENESIS_78K0R )


    #elif ( defined SEL_GREEN_HILLS )

        #if( ( __GHS_VERSION_NUMBER != EXPECTED_COMPILER_VERSION ) )
            #error "Incorrect Green hills tootl chain version being used - Please Check!"
        #endif // #if( __GHS_VERSION_NUMBER != EXPECTED_COMPILER_VERSION )

	#elif ( defined SEL_COMP_TI_ARM )

	// Defined to a 7-9 digit integer, depending on if X has 1, 2, or 3 digits. The number does not contain a decimal. For example, version 3.2.1 is represented as 3002001.
	// The leading zeros are dropped to prevent the number being interpreted as an octal.

		// check that correct compiler version is being used
		#if( __TI_COMPILER_VERSION__ != EXPECTED_COMPILER_VERSION )
			#error "Incorrect Texas Instruments ARM Compiler version being used - Please Check!"
		#endif // #if( __TI_COMPILER_VERSION__ != EXPECTED_COMPILER_VERSION )

    #else
        #error "SEL_COMP_XXX... Compiler type not defined"

    #endif // #if SEL_COMP_COSMIC

#endif // #if ( MUX_GENERATOR_RUN == 1 )

#endif // #ifndef COMPILER_CHECK_H
