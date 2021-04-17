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

// Pad control definitions
#define IOPCTL_PIO_ANAMUX_DI 0x00u        /*!<@brief Analog mux is disabled */
#define IOPCTL_PIO_FULLDRIVE_DI 0x00u     /*!<@brief Normal drive */
#define IOPCTL_PIO_FULLDRIVE_EN 0x0100u   /*!<@brief Full drive enable */
#define IOPCTL_PIO_FUNC1 0x01u            /*!<@brief Selects pin function 1 */
#define IOPCTL_PIO_FUNC5 0x05u            /*!<@brief Selects pin function 5 */
#define IOPCTL_PIO_FUNC6 0x06u            /*!<@brief Selects pin function 6 */
#define IOPCTL_PIO_INBUF_DI 0x00u         /*!<@brief Disable input buffer function */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_PUPD_EN 0x10u          /*!<@brief Enable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */

// Flash base address when memory mapped
#define MCU_FLASH_BASE_ADDR (0x08000000)

// MCU name
#define MCU_NAME "MIMXRT6xx"

// Adesto lookup table LUT for FlexSPI
extern uint32_t adesto_lut[];

#endif
