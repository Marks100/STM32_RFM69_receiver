/*! \file
*               $Revision: 20616 $
*
*               $Author: mstewart $
*
*               $Date: 2014-07-18 14:11:39 +0100 (Fri, 18 Jul 2014) $
*
*               $HeadURL:
*
*   \brief      Functions used to configure EE manager
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
*         (C) $Date:: 2014#$ Schrader Electronics Ltd.
*
****************************************************************************************************
*/


/***************************************************************************************************
**                              Includes                                                          **
***************************************************************************************************/
#ifndef UNIT_TEST
    #include "stm32f10x_flash.h"
    #include "stm32f10x.h"
#endif

#include "C_defs.h"
#include "COMPILER_defs.h"
#include "STDC.h"
#include "HAL_BRD.h"
#include "CHKSUM.h"
#include "NVM.h"
#include "NVM_defaults.h"


STATIC false_true_et            NVM_write_request_s = FALSE;
STATIC NVM_module_state_et      NVM_module_state_s;
NVM_info_st                     NVM_info_s;

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;


/*!
****************************************************************************************************
*
*   \brief         Function to initialise all the variables needed
*
*   \author        MS
*
*
*   \return        None
***************************************************************************************************/
void NVM_init(void)
{
    /* copy across the stored NVM data to a ram block*/
     STDC_memcpy( &NVM_info_s, (u8_t*)NVM_FLASH_PTR_START_ADDR, sizeof( NVM_info_s ) );

     /* Now do a consistency check */
     if( NOK == NVM_check_blk_crc_and_version( ) )
     {
        /* Something is wrong as the CRC is not correct, lets grab the default values */
        NVM_info_s.NVM_generic_data_blk_s = NVM_DEFAULT_GENERIC_DATA_BLK_s;

        /* zero the current wite count  */
        NVM_info_s.write_count = 0u;

        /* Write the defaults back down to NVM by triggering a FLUSH */
        NVM_write_request_s = TRUE;
     }

     /* Setup the state as NVM has now been initialised */
     NVM_module_state_s = NVM_STATE_INITIALISED;

     NVM_tick();
}




/*!
****************************************************************************************************
*
*   \brief         Function to initiate a write to NVM
*
*   \author        MS
*
*
*   \return        None
***************************************************************************************************/
void NVM_request_flush(void)
{
    if( NVM_module_state_s == NVM_STATE_INITIALISED )
    {
        NVM_write_request_s = TRUE;
        NVM_tick();
    }
}



/*!
****************************************************************************************************
*
*   \brief          Returns the state of the module
*
*   \author         MS
*
*   \return         NVM_ERFS_module_state_et NVM_ERFS_module_state_s - state of the NVM ERFS module
*
***************************************************************************************************/
NVM_module_state_et NVM_get_status( void )
{
    return NVM_module_state_s ;
}





/*!
****************************************************************************************************
*
*   \brief          NVM module tick
*
*   \author         MS
*
*   \return         none
*
***************************************************************************************************/
void NVM_tick( void )
{
    false_true_et write_required;

    switch( NVM_module_state_s )
    {
        case NVM_STATE_INITIALISED:
        {
            if( NVM_write_request_s == TRUE )
            {
                /*
                    Have we got something to write?
                    Has the data changed?
                */
                write_required = NVM_populate_blk_crc_and_version();

                if( write_required == TRUE )
                {
                    /* Increment the write ctr */
                    NVM_info_s.write_count ++;

                    /* Write the block to NVM */
                    NVM_write_segment();

                    /* Block has now been written so reset the flag */
                    NVM_write_request_s = FALSE;
                }
                else
                {
                    /* No difference in the data - don't do the write */
                    NVM_write_request_s = FALSE;
                }
            }
        }

        default:
            break;
    }
}


/*!
****************************************************************************************************
*
*   \brief          Checks the crc and version is correct!
*
*   \author         MS
*
*   \param
*
*   \return         OK_nok_et result - Result of whether the CRC and version are correct
*
***************************************************************************************************/
ok_nok_et NVM_check_blk_crc_and_version ( void )
{
    u16_t crc;
    ok_nok_et result = NOK;

    crc = CHKSUM_calc_xor_checksum(  (u8_t*)&NVM_info_s.NVM_generic_data_blk_s, sizeof( NVM_info_s.NVM_generic_data_blk_s ) );

    if( NVM_info_s.checksum == crc )
    {
        /* CRC matches so lets now do a version check */
        if( NVM_info_s.version == (u8_t)NVM_VERS_GENERIC_DATA )
        {
            result = OK;
        }
    }

    return result;
}



/*!
****************************************************************************************************
*
*   \brief          Calculate and populate crc
*                   There is a chance that the data could in the PIM could match the data in the
*                   global NVM structure but have an different checksum i.e. on a virgin ECU we check
*                   the checksum on start up and if it doesn't match then we copy the defaults over.
*                   If the defaults are the same as the data already there i.e. all 0s or FFs then we
*                   wouldn't actually request a write resulting in the checksum remaining wrong.
*
*   \author         MS
*
*   \param          block_id - the block to populate
*
*   \return         false_true_et write_required - An NVM update is required
*
***************************************************************************************************/
false_true_et NVM_populate_blk_crc_and_version( void )
{
    u16_t crc;
    false_true_et data_equal;
    false_true_et write_required = FALSE;

    /* Check to see if the data is different */
    data_equal = STDC_memcompare( (u8_t*)NVM_FLASH_PTR_START_ADDR, &NVM_info_s, sizeof(NVM_info_s) );

    /* Generate the checksum for the block */
    crc = CHKSUM_calc_xor_checksum( (u8_t*)&NVM_info_s.NVM_generic_data_blk_s, sizeof( NVM_info_s.NVM_generic_data_blk_s ) );

    if ( ( crc != NVM_info_s.checksum ) || ( data_equal == FALSE ) )
    {
        NVM_info_s.checksum = crc;
        NVM_info_s.version = (u8_t)NVM_VERS_GENERIC_DATA;

        write_required = TRUE;
    }
    else
    {
        /* The data is the same - don't need to do the write */
    }

    return ( write_required );
}




/*!
****************************************************************************************************
*
*   \brief         Function to Clear a segment of flash and then copy the contents
*                  of a buffer into that memory location
*
*   \author        MS
*
*
*   \return        None
***************************************************************************************************/
false_true_et NVM_write_segment ( void )
{
    false_true_et returnType = FALSE;

#ifndef UNIT_TEST
	/* pointer to the structure */
    NVM_info_st *pointer_struct_p;

	/* Pointer to the memory bank start address */
	u32_t address = 0u;
	address = NVM_FLASH_PTR_START_ADDR;

	/* give the pointers the relevant addresses */
	pointer_struct_p = &NVM_info_s;

	/* Porgram FLASH Bank1 ********************************************************/
	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();

	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	FLASHStatus = FLASH_ErasePage(address);

	u8_t i = 0u;
	while((address < NVM_FLASH_PTR_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
	{
		FLASHStatus = FLASH_ProgramWord( address, *(( u32_t*)pointer_struct_p + i) );
		address = address + 4;
		i++;
	}

	FLASH_LockBank1();

	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	/* set the structure back to 0 prior to reading the memory ( just a test ) */
	STDC_memset(pointer_struct_p, 0x00, sizeof (NVM_info_s ));

	address = NVM_FLASH_PTR_START_ADDR;

	/* Now read the memory back, starting at the memory register start address */
	STDC_memcpy( (u8_t*)pointer_struct_p, address, sizeof (NVM_info_st) );

#else

#endif

    return returnType;
}
