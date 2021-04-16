/* -----------------------------------------------------------------------------
 * Copyright (c) 2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 * --------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 * This source file has been modified by Adesto Technologies for supporting
 * the Adesto flash algorithm.
 * --------------------------------------------------------------------------- */


#include "FlashOS.h"
#include "adesto_defs.h"
#include <string.h>

/*
 * Check if Program/Erase completed using the toggle bit
 *    Parameter:      adr:  Block Start Address
 *    Return Value:   0 - OK,  1 - Failed


int Polling (unsigned long adr) {
	return 0;
}
 */ 

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

    uint32_t status;

	// Adesto driver initialization
	status = adesto_drv_init();
	
	return status;
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
	return 0;
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
	uint8_t reg1_val;
    uint32_t status;

	status = adesto_write_enable();
	if(status != ADESTO_STATUS_OK)
			return 1;

	status = adesto_chip_erase();
	if(status != ADESTO_STATUS_OK)
			return 1;

	do 
	{
		status = adesto_read_reg1(&reg1_val);
		if(status != ADESTO_STATUS_OK)
			return 1;
	} while(reg1_val & BUSY_BIT_MASK);

    return 0;
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long addr) {
	uint8_t reg1_val;
	uint32_t status;

	status = adesto_write_enable();
	if(status != ADESTO_STATUS_OK)
			return 1;

	status = adesto_block_erase_4k(addr - adesto_get_flash_base_addr());
	if(status != ADESTO_STATUS_OK)
			return 1;

	do 
	{
		status = adesto_read_reg1(&reg1_val);
		if(status != ADESTO_STATUS_OK)
			return 1;
	} while(reg1_val & BUSY_BIT_MASK);
	
	return 0;
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long addr, unsigned long sz, unsigned char *buf) {
    uint32_t flash_offset, status;
	uint8_t reg1_val;

    flash_offset = addr - adesto_get_flash_base_addr();

	status = adesto_write_enable();
	if(status != ADESTO_STATUS_OK)
			return 1;

	status = adesto_page_prog(flash_offset, buf, sz);
	if(status != ADESTO_STATUS_OK)
		return 1;

	do 
	{
		status = adesto_read_reg1(&reg1_val);
		if(status != ADESTO_STATUS_OK)
			return 1;
	} while(reg1_val & BUSY_BIT_MASK);

	return 0;
}

#define MEM_MAPPED_READ

#ifdef MEM_MAPPED_READ

#define MAX_READ_SIZE 1024

/* BlankCheck
   Returns status information:
   * 0 when the block content is equal to the pattern pat.
   * 1 when the block content differs from the pattern pat.
*/
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
	uint32_t read_addr, read_sz, i;

	read_addr = adr;

	while(sz > 0)
	{
		read_sz = sz < MAX_READ_SIZE ? sz : MAX_READ_SIZE;

		for( i = 0; i < read_sz; i++)
			if(*((uint32_t *)read_addr) != pat)
				return 1;
		sz -= read_sz;
		read_addr += read_sz;
	}
	return 0;
}

/* Verify 
   Returns status information:
   * the sum of (adr+sz) - on success.
   * any other number - on failure, and represents the failing address.
*/
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
	uint32_t read_addr, read_sz, sz_left;

	read_addr = adr;
	sz_left = sz;

	while(sz_left > 0)
	{
		read_sz = sz_left < MAX_READ_SIZE ? sz_left : MAX_READ_SIZE;
		if(memcmp((uint32_t *)(read_addr), buf, read_sz) != 0)
			return read_addr;
		sz_left -= read_sz;
		read_addr += read_sz;
		buf += read_sz;
	}
	return adr + sz;
}

#else

#define MAX_READ_SIZE 256
static uint8_t read_buf[MAX_READ_SIZE];

/* BlankCheck
   Returns status information:
   * 0 when the block content is equal to the pattern pat.
   * 1 when the block content differs from the pattern pat.
*/
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
	uint32_t flash_offset, read_sz, status, i;

	flash_offset = adr - adesto_get_flash_base_addr();

	while(sz > 0)
	{
		read_sz = sz < MAX_READ_SIZE ? sz : MAX_READ_SIZE;
		status = adesto_fast_read_array(flash_offset, read_buf, read_sz);
		if(status != ADESTO_STATUS_OK)
			return 1;
		for( i = 0; i < read_sz; i++)
			if(read_buf[i] != pat)
				return 1;
		sz -= read_sz;
		flash_offset += read_sz;
	}
	return 0;
}


/* Verify 
   Returns status information:
   * the sum of (adr+sz) - on success.
   * any other number - on failure, and represents the failing address.
*/
unsigned long Verify (unsigned long adr, unsigned long sz, unsigned char *buf)
{
	uint32_t flash_offset, read_sz, status, sz_left;

	flash_offset = adr - adesto_get_flash_base_addr();
	sz_left = sz;

	while(sz_left > 0)
	{
		read_sz = sz_left < MAX_READ_SIZE ? sz_left : MAX_READ_SIZE;
		status = adesto_fast_read_array(flash_offset, read_buf, read_sz);
		if(status != ADESTO_STATUS_OK)
			return adr + sz - sz_left;
		if(memcmp(read_buf, buf, read_sz) != 0)
			return adr + sz - sz_left;
		sz_left -= read_sz;
		flash_offset += read_sz;
		buf += read_sz;
	}
	return adr + sz;
}
#endif
