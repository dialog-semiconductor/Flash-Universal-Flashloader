#ifndef ADESTO_DEFS_H
#define ADESTO_DEFS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Device specific
#define ADESTO_FLASH_PAGE_SIZE  (256)
#define ADESTO_FLASH_SECTOR_SIZE (4096)
#define ADESTO_FLASH_BLOCK_SIZE (4096)
#define ADESTO_FLASH_BLOCK_MASK 0xfffff000

#define ADESTO_DEFAULT_FLASH_SIZE 0x1000000 //16Mbytes (128Mbits)

// Status Register busy bit
#define BUSY_BIT_MASK 0x01

// Adesto commands
#define	CMD_WRITE_STATUS_REG_1			0x01	// Write Status Register 1 	
#define	CMD_PAGE_PROG					0x02	// Byte/Page Program (1 - 256 Bytes) 	
#define	CMD_READARRAY					0x03	// Read Array 	
#define	CMD_READ_STATUS_REG_1			0x05	// Read Status Register 1 	
#define	CMD_WRITE_ENABLE				0x06	// Write Enable
#define	CMD_FAST_READARRAY				0x0B	// Fast Read Array
#define	CMD_BLOCK_ERASE_4K				0x20	// Block Erase (4 Kbytes) 	
#define	CMD_READ_JEDEC_ID				0x9F	// Read JEDEC ID
#define	CMD_CHIP_ERASE					0x60	// Erase Chip

// Adesto command types
#define CMD_TYPE_NORW	1
#define CMD_TYPE_READ	2
#define CMD_TYPE_WRITE 3

//Adesto status code
#define ADESTO_STATUS_OK 0
#define ADESTO_STATUS_FAIL 1

// flash part info structure
typedef struct
{
	//char *partno; // part number
	uint32_t density; // memory array size in bytes
	uint8_t device_id[2]; // device id (2 bytes)
	uint32_t addr_size; // address size in bytes used by this part
	bool unprotect; // unprotect required before erasing/programing
} flash_part_info_t;

extern uint32_t adesto_drv_init(void);
extern uint32_t adesto_write_enable(void);
extern uint32_t adesto_read_reg1(uint8_t * val_p);
extern uint32_t adesto_write_reg1(uint8_t * val_p);
extern uint32_t adesto_block_erase_4k(uint32_t addr);
extern uint32_t adesto_chip_erase(void);
extern uint32_t adesto_page_prog(uint32_t addr, uint8_t *data, uint32_t data_size);
extern uint32_t adesto_read_jedec_id(uint32_t nbytes, uint8_t * jid);
extern uint32_t adesto_fast_read_array(uint32_t addr, uint8_t *data_p, uint32_t data_size);
extern uint32_t adesto_get_flash_base_addr(void);
extern uint32_t fhc_init(void);
extern uint32_t fhc_send_cmd(uint8_t opcode, uint8_t cmd_type, uint32_t addr, uint32_t addr_size,
			uint8_t *data_p, uint32_t data_size);
extern uint32_t fhc_devdep_init(const flash_part_info_t *part_info_p);
extern uint32_t fhc_get_flash_base_addr(void);
extern void sys_gen_init(void);
extern const flash_part_info_t * adesto_part_detect(void);

#endif
