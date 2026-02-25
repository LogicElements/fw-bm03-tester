/**
 * @file       signal.c
 * @brief      file_brief
 * @addtogroup gr
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "signal.h"
#include "configuration.h"
#include "dac.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/

#define VOLT_TO_DAC(x) ( (x) * 4095 / 3300)

/* Private typedefs ----------------------------------------------------------*/

/**
 * Declaration of all private variables
 */
typedef struct
{
  uint32_t tick;
  uint16_t value;
  uint16_t down_n_up;
  uint16_t sync;
  uint32_t sync_tick;
  uint8_t in_prev;
}Signal_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * Instance of all private variables (except HAL handles)
 */
static Signal_Private_t sig;

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static Status_t Signal_Restart(void);

void Signal_OutputValue(uint8_t down_n_up);

/* Functions -----------------------------------------------------------------*/

Status_t Signal_Init(void)
{
  Status_t ret = STATUS_OK;

  memset(&sig, 0, sizeof(Signal_Private_t));

  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

  Signal_Restart();

  sig.in_prev = HAL_GPIO_ReadPin(SYNC_IN_GPIO_Port, SYNC_IN_Pin);

  return ret;
}

Status_t Signal_Handle(void)
{
  Status_t ret = STATUS_OK;
  uint8_t in;

  in = HAL_GPIO_ReadPin(SYNC_IN_GPIO_Port, SYNC_IN_Pin);

  if (in == GPIO_PIN_SET && sig.in_prev == GPIO_PIN_RESET)
  {
    /* Rising edge of sync */
    sig.sync = 1;
    sig.sync_tick = HAL_GetTick() + 10000;

    Signal_OutputValue(0);
    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, 1);
  }
  else
  {
    if (sig.sync)
    {
      if (in == GPIO_PIN_RESET && sig.in_prev == GPIO_PIN_SET)
      {
        Signal_OutputValue(1);
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, 0);
      }

      if (TICK_EXPIRED(sig.sync_tick))
      {
        sig.sync = 0;
      }
    }
    else
    {
      /* Zero amplitude means stop generating */
      if (conf.bm.signal_amplitude == 0)
      {
        Signal_Restart();
      }
      else if (TICK_EXPIRED(sig.tick))
      {
        sig.tick = HAL_GetTick() + (conf.bm.signal_period >> 1);

        Signal_OutputValue(sig.down_n_up);
      }
    }
  }
  sig.in_prev = in;




  return ret;
}


void Signal_OutputValue(uint8_t down_n_up)
{
  if (down_n_up == 0)
  {
    sig.value = VOLT_TO_DAC(conf.bm.signal_base + conf.bm.signal_amplitude);
    HAL_GPIO_WritePin(SYNC_OUT_GPIO_Port, SYNC_OUT_Pin, 1);
    sig.down_n_up = 1;
  }
  else
  {
    sig.value = VOLT_TO_DAC(conf.bm.signal_base - conf.bm.signal_amplitude);
    HAL_GPIO_WritePin(SYNC_OUT_GPIO_Port, SYNC_OUT_Pin, 0);
    sig.down_n_up = 0;
  }

  /* Set value to DAC */
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sig.value);
}


/* Private Functions ---------------------------------------------------------*/

static Status_t Signal_Restart(void)
{
  Status_t ret = STATUS_OK;

  sig.tick = HAL_GetTick();// + (conf.bm.signal_period >> 1);
  sig.down_n_up = 0;
  sig.value = VOLT_TO_DAC(conf.bm.signal_base);

  /* Set value to DAC */
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sig.value);

  return ret;
}


/** @} */
