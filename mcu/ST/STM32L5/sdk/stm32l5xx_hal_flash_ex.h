/**
  ******************************************************************************
  * @file    stm32l5xx_hal_flash_ex.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L5xx_HAL_FLASH_EX_H
#define __STM32L5xx_HAL_FLASH_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_hal_def.h"

/** @addtogroup STM32L5xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASHEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/** @addtogroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */

/**
  * @brief  FLASH Block-based security structure definition
  */
typedef struct
{
  uint32_t Bank;             /*!< Configuration of the associated bank of Block-based Secure Area.
                                  This parameter must be a value of @ref FLASH_Banks */
  uint32_t BBSecReg1;        /*!< Secure page attribute for pages 0 to 31.
                                  This parameter is a bit field and has a value between 0x0 and 0xFFFFFFFF */
  uint32_t BBSecReg2;        /*!< Secure page attribute for pages 32 to 63.
                                  This parameter is a bit field and has a value between 0x0 and 0xFFFFFFFF */
  uint32_t BBSecReg3;        /*!< Secure page attribute for pages 64 to 95.
                                  This parameter is a bit field and has a value between 0x0 and 0xFFFFFFFF */
  uint32_t BBSecReg4;        /*!< Secure page attibute for pages 96 to 127.
                                  This parameter is a bit field and has a value between 0x0 and 0xFFFFFFFF */
} FLASH_BBSecInitTypeDef;
/**
  * @}
  */
#endif

/* Exported constants --------------------------------------------------------*/
/** @addtogroup FLASHEx_Exported_Constants
  * @{
  */
/** @defgroup PRIV_MODE_CFG FLASH privilege mode configuration
  * @{
  */
#define FLASH_PRIV_GRANTED   0x00000000U          /*!< access to Flash registers is granted */
#define FLASH_PRIV_DENIED    FLASH_PRIVCFGR_PRIV /*!< access to Flash registers is denied to non-privilege access */
/**
  * @}
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/** @defgroup SEC_INVERSION_CFG FLASH security inversion configuration
  * @{
  */
#define FLASH_INV_DISABLE    0x00000000U        /*!< Security state of Flash is not inverted */
#define FLASH_INV_ENABLE     FLASH_SECCR_SECINV /*!< Security state of Flash is inverted */
/**
  * @}
  */
#endif

/** @defgroup LVE_PIN_CFG FLASH LVE pin configuration
  * @{
  */
#define FLASH_LVE_PIN_CTRL   0x00000000U       /*!< LVEA/B FLASH pin controlled by power controller */
#define FLASH_LVE_PIN_FORCED FLASH_ACR_LVEN /*!< LVEA/B FLASH pin enforced to low */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASHEx_Exported_Functions
  * @{
  */

/* Extended Program operation functions  *************************************/
/** @addtogroup FLASHEx_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
void              HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
HAL_StatusTypeDef HAL_FLASHEx_ConfigBBSec(FLASH_BBSecInitTypeDef *pBBSecInit);
void              HAL_FLASHEx_GetBBSec(FLASH_BBSecInitTypeDef *pBBSecInit);
void              HAL_FLASHEx_EnableSecHideProtection(uint32_t Banks);
#endif
/**
  * @}
  */

/** @addtogroup FLASHEx_Exported_Functions_Group2
  * @{
  */
void              HAL_FLASHEx_ConfigPrivMode(uint32_t PrivMode);
uint32_t          HAL_FLASHEx_GetPrivMode(void);
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
HAL_StatusTypeDef HAL_FLASHEx_ConfigSecInversion(uint32_t Cfg);
uint32_t          HAL_FLASHEx_GetSecInversion(void);
#endif
HAL_StatusTypeDef HAL_FLASHEx_ConfigLVEPin(uint32_t ConfigLVE);
uint32_t          HAL_FLASHEx_GetLVEPin(void);
/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Macros FLASH Private Macros
 *  @{
 */
#define IS_FLASH_CFGPRIVMODE(CFG)          (((CFG) == FLASH_PRIV_GRANTED) || \
                                            ((CFG) == FLASH_PRIV_DENIED))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_FLASH_CFGSECINV(CFG)            (((CFG) == FLASH_INV_DISABLE) || \
                                            ((CFG) == FLASH_INV_ENABLE))
#endif

#define IS_FLASH_CFGLVEPIN(CFG)            (((CFG) == FLASH_LVE_PIN_CTRL) || \
                                            ((CFG) == FLASH_LVE_PIN_FORCED))

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L5xx_HAL_FLASH_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
