//*****************************************************************************
// mcux_flash_memdev.c
//
// MCUXpresso flash driver interface (Messaged) - Memory device definition
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

/***********************************************************
 *
 *  WARNING: This file is intended to be identical in both the
 *           host debug stub and the target flash driver.
 *           Do not modify one without modifying the other.
 *
 ***********************************************************/

#include <mcux_flash_driver.h>

// Symbols defined in the linker script
extern uint32_t   _b_bss;
extern uint32_t   _e_bss;
extern uint32_t   __load_base;
extern uint32_t   __image_size;
extern uint32_t   __cache;
extern uint32_t   __cache_size;   // redefine in instance structure
extern uint32_t   __initial_sp;
extern uint32_t   __stack_size;
extern uint32_t   __opmap_val;

// MemoryDevice Instance (fill it in)
EXTERN_MEMORY_DEVICE
MemoryDeviceMsg_t MemoryDevice =
{
  MEM_FLASH_VER2_MAJ+0x0,       // Version of flash interface
                                // Magic number to identify flash driver
                                // interface
  { 0x01, 0x23, 0x45, 0x00, 0x00, 0x54, 0x32, 0x10 },
  (uint32_t)&__load_base,         // Driver load address
  (uint32_t)&__image_size,        // Size of .text and .data
  (uint32_t)&__cache,             // RAM buffer location
  (uint32_t)&__cache_size,        // RAM buffer size
  (uint32_t)&__initial_sp,        // Stack top
  (uint32_t)&__stack_size,        // Stack size
  (uint32_t)&__opmap_val,        // Bitmap of available operations - 0 = everything there
  &FlashDevice,                 // Flash Device configuration
  ServiceMessages,              // Service mailbox flash operation message
  0                              // Reserved
};

