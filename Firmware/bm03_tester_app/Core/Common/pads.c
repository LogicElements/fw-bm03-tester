/**
 * @file       pads.c
 * @brief      file_brief
 * @addtogroup gr
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "pads.h"
#include "mux.h"
#include "at21cs01.h"
#include "system_msp.h"
#include "configuration.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

/**
 * Declaration of all private variables
 */
typedef struct
{
  uint8_t index;
  PadData_t data[PADS_NUMBER];
}Pads_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * Instance of all private variables (except HAL handles)
 */
static Pads_Private_t pads;

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

Status_t Pads_Init(void)
{
  Status_t ret = STATUS_OK;

  memset(&pads, 0, sizeof(Pads_Private_t));

  return ret;
}


Status_t Pads_Handle(void)
{
  Status_t ret = STATUS_OK;

  if (conf.bm.pad_mode == 1)
  {
    conf.bm.pad_mode = 0;
    conf.bm.pads = 0;
    ret = Pads_Discovery(&conf.bm.pads);
  }

  return ret;
}

Status_t Pads_Discovery(uint8_t* number)
{
  Status_t ret = STATUS_OK;
  uint16_t status;
  uint32_t serial;
  uint32_t index;

  System_ReloadWdg();

  /* For each channel */
  for (int i = 0; i < PADS_NUMBER; i++)
  {
    /* Set mux, reset, and detect presence of memory */
    Mux_Set(i, 1, 1);
    status = AT21CS01_ResetAndPresenceDet();


    if (status == STATUS_OK)
    {
      /* Memory present */
      pads.data[i].present = 1;
      if (number != NULL)
      {
        *number += 1;
      }

      /* Read Serial number */
      serial = 0;
      status = AT21CS01_ReadWordValidate(AT21CS01_SN_ADDR, &serial, &pads.data[i].serial);
      if (status == STATUS_OK)
      {
        pads.data[i].serial = serial;
      }

      /* Read Cu index */
      index = 0;
      status = AT21CS01_ReadWordValidate(AT21CS01_CU_CONN_NEWEST_CU_SN_IDX_ADDR, &index, &pads.data[i].index);
      if (status == STATUS_OK)
      {
        pads.data[i].index = index;
      }

      if (serial == 0xFFFFFFFF || index == 0xFFFFFFFF)
      {
        Pads_ResetFactory(1000000 + i);
        System_ReloadWdg();
      }

    }
    else
    {
      /* Memory not present */
      pads.data[i].present = 0;
    }
  }

  Mux_Set(0, 1, 0);

  return ret;
}

Status_t Pads_ResetFactory(uint32_t serial)
{
  Status_t ret = STATUS_OK;

  ret = AT21CS01_WriteWord(0, serial);

  for (int i = 4; i < 128; i += 4)
  {
    ret += AT21CS01_WriteWord(i, 0);
  }

  return ret;
}



/* Private Functions ---------------------------------------------------------*/


uint8_t Sys_CalcCRC8(uint8_t* dataPtr, uint32_t dataSize)
{
  uint32_t i, j;
  uint8_t crc = 0;
  uint8_t polynom = 0x1D;

  for (i = 0; i < dataSize; i++)
  {
    crc ^= dataPtr[i];
    for (j = 0; j < 8; j++)
    {
      if (crc & 1)
        crc ^= polynom;
      crc >>= 1;
    }
  }
  return crc;
}


void Sys_GetValidMaxValueAndIdxFromBlocks(uint32_t* blockBase, uint8_t blockCnt, uint32_t* maxValueDst, uint32_t* maxValueIdxDst)
{
  uint32_t blockValidVals[4];
  uint8_t blockCrcResults[4];
  uint32_t maxValidVal = 0;
  uint32_t maxValueIdx = 0;
  uint32_t blockValue;

  for(uint8_t i=0; i<blockCnt; i++)
  {
    blockValue = (blockBase[i] >> 8) & 0xFFFFFF;

    blockCrcResults[i] = Sys_CalcCRC8((uint8_t*)&blockValue, 3);

    if(blockCrcResults[i] == (blockBase[i] & 0xFF))
    {
      blockValidVals[i] = (blockBase[i] >> 8) & 0xFFFFFF;

      if(blockValidVals[i] > maxValidVal)
      {
        maxValidVal = blockValidVals[i];
        maxValueIdx = i;
      }
    }
  }

  *maxValueDst = maxValidVal;
  *maxValueIdxDst = maxValueIdx;
}


/** @} */
