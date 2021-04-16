//*****************************************************************************
// mcux_flash_msgif.h
//
// MCUXpresso flash driver message-based interface (V2) header
//*****************************************************************************
//
// Copyright 2014-2019 NXP
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

#ifndef _MCUX_FLASH_MSGIF_H
#define _MCUX_FLASH_MSGIF_H

/***********************************************************
 *
 *  WARNING: This file is intended to be identical in both the
 *           host debug stub and the target flash driver.
 *           Do not modify one without modifying the other.
 *
 ***********************************************************/

#include <stdint.h>

#include "mcux_flash_info.h"  /* for FlashDeviceV_t */

#define MEM_FLASH_VER2_MAJ 0x0020  // (defined only in V2 devices)

#define MEM_FLASH_MAJ_VERSION(ver) ((ver) >> 4)
#define MEM_FLASH_SAME_VER(required, submitted) \
	(MEM_FLASH_MAJ_VERSION(submitted) == MEM_FLASH_MAJ_VERSION(required))
#define MEM_FLASH_IS_VER1(submitted) \
        MEM_FLASH_SAME_VER(MEM_FLASH_VER1_MAJ, submitted)
#define MEM_FLASH_IS_VER2(submitted) \
        MEM_FLASH_SAME_VER(MEM_FLASH_VER2_MAJ, submitted)

#define MEM_FLASH_MAGIC_STR "\x01\x23\x45\x00\x00\x54\x32\x10"


#ifdef __cplusplus
 extern "C" {
#endif


// ===========================
// Mailbox and messages
// ===========================

typedef enum {
  // Generic
  FOP_NONE               = 0x00000000,
  FOP_OPEN               = 0x00000001,   // internal pseudo op
  FOP_PROGRAM            = 0x00000002,
  FOP_ERASE_SECTOR       = 0x00000008,
  FOP_ERASE_ALL          = 0x00000010,
  FOP_BLANK_CHECK_SECTOR = 0x00000020,
  FOP_VERIFY             = 0x00000040,
  FOP_CLOSE              = 0x00000080,   // internal pseudo op
  FOP_TERMINATE          = 0x00000100,
  FOP_PROGRAM_SECTORS    = 0x00004000
} fop_t;
#define e_FCMD  uint32_t

#define fop_str(op) (                          \
      (op) == FOP_NONE?               "NoOperation": \
      (op) == FOP_OPEN?               "Init": \
      (op) == FOP_PROGRAM?            "ProgramPage": \
      (op) == FOP_ERASE_SECTOR?       "EraseSector": \
      (op) == FOP_ERASE_ALL?          "EraseChip": \
      (op) == FOP_BLANK_CHECK_SECTOR? "BlankCheck": \
      (op) == FOP_VERIFY?             "Verify": \
      (op) == FOP_CLOSE?              "Close": \
      (op) == FOP_TERMINATE?          "Terminate": \
      (op) == FOP_PROGRAM_SECTORS?    "ProgramSectors": \
                                      "<cmd unknown>")

typedef enum {
  FSTATUS_BUSY       = 0,    // executing previous op
  FSTATUS_ERROR      = 0x01, // operation caused an error
  FSTATUS_NOSUPPORT  = 0x02, // operation is not supported
  FSTATUS_INITFAIL   = 0x40, // init failed - not executed
  FSTATUS_READY      = 0x80, // now ready for next op
  FSTATUS_ERR_MASK   = 0x7F  // any of these set => failed
} fstatus_t;

#define FSTATUS_VALID(stat) (((stat) & 0xFFFFFF00) == 0)

#define FSTATUS_IAP_ERROR FSTATUS_ERROR
//< deprecated form of name for backward compatibility

#define eFSTAT  uint32_t

// Mailbox parameter block (converted to IAP call format by resident driver)
// param[0]: destination address
// param[1]: source address
// param[2]: number of bytes
// param[3]: start_sector
// param[4]: end_sector
// param[5]: system clock frequency (Hz)
// param[6]: IAP entry point

// result block format
// result[0]: status code (zero == success)

// Mailbox parameter block format (used by resident driver)
typedef struct {
    volatile uint32_t  result;
    volatile uint32_t  param[3];
    volatile uint32_t  status;     // slave only
    volatile uint32_t  command;    // master only
    // NOTE: This field MUST be the last in the data structure
    //       It is assumed that when a copy of this mailbox
    //       is made into the target address space that the
    //       command will be the very last item updated.
    //       (And that it will be updated atomically)
} Mailbox_t; // 24 bytes

//typedef volatile void (*IAP_IF)(volatile uint32_t *command,
//                                volatile uint32_t *result);

// these args are delivered through being present in the
// mailbox when it is first opened
#define FOPARG__OPEN_DEVADDR        0  // device address on open
#define FOPARG__OPEN_CLKHZ          1  // clock frequence in Hz
// In an open for a dynamic driver a pointer is left in the RESULT field
// of the Init which points to an Mailbox_Init_Dyninfo structure (see below).

// no arguments for ERASE_ALL

#define FOPARG_ERASESECT_FADDR      0  // flash address of first sector
#define FOPARG_ERASESECT_SECTORS    2  // number of sectors to erase

#define FOPARG_BLANKCHECK_FADDR     0  // flash address of first sector
#define FOPARG_BLANKCHECK_SECTORS   2  // number of sectors to check

#define FOPARG_PROGRAMPAGES_FADDR   0  // destination flash address
#define FOPARG_PROGRAMPAGES_MEMADDR 1  // source memory address
#define FOPARG_PROGRAMPAGES_BYTES   2  // number of bytes to program

#define FOPARG_VERIFY_FADDR         0  // verified flash address
#define FOPARG_VERIFY_MEMADDR       1  // correct source in memory address
#define FOPARG_VERIFY_BYTES         2  // number of bytes to program

#define FOPARG_PROGSECTS_FADDR      0  // destination flash address
#define FOPARG_PROGSECTS_MEMADDR    1  // source memory address
#define FOPARG_PROGSECTS_SECTORS    2  // number of sectors to program

// =======================================
// Flash Programming Information Structure
// =======================================

/*
 *  Service Flash Operation Messages
 *    Parameter:      mailbox:  Address where messages are read/written
 *    Return Value:   None - does not return
 */
typedef void ServiceMessages_fn_t (Mailbox_t *mailbox);


/* Current message-based flash driver */
typedef struct {
    /* V.1 and V.2 common header */
    uint16_t        version;      /* Version 2 of Code Red flash interface */
    uint8_t         magic_no[8];  /* Magic no. to identify flash driver file */
    uint32_t        load_base;    /* Driver load address */
    uint32_t        image_size;   /* Size of .text and .data */
    uint32_t        pcache;       /* RAM buffer location */
    uint32_t        cache_size;   /* RAM buffer size */
    uint32_t        pstack;       /* Stack top */
    uint32_t        stack_size;   /* Stack size */

    /* Flash device structure (V.2) */
    uint32_t        opmap;        /* Bitmap of available operations */
    MEM_FLASH_PTR(const FlashDeviceV_t) dev;    /* Flash description */

    MEM_FLASH_PTR(ServiceMessages_fn_t) ServiceMessages;
    uint32_t        _ReserveD;

} MemoryDeviceMsg_t; /* V2 definition kept for minimizing static mem use */

/* V.2 operation bit map - (default value is zero) - bit definitions */

#define OPMAP_NO_ERASE_CHIP     0x0001  /* 'EraseChip' not implemented */
#define OPMAP_NO_VERIFY         0x0002  /* 'Verify' not implemented */
#define OPMAP_PROGRAM_NOVERIFY  0x0004  /* 'Program' does not 'Verify' too */
#define OPMAP_HAS_PROGRAM_SECTS 0x0008  /* 'ProgramSectors' is implemented */
#define OPMAP_HAS_BLANKCHECK    0x0010  /* 'BlankCheck' is implemented */
#define OPMAP_HAS_SLOWERASE     0x0020  /* 'EraseSector' O(sectors) not O(1) */
#define OPMAP_DEVINFO_DYNAMIC   0x0040  /* New dev info available after Init */
#define OPMAP_FLASH_UNMAPPED    0x0080  /* Flash not accessible as memory */
#define OPMAP_RST_CLEAN         0x0100  /* Core reset unneeded between 2 runs */
#define OPMAP_RST_VULNERABLE    0x0200  /* Don't reset core after running */
#define OPMAP_ERASED_WO         0x0400  /* Read of erased area causes error */

/* Note - if OPMAP_PROGRAM_NOVERIFY is set and OPMAP_NO_VERIFY is clear
 *        programmed data will be verified by reading it back to the host
 *        (at some expense) */

/*! Data provided only by drivers with the OPMAP_DEVINFO_DYNAMIC flag set */
typedef struct {
    MEM_FLASH_PTR(const FlashDeviceV_t) dyn_dev;  /* updated flash info */
    MEM_FLASH_PTR(const char) devname;   /* C string with device name */
    /* devname normally expected to be 63 characters or less */
    uint32_t partid_bytes;               /* bytes in the part ID, 0 if unset */
    MEM_FLASH_PTR(const uint8_t) partid; /* binary bytes of flash part ID */
} Mailbox_Init_DynInfo_t;

#ifdef __cplusplus
}
#endif


#endif  // _MCUX_FLASH_MSGIF_H
