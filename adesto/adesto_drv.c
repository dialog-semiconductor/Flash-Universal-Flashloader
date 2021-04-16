#include "adesto_defs.h"

static const flash_part_info_t *adesto_part_info_p;


///////////////////////////////////////////////////////////////////////////////
// Function: adesto_write_enable
// Send write-enable command to flash device
// Arguments:
// None
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_write_enable(void)
{
	return fhc_send_cmd(CMD_WRITE_ENABLE, CMD_TYPE_NORW, 0, 0, NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_read_jedec_id
// Send read JEDEC ID command to flash device
// Arguments:
// nbytes: number of JEDEC ID bytes to read from flash device
// jid: pointer to jid buffer to which the device's response will be copied
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_read_jedec_id(uint32_t nbytes, uint8_t *jid)
{
	return fhc_send_cmd(CMD_READ_JEDEC_ID, CMD_TYPE_READ, 0, 0, jid, nbytes);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_read_reg1
// Send read register 1 command to flash device
// Arguments:
// reg1_val_p: pointer to a buffer to which the device's response (register 1
// value) will be copied
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_read_reg1(uint8_t * reg1_val_p)
{
	return fhc_send_cmd(CMD_READ_STATUS_REG_1, CMD_TYPE_READ, 0, 0, reg1_val_p, 1);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_write_reg1
// Send write register 1 command to flash device
// Arguments:
// reg1_val_p: pointer to a buffer holding the value to be written to register 1
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_write_reg1(uint8_t * reg1_val_p)
{
	return fhc_send_cmd(CMD_WRITE_STATUS_REG_1, CMD_TYPE_WRITE, 0, 0, reg1_val_p, 1);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_block_erase_4k
// Send block erase 4K command to flash device
// Arguments:
// addr: the address of the 4K block to be erased
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_block_erase_4k(uint32_t addr)
{
	return fhc_send_cmd(CMD_BLOCK_ERASE_4K, CMD_TYPE_NORW, addr, adesto_part_info_p->addr_size, NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_chip_erase
// Send erase chip command to flash device
// Arguments:
// None
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_chip_erase(void)
{
	return fhc_send_cmd(CMD_CHIP_ERASE, CMD_TYPE_NORW, 0, 0, NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_page_prog
// Send page program command to flash device
// Arguments:
// addr: program operation start address in flash memory
// data_p: pointer to a buffer holding the data to be written
// data_size: number of bytes to be programmed
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_page_prog(uint32_t addr, uint8_t *data_p, uint32_t data_size)
{

	return fhc_send_cmd(CMD_PAGE_PROG, CMD_TYPE_WRITE, addr, adesto_part_info_p->addr_size, data_p, data_size);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_fast_read_array
// Send fast read array command to flash device
// Arguments:
// addr: read operation start address in flash memory
// data_p: pointer to a buffer to which the read data will be copied
// data_size: number of bytes to be read
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_fast_read_array(uint32_t addr, uint8_t *data_p, uint32_t data_size)
{

	return fhc_send_cmd(CMD_FAST_READARRAY, CMD_TYPE_READ, addr, adesto_part_info_p->addr_size, data_p, data_size);
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_get_flash_base_addr
// Return the base address of the external flash memory in the MCU space
// Arguments:
// None
// Returns:
// Flash based address
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_get_flash_base_addr(void)
{

	return fhc_get_flash_base_addr();
}

///////////////////////////////////////////////////////////////////////////////
// Function: adesto_drv_init
// Initialize the system and the flash host controller, detect target flash 
// device and unprotect it of necessary. After existing this function system should
// be ready to write into flash memory.
// Arguments:
// None
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t adesto_drv_init(void)
{
	uint8_t write_val = 0;
	uint32_t stat;

	// Perform general system initializations required to operate on teh flash device
	sys_gen_init();

	// Initialize the flash host controller driver for the target host
	stat = fhc_init();
	if(stat != ADESTO_STATUS_OK)
		return ADESTO_STATUS_FAIL;

	// Detect the Adesto device used by the target system 
	adesto_part_info_p = adesto_part_detect();

	if(adesto_part_info_p == NULL)
		return ADESTO_STATUS_FAIL;

	// Perform additional device-dpendent initializations using information
	// available after the target flash device is known.
	fhc_devdep_init(adesto_part_info_p);

	// Unprotect devices which need to be unprotected before erae/program
	if(adesto_part_info_p->unprotect)
	{
		stat = adesto_write_enable();
		if(stat != ADESTO_STATUS_OK)
			return ADESTO_STATUS_FAIL;
		adesto_write_reg1(&write_val);
		if(stat != ADESTO_STATUS_OK)
			return ADESTO_STATUS_FAIL;
	}

	return ADESTO_STATUS_OK;
}
