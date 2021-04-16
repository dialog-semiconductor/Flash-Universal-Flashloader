#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_ospi.h"




//This is the  pin selection for the NUCLEO-H7A3ZI board
/* Definition for OSPI Pins */
/* OSPI_CS */
#define OSPI_CS_PIN                       GPIO_PIN_6
#define OSPI_CS_GPIO_PORT                 GPIOG
#define OSPI_CS_PIN_AF                    GPIO_AF10_OCTOSPIM_P1
/* OSPI_CLK */
#define OSPI_CLK_PIN                      GPIO_PIN_2
#define OSPI_CLK_GPIO_PORT                GPIOB
#define OSPI_CLK_PIN_AF                   GPIO_AF9_OCTOSPIM_P1
/* OSPI_D0 */
#define OSPI_D0_PIN                       GPIO_PIN_11
#define OSPI_D0_GPIO_PORT                 GPIOD
#define OSPI_D0_PIN_AF                    GPIO_AF9_OCTOSPIM_P1
/* OSPI_D1 */
#define OSPI_D1_PIN                       GPIO_PIN_12
#define OSPI_D1_GPIO_PORT                 GPIOD
#define OSPI_D1_PIN_AF                    GPIO_AF9_OCTOSPIM_P1
/* OSPI_D2 */
#define OSPI_D2_PIN                       GPIO_PIN_2
#define OSPI_D2_GPIO_PORT                 GPIOE
#define OSPI_D2_PIN_AF                    GPIO_AF9_OCTOSPIM_P1
/* OSPI_D3 */
#define OSPI_D3_PIN                       GPIO_PIN_13
#define OSPI_D3_GPIO_PORT                 GPIOD
#define OSPI_D3_PIN_AF                    GPIO_AF9_OCTOSPIM_P1
/* OSPI_D4 */
#define OSPI_D4_PIN                       GPIO_PIN_4
#define OSPI_D4_GPIO_PORT                 GPIOD
#define OSPI_D4_PIN_AF                    GPIO_AF10_OCTOSPIM_P1
/* OSPI_D5 */
#define OSPI_D5_PIN                       GPIO_PIN_5
#define OSPI_D5_GPIO_PORT                 GPIOD
#define OSPI_D5_PIN_AF                    GPIO_AF10_OCTOSPIM_P1
/* OSPI_D6 */
#define OSPI_D6_PIN                       GPIO_PIN_6
#define OSPI_D6_GPIO_PORT                 GPIOD
#define OSPI_D6_PIN_AF                    GPIO_AF10_OCTOSPIM_P1
/* OSPI_D7 */
#define OSPI_D7_PIN                       GPIO_PIN_7
#define OSPI_D7_GPIO_PORT                 GPIOD
#define OSPI_D7_PIN_AF                    GPIO_AF10_OCTOSPIM_P1
/* OSPI_DQS */
#define OSPI_DQS_PIN                       GPIO_PIN_1
#define OSPI_DQS_GPIO_PORT                 GPIOA
#define OSPI_DQS_PIN_AF                    GPIO_AF11_OCTOSPIM_P1


/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()           __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()          __HAL_RCC_OSPI1_CLK_DISABLE()
#define OSPIM_CLK_ENABLE()          __HAL_RCC_OCTOSPIM_CLK_ENABLE()
#define OSPIM_CLK_DISABLE()         __HAL_RCC_OCTOSPIM_CLK_DISABLE()
#define OSPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOG_CLK_ENABLE()
#define OSPI_CLK_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define OSPI_FORCE_RESET()          __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI_RELEASE_RESET()        __HAL_RCC_OSPI1_RELEASE_RESET()


// Misc defs
#define EXT_FLASH_PAGE_SIZE  256
#define EXT_FLASH_SECTORE_SIZE 4096
#define EXT_FLASH_SIZE 0x1000000
#define EXT_FLASH_SIZE_LOG2 23 //log2 of flash size - necessary for one of the OSPI fields

