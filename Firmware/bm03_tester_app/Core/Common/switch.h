/**
  * @file       switch.h
  * @version    $(APP_VERSION)
  * @date       $(RELEASE_DATE)
  * @brief      DIP switch driver
  * @author     radek.salom
  *
  * @copyright  Logic Elements Copyright
  *
  * @defgroup grSwitch
  * @{
  * @brief Driver to handle DIP switches
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
#ifndef SWITCH_H_
#define SWITCH_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"


/* Definitions----------------------------------------------------------------*/

#define SWITCH_NUMBER           2          ///< Number of supported DIP switches


/* Typedefs-------------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * Initialize DIP Switch module
 *
 * @param none
 * @return void
 */
void Switch_Init(void);

/**
 * Get values of all dip switches in a single number
 * @return
 */
uint16_t Switch_GetAll(void);

Status_t Switch_WriteAll(void);

#endif /* SWITCH_H_ */
/** @} */
