//*****************************************************************************
// ServiceMessages.c
//
// Polling wrapper for MCUXpresso flash driver (Messaged)
//*****************************************************************************
//
// Copyright 2014-2015, 2018 NXP
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

/***********************************************************
 *
 *  WARNING: This target flash driver file is intended to
 *           function in conjunction with host debug stub.
 *           Do not modify one without modifying the other.
 *
 ***********************************************************/

#include <mcux_flash_driver.h>
#include <mcux_flash_msgif.h>
#include <stdbool.h>
#include <stdint.h>

__attribute__ ((naked))
void ServiceMessages(Mailbox_t *msg) {
	//	__disable_irq();
	__asm volatile ("cpsid i");

	uint32_t initres = 0;
	uint32_t cmd;
	uint32_t status;
	initres = Init();
	msg->result = initres;
	msg->command = FOP_NONE;

	do {
		__asm volatile ("dmb");
		cmd = msg->command;
		int resultCode = 0;

		if (cmd != 0) {
			// Message from host has been received
			status = msg->status = FSTATUS_BUSY;

			// Check if Init() call failed - in which case we don't want
			//  to try to program the flash!
			if (initres != 0) {
				msg->result = initres;
				status |= FSTATUS_INITFAIL;

			} else {
				if (cmd & FOP_ERASE_SECTOR) {
					resultCode = EraseSectors(
							msg->param[FOPARG_ERASESECT_FADDR],
							msg->param[FOPARG_ERASESECT_SECTORS]);
#if !defined (MINRELEASE)
				} else if (cmd & FOP_ERASE_ALL) {
					if (EraseChip)
						resultCode = EraseChip();
#endif
				} else if (cmd & (FOP_PROGRAM | FOP_VERIFY)) {
					if (cmd & FOP_PROGRAM) {
						/* Implement this as 'program and verify' */
						resultCode =
								ProgramPage(
										msg->param[FOPARG_PROGRAMPAGES_FADDR],
										msg->param[FOPARG_PROGRAMPAGES_BYTES],
										(uint8_t *) (msg->param[FOPARG_PROGRAMPAGES_MEMADDR]));
					}
#if !defined (MINRELEASE)
					/* could ignore opcode - it gets handled in FOP_PROGRAM now */
					if (!resultCode && Verify)
						resultCode =
								Verify(msg->param[FOPARG_VERIFY_FADDR],
										msg->param[FOPARG_VERIFY_BYTES],
										(uint8_t *) (msg->param[FOPARG_VERIFY_MEMADDR]));
#endif
				} else if (cmd & (FOP_TERMINATE | FOP_CLOSE)) {
#if !defined (MINRELEASE)
					UnInit();
#endif
				} else {
					status |= FSTATUS_NOSUPPORT;
					resultCode = cmd;
				}
				msg->result = resultCode;
				if (resultCode != 0)
					status |= FSTATUS_IAP_ERROR;
			}
			msg->status = status;
			msg->command = FOP_NONE;
			/*< release mailbox for reuse by the client */
		}

	} while (cmd != FOP_TERMINATE);
	while (true)
		continue; /* we're not expected to return - even after a terminate message */
}
