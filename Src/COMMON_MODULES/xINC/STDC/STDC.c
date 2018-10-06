/*! \file
*               $Revision: 12848 $
*
*               $Author: amillen $
*
*               $Date: 2013-04-11 11:14:15 +0100 (Thu, 11 Apr 2013) $
*
*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/LIBS/trunk/STDC/STDC.c $
*
*   \brief      Standard C source code - Not specific to any application
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
#include "STDC.h"


/***************************************************************************************************
**                              Data declarations and definitions                                 **
***************************************************************************************************/
// define Module Id for purposes of assert functionality
#define STDC_MODULE_ID   STDC_MOD_STDC

#define STDC_START_SEC_VAR_INIT_UNSPECIFIED
#include "MEM_placement.h"

#define STDC_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MEM_placement.h"




/***************************************************************************************************
**                              Public Functions                                                  **
***************************************************************************************************/
/*!
****************************************************************************************************
*
*   \brief          Takes byte and copies to a buffer most significant byte first
*
*   \note           ;-)
*
*   \author         IR
*
*   \param          buffer_p - pointer to buffer to place data
*   \param          var_8bit - 8bit to be copied
*
*   \return         None
*
***************************************************************************************************/
void STDC_copy_8bit_to_buffer_msb_first( void* buffer_p, u8_t var_8bit )
{
    u8_t* temp_ptr;
    temp_ptr = buffer_p;

    temp_ptr[0] = var_8bit;

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Takes word and copies to a buffer most significant byte first
*
*   \author         IR
*
*   \param          buffer_p - pointer to buffer to place data
*   \param          var_16bit - 16bit to be copied
*
*   \return         None
*
***************************************************************************************************/
void STDC_copy_16bit_to_buffer_msb_first( void* buffer_p, u16_t var_16bit )
{
    u8_t* temp_ptr;
    temp_ptr = buffer_p;

    temp_ptr[0u] = (u8_t)(var_16bit >> 8u);
    temp_ptr[1u] = (u8_t)(var_16bit);

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Takes long and copies to a buffer most significant byte first
*
*   \author         IR
*
*   \param          buffer_p - pointer to buffer to place data
*   \param          var_32bit - 32bit to be copied
*
*   \return         None
*
***************************************************************************************************/
void STDC_copy_32bit_to_buffer_msb_first( void* buffer_p, u32_t var_32bit )
{
    u8_t* temp_ptr;
    temp_ptr = buffer_p;

    temp_ptr[0u] = (u8_t)(var_32bit >> 24ul);
    temp_ptr[1u] = (u8_t)(var_32bit >> 16ul);
    temp_ptr[2u] = (u8_t)(var_32bit >> 8ul);
    temp_ptr[3u] = (u8_t)(var_32bit);

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Takes buffer and copies to 16bit variable most significant byte first
*
*   \author         IR
*
*   \param          buffer_p - pointer to buffer to copy
*
*   \return         var_16bit - 16bit to be created
*
***************************************************************************************************/
u16_t STDC_copy_buffer_msb_first_to_16bit( void* buffer_p )
{
    u16_t var_16bit;
    u8_t* temp_ptr;
    temp_ptr = buffer_p;

    var_16bit = ( ( (u16_t)temp_ptr[0u] << 8ul ) | (u16_t)temp_ptr[1u] );

    return( var_16bit );
}

/*!
****************************************************************************************************
*
*   \brief          Takes buffer and copies to 32bit variable most significant byte first
*
*   \author         IR
*
*
*   \param          var_32bit - 32bit to be created
*   \param          buffer_p - pointer to buffer to copy
*
*   \return         None
*
***************************************************************************************************/
u32_t STDC_copy_buffer_msb_first_to_32bit( void* buffer_p )
{
    u32_t var_32bit;
    u8_t* temp_ptr;
    temp_ptr = buffer_p;

    var_32bit = ( ((u32_t)temp_ptr[0u] << 24ul) |
                  ((u32_t)temp_ptr[1u] << 16ul) |
                  ((u32_t)temp_ptr[2u] << 8ul) |
                   (u32_t)temp_ptr[3u] );

    return( var_32bit );
}

/*!
****************************************************************************************************
*
*   \brief          Shifts a buffer of data up to 7 bits to the left
*
*   \author         IR
*
*   \param          buffer_p - pointer to buffer to shift
*   \param          buffer_length - length of the buffer in bytes
*   \param          shift_count - number of bits to shift
*
*   \return         None
*
***************************************************************************************************/
void STDC_left_shift_buffer( void* buffer_p, u8_t buffer_length, u8_t shift_count )
{
    // check input parameters
    if( shift_count == 0 )
    {
        // Nothing to be done
    }
    else if( shift_count < 8 )
    {
        u8_t index;
        u8_t* buffer_u8_t_p;

        // assign pointer
        buffer_u8_t_p = (u8_t*)buffer_p;

        // Loop starting at 1st index and working up to buffer length
        for( index = 0; index < ( buffer_length - 1 ); index++ )
        {
            buffer_u8_t_p[ index ]  = (u8_t)( (u8_t)( buffer_u8_t_p[ index ] << shift_count ) | (u8_t)( buffer_u8_t_p[ index + 1 ] >> ( 8 - shift_count ) ) );
        }

        // finally shift last index
        buffer_u8_t_p[ index ] = (u8_t)( buffer_u8_t_p[ index ] << shift_count );
    }
    else
    {
        // Invalid shift count
        //STDC_assert( 1 );
    }

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Shifts a buffer of data up to 7 bits to the right
*
*   \author         IR
*
*   \param          buffer_p - pointer to buffer to shift
*   \param          buffer_length - length of the buffer in bytes
*   \param          shift_count - number of bits to shift
*
*   \return         None
*
***************************************************************************************************/
void STDC_right_shift_buffer( void* buffer_p, u8_t buffer_length, u8_t shift_count )
{
    // check input parameters
    if( shift_count == 0 )
    {
        // Nothing to be done
    }
    else if( shift_count < 8 )
    {
        u8_t index;
        u8_t* buffer_u8_t_p;

        // assign pointer
        buffer_u8_t_p = (u8_t*)buffer_p;

        // Loop starting at last index and working down to 0
        for( index = (u8_t)( buffer_length - 1 ); index > 0; index-- )
        {
            buffer_u8_t_p[ index ]  = (u8_t)( (u8_t)( buffer_u8_t_p[ index ] >> shift_count ) | (u8_t)( buffer_u8_t_p[ index - 1 ] << ( 8 - shift_count ) ) );
        }

        // shift last array index by itself
        buffer_u8_t_p[ 0 ]  = (u8_t)( buffer_u8_t_p[ 0 ] >> shift_count );
    }
    else
    {
        // Invalid shift count
        //STDC_assert( 1 );
    }

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Swaps the byte order for a buffer
*
*   \author         IR
*
*
*   \param          buffer_p - pointer to buffer to swap
*   \param          buffer_length - length of the buffer in bytes
*
*   \return         None
*
***************************************************************************************************/
void STDC_swap_buffer_byte_order( void* buffer_p, u8_t buffer_length )
{
    u8_t swap_byte;
    u8_t* temp_ptr;
    u8_t swap_index1, swap_index2;

    temp_ptr = buffer_p;

    // init swap indices
    swap_index1 = 0u;
    swap_index2 = (u8_t)(buffer_length - 1u);

    // keep swaping until we reach the half way mark in the buffer
    while( swap_index1 < swap_index2 )
    {
        // take copy of swap index 1
        swap_byte = temp_ptr[ swap_index1 ];

        // swap index 1 to index 2
        temp_ptr[ swap_index1 ] = temp_ptr[swap_index2];

        // swap index 2 to index 1
        temp_ptr[ swap_index2 ] = swap_byte;

        // move onto next indecies to be swapped
        swap_index1++;
        swap_index2--;
    }

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Packs 2 nibbles into 1 byte
*
*   \author         Unknown
*
*   \param          b1 - MSnibble
*   \param          b2 - LSnibble
*
*   \return         x - byte created
*
***************************************************************************************************/
u8_t STDC_pack_byte( u8_t b1, u8_t b2 )
{
    u8_t x, y ;

    x = ( u8_t )( b1 & 0x0F );
    y = ( u8_t )( b2 & 0x0F );

    x = ( u8_t )( x * 16u );
    x = ( u8_t )( x & 0xF0 );
    x = ( u8_t )( x | y ) ;

    return( x ) ;
}


/*!
****************************************************************************************************
*
*   \brief          Counts the number of bits set in a byte
*
*   \author         Unknown
*
*   \param          abyte - Byte containing the bits you need to count
*
*   \return         cnt - Number of bits set in abyte
*
***************************************************************************************************/
u8_t STDC_bit_count( u8_t abyte )
{
    u8_t cnt;

    cnt = 0 ;

    // Test bit 7

    if ( abyte & 0x80 )
    {
        cnt++;   // Test bit 7
    }

    if ( abyte & 0x40 )
    {
        cnt++;
    }

    if ( abyte & 0x20 )
    {
        cnt++;
    }

    if ( abyte & 0x10 )
    {
        cnt++;
    }

    if ( abyte & 0x08 )
    {
        cnt++;
    }

    if ( abyte & 0x04 )
    {
        cnt++;
    }

    if ( abyte & 0x02 )
    {
        cnt++;
    }

    if ( abyte & 0x01 )
    {
        cnt++;
    }

    return( cnt );
}


/*!
****************************************************************************************************
*
*   \brief          Counts the number of bits set in a word
*
*   \author         Unknown
*
*   \param          aword - Word containing the bits you need to count
*
*   \return         cnt - Number of bits set in aword
*
***************************************************************************************************/
u8_t STDC_bit_count_word( u16_t aword )
{
    unsigned char cnt;

    cnt = 0 ;

    // Test bit 15

    if ( aword & 0x8000 )
    {
        cnt++;   // Test bit 15
    }

    if ( aword & 0x4000 )
    {
        cnt++;
    }

    if ( aword & 0x2000 )
    {
        cnt++;
    }

    if ( aword & 0x1000 )
    {
        cnt++;
    }

    if ( aword & 0x0800 )
    {
        cnt++;
    }

    if ( aword & 0x0400 )
    {
        cnt++;
    }

    if ( aword & 0x0200 )
    {
        cnt++;
    }

    if ( aword & 0x0100 )
    {
        cnt++;
    }

    if ( aword & 0x0080 )
    {
        cnt++;   // Test bit 7
    }

    if ( aword & 0x0040 )
    {
        cnt++;
    }

    if ( aword & 0x0020 )
    {
        cnt++;
    }

    if ( aword & 0x0010 )
    {
        cnt++;
    }

    if ( aword & 0x0008 )
    {
        cnt++;
    }

    if ( aword & 0x0004 )
    {
        cnt++;
    }

    if ( aword & 0x0002 )
    {
        cnt++;
    }

    if ( aword & 0x0001 )
    {
        cnt++;
    }

    return ( cnt ) ;
}


/*!
****************************************************************************************************
*
*   \brief          Convert a bit number to a bit mask
*
*   \author         Unknown
*
*   \param          bitnum - bit number (0 .. 7)
*
*   \return         bitmask
*
***************************************************************************************************/
u8_t STDC_bit_num_to_mask( u8_t bitnum )
{
    u8_t ret_val;

    switch ( bitnum )
    {

        case 0:
        {
            ret_val = 0x01;
        }

        break;

        case 1:
        {
            ret_val = 0x02;
        }

        break;

        case 2:
        {
            ret_val = 0x04;
        }

        break;

        case 3:
        {
            ret_val = 0x08;
        }

        break;

        case 4:
        {
            ret_val = 0x10;
        }

        break;

        case 5:
        {
            ret_val = 0x20;
        }

        break;

        case 6:
        {
            ret_val = 0x40;
        }

        break;

        case 7:
        {
            ret_val = 0x80;
        }

        break;

        default:
            ret_val = 0x00;
            break;
    }

    return( ret_val ) ;
}


/*!
****************************************************************************************************
*
*   \brief          Returns byte number of a bit number
*
*   \author         Unknown
*
*   \param          bitnum - bit number
*
*   \return         bytenum
*
***************************************************************************************************/
u8_t STDC_bit_byte_num( u8_t bitnum )
{
    return( ( u8_t )( bitnum / 8u ) );
}


/*!
****************************************************************************************************
*
*   \brief          Returns bitnum % 8
*
*   \author         Unknown
*
*   \param          bitnum - bit number
*
*   \return         bitnum
*
***************************************************************************************************/
u8_t STDC_bit_bit_num( u8_t bitnum )
{
    return( ( u8_t )( bitnum % 8 ) ) ;
}


#ifndef STDC_memset
/*!
****************************************************************************************************
*
*   \brief          Sets area of memory to requested fill byte
*
*   \author         IR
*
*   \param          address - start address of fill region
*   \param          fill_byte - byte to fill area with
*   \param          number_of_bytes - number of bytes to fill from start address
*
*   \return         None
*
***************************************************************************************************/
void STDC_memset( void* address, u8_t fill_byte, u16_t number_of_bytes )
{
    u32_t i;

    for ( i = 0; i < number_of_bytes; i++ )
    {
        *( ( u8_t* )address + i ) = fill_byte;
    }

    return;
}

#endif // #ifndef STDC_memset

#ifndef STDC_memcpy
/*!
****************************************************************************************************
*
*   \brief          Copys memory from one area to another
*
*   \author         IR
*
*   \param          dest_address - destination address, area to copy to
*   \param          source_address - source address, area to copy from
*   \param          number_of_bytes - number of bytes to copy
*
*   \return         None
*
***************************************************************************************************/
void  STDC_memcpy( void* dest_address, const void* source_address, u16_t number_of_bytes )
{
    u32_t i;

    for ( i = 0; i < number_of_bytes; i++ )
    {
        *( ( u8_t* )dest_address + i ) = *( ( u8_t* )source_address + i );
    }

    return;
}

#endif // #ifndef STDC_memcpy
/*!
****************************************************************************************************
*
*   \brief          Compares two locations in memory for identical data.
*
*   \author         IR
*
*   \param          source_addr1 - start address of first data
*   \param          source_addr2 - start address of second data
*   \param          number_of_bytes - number of bytes to compare
*
*   \return         TRUE for match, FALSE for no match
*
***************************************************************************************************/
false_true_et  STDC_memcompare( void* source_addr1, void* source_addr2, u32_t number_of_bytes )
{
    u32_t i;

    for ( i = 0; i < number_of_bytes; i++ )
    {
        if ( *( ( u8_t* )source_addr1 + i ) != *( ( u8_t* )source_addr2 + i ) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*!
****************************************************************************************************
*
*   \brief          Compares a location in memory for given data byte
*
*   \author         SS / AM
*
*   \param          source_addr  - start address of first data
*   \param          data_pattern - byte pattern to look for
*   \param          number_of_bytes - number of bytes to compare
*
*   \return         TRUE if all memory matches value given, otherwise FALSE
*
***************************************************************************************************/
false_true_et  STDC_memcompare_value( void* source_addr, u8_t data_pattern, u32_t number_of_bytes )
{
    u32_t i;

    for ( i = 0; i < number_of_bytes; i++ )
    {
        if ( *( ( u8_t* )source_addr + i ) != data_pattern )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*!
****************************************************************************************************
*
*   \brief          sorts an array of anything into ascending order
*
*   \author         IR
*
*   \param          unsorted_element_buffer - pointer to array to be sorted
*   \param          sorted_indices_buffer  - pointer to sorted array (specifies index into unsorted_element_buffer)
*   \param          element_count - number of elements in the array
*
*   \return         None
*
***************************************************************************************************/
void  STDC_sort_ascending( u16_t* unsorted_element_buffer, u8_t* sorted_indices_buffer, u8_t element_count )
{
    u8_t i, j;
    u16_t temp, temp_array[4];

    //STDC_assert( element_count > 4 );

    // take copy of unsorted_element_buffer to work with
    STDC_memcpy( ( u8_t* )temp_array, ( u8_t* )unsorted_element_buffer, element_count*2 );

    // populate result buffer with starting indicies

    for ( i = 0; i < element_count; i++ )
    {
        sorted_indices_buffer[i] = i;
    }

    for ( i = 0; i < element_count; i++ )
    {
        for ( j = 0; j < ( element_count - 1 ); j++ )
        {
            if ( temp_array[j] > temp_array[j + 1] )
            {
                // need to switch elements being compared
                temp = temp_array[j + 1];
                temp_array[j + 1] = temp_array[j];
                temp_array[j] = temp;

                // need to switch sorted indicies
                temp = sorted_indices_buffer[j + 1];
                sorted_indices_buffer[j + 1] = sorted_indices_buffer[j];
                sorted_indices_buffer[j] = ( u8_t )temp;
            }
        }
    }

    return;
}

/*!
****************************************************************************************************
*
*   \brief          sorts an array of anything into decending order
*
*   \author         IR
*
*   \param          unsorted_element_buffer - pointer to array to be sorted
*   \param          sorted_indices_buffer  - pointer to sorted array (specifies index into unsorted_element_buffer)
*   \param          element_count - number of elements in the array
*
*   \return         None
*
***************************************************************************************************/
void  STDC_sort_decending( u16_t* unsorted_element_buffer, u8_t* sorted_indices_buffer, u8_t element_count )
{
    u8_t i, j;
    u16_t temp, temp_array[4];

    //STDC_assert( element_count > 4 );

    // take copy of unsorted_element_buffer to work with
    STDC_memcpy( ( u8_t* )temp_array, ( u8_t* )unsorted_element_buffer, element_count*2 );

    // populate result buffer with starting indicies

    for ( i = 0; i < element_count; i++ )
    {
        sorted_indices_buffer[i] = i;
    }

    for ( i = 0; i < element_count; i++ )
    {
        for ( j = 0; j < ( element_count - 1 ); j++ )
        {
            if ( temp_array[j] < temp_array[j + 1] )
            {
                // need to switch elements being compared
                temp = temp_array[j + 1];
                temp_array[j + 1] = temp_array[j];
                temp_array[j] = temp;

                // need to switch sorted indicies
                temp = sorted_indices_buffer[j + 1];
                sorted_indices_buffer[j + 1] = sorted_indices_buffer[j];
                sorted_indices_buffer[j] = ( u8_t )temp;
            }
        }
    }

    return;
}


/*!
****************************************************************************************************
*
*   \brief          Called whenever an assert has evaluated to TRUE
*
*   \author         IR
*
*   \return         None
*
***************************************************************************************************/
void STDC_basic_assert( void )
{
  // ***Place breakpoint here in order to catch and debug any asserts which occur***
    return;
}



/*!
****************************************************************************************************
*
*   \brief          Takes a 1byte and converts it to its ASCII equivalent
*
*   \author         DC
*
*   \param          byte - 8bit word
*   \param          dest - pointer to converted ASCII storage area
*
*   \return         None
*
***************************************************************************************************/
void STDC_u8_t_2_ascii( u8_t byte, u8_t * dest )
{
    u8_t upper, lower;

    /* take upper nibble of byte */
    upper = ( u8_t )( byte >> 4u );

    if ( upper <= 9u )
    {
        upper += 48u;
    }
    else
    {
        upper += 55u;
    }

    dest[0] = upper;

    lower = ( u8_t )( byte & 0x0F );

    if ( lower <= 9u )
    {
        lower += 48u;
    }
    else
    {
        lower += 55u;
    }

    dest[1] = lower;

}

/*!
****************************************************************************************************
*
*   \brief          Takes a 16bit word and converts it to its ASCII equivalent
*
*   \author         DC
*
*   \param          word - 16bit word
*   \param          dest - pointer to converted ASCII storage area
*
*   \return         None
*
***************************************************************************************************/
void STDC_u16_t_2_ascii( u16_t word, u8_t * dest )
{
    u8_t upper_byte, lower_byte;
    u8_t temp_buf_lower[2];
    u8_t temp_buf_upper[2];

    upper_byte = ( u8_t )( word >> 8u );
    lower_byte = ( u8_t )( word & 0x00FF );

    STDC_u8_t_2_ascii( upper_byte, temp_buf_upper );
    STDC_u8_t_2_ascii( lower_byte, temp_buf_lower );

    /* copy temp bufs to dest ptr */
    dest[0] = temp_buf_upper[0];
    dest[1] = temp_buf_upper[1];

    dest[2] = temp_buf_lower[0];
    dest[3] = temp_buf_lower[1];

}

/*!
****************************************************************************************************
*
*   \brief          Takes a 32bit long and converts it to its ASCII equivalent
*
*   \author         IR
*
*   \param          long - 32bit long
*   \param          dest - pointer to converted ASCII storage area
*
*   \return         None
*
***************************************************************************************************/
void STDC_u32_t_2_ascii( u32_t dword, u8_t * dest )
{
    u8_t i, temp;
    u8_t temp_ascii_buf[2];

    for ( i = 0; i < 4; i++ )
    {
        temp = ( u8_t )( dword >> ( i * 8 ) );
        STDC_u8_t_2_ascii( temp, temp_ascii_buf );

        /* copy temp bufs to dest ptr */
        dest[7 - ( i * 2 )] = temp_ascii_buf[1];
        dest[7 - ( i * 2 + 1 )] = temp_ascii_buf[0];
    }

}

/*!
****************************************************************************************************
*
*   \brief          Converts a buffer of bytes to ASCII
*
*   \note           source_p needs to be twice the size of num_bytes
*
*   \author         IR
*
*   \return         none
*
***************************************************************************************************/
void STDC_buffer_2_ascii( u8_t* dest_p, u8_t* source_p, u8_t num_bytes )
{
    u8_t i;

    for ( i = 0; i < num_bytes; i++ )
    {
        // convert byte
        STDC_u8_t_2_ascii( source_p[i], &dest_p[i*2] );
    }

    return;
}


/*!
****************************************************************************************************
*
*   \brief          Converts a packed serial number to ASCII
*
*   \note           destination needs to be 19 bytes long!
*
*   \author         AM
*
*   \return         none
*
***************************************************************************************************/
void STDC_packed_serial_num_2_ascii( void *source, u8_t* dest )
{
    #define BASE_10                  ( 10u )

    u16_t product, day, year, serial, line, cell, last;
    u8_t* src;

    src = (u8_t*) source;

    // check if Serial number has been programmed, since could be an ECU where serial number has not been programmed
    if( STDC_memcompare_value( src, 0xFF, 6 ) == TRUE )
    {
        // Serial number is not programmed
        STDC_memcpy( dest, "SERIAL_NUMB_UNAVAIL", 19u );
    }
    else
    {
        // Serial number is programmed

        //'-------------------------------------------------------------------------------------------------------------------
        //'Byte  |         0       |         1       |         2       |         3       |         4       |        5        |
        //'-------------------------------------------------------------------------------------------------------------------
        //'Bit   | 7 6 5 4 3 2 1 0 | 7 6 5 4 3 2 1 0 | 7 6 5 4 3 2 1 0 | 7 6 5 4 3 2 1 0 | 7 6 5 4 3 2 1 0 | 7 6 5 4 3 2 1 0 |
        //'-------------------------------------------------------------------------------------------------------------------
        //'Field |           A          |         B           |       C   |             D               |    E    | F |   G  |
        //'-------------------------------------------------------------------------------------------------------------------
        product = ( (u16_t)src[0] << 2 ) + ( ( (u16_t)src[1] >> 6 ) & 0x03 );
        day     = ( (u16_t)( src[1] & 0x3F ) << 4 ) + ( ( (u16_t)src[2] >> 4 ) & 0x0F );
        year    = ( (u16_t)( src[2] & 0x0F ) << 1 ) + ( ( (u16_t)src[3] >> 7 ) & 0x01 );
        serial  = ( (u16_t)( src[3] & 0x7F ) << 7 ) + ( ( (u16_t)src[4] >> 1 ) & 0x7F );
        line    = ( (u16_t)( src[4] & 0x01 ) << 3 ) + ( ( (u16_t)src[5] >> 5 ) & 0x07 );
        cell    = ( (u16_t)src[5] >> 3 ) & 0x03;
        last    = (u16_t)src[5] & 0x07;

        STDC_val_2_ascii( product,  &dest[ 0u ],  4u, BASE_10 );
        STDC_val_2_ascii( day,      &dest[ 4u ],  4u, BASE_10 );
        STDC_val_2_ascii( year,     &dest[ 8u ],  2u, BASE_10 );
        STDC_val_2_ascii( serial,   &dest[ 10u ], 5u, BASE_10 );
        STDC_val_2_ascii( line,     &dest[ 15u ], 2u, BASE_10 );
        STDC_val_2_ascii( cell,     &dest[ 17u ], 1u, BASE_10 );
        STDC_val_2_ascii( last,     &dest[ 18u ], 1u, BASE_10 );
    }

    return;
}


/*!
****************************************************************************************************
*
*   \brief          Converts a POSITIVE number to given length of ascii characters
*                   for the specified base
*
*   \author         AM
*
*   \param          val - value to convert
*   \param          buffer - to populate
*   \param          base - the base to use for returned value
*   \param          num_chars - the number of ascii characters to generate
*
*   \note           If the number in ascii is larger than the specified length
*                   it will be truncated and therefore inaccurate
*
*   \return         none
*
***************************************************************************************************/
void STDC_val_2_ascii( u32_t val, u8_t* buffer, u8_t num_chars, u8_t base )
{
    u8_t i;

    //STDC_assert( num_chars < 1u );
    //STDC_assert( base < 2u || base > 16u );

    for( i = num_chars; i > 0u; i-- )
    {
        buffer[ i - 1u ] = "0123456789abcdef"[val % base];
        val /= base;
    }

    return;
}

/*!
****************************************************************************************************
*
*   \brief          Takes a BCD value and converts it to a u8_t
*
*   \author         JD
*
*   \param          bcd - 8bit
*
*   \return         value - u8_t
*
***************************************************************************************************/
u8_t STDC_bcd_2_u8_t( u8_t bcd )
{
    u8_t upper, lower, value;

    /* take upper nibble of byte */
    upper = ( u8_t )( bcd >> 4u );

    /* take lower nibble of byte */
    lower = ( u8_t )( bcd & 0x0F );

    value = (u8_t)(( upper * 10 ) + lower);

    return ( value );
}

/*!
****************************************************************************************************
*
*   \brief          Takes a u8_t and converts it to a bcd value
*
*   \author         JD
*
*   \param          byte - 8bit
*
*   \return         value - u8_t
*
***************************************************************************************************/
u8_t STDC_u8_t_2_bcd( u8_t byte )
{
    u8_t upper, lower, value;

    /* Work out upper nibble of BCD */
    upper = ( u8_t )( byte / 10u );

    /* Work out lower nibble of BCD */
    lower = ( u8_t )( byte % 10u );

    value = (u8_t)( ( upper << 4u ) | lower );

    return ( value );
}



/***************************************************************************************************
**                              Private Functions                                                 **
***************************************************************************************************/
/* None */


/****************************** END OF FILE *******************************************************/
