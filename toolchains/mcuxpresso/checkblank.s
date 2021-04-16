//*****************************************************************************
// MCUXpresso Flash driver - checkblank.s
//
// Assembler routine to check flash is erased
//
//*****************************************************************************
//
// Copyright 2014, 2018 NXP
// All rights reserved.
//
// Software that is described herein is for illustrative purposes only
// which provides customers with programming information regarding the
// LPC products.  This software is supplied "AS IS" without any warranties of
// any kind, and NXP Semiconductors and its licensor disclaim any and
// all warranties, express or implied, including all implied warranties of
// merchantability, fitness for a particular purpose and non-infringement of
// intellectual property rights.  NXP Semiconductors assumes no responsibility
// or liability for the use of the software, conveys no license or rights under any
// patent, copyright, mask work right, or any other intellectual property rights in
// or to any products. NXP Semiconductors reserves the right to make changes
// in the software without notification. NXP Semiconductors also makes no
// representation or warranty that such application will be suitable for the
// specified use without further testing or modification.
//
// Permission to use, copy, modify, and distribute this software and its
// documentation is hereby granted, under NXP Semiconductors' and its
// licensor's relevant copyrights in the software, without fee, provided that it
// is used in conjunction with NXP Semiconductors microcontrollers.  This
// copyright, permission, and disclaimer notice must appear in all copies of
// this code.
//*****************************************************************************

.syntax unified
 /* Put these codes into .text section */
 .text
 .align 4
 .section ".text.checkblank","ax",%progbits

/*uint32_t checkblank(uint32_t adr, uint32_t words, uint32_t blankval)
*/
  .global checkblank
   /* Declare  as global..Otherwise the linker won't find it */
  .func
  .thumb_func
checkblank:
    PUSH {r4-r11,lr}
loop:
    LDMIA r0!, {r4-r11}
    CMP r4, R2
	BNE fail
    CMP r5, R2
	BNE fail
	CMP r6, R2
	BNE fail
	CMP r7, R2
	BNE fail
	CMP r8, R2
	BNE fail
    CMP r9, R2
	BNE fail
	CMP r10, R2
	BNE fail
	CMP r11, R2
	BNE fail
	SUBS r1,#8
	BNE loop
blank:
    MOVS r0,#0
    POP {r4-r11,pc}

fail:
    MOVS r0,#1
    POP {r4-r11,pc}


