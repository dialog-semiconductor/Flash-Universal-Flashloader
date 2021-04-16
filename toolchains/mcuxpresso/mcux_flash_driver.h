//*****************************************************************************
// mcux_flash_driver.h
//
// MCUXpresso flash driver header (Messaged)
//*****************************************************************************
//
// Copyright 2014-2015, 2018 NXP
// All rights reserved.
//
// Software that is described herein is for illustrative purposes only which
// provides customers with programming information regarding the LPC products.
// This software is supplied "AS IS" without any warranties of any kind, and
// NXP Semiconductors and its licensor disclaim any and all warranties, express
// or implied, including all implied warranties of merchantability, fitness for
// a particular purpose and non-infringement of intellectual property rights.
// NXP Semiconductors assumes no responsibility or liability for the use of the
// software, conveys no license or rights under any patent, copyright, mask
// work right, or any other intellectual property rights in or to any
// products. NXP Semiconductors reserves the right to make changes in the
// software without notification. NXP Semiconductors also makes no
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

#ifndef _MCUX_FLASH_DRIVER_H
#define _MCUX_FLASH_DRIVER_H

/***********************************************************
 *
 *  WARNING: This file is intended to be identical in both the
 *           host debug stub and the target flash driver.
 *           Do not modify one without modifying the other.
 *
 ***********************************************************/
#include <mcux_flash_msgif.h>
#include <stdint.h>

#define WEAK                 __attribute__ ((weak))
#define FLASHDEV_SECTION     __attribute__ ((section(".FlashDevice")))
#define EXTERN_MEMORY_DEVICE __attribute__ ((section(".MemoryDevice")))

#define VERS 1                             // struct FlashDevice minor version
#define FLASH_DRV_VERS FLASH_DRV_VER(VERS) // FlashDriver Version

#ifdef __cplusplus
 extern "C" {
#endif

// ===========================
// Flash Programming Functions
// ===========================

 /*
  *  Initialize Flash Programming Functions
  *    Return Value:   0 - OK,  1 - Failed
  */
typedef uint32_t Init_fn_t (void);

  /*
   *  De-Initialize Flash Programming Functions
   *    Return Value:   0 - OK,  1 - Failed
   */
typedef uint32_t UnInit_fn_t (void);

  /*
   *  Erase complete Flash Memory
   *    Return Value:   0 - OK,  1 - Failed
   */
typedef uint32_t EraseChip_fn_t (void);

  /*
   *  Erase Sector in Flash Memory
   *    Parameter:      adr:  Sector Address
   *                numsecs: Number of sectors
   *    Return Value:   0 - OK,  1 - Failed
   */
typedef uint32_t EraseSectors_fn_t (uint32_t adr, uint32_t numsecs);

  /*
   *  Program Page in Flash Memory
   *    Parameter:      adr:  Page Start Address
   *                    sz:   Page Size
   *                    buf:  Page Data
   *    Return Value:   0 - OK,  1 - Failed
   */
typedef uint32_t ProgramPage_fn_t (uint32_t adr, uint32_t sz, uint8_t *buf);

/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
typedef uint32_t Verify_fn_t (uint32_t adr, uint32_t sz, uint8_t *buf);


/* NB - weak references to optional driver functions */
     
     Init_fn_t             Init;
     UnInit_fn_t           UnInit;
WEAK EraseChip_fn_t        EraseChip;
     EraseSectors_fn_t     EraseSectors;
     ProgramPage_fn_t      ProgramPage;
WEAK Verify_fn_t           Verify;

/* weak references to messaged driver function proformas */
     
ServiceMessages_fn_t  ServiceMessages;

/* The flash device instance */
extern FlashDeviceV_t FlashDevice;

     
#ifdef __cplusplus
}
#endif


#endif  // _MCUX_FLASH_DRIVER_H
