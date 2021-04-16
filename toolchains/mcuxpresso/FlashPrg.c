#include <mcux_flash_driver.h>
#include <stdint.h>
#include <string.h>
#include "adesto_defs.h"
#include "FlashConfig.h"

//static uint32_t lmem[21];

Mailbox_Init_DynInfo_t Mailbox_Init_DynInfo;

// ************************************
// Flash driver functions
// ************************************

/*
 *  Initialize Flash Programming Functions
 *    Return Value:   0 - OK,  1 - Failed
 */
uint32_t Init (void)
{
    uint32_t status;

	// Adesto driver initialization
	status = adesto_drv_init(); 

	return status;
}

// ************************************

/*  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
uint32_t UnInit(void) {
	return 0;
}

// ************************************

#if !defined(DONT_PROVIDE_ERASECHIP)
/*  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed */
uint32_t EraseChip(void)
{
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

#endif


extern uint32_t checkblank(uint32_t adr, uint32_t words, uint32_t blankval);

// ************************************

/*  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
uint32_t EraseSectors(uint32_t addr, uint32_t numsecs) {

	uint32_t flash_offset, status, sector;
	uint8_t reg1_val;

	flash_offset = addr - adesto_get_flash_base_addr();

	for (sector = 0; sector < numsecs; sector++)
	{
		status = adesto_write_enable();
		if(status != ADESTO_STATUS_OK)
				return 1;

		status = adesto_block_erase_4k(flash_offset);
		if(status != ADESTO_STATUS_OK)
				return 1;

		do 
		{
			status = adesto_read_reg1(&reg1_val);
			if(status != ADESTO_STATUS_OK)
				return 1;
		} while(reg1_val & BUSY_BIT_MASK);

		flash_offset += FLASH_SECTOR_SIZE;
	}

	return 0;
}

/*  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

uint32_t ProgramPage(uint32_t addr, uint32_t sz, uint8_t *buf) {

    uint32_t flash_offset, prog_size, status;
	uint8_t reg1_val;

    flash_offset = addr - adesto_get_flash_base_addr();

	while(sz > 0)
	{
		prog_size = (sz > FLASH_PAGE_SIZE ? FLASH_PAGE_SIZE : sz);

		status = adesto_write_enable();
		if(status != ADESTO_STATUS_OK)
				return 1;

		status = adesto_page_prog(flash_offset, buf, prog_size);
		if(status != ADESTO_STATUS_OK)
			return 1;

		do 
		{
			status = adesto_read_reg1(&reg1_val);
			if(status != ADESTO_STATUS_OK)
				return 1;
		} while(reg1_val & BUSY_BIT_MASK);

		sz -= prog_size;
		flash_offset += prog_size;
		buf += prog_size;
	}

	return 0;
}

static uint8_t read_buf[0x1000];
// ************************************
/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   0 - OK, Failed - Address
 */
#if !defined(DONT_PROVIDE_VERIFY)
uint32_t Verify(uint32_t adr, uint32_t sz, uint8_t *buf) {

	uint32_t status = 0;

	adesto_fast_read_array(adr-adesto_get_flash_base_addr(), read_buf, sz);
	//status = memcmp((void *) adr, buf, sz);
	status = memcmp(read_buf, buf, sz);

	return (status);
}
#if defined (USE_SMALL_MEMCMP)
int memcmp(const void *a, const void *b, size_t n)
{	const unsigned char *ac = (const unsigned char *)a,
	*bc = (const unsigned char *)b;
	while (n-- > 0)
	{	unsigned char c1,c2; /* unsigned cmp seems more intuitive */
		if ((c1 = *ac++) != (c2 = *bc++)) return c1 - c2;
	}
	return 0;
}
#endif // USE_SMALL_MEMCMP
#endif // DONT_PROVIDE_VERIFY

