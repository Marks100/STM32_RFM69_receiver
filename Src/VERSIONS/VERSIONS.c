/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#include "STDC.h"
#include "autoversion.h"
#include "VERSIONS.h"



/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/



/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/
/*!
****************************************************************************************************
*
*   \brief         returns the SW version number
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void VERSIONS_get_SW_version_num( u8_t *version_num_p )
{
	/* Reference the included autoversion.h file so that we can grab the SW version number */
	char version_num_array[SW_VERSION_NUM_SIZE];

	version_num_array[0] = STM32_RF_HUB_VERSION_MAJOR;
	version_num_array[1] = STM32_RF_HUB_VERSION_PATCH;
	version_num_array[2] = STM32_RF_HUB_VERSION_VERIFICATION;

	/* Now copy the local version array back to the pointer */
	STDC_memcpy( version_num_p, version_num_array, sizeof( version_num_array ) );
}



/*!
****************************************************************************************************
*
*   \brief         returns the HW version number
*
*   \author        MS
*
*   \return        None
*
***************************************************************************************************/
void VERSIONS_get_HW_version_num( u8_t *version_num_p )
{
	version_num_p[0] = 1u;
	version_num_p[1] = 0u;
}


/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/
/* None */



/****************************** END OF FILE *******************************************************/
