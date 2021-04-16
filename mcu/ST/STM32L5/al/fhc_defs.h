#include "stm32l5xx_hal.h"
#include "stm32l5xx_hal_ospi.h"


#ifdef STM32L5_EVAL
//This is the  pin selection for the STM32L5-EVAL board
/* Definition for OSPI Pins */
/* OSPI_CLK */
#define OSPI_CLK_PIN                      GPIO_PIN_3
#define OSPI_CLK_GPIO_PORT                GPIOA
#define OSPI_CLK_PIN_AF                   GPIO_AF10_OCTOSPI1
/* OSPI_DQS */
#define OSPI_DQS_PIN                      GPIO_PIN_2
#define OSPI_DQS_GPIO_PORT                GPIOB
#define OSPI_DQS_PIN_AF                   GPIO_AF10_OCTOSPI1
/* OSPI_CS */
#define OSPI_CS_PIN                       GPIO_PIN_2
#define OSPI_CS_GPIO_PORT                 GPIOA
#define OSPI_CS_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D0 */
#define OSPI_D0_PIN                       GPIO_PIN_1
#define OSPI_D0_GPIO_PORT                 GPIOB
#define OSPI_D0_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D1 */
#define OSPI_D1_PIN                       GPIO_PIN_0
#define OSPI_D1_GPIO_PORT                 GPIOB
#define OSPI_D1_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D2 */
#define OSPI_D2_PIN                       GPIO_PIN_7
#define OSPI_D2_GPIO_PORT                 GPIOA
#define OSPI_D2_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D3 */
#define OSPI_D3_PIN                       GPIO_PIN_6
#define OSPI_D3_GPIO_PORT                 GPIOA
#define OSPI_D3_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D4 */
#define OSPI_D4_PIN                       GPIO_PIN_1
#define OSPI_D4_GPIO_PORT                 GPIOC
#define OSPI_D4_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D5 */
#define OSPI_D5_PIN                       GPIO_PIN_2
#define OSPI_D5_GPIO_PORT                 GPIOC
#define OSPI_D5_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D6 */
#define OSPI_D6_PIN                       GPIO_PIN_3
#define OSPI_D6_GPIO_PORT                 GPIOC
#define OSPI_D6_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D7 */
#define OSPI_D7_PIN                       GPIO_PIN_0
#define OSPI_D7_GPIO_PORT                 GPIOC
#define OSPI_D7_PIN_AF                    GPIO_AF3_OCTOSPI1

/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()                 __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()                __HAL_RCC_OSPI1_CLK_DISABLE()

#define OSPI_CLK_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_NOR_CLK_N_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()

#define OSPI_FORCE_RESET()                __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI_RELEASE_RESET()              __HAL_RCC_OSPI1_RELEASE_RESET()

#endif

#ifdef STM32L5_NUCLEO_QUAD_SHIELD

//This is the  pin selection for the NUCLEO-L552ZEQ board
/* Definition for OSPI Pins */
/* OSPI_CS */
#define OSPI_CS_PIN                       GPIO_PIN_2
#define OSPI_CS_GPIO_PORT                 GPIOA
#define OSPI_CS_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_CLK */
#define OSPI_CLK_PIN                      GPIO_PIN_10
#define OSPI_CLK_GPIO_PORT                GPIOB
#define OSPI_CLK_PIN_AF                   GPIO_AF10_OCTOSPI1
/* OSPI_D0 */
#define OSPI_D0_PIN                       GPIO_PIN_12
#define OSPI_D0_GPIO_PORT                 GPIOE
#define OSPI_D0_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D1 */
#define OSPI_D1_PIN                       GPIO_PIN_0
#define OSPI_D1_GPIO_PORT                 GPIOB
#define OSPI_D1_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D2 */
#define OSPI_D2_PIN                       GPIO_PIN_14
#define OSPI_D2_GPIO_PORT                 GPIOE
#define OSPI_D2_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D3 */
#define OSPI_D3_PIN                       GPIO_PIN_15
#define OSPI_D3_GPIO_PORT                 GPIOE
#define OSPI_D3_PIN_AF                    GPIO_AF10_OCTOSPI1

// the rest of the pins are unused in this config but need to
// be defined

/* OSPI_D4 */
#define OSPI_D4_PIN                       GPIO_PIN_1
#define OSPI_D4_GPIO_PORT                 GPIOC
#define OSPI_D4_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D5 */
#define OSPI_D5_PIN                       GPIO_PIN_2
#define OSPI_D5_GPIO_PORT                 GPIOC
#define OSPI_D5_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D6 */
#define OSPI_D6_PIN                       GPIO_PIN_3
#define OSPI_D6_GPIO_PORT                 GPIOC
#define OSPI_D6_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_D7 */
#define OSPI_D7_PIN                       GPIO_PIN_4
#define OSPI_D7_GPIO_PORT                 GPIOC
#define OSPI_D7_PIN_AF                    GPIO_AF10_OCTOSPI1
/* OSPI_DQS */
#define OSPI_DQS_PIN                      GPIO_PIN_3
#define OSPI_DQS_GPIO_PORT                GPIOE
#define OSPI_DQS_PIN_AF                   GPIO_AF3_OCTOSPI1

/* Definition for OSPI clock resources */
#define OSPI_CLK_ENABLE()           __HAL_RCC_OSPI1_CLK_ENABLE()
#define OSPI_CLK_DISABLE()          __HAL_RCC_OSPI1_CLK_DISABLE()

#define OSPI_CS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define OSPI_CLK_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D0_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_D1_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define OSPI_D2_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_D3_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define OSPI_D4_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D5_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D6_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_D7_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define OSPI_DQS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOE_CLK_ENABLE()


#define OSPI_FORCE_RESET()          __HAL_RCC_OSPI1_FORCE_RESET()
#define OSPI_RELEASE_RESET()        __HAL_RCC_OSPI1_RELEASE_RESET()

#endif

// Misc defs
#define EXT_FLASH_PAGE_SIZE  256
#define EXT_FLASH_SECTORE_SIZE 4096
#define EXT_FLASH_SIZE 0x1000000
#define EXT_FLASH_SIZE_LOG2 23 //log2 of flash size - necessary for one of the OSPI fields

