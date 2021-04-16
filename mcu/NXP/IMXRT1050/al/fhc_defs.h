#ifndef FHC_DEFS_H
#define FHC_DEFS_H

// Definitions unique to the flash host controller of the current target system


#include "adesto_defs.h"

// LUT defs
#define LUT_SIZE 64
#define LUT_SEQ_SIZE 4

// Adesto look up table (LUT) sequence index list
#define LUT_SEQIDX_FAST_READARRAY				0
#define LUT_SEQIDX_WRITE_ENABLE					1
#define LUT_SEQIDX_READ_STATUS_REG_1			2
#define LUT_SEQIDX_WRITE_STATUS_REG_1			3
#define LUT_SEQIDX_BLOCK_ERASE_4K				4
#define LUT_SEQIDX_PAGE_PROG					5
#define LUT_SEQIDX_READ_JEDEC_ID				6
#define LUT_SEQIDX_CHIP_ERASE					7

// default host drive strength
#define DEFAULT_HOST_DRIVE_STRENGTH 6

// Flash base address when memory mapped
#define MCU_FLASH_BASE_ADDR (0x60000000)

// MCU name
#define MCU_NAME "MIMXRT105x"

// Adesto lookup table LUT for FlexSPI
extern uint32_t adesto_lut[];

#endif
