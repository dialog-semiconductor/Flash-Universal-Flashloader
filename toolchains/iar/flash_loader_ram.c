//------------------------------------------------------------------------------
//
// Copyright (c) 2008-2015 IAR Systems
//
// Licensed under the Apache License, Version 2.0 (the "License")
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// $Revision: 38952 $
//
//------------------------------------------------------------------------------

#include "flash_loader.h"
#include "flash_loader_extra.h"
#include "adesto_defs.h"

#define PAGE_SIZE 256

static void *flash_based_addr; 

void strcopy(char *to, char* from);
extern void __iar_data_init3(void);

uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags)
{
	uint32_t status;

	// This IAR function clears uninitialzed variables sections (which are
	// known elsewhere as .bss) with zeroes. For some reason it was not 
	// included in the original example but it is important to have!
	__iar_data_init3();

	flash_based_addr = base_of_flash;

	// Adesto driver initialization
	status = adesto_drv_init();
	if(status != ADESTO_STATUS_OK)
		return RESULT_ERROR;

	return RESULT_OK;
}

uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{

	uint32_t flash_offset, status, prog_size;
	uint8_t reg1_val;

	flash_offset = (uint32_t)block_start - (uint32_t)flash_based_addr + offset_into_block;

	while(count > 0)
	{
		prog_size = (count > PAGE_SIZE ? PAGE_SIZE : count);

		status = adesto_write_enable();
		if(status != ADESTO_STATUS_OK)
				return RESULT_ERROR;

		status = adesto_page_prog(flash_offset, buffer, prog_size);
		if(status != ADESTO_STATUS_OK)
			return RESULT_ERROR;

		do 
		{
			status = adesto_read_reg1(&reg1_val);
			if(status != ADESTO_STATUS_OK)
				return RESULT_ERROR;
		} while(reg1_val & BUSY_BIT_MASK);

		count -= prog_size;
		flash_offset += prog_size;
		buffer += prog_size;
	}

	return RESULT_OK;
}

uint32_t FlashErase(void *block_start,
                    uint32_t block_size)
{
	uint32_t flash_offset, status;
	uint8_t reg1_val;

	flash_offset = (uint32_t)block_start - (uint32_t)flash_based_addr;
	//*((volatile uint32_t *)(0x20011000)) = (uint32_t)0xbadabada;
	//*((volatile uint32_t *)(0x20011004)) = (uint32_t)flash_based_addr;

	status = adesto_write_enable();
	if(status != ADESTO_STATUS_OK)
			return RESULT_ERROR;

	status = adesto_block_erase_4k(flash_offset);
	if(status != ADESTO_STATUS_OK)
			return RESULT_ERROR;

	do 
	{
		status = adesto_read_reg1(&reg1_val);
		if(status != ADESTO_STATUS_OK)
			return RESULT_ERROR;
	} while(reg1_val & BUSY_BIT_MASK);

	//*((volatile uint32_t *)(0x20012000)) = 0xcacacaca;

	return RESULT_OK;
}

#if 0

OPTIONAL_CHECKSUM
uint32_t FlashChecksum(void const *begin, uint32_t count)
{
  return Crc16((uint8_t const *)begin, count);
}

OPTIONAL_SIGNOFF
uint32_t FlashSignoff(void)
{
  return 0;
}
#endif

void strcopy(char *to, char* from)
{
  while (*to++ = *from++) ;
}



