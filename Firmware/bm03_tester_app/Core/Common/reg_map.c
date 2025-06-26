/**
 * @file       reg_map.c
 * @brief      Register map implementation
 * @addtogroup grRegMap
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "reg_map.h"
#include "configuration.h"

/* Private defines -----------------------------------------------------------*/

/* Private macros  -----------------------------------------------------------*/

/* Private typedefs ----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/** @defgroup grRegMapStorage
 *  @ingroup grRegMap
 *  @brief Definition of storage memory structure for all registers
 *  @{
 */

/**
 * Storage for the entire register map (structure of structures)
 */
conf_reg_t conf;


uint8_t* const CONF_REG[CONF_REG_BLOCK_NUMBER] = {(uint8_t*)&conf.sys, (uint8_t*)&conf.fact, (uint8_t*)&conf.firm, NULL, (uint8_t*)&conf.calib, (uint8_t*)&conf.bm, (uint8_t*)&conf.dbg, (uint8_t*)&conf.pad, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, };

const uint32_t CONF_REG_LIMIT[CONF_REG_BLOCK_NUMBER] = {
24, 16, 16, 0, 4, 18, 4, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

const uint32_t CONF_REG_FLASH[CONF_REG_FLASH_NUMBER] = {
CONF_SYS_REGMAP_VERSION, CONF_BM_MUX_DEFAULT, };

const uint32_t CONF_REG_LOGGER[CONF_REG_LOGGER_NUMBER] = {
};

const uint32_t CONF_REG_CALIB[CONF_REG_CALIB_NUMBER] = {
CONF_FACT_SERIAL_NUMBER, };

const uint32_t CONF_REG_SYNCED[CONF_REG_SYNCED_NUMBER] = {
};



/** @} */

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/


Status_t RegMap_RestoreFactoryValues(void)
{
  Status_t ret = STATUS_OK;
  //uint8_t *id = (uint8_t *)0x1FFF7A10;

  /* Copy firmware info */
  memcpy(&conf.firm, CONF_C_APPLICATION_OFFSET + (uint32_t)CONF_FW_INFO_OFFSET, sizeof(conf.firm));

  /* Copy Hardware info */
  memcpy(&conf.fact, CONF_C_BOOTLOADER_OFFSET + (uint32_t)CONF_FW_INFO_OFFSET, sizeof(conf.fact));

  CONF_INT(CONF_SYS_UPTIME)              = 0;
  CONF_INT(CONF_SYS_REGMAP_VERSION)      = 1001;
  CONF_SHORT(CONF_CALIB_MODE)            = 0;
  CONF_BYTE(CONF_BM_MUX)                 = 0;
  CONF_BYTE(CONF_BM_MUX_DEFAULT)         = 0;
  CONF_BYTE(CONF_BM_PADS)                = 0;
  CONF_BYTE(CONF_BM_PAD_MODE)            = 1;
  CONF_SHORT(CONF_BM_SIGNAL_PERIOD)      = 2000;
  CONF_SHORT(CONF_BM_SIGNAL_BASE)        = 1750;
  CONF_SHORT(CONF_BM_SIGNAL_AMPLITUDE)   = 50;
  CONF_SHORT(CONF_BM_POWER_0)            = 0;
  CONF_SHORT(CONF_BM_POWER_1)            = 0;
  CONF_SHORT(CONF_BM_ALARM_0)            = 0;
  CONF_SHORT(CONF_BM_ALARM_1)            = 0;


  return ret;
}


/** @} */
