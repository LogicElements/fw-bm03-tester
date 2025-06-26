/**
 * @file       switch.c
 * @brief      DIP switch driver
 * @addtogroup grSwitch
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "switch.h"
#include "main.h"
#include "configuration.h"

/* Private defines -----------------------------------------------------------*/

#define SW_DEBOUNCE_COUNT       5

/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

typedef struct
{
  uint16_t debounceCount[2];
  uint8_t valPrev[2];
}Switch_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static Switch_Private_t sw;

/**
 * @brief Switch GPIO Port
 */
static GPIO_TypeDef* Switch_Port[SWITCH_NUMBER] =
{
    ALARM_0_GPIO_Port, ALARM_1_GPIO_Port
};

/**
 * @brief Switch GPIO Pin
 */
static const uint16_t Switch_Pin[SWITCH_NUMBER] =
{
    ALARM_0_Pin, ALARM_1_Pin
};


/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/


void Switch_Init(void)
{


  return;
}


uint16_t Switch_GetAll(void)
{
  uint16_t val = 0;

  val = HAL_GPIO_ReadPin(Switch_Port[0], Switch_Pin[0]);

  if (val != sw.valPrev[0])
  {
    if (++sw.debounceCount[0] >= SW_DEBOUNCE_COUNT)
    {
      sw.valPrev[0] = val;
      sw.debounceCount[0] = 0;
      if (val)
      {
        conf.bm.alarm_0++;
      }
    }
  }

  val = HAL_GPIO_ReadPin(Switch_Port[1], Switch_Pin[1]);

  if (val != sw.valPrev[1])
  {
    if (++sw.debounceCount[1] >= SW_DEBOUNCE_COUNT)
    {
      sw.valPrev[1] = val;
      sw.debounceCount[1] = 0;
      if (val)
      {
        conf.bm.alarm_1++;
      }
    }
  }

  return *((uint16_t*)sw.valPrev);
}


Status_t Switch_WriteAll(void)
{
  Status_t ret = STATUS_OK;

  HAL_GPIO_WritePin(PWR_0_GPIO_Port, PWR_0_Pin, conf.bm.power_0 == 0);
  HAL_GPIO_WritePin(PWR_1_GPIO_Port, PWR_1_Pin, conf.bm.power_1 == 0);

  return ret;
}


/* Private Functions ---------------------------------------------------------*/

/** @} */
