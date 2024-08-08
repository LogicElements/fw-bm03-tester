/**
 * @file       mux.c
 * @brief      file_brief
 * @addtogroup gr
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "mux.h"
#include "main.h"
#include "gpio.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

/**
 * Declaration of all private variables
 */
typedef struct
{
  uint8_t mode;
  uint8_t select;
  uint8_t output_enable;
  uint8_t pullup;
  uint8_t pullup_prev;
}Mux_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * Instance of all private variables (except HAL handles)
 */
static Mux_Private_t mux;

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

Status_t Mux_Init(void)
{
  Status_t ret = STATUS_OK;

  memset(&mux, 0, sizeof(Mux_Private_t));

  mux.pullup_prev = -1;

  return ret;
}

Status_t Mux_Handle(void)
{
  Status_t ret = STATUS_OK;

  /* Apply select pins */
  HAL_GPIO_WritePin(MUX_S0_GPIO_Port, MUX_S0_Pin, (mux.select >> 0) & 1);
  HAL_GPIO_WritePin(MUX_S1_GPIO_Port, MUX_S1_Pin, (mux.select >> 1) & 1);
  HAL_GPIO_WritePin(MUX_S2_GPIO_Port, MUX_S2_Pin, (mux.select >> 2) & 1);

  /* Apply output enable pin */
  HAL_GPIO_WritePin(MUX_OE_GPIO_Port, MUX_OE_Pin, (mux.output_enable >> 0) & 1);

  /* Pull up change pin mode */
  if (mux.pullup != mux.pullup_prev)
  {
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    mux.pullup_prev = mux.pullup;

    if (mux.pullup)
    {
      /*Configure GPIO pin : PtPin */
      GPIO_InitStruct.Pin = MUX_PU_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      HAL_GPIO_Init(MUX_PU_GPIO_Port, &GPIO_InitStruct);
    }
    else
    {
      /*Configure GPIO pin : PtPin */
      GPIO_InitStruct.Pin = MUX_PU_Pin;
      HAL_GPIO_Init(MUX_PU_GPIO_Port, &GPIO_InitStruct);
    }

  }

  return ret;
}



Status_t Mux_Set(uint8_t select, uint8_t output_enable, uint8_t pullup)
{
  Status_t ret = STATUS_OK;

  mux.select = select;
  mux.output_enable = (output_enable) ? (0) : (1);
  mux.pullup = pullup;

  Mux_Handle();

  return ret;
}



/* Private Functions ---------------------------------------------------------*/

/** @} */
