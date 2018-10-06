#ifndef VERSIONS_H
#define VERSIONS_H

/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "C_defs.h"


/***************************************************************************************************
**                              Defines                                                           **
***************************************************************************************************/

#define STRINGIZE_NX(a)         #a
#define STRINGIZE(A)            STRINGIZE_NX(A)

#define MAX_VERSION_LENGTH      34


#define VER_MAJOR_SW          1   // Keep these clean with no brackets or
#define VER_MINOR_SW          0   // qualifiers surrounding them as they
#define VER_VERIFICATION_SW   0   // will get converted to strings for the
#define VER_RELEASE_TYPE_SW   0   // version string in flash.
#define SPECIAL_RELEASE_SW	  0


#define VER_MAJOR_HW          1
#define VER_MINOR_HW          0
#define VER_VERIFICATION_HW   0


/***************************************************************************************************
**                              Data Types and Enums                                              **
***************************************************************************************************/
/* None allowed */

extern const sw_version_format_st VERSIONS_software_c;
extern const hw_version_format_st VERSIONS_hardware_c;

/***************************************************************************************************
**                              Exported Globals                                                  **
***************************************************************************************************/
/* None allowed */


/***************************************************************************************************
**                              Function Prototypes                                               **
***************************************************************************************************/
/* None allowed */


#endif /* Multiple inclusion guard */
/****************************** END OF FILE *******************************************************/
