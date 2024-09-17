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

/* Functions -----------------------------------------------------------------*/

Status_t Signal_Init(void)
{
  Status_t ret = STATUS_OK;

  memset(&sig, 0, sizeof(Signal_Private_t));

  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

  Signal_Restart();

  return ret;
}

Status_t Signal_Handle(void)
{
  Status_t ret = STATUS_OK;

  /* Zero amplitude means stop generating */
  if (conf.bm.signal_amplitude == 0)
  {
    Signal_Restart();
  }
  else if (TICK_EXPIRED(sig.tick))
  {
    sig.tick = HAL_GetTick() + (conf.bm.signal_period >> 1);

    if (sig.down_n_up == 0)
    {
      sig.value = VOLT_TO_DAC(conf.bm.signal_base + conf.bm.signal_amplitude);
      sig.down_n_up = 1;
    }
    else
    {
      sig.value = VOLT_TO_DAC(conf.bm.signal_base - conf.bm.signal_amplitude);
      sig.down_n_up = 0;
    }
  }

  /* Set value to DAC */
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sig.value);

  return ret;
}



/* Private Functions ---------------------------------------------------------*/

static Status_t Signal_Restart(void)
{
  Status_t ret = STATUS_OK;

  sig.tick = HAL_GetTick() + (conf.bm.signal_period >> 1);
  sig.down_n_up = 0;
  sig.value = VOLT_TO_DAC(conf.bm.signal_base);

  /* Set value to DAC */
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sig.value);

  return ret;
}


/** @} */
