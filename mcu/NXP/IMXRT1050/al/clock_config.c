/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "clock_config.h"

//ARM PLL configuration for RUN mode
const clock_arm_pll_config_t armPllConfig = {.loopDivider = 88};

// SYS PLL configuration for RUN mode
//const clock_sys_pll_config_t sysPllConfig = {.loopDivider = 1U};

// USB1 PLL configuration for RUN mode
const clock_usb_pll_config_t usb1PllConfig = {.loopDivider = 0U};

// System clock frequency
extern uint32_t SystemCoreClock;

void BOARD_BootClockRUN(void)
{
    // Boot ROM did initialize the XTAL, here we only sets external XTAL OSC freq
    CLOCK_SetXtalFreq(24000000U);
    CLOCK_SetRtcXtalFreq(32768U);

	// Before PLLs are configured select a temporary safe clock source for CPU core 
	// and bus clocks
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1); // Set PERIPH_CLK2 source to OSC
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);     // Set PERIPH_CLK source to PERIPH_CLK2

	// PLL1 will be used for CPU and bus clocks
    CLOCK_InitArmPll(&armPllConfig); // Configure ARM PLL to 1056MHz

	// PLL2 (a.k.a. system PLL) is not used for now 
    //CLOCK_InitSysPll(&sysPllConfig); // Configure SYS PLL to 528M

	// PLL3 (a.k.a. USB1 PLL) will be used as source of FlexSPI clock
    CLOCK_InitUsb1Pll(&usb1PllConfig); // Configure USB1 PLL to 480M


	// Set drividers for CPU and bus clocks
    CLOCK_SetDiv(kCLOCK_ArmDiv, 0x1); // Set ARM PODF to 1, divide by 2
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0); // Set AHB PODF to 0, divide by 1
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); // Set IPG PODF to 3, divede by 4 (IPG = peripheral bus?)

	// Now that PLLs have been configured select PLL1 as source for CPU core and bus clocks
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x3); // Set PRE_PERIPH_CLK source to PLL1
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);    // Set PERIPH_CLK MUX source to PRE_PERIPH_CLK

    /* Disable unused clock */
    //BOARD_BootClockGate();

#if 0
    // Configure UART divider to default
    CLOCK_SetMux(kCLOCK_UartMux, 0); // Set UART source to PLL3 80M
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); // Set UART divider to 1
#endif

    // Update core clock information
	// This is for book keeping purposes - no change of HW state
    SystemCoreClockUpdate();
}
