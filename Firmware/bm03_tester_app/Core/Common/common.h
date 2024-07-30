/**
  * @file       common.h
  * @version    $(APP_VERSION)
  * @date       $(RELEASE_DATE)
  * @brief      Common header include
  * @author     jan.bartovsky
  *
  * @copyright  Logic Elements Copyright
  *
  * @defgroup grCommon Common definitions
  * @{
  * @brief Common definitions for all modules
  *
  * This module contains common definitions for all modules.
  *
  * @par Main features:
  * - Status and error codes, types
  * - Error status checking macros
  * - Assert parameters checking macros
  *
  * @par Example
  * @code
  * #include "common.h"
  * @endcode
  */

#ifndef APPLICATION_COMMON_H_
#define APPLICATION_COMMON_H_

/* Includes ------------------------------------------------------------------*/

#include "stm32l4xx_hal.h"
#include "string.h"

/* Definitions - Compilation flags--------------------------------------------*/
/* Definitions----------------------------------------------------------------*/

/**
 * Success status
 */
#define STATUS_OK        0

/**
 * Error or fail status
 */
#define STATUS_ERROR     1

/**
 * Error or fail status
 */
#define STATUS_TIMEOUT   2

/**
 * Busy status
 */
#define STATUS_BUSY      3


/**
 * Interrupt priorities
 */
#define PRIO_IRQ_SYSTICK            1      ///< Systick interrupt
#define PRIO_IRQ_USB                8
#define PRIO_IRQ_MODBUS             7      ///< Modbus uart and timeout

/**
 * Timers assignment
 *
 * TIM1 -
 * TIM2 -
 * TIM3 - Modbus timeout
 * TIM4 -
 * TIM5 -
 * TIM6 -
 * TIM7 -
 * TIM8 - Opentherm IO
 * TIM14-
 * TIM15-
 * TIM16-
 *
 */

/* Macros ------------------------------------------------------------------*/

/**
 * Minimum of two arguments
 */
#ifndef MIN
#define MIN(a, b)   (((a)>(b))?(b):(a))
#endif

/**
 * Maximum of two arguments
 */
#ifndef MAX
#define MAX(a, b)   (((a)<(b))?(b):(a))
#endif

/**
 * Saturate the X to the given upper bound VAL
 */
#define SAT_UP(x, val)      ((x) = ((x)>(val))?(val):(x))

/**
 * Saturate the X to the given lower bound VAL
 */
#define SAT_DOWN(x, val)    ((x) = ((x)<(val))?(val):(x))

/**
 * General printf-like definition
 */
#define PRINTF(...)
//#define PRINTF(...) printf(__VA_ARGS__)

/**
 * Parameters assertion. If the expr is false, the warning message is printed.
 */
#define ASSERT_PARAM(expr)
/**
 * Common Error message
 */
#define ERR_PRINT(ret, code)

/**
 * Error catching macro. If retValue is non-zero, the error message is printed with given errorCode.
 */
#define CATCH_ERROR(retValue, errorCode)  \
    do{\
  if ((retValue) != 0)\
  {\
    ERR_PRINT(retValue, errorCode);\
  }\
    }while(0)

#ifndef __weak
    #define __weak   __attribute__((weak))
#endif /* __weak */

/**
 * Macro to check 32-bit unsigned tick timeout constant
 */
#define TICK_EXPIRED(a)     (HAL_GetTick() - (a) < 0x7fffffff)


/**
 * Backward compatibility of HAL error handlers
 */
//#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
 * Get byte macros
 */
#define GET_BYTE_0(a)       ( (uint8_t) ((a) & 0xff))
#define GET_BYTE_1(a)       ( (uint8_t) (((a) >> 8) & 0xff))
#define GET_BYTE_2(a)       ( (uint8_t) (((a) >> 16) & 0xff))
#define GET_BYTE_3(a)       ( (uint8_t) (((a) >> 24) & 0xff))

/**
 * Disable Event
 */

#define EventMngr_Event(...)

/* Typedefs-------------------------------------------------------------------*/

/**
 * General system pointer to function type
 */
typedef void (*System_Callback_t)(void);

/**
 * General status return type
 */
typedef int16_t Status_t;

/**
 * General boolean type
 */
typedef enum { FALSE, TRUE } bool_t;

/* Functions -----------------------------------------------------------------*/

void _Error_Handler(char *file, int line);

void Error_Handler(void);

#endif /* APPLICATION_COMMON_H_ */
/** @} */
