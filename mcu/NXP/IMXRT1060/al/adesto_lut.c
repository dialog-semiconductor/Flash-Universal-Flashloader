#include "adesto_defs.h"
#include "fhc_defs.h"
#include "fsl_flexspi.h"

#define ADDR_UNDEF 0

///////////////////////////////////////////////////////////////////////////////
// Lookup table (LUT)
///////////////////////////////////////////////////////////////////////////////

uint32_t adesto_lut[LUT_SIZE] =
{
	// Write Enable 
	[4 * LUT_SEQIDX_WRITE_ENABLE] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_WRITE_ENABLE, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

	// Read Status Register 1 
	[4 * LUT_SEQIDX_READ_STATUS_REG_1] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_READ_STATUS_REG_1, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 1),

	// Write Status Register 1 
	[4 * LUT_SEQIDX_WRITE_STATUS_REG_1] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_WRITE_STATUS_REG_1, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 1),

	// Byte/Page Program (1 - 256 Bytes) 
	[4 * LUT_SEQIDX_PAGE_PROG] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_PAGE_PROG, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, ADDR_UNDEF),
	[4 * LUT_SEQIDX_PAGE_PROG + 1] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 128, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

	// Fast Read Array 
	[4 * LUT_SEQIDX_FAST_READARRAY] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_FAST_READARRAY, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, ADDR_UNDEF),
	[4 * LUT_SEQIDX_FAST_READARRAY + 1] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 8, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 128),

	// Block Erase (4 Kbytes) 
	[4 * LUT_SEQIDX_BLOCK_ERASE_4K] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_BLOCK_ERASE_4K, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, ADDR_UNDEF),

	// Read JEDEC ID 
	[4 * LUT_SEQIDX_READ_JEDEC_ID] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_READ_JEDEC_ID, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 5),

	// Chip Erase 
	[4 * LUT_SEQIDX_CHIP_ERASE] =
		FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, CMD_CHIP_ERASE, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),
};
