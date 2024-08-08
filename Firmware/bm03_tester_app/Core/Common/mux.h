/**
 * @file       mux.h
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
#ifndef COMMON_MUX_H_
#define COMMON_MUX_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/

/* Typedefs-------------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
 * Initialize multiplexer output
 * @return Status
 */
Status_t Mux_Init(void);

/**
 * Periodic handle for setting multiplexer output pins
 * @return Status
 */
Status_t Mux_Handle(void);


/**
 * Set desired function of multiplexer
 * @param select Select channel 0 - 7
 * @param output_enable Enable MUX output
 * @param pullup Enable 1 kOhm pull-up
 * @return
 */
Status_t Mux_Set(uint8_t select, uint8_t output_enable, uint8_t pullup);


#endif /* COMMON_MUX_H_ */
/** @} */
