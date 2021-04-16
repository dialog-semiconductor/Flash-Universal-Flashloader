#include "clock_config.h"
#include "fsl_common.h"

void sys_gen_init(void)
{
	// Various system initializations (could be optional but we want to play safe. 
	// Normally this is called from the reset handler but we don't have one in the
	// flash loader environment.
	SystemInit();

	// Configure clock tree (configure muxes to set various clock sources and initialize PLLs) 
    BOARD_BootClockRUN();
}
