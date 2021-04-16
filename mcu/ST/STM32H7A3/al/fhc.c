#include <stdlib.h>
#include <stdio.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_ospi.h"
#include "adesto_defs.h"
#include "fhc_defs.h"

#define NUM_OSPI_PINS 6

OSPI_HandleTypeDef OSPIHandle;
OSPI_RegularCmdTypeDef flash_cmd;


typedef struct
{
	GPIO_TypeDef * port;
	uint32_t pin;
	uint32_t alt_func;
	uint32_t pull;
} pin_assignment_t;

pin_assignment_t ospi_pin_assignment[NUM_OSPI_PINS] = 
{
	{.port = OSPI_CS_GPIO_PORT, .pin = OSPI_CS_PIN, .alt_func = OSPI_CS_PIN_AF, .pull = GPIO_PULLUP},
	{.port = OSPI_CLK_GPIO_PORT, .pin = OSPI_CLK_PIN, .alt_func = OSPI_CLK_PIN_AF, .pull = GPIO_NOPULL},
	{.port = OSPI_D0_GPIO_PORT, .pin = OSPI_D0_PIN, .alt_func = OSPI_D0_PIN_AF, .pull = GPIO_NOPULL},
	{.port = OSPI_D1_GPIO_PORT, .pin = OSPI_D1_PIN, .alt_func = OSPI_D1_PIN_AF, .pull = GPIO_NOPULL},
	{.port = OSPI_D2_GPIO_PORT, .pin = OSPI_D2_PIN, .alt_func = OSPI_D2_PIN_AF, .pull = GPIO_NOPULL},
	{.port = OSPI_D3_GPIO_PORT, .pin = OSPI_D3_PIN, .alt_func = OSPI_D3_PIN_AF, .pull = GPIO_NOPULL},
};

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_primitive_delay
// This function is a primitive implementation of a time delay as this
// minimalistic implementation does not use a timer. It performs 10 million
// loop interations. Even if the target processor runs at 1GHz and each loop
// iterations is only 5 CPU clocks the delay will take 5ms which is more than
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
// Function: fhc_assign_pins_to_gpio
// This function assigns 6 flash interface pins to the GPIO controller 
// Arguments:
// None
///////////////////////////////////////////////////////////////////////////////
static void fhc_assign_pins_to_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin       = OSPI_CS_PIN;
	HAL_GPIO_Init(OSPI_CS_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin       = OSPI_CLK_PIN;
	HAL_GPIO_Init(OSPI_CLK_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin       = OSPI_D0_PIN;
	HAL_GPIO_Init(OSPI_D0_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin       = OSPI_D1_PIN;
	HAL_GPIO_Init(OSPI_D1_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin       = OSPI_D2_PIN;
	HAL_GPIO_Init(OSPI_D2_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin       = OSPI_D3_PIN;
	HAL_GPIO_Init(OSPI_D3_GPIO_PORT, &GPIO_InitStruct);
}

///////////////////////////////////////////////////////////////////////////////
// Function: fhc_assign_pins_to_octospi
// This function assigns the pins to the OctoSPI host controller by setting the
// pins alternate function in the pin mux. It also configures the pin attributes.
// Arguments:
// None
///////////////////////////////////////////////////////////////////////////////
static void fhc_assign_pins_to_octospi(void)
{
	uint32_t i;
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

	for(i = 0; i < NUM_OSPI_PINS; i++)
	{
		GPIO_InitStruct.Pin       = ospi_pin_assignment[i].pin;
		GPIO_InitStruct.Pull      = ospi_pin_assignment[i].pull;
		GPIO_InitStruct.Alternate = ospi_pin_assignment[i].alt_func;
		HAL_GPIO_Init(ospi_pin_assignment[i].port, &GPIO_InitStruct);
	}
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
static void fhc_jreset(void)
{
	uint32_t i;

	// Perform a reset sequence:
	// CS goes low 4 times with alternating values of SOUT
	// SCK is drive low or high and must stay in one state
	HAL_GPIO_WritePin(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN, GPIO_PIN_RESET); // set SCK low
	for(i = 0; i < 4; i++)
	{
		// drive CS low
		HAL_GPIO_WritePin(OSPI_CS_GPIO_PORT, OSPI_CS_PIN, GPIO_PIN_RESET);
		//for(j=0; j<100; j++);
		// drive SI low or high: alternate its state every iteration
		HAL_GPIO_WritePin(OSPI_D0_GPIO_PORT, OSPI_D0_PIN, (i&1)?GPIO_PIN_SET:GPIO_PIN_RESET);
		// drive CS high; SI state will be captured on the CS rising edge
		HAL_GPIO_WritePin(OSPI_CS_GPIO_PORT, OSPI_CS_PIN, GPIO_PIN_SET);
		//for(j=0; j<100; j++);
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

	// drive all data pins high
	HAL_GPIO_WritePin(OSPI_D0_GPIO_PORT, OSPI_D0_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(OSPI_D1_GPIO_PORT, OSPI_D1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(OSPI_D2_GPIO_PORT, OSPI_D2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(OSPI_D3_GPIO_PORT, OSPI_D3_PIN, GPIO_PIN_SET);
	// drive SCK low
	HAL_GPIO_WritePin(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN, GPIO_PIN_RESET);

	// Start with a sequence of 2 clock cycles. For devices which are in QPI
	// mode this will send the opcode FFh which is the "exit QPI" command

	// drive CS low
	HAL_GPIO_WritePin(OSPI_CS_GPIO_PORT, OSPI_CS_PIN, GPIO_PIN_RESET);

	for(i = 0; i < 2; i++)
	{
		HAL_GPIO_WritePin(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN, GPIO_PIN_RESET);
	}

	// drive CS high
	HAL_GPIO_WritePin(OSPI_CS_GPIO_PORT, OSPI_CS_PIN, GPIO_PIN_SET);

	
	// Finally send a sequence of 8 clock cycles. For devices which are in
	// 0-4-4 mode this will look like a read command with a mode byte of FFh
	// which will cause it to exit 0-4-4.

	// drive CS low
	HAL_GPIO_WritePin(OSPI_CS_GPIO_PORT, OSPI_CS_PIN, GPIO_PIN_RESET);

	for(i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN, GPIO_PIN_RESET);
	}

	// drive CS high
	HAL_GPIO_WritePin(OSPI_CS_GPIO_PORT, OSPI_CS_PIN, GPIO_PIN_SET);

}


///////////////////////////////////////////////////////////////////////////////
// Function: fhc_init
// This function initializes the OctoSPI flash host controller
// Arguments:
// None
// Returns:
// Completion status: one of ADESTO_STATUS_OK, ADESTO_STATUS_FAIL.
///////////////////////////////////////////////////////////////////////////////
uint32_t fhc_init(void)
{
	/* Enable the OSPI memory interface clock */
	OSPI_CLK_ENABLE();

	/* Reset the OSPI memory interface */
	OSPI_FORCE_RESET();
	OSPI_RELEASE_RESET();

	/* Enable GPIO clocks */
	OSPI_CS_GPIO_CLK_ENABLE();
	OSPI_CLK_GPIO_CLK_ENABLE();
	OSPI_D0_GPIO_CLK_ENABLE();
	OSPI_D1_GPIO_CLK_ENABLE();
	OSPI_D2_GPIO_CLK_ENABLE();
	OSPI_D3_GPIO_CLK_ENABLE();
	OSPI_D4_GPIO_CLK_ENABLE();
	OSPI_D5_GPIO_CLK_ENABLE();
	OSPI_D6_GPIO_CLK_ENABLE();
	OSPI_D7_GPIO_CLK_ENABLE();
	OSPI_DQS_GPIO_CLK_ENABLE();

	// First make interface pins GPIOs. We'll need that to perform the
	// reset operations
	fhc_assign_pins_to_gpio();

	// Reset flash using JEDEC reset sequence
	fhc_jreset();

	// Perform other potentially necessary reset operations in case the
	// target flash part is in QPI mode or 0-4-4 mode.These modes must be
	// exited in order to communicate with the target in normal SPI mode.
	fhc_misc_reset();

	/* Initialize OSPI ----------------------------------------------------- */
	OSPIHandle.Instance = OCTOSPI1;

	OSPIHandle.Init.ClockPrescaler        = 4; /* OSPI clock = 280MHz / 4 = 70MHz */
	//OSPIHandle.Init.ClockPrescaler        = 3; /* OSPI clock = 280MHz / 3 = 93.33MHz */
	OSPIHandle.Init.FifoThreshold         = 4;
	OSPIHandle.Init.MemoryType            = HAL_OSPI_MEMTYPE_MICRON;
	OSPIHandle.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE; // may have to try HAL_OSPI_SAMPLE_SHIFTING_NONE
	OSPIHandle.Init.DeviceSize             = EXT_FLASH_SIZE_LOG2 - 1;
	OSPIHandle.Init.ChipSelectHighTime    = 3;
	OSPIHandle.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
	OSPIHandle.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
	OSPIHandle.Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
	//OSPIHandle.Init.FlashID               = HAL_OSPI_FLASH_ID_1;
	OSPIHandle.Init.DualQuad             = HAL_OSPI_DUALQUAD_DISABLE;
	OSPIHandle.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
	OSPIHandle.Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_BYPASSED;
	OSPIHandle.Init.ChipSelectBoundary    = 0;


	if (HAL_OSPI_Init(&OSPIHandle) != HAL_OK)
		return ADESTO_STATUS_FAIL;

	// Initialize the command structure with defualt values
	flash_cmd.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
	flash_cmd.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	flash_cmd.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
	flash_cmd.AlternateBytes     = HAL_OSPI_ALTERNATE_BYTES_NONE;
	flash_cmd.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
	flash_cmd.Address            = 0;
	flash_cmd.DQSMode            = HAL_OSPI_DQS_DISABLE;
	flash_cmd.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

	// Assign flash interface pins to OctoSPI
	fhc_assign_pins_to_octospi();

	return ADESTO_STATUS_OK;
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
	return ADESTO_STATUS_OK;
}

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
uint32_t fhc_send_cmd(uint8_t opcode, uint8_t cmd_type, uint32_t addr, uint32_t addr_size,
						uint8_t *data_p, uint32_t data_size)
{
	flash_cmd.Instruction = opcode;
	if(addr_size != 0)
	{
		flash_cmd.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
		flash_cmd.Address = addr;
		flash_cmd.AddressSize = (addr_size == 3) ? HAL_OSPI_ADDRESS_24_BITS : HAL_OSPI_ADDRESS_32_BITS;
	}
	else
	{
		flash_cmd.AddressMode = HAL_OSPI_ADDRESS_NONE;
		flash_cmd.AddressSize = 0;
	}

	flash_cmd.NbData = data_size;
	if(data_size != 0)
		flash_cmd.DataMode = HAL_OSPI_DATA_1_LINE;
	else
		flash_cmd.DataMode = HAL_OSPI_DATA_NONE;
	
	if(opcode == CMD_FAST_READARRAY)
		flash_cmd.DummyCycles = 8;
	else
		flash_cmd.DummyCycles = 0;

	if (HAL_OSPI_Command(&OSPIHandle, &flash_cmd, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
        return ADESTO_STATUS_FAIL;

	if(cmd_type == CMD_TYPE_READ)
		if (HAL_OSPI_Receive(&OSPIHandle, data_p, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
			return ADESTO_STATUS_FAIL;
	if(cmd_type == CMD_TYPE_WRITE)
		if (HAL_OSPI_Transmit(&OSPIHandle, data_p, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
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

	return OCTOSPI1_BASE;
}