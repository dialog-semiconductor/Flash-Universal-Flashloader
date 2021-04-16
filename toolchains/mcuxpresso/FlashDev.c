//*****************************************************************************
// FlashDev.c
// MCUXpresso Flash driver - Flash Device settings
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

#include <FlashConfig.h>
#include <mcux_flash_driver.h>
#include "fsl_common.h"

#if defined(CPU_MIMXRT1052DVL6B) || defined(CPU_MIMXRT1062DVL6A) || defined(CPU_MIMXRT685SFVKB)
#define FLEXSPI_FLASH_BASE_ADDR FlexSPI_AMBA_BASE
#endif
#if defined(CPU_MIMXRT1176AVM8A_cm7)
#define FLEXSPI_FLASH_BASE_ADDR FlexSPI1_AMBA_BASE
#endif
#if defined(CPU_MIMXRT595SFFOB)
#define FLEXSPI_FLASH_BASE_ADDR FlexSPI0_AMBA_BASE
#endif

FLASHDEV_SECTION
FlashDeviceV_t FlashDevice  =  {
   FLASH_DRV_VERS,				// Driver Version, do not modify!
   "Adesto Universal Flash Loader "__DATE__" "__TIME__,
   EXTSPI,     					// Device Type
   FLEXSPI_FLASH_BASE_ADDR, 	// Device Start Address
   FLASH_SIZE,   				// Device Size
   PSEUDO_PAGE_SIZE,			// Programming Page Size
   0,          					// Reserved, must be 0
   0xFF,       					// Initial Content of Erased Memory
   3000,       					// Program Page Timeout
   6000,      					// Erase Sector Timeout
   // Specify Size and Address of Sectors
   {{FLASH_SECTOR_SIZE, 0},		// Sector sizes
   {SECTOR_END}}
};



