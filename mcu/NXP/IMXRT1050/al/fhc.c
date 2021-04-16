#include "adesto_defs.h"
#include "fhc_defs.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_flexspi.h"
//#include "fsl_debug_console.h"

#define FLEXSPI_SOURCE_CLOCK 480000000

#define SCLK_FREQ 60000000

#define DEFAULT_HOST_DS 6

FLEXSPI_Type *flexspi_handle; //Pointer to the base of the FlexSPI in use for Fusion HD
flexspi_config_t flexspi_config;
flexspi_transfer_t flashxfer;
uint32_t flexspi_root_clock;

flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 0, // SPI root clock (will be set up later)
    .flashSize = ADESTO_DEFAULT_FLASH_SIZE/1024, // expressed in KByte
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 5,
    .CSHoldTime = 2,
    .CSSetupTime = 4,
    .dataValidTime = 1,
    .columnspace = 0, // we don't use colums
    .enableWordAddress = 0,
    .ARDSeqIndex = LUT_SEQIDX_FAST_READARRAY,
	.ARDSeqNumber = 1,
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
};

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_assign_pins_to_gpio
// This function assigns 6 flash interface pins to the GPIO controller 
// Arguments:
// None
///////////////////////////////////////////////////////////////////////////////
static void fhc_assign_pins_to_gpio(void)
{
	// Configure 6 flash interface pins to GPIO
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_GPIO3_IO06, 1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_GPIO3_IO07, 1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_GPIO3_IO08, 1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_GPIO3_IO09, 1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_GPIO3_IO10, 1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_GPIO3_IO11, 1);
	
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_assign_pins_to_flexspi
// This function assigns the pins to the FLEXSPI host controller by setting the
// pins alternate function in the pin mux. It also configures the pin attributes.
// Arguments:
// host_drv_strength: the pins drive strength (range is 1-7 - higher value 
// means higher drive strength; see the MCU reference manual for details)
///////////////////////////////////////////////////////////////////////////////
static void fhc_assign_pins_to_flexspi(uint8_t host_drv_strength)
{
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B, 1U); 
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK, 1U);                   
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, 1U); 
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, 1U); 
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, 1U); 
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, 1U);   
     
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B, 0x10C1u | IOMUXC_SW_PAD_CTL_PAD_DSE(host_drv_strength )); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK, 0x10C1u | IOMUXC_SW_PAD_CTL_PAD_DSE(host_drv_strength ));              
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, 0x10C1u | IOMUXC_SW_PAD_CTL_PAD_DSE(host_drv_strength )); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, 0x10C1u | IOMUXC_SW_PAD_CTL_PAD_DSE(host_drv_strength )); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, 0x10C1u | IOMUXC_SW_PAD_CTL_PAD_DSE(host_drv_strength )); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, 0x10C1u | IOMUXC_SW_PAD_CTL_PAD_DSE(host_drv_strength )); 
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_primitive_delay
// This function is a primitive implementation of a time delay as this
// minimalistic implementation does not use a timer. It performs 10 million
// loop interations. Even if the target processor runs at 1GHz and each loop
// iterations is only 5 CPU clocks the delay will take 50ms which is more than
// enough for the delay period this driver needs.
// Arguments:
// None
///////////////////////////////////////////////////////////////////////////////
static void fhc_primitive_delay(void)
{
	volatile uint32_t i;

	for(i = 0; i < 10000000; i++);
}



///////////////////////////////////////////////////////////////////////////////
// Function: fhc_jreset
// This function performs a flash JEDEC reset on the flash device.
// A JEDEC reset is a sequence of signals sent by a host MCU to a flash device
// resulting in a reset event on the flash device. The sequence is transmitted
// by the host over two (for the most part) of the SPI interface pins
// Arguments:
// None
///////////////////////////////////////////////////////////////////////////////
void fhc_jreset()
{
	uint32_t i;
	gpio_pin_config_t jreset_pin_config = 
	{
		kGPIO_DigitalOutput, 1, kGPIO_NoIntmode
	};

	// Set the direction of 3 pins used in JEDEC reset to output
	GPIO_PinInit(GPIO3, 6, &jreset_pin_config); // CS
	GPIO_PinInit(GPIO3, 8, &jreset_pin_config); // SI/IO0
	GPIO_PinInit(GPIO3, 7, &jreset_pin_config); // SCK


	// Perform a reset sequence:
	// CS goes low 4 times with alternating values of SOUT
	// SCK is drive low or high and must stay in one state
	GPIO_WritePinOutput(GPIO3, 7, 0); // set SCK low
	for(i = 0; i < 4; i++)
	{
		// drive CS low
		GPIO_WritePinOutput(GPIO3, 6, 0);
		// drive SI low or high: alternate its state every iteration
		GPIO_WritePinOutput(GPIO3, 8, (i&1));
		// drive CS high
		GPIO_WritePinOutput(GPIO3, 6, 1);
	}

	// Let flash settle down
	fhc_primitive_delay();
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_misc_reset
// This function performs a couple of sequences to reset flash parts which do
// not support JEDEC reset
// Arguments:
// None
///////////////////////////////////////////////////////////////////////////////
void fhc_misc_reset(void)
{
	uint32_t i;

	// drive all data pins high (1)
	GPIO_WritePinOutput(GPIO3, 8, 1);
	GPIO_WritePinOutput(GPIO3, 9, 1);
	GPIO_WritePinOutput(GPIO3, 10, 1);
	GPIO_WritePinOutput(GPIO3, 11, 1);
	// drive SCK low
	GPIO_WritePinOutput(GPIO3, 7, 0);

	// Start with a sequence of 2 clock cycles. For devices which are in QPI
	// mode this will send the opcode FFh which is the "exit QPI" command

	// drive CS low
	GPIO_WritePinOutput(GPIO3, 6, 0);

	for(i = 0; i < 2; i++)
	{
		GPIO_WritePinOutput(GPIO3, 7, 1);
		GPIO_WritePinOutput(GPIO3, 7, 0);
	}

	// drive CS high 
	GPIO_WritePinOutput(GPIO3, 6, 1);

	// Finally send a sequence of 8 clock cycles. For devices which are in
	// 0-4-4 mode this will look like a read command with a mode byte of FFh
	// which will cause it to exit 0-4-4.

	// drive CS low
	GPIO_WritePinOutput(GPIO3, 6, 0);

	for(i = 0; i < 8; i++)
	{
		GPIO_WritePinOutput(GPIO3, 7, 1);
		GPIO_WritePinOutput(GPIO3, 7, 0);
	}

	// drive CS high 
	GPIO_WritePinOutput(GPIO3, 6, 1);
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_init
// This function initializes the FlexSPI flash host controller
// Arguments:
// None
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t fhc_init(void)
{
   // Set flexspi source clock to 166MHZ.
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

	// First make interface pins GPIOs. We'll need that to perform the
	// reset operations
	fhc_assign_pins_to_gpio();

	// Reset flash using JEDEC reset sequence
	fhc_jreset();

	// Perform other potentially necessary reset operations in case the
	// target flash part is in QPI mode or 0-4-4 mode.These modes must be
	// exited in order to communicate with the target in normal SPI mode.
	fhc_misc_reset();

	// Assign pins to FlexSPI and configure them 
	fhc_assign_pins_to_flexspi(DEFAULT_HOST_DS);

	flexspi_handle = FLEXSPI;

	// Initialize PLL3 (a.k.a USB PLL) 
    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);

	// Set PLL3 PFD0 clock frequency to the desired FlexSPI source frequency
	CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 480000*18/(FLEXSPI_SOURCE_CLOCK/1000));

	// Choose PLL3 PFD0 clock as FlexSPI source clock
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3);

	flexspi_root_clock = SCLK_FREQ;
	deviceconfig.flexspiRootClk = flexspi_root_clock;

	// Set FlexSPI clok divider for desired FlexSPI root clock
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, (FLEXSPI_SOURCE_CLOCK/flexspi_root_clock) - 1); 

    // Get FLEXSPI default settings and configure the flexspi.
    FLEXSPI_GetDefaultConfig(&flexspi_config);

	// Enable AHB prefetching (good to have if AHB mode reads will be performed by the host)
	flexspi_config.ahbConfig.enableAHBPrefetch = true;
////////////////////////
	// Enable AHB prefetching and set buffer sizes
	flexspi_config.ahbConfig.enableAHBPrefetch = true;
    for (uint8_t i = 0; i < FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 1; i++)
    {
        flexspi_config.ahbConfig.buffer[i].bufferSize = 0; /* zero first 3 buffer sizes - they are not used*/
		flexspi_config.ahbConfig.buffer[i].masterIndex = 0;
    }
	flexspi_config.ahbConfig.buffer[FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 1].bufferSize = 1024; /* last buffer gets the entire space allocation */
	flexspi_config.ahbConfig.buffer[FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 1].enablePrefetch = true; /* enable prefetch for last buffer */
	flexspi_config.ahbConfig.buffer[FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNT - 1].masterIndex = 3;
////////////////////////
	// Use internal clock loopback for sampling 
	// Note: if you prefer using ReadSampleClkLoopbackFromDqsPad you *must* assign
	// DQS to FlexSPI as part of fhc_assign_pins_to_flexspi().
	flexspi_config.rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackInternally;

    // Init FLEXSPI
    FLEXSPI_Init(flexspi_handle, &flexspi_config);

    // Configure flash settings according to serial flash feature
    FLEXSPI_SetFlashConfig(flexspi_handle, &deviceconfig, kFLEXSPI_PortA1);

    // Update FlexSPI lookup table (LUT)
    FLEXSPI_UpdateLUT(flexspi_handle, 0, adesto_lut, LUT_SIZE);

    // Perform FlexSPI software reset
    FLEXSPI_SoftwareReset(flexspi_handle);

	// Initialize fields in the flashxfer structure we will send to the FlexSPI driver.
	// These fields will always have the same values in our case.
	flashxfer.port = kFLEXSPI_PortA1;
	flashxfer.SeqNumber = 1;

	return ADESTO_STATUS_OK;
}


// Table for converting a flash command opcode to LUT index
typedef struct 
{
	uint8_t opcode;
	uint32_t lut_seq_idx;
} opc_to_lutidx_t;

const opc_to_lutidx_t opc_to_lutidx[] =
{
	{CMD_WRITE_STATUS_REG_1, LUT_SEQIDX_WRITE_STATUS_REG_1},
	{CMD_PAGE_PROG, LUT_SEQIDX_PAGE_PROG},
	{CMD_READ_STATUS_REG_1, LUT_SEQIDX_READ_STATUS_REG_1},
	{CMD_WRITE_ENABLE, LUT_SEQIDX_WRITE_ENABLE},
	{CMD_FAST_READARRAY, LUT_SEQIDX_FAST_READARRAY},
	{CMD_BLOCK_ERASE_4K, LUT_SEQIDX_BLOCK_ERASE_4K},
	{CMD_READ_JEDEC_ID, LUT_SEQIDX_READ_JEDEC_ID},
	{CMD_CHIP_ERASE, LUT_SEQIDX_CHIP_ERASE},
};

#define OPC_TO_LUTIDX_TAB_SIZE (sizeof(opc_to_lutidx)/sizeof(opc_to_lutidx_t))

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_send_cmd
// This function sends once command to the flash device
// Arguments:
// opcode: the command opcode
// cmd_type: type of command (read, write or other)
// addr: address value (if address is used by the command)
// addr_size: size of address in bytes; if 0 - address is not used by this command
// data_p: pointer to input data (read), output data (write) or NULL if no data used
//         by command
// data_size: # of input bytes (read), # of output bytes (write), 0 if no data used
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t  fhc_send_cmd(uint8_t opcode, uint8_t cmd_type, uint32_t addr, uint32_t addr_size,
			uint8_t *data_p, uint32_t data_size)
{
	status_t flexspi_status;
	uint32_t i, lut_seq_idx;

	// Find command LUT index using conversion table 
	for(i = 0; i < OPC_TO_LUTIDX_TAB_SIZE; i++)
		if(opcode == opc_to_lutidx[i].opcode)
			break;
	if(i == OPC_TO_LUTIDX_TAB_SIZE)
		return ADESTO_STATUS_FAIL;

	lut_seq_idx = opc_to_lutidx[i].lut_seq_idx;
	flashxfer.seqIndex = lut_seq_idx;

	// Set command type for FlexSPI driver
	switch(cmd_type)
	{
		case CMD_TYPE_NORW:
			flashxfer.cmdType = kFLEXSPI_Command;
			break;
		case CMD_TYPE_READ:
			flashxfer.cmdType = kFLEXSPI_Read;
			break;
		case CMD_TYPE_WRITE:
			flashxfer.cmdType = kFLEXSPI_Write;
			break;
	}
    
	// If address is used by command - patch LUT entry to reflcet the number
	// of address bytes used by command and set the address for FlexSPI
	if(addr_size != 0)
	{
		flashxfer.deviceAddress = addr;
#if 0
		//patch the lut entry with the correct address size
		adesto_lut[4 * lut_seq_idx] &= ~FLEXSPI_LUT_OPERAND1_MASK;
		adesto_lut[4 * lut_seq_idx] |= FLEXSPI_LUT_OPERAND1(addr_size * 8);
		FLEXSPI_UpdateLUT(FLEXSPI, 4 * lut_seq_idx, &adesto_lut[4 * lut_seq_idx], 4 * (lut_seq_idx + 1));
#endif
	}
	else
		flashxfer.deviceAddress = 0;

	// Set data pointer and data size in bytes for FlexSPI driver
	flashxfer.data = (uint32_t *)data_p;
	flashxfer.dataSize = data_size;

	// Call FlexSPI driver to perform the transaction
	flexspi_status = FLEXSPI_TransferBlocking(flexspi_handle, &flashxfer);
    if (flexspi_status != kStatus_Success)
        return ADESTO_STATUS_FAIL;

	return ADESTO_STATUS_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_get_flash_base_addr
// Return the base address of the external flash memory in the MCU space
// Arguments:
// None
// Returns:
// Flash based address
///////////////////////////////////////////////////////////////////////////////
uint32_t fhc_get_flash_base_addr(void)
{

	return FlexSPI_AMBA_BASE;
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_devdep_init
// Perform additional device-dpendent initializations using information
// available after the target flash device is known.
// Arguments:
// None
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t fhc_devdep_init(const flash_part_info_t * part_info_p)
{
	uint32_t addr_size = part_info_p->addr_size;

	// patch Page Program entry with size of address used by device
	adesto_lut[4 * LUT_SEQIDX_PAGE_PROG] &= ~FLEXSPI_LUT_OPERAND1_MASK;
	adesto_lut[4 * LUT_SEQIDX_PAGE_PROG] |= FLEXSPI_LUT_OPERAND1(addr_size * 8);
	FLEXSPI_UpdateLUT(flexspi_handle, 4 * LUT_SEQIDX_PAGE_PROG, &adesto_lut[4 * LUT_SEQIDX_PAGE_PROG], 4 * (LUT_SEQIDX_PAGE_PROG + 1));

	// patch 4K Block Erase entry with size of address used by device
	adesto_lut[4 * LUT_SEQIDX_BLOCK_ERASE_4K] &= ~FLEXSPI_LUT_OPERAND1_MASK;
	adesto_lut[4 * LUT_SEQIDX_BLOCK_ERASE_4K] |= FLEXSPI_LUT_OPERAND1(addr_size * 8);
	FLEXSPI_UpdateLUT(flexspi_handle, 4 * LUT_SEQIDX_BLOCK_ERASE_4K, &adesto_lut[4 * LUT_SEQIDX_BLOCK_ERASE_4K], 4 * (LUT_SEQIDX_BLOCK_ERASE_4K + 1));

	// patch 4K Block Erase entry with size of address used by device
	adesto_lut[4 * LUT_SEQIDX_FAST_READARRAY] &= ~FLEXSPI_LUT_OPERAND1_MASK;
	adesto_lut[4 * LUT_SEQIDX_FAST_READARRAY] |= FLEXSPI_LUT_OPERAND1(addr_size * 8);
	FLEXSPI_UpdateLUT(flexspi_handle, 4 * LUT_SEQIDX_FAST_READARRAY, &adesto_lut[4 * LUT_SEQIDX_FAST_READARRAY], 4 * (LUT_SEQIDX_FAST_READARRAY + 1));

	return ADESTO_STATUS_OK;
}
