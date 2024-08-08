/**
 * @file       pads.h
 * @version    $(APP_VERSION)
 * @date       $(RELEASE_DATE)
 * @brief      file_brief
 * @author     jan.bartovsky
 *
 * @copyright  Logic Elements Copyright
 *
 * @defgroup gr group_name
 * @{
 * @brief group_brief
 *
 * This module contains
 *
 * @par Main features:
 *
 * @par Example
 * @code
 *
 * @endcode
 */
#ifndef COMMON_PADS_H_
#define COMMON_PADS_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/

#define PADS_NUMBER       8

/* Typedefs-------------------------------------------------------------------*/

typedef struct
{
  uint8_t present;
  uint32_t serial;
  uint32_t index;

}PadData_t;

/* Functions -----------------------------------------------------------------*/

Status_t Pads_Init(void);

Status_t Pads_Discovery(uint8_t* number);

Status_t Pads_ResetFactory(uint32_t serial);

#endif /* COMMON_PADS_H_ */
/** @} */
