/**
 * @file       AT21CS01.c
 * @brief      AT21CS01 Single-Wire EEPROM driver
 * @addtogroup gr
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "at21cs01.h"
#include "system_msp.h"


/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * AT21CS01 us delay loop
 *
 * @param[in] delayUs Delay in us
 * @return void
 */
__always_inline inline void AT21CS01_Delay(volatile uint32_t delayUs);

/**
 * AT21CS01 1us delay
 *
 * @return void
 */
__always_inline inline void AT21CS01_Delay1us(void);

/**
 * Enter Critical section
 *
 * @return void
 */
static void AT21CS01_CriticalSectionEnter(void);


__attribute__((always_inline)) inline void AT21CS01_SetOutputHigh()
{
  MUX_A_GPIO_Port->BSRR = MUX_A_Pin;
}

__attribute__((always_inline)) inline void AT21CS01_SetOutputLow()
{
  MUX_A_GPIO_Port->BRR = MUX_A_Pin;
}


/**
 * Leave Critical section
 *
 * @return void
 */
static void AT21CS01_CriticalSectionExit(void);

/**
 * Write bit to Single-Wire bus
 *
 * @param[in] bit Bit to be written
 * @param[in] padPtr Pointer on pad structure to be used
 * @return void
 */
static void AT21CS01_WriteBit(uint8_t bit);

/**
 * Read bit from 1-Wire bus
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @return Read bit
 */
static uint8_t AT21CS01_ReadBit();

/**
 * Write address and operation code to device
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] address Address of device on bus
 * @param[in] opCode  Operation code of operation
 * @param[in] direction Direction type
 * @return Read bit
 */
static Status_t AT21CS01_WriteOpCode(uint8_t address, uint8_t opCode, direction_t direction);

/**
 * Transmit byte on 1-Wire bus
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] bit Byte to be written
 * @return void
 */
static AckState_t AT21CS01_TransmitByte(uint8_t byte);

/**
 * Read byte from 1-Wire bus
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @return Read byte
 */
static uint8_t AT21CS01_ReceiveByte();

/**
 * AT21CS01 Write word to EEPROM
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] address Address of word destination (must be word aligned)
 * @param[in] word Word (32 bits) value to write
 * @return Status_t
 */
static Status_t AT21CS01_WriteWordOnly(uint8_t address, uint32_t word);

/* Functions -----------------------------------------------------------------*/
/* Inline functions */
//extern __always_inline void AT21CS01_SetOutputLow(Pad_t* padPtr);
//extern __always_inline void AT21CS01_SetOutputHigh(Pad_t* padPtr);
//extern __always_inline uint8_t AT21CS01_GetInputValue(Pad_t* padPtr);


Status_t AT21CS01_ResetAndPresenceDet()
{
  Status_t ret = STATUS_OK;           ///< Return status
  uint8_t presencePulse;              ///< Presence pulse detection value

  AT21CS01_CriticalSectionEnter();

  /* Reset Low Time tRSTL > 96 us (new datasheet)
   * tRSTL > 48 us (old datasheet, wrong value)
   * More than 150us for hard reset */
  AT21CS01_SetOutputLow();
  AT21CS01_Delay(100);

  /* Release line and wait for Presence Detect Sample Time tMSP = 8-10us */
  AT21CS01_SetOutputHigh();
  AT21CS01_Delay(9);

  //Drive line low for 1 - 2-tPup
  AT21CS01_SetOutputLow();
  AT21CS01_Delay(2);

  AT21CS01_SetOutputHigh();

  //Sample discovery response acknowledge (after 2us)
  AT21CS01_Delay(2);

  /* Sample line value */
  presencePulse = AT21CS01_GetInputValue();

  /* Presence pulse detected - some 1-wire device present */
  if (presencePulse == 0)
  {
    ret = STATUS_OK;
  }
  /* Presence pulse not detected - no 1-wire device present */
  else
  {
    ret = ERR_AT21CS01_NO_DEVICE;
  }

  /* Leave Critical Section */
  AT21CS01_CriticalSectionExit();

  /* Generate stop condition = min 150us */
  AT21CS01_SetOutputHigh();
  AT21CS01_Delay(160);

  /* Return value of presence pulse, 0 = OK, 1 = ERROR */
  return ret;
}

Status_t AT21CS01_ReadWordValidate(uint8_t address, uint32_t* word, uint32_t* storage)
{
  Status_t ret = STATUS_ERROR;
  uint8_t countdown = 5;
  uint32_t value;
  *word = -1;

  do
  {
    value = *word;
    AT21CS01_ReadWord(address, word);
  }
  while (countdown-- && *word != *storage && *word != value);

  if (countdown)
  {
    *storage = *word;
    ret = STATUS_OK;
  }

  return ret;
}

Status_t AT21CS01_ReadWord(uint8_t address, uint32_t* word)
{
  Status_t ret = STATUS_OK;           ///< Return status
  AckState_t ackState;
  uint32_t readByte = 0;

  *word = 0;

    AT21CS01_CriticalSectionEnter();

    /* Send device address with write direction bit */
    ret = AT21CS01_WriteOpCode(AT21CS01_DEV_ADDR, AT21CS01_EEPROM_ACCESS, Direction_Write);

    if(ret == STATUS_OK)
    {
      /* Send Starting Memory address */
      ackState = AT21CS01_TransmitByte(address);

      if(ackState == Ack)
      {
        //Generate repeated start condition
        AT21CS01_Delay(160);

        /* Send device address with read direction bit */
        ret = AT21CS01_WriteOpCode(AT21CS01_DEV_ADDR, AT21CS01_EEPROM_ACCESS, Direction_Read);

        if(ret == STATUS_OK)
        {
          for(uint8_t byteIdx = 0; byteIdx < 4; byteIdx++)
          {
            readByte = AT21CS01_ReceiveByte();

            /* Generate NACK */
            AT21CS01_WriteBit((byteIdx == 3)?(1):(0));

            *word |= readByte << (8*byteIdx);
          }
        }
        else
        {
          ret = ERR_AT21CS01_DEV_ADDR_OPCODE_NACK;
        }
      }
      else
      {
        ret = ERR_AT21CS01_MEM_ADDR_NACK;
      }
    }
    else
    {
      ret = ERR_AT21CS01_DEV_ADDR_OPCODE_NACK;
    }

    /* Generate stop condition */
    AT21CS01_Delay(160);

    AT21CS01_CriticalSectionExit();

  return ret;
}

Status_t AT21CS01_WriteWord(uint8_t address, uint32_t word)
{
  uint32_t read_back;
  uint8_t countdown = 3;

  /* Write word and perform read back until */
  do
  {
    AT21CS01_WriteWordOnly(address, word);
    AT21CS01_ReadWord(address, &read_back);
  }
  /* Countdown reached or read back is equal to written value */
  while (countdown-- && word != read_back);

  return (word == read_back)?(STATUS_OK):(STATUS_ERROR);
}

static Status_t AT21CS01_WriteWordOnly(uint8_t address, uint32_t word)
{
  Status_t ret = STATUS_OK;           ///< Return status
  AckState_t ackState;

      AT21CS01_CriticalSectionEnter();

      /* Send device address with write direction bit */
      ret = AT21CS01_WriteOpCode(AT21CS01_DEV_ADDR, AT21CS01_EEPROM_ACCESS, Direction_Write);

      if(ret == STATUS_OK)
      {
        /* Send Starting Memory address */
        ackState = AT21CS01_TransmitByte(address);

        if(ackState == Ack)
        {
          /* Write bytes */
          for(uint8_t byteIdx=0; byteIdx < 4; byteIdx++)
          {
            ackState = AT21CS01_TransmitByte((uint8_t)((word>>(8*byteIdx)) & 0xFF));

            if(ackState != Ack)
            {
              break;
            }
          }

          /* Generate stop conditions */
          AT21CS01_Delay(160);

          if(ackState == Nack)
          {
            ret = ERR_AT21CS01_BYTE_WRITE_NACK;
          }
        }
        else
        {
          ret = ERR_AT21CS01_MEM_ADDR_NACK;
        }
      }
      else
      {
        ret = ERR_AT21CS01_DEV_ADDR_OPCODE_NACK;
      }

      AT21CS01_CriticalSectionExit();

      /* Wait at least 5 ms (eeprom burn) */
      HAL_Delay(6);


  return ret;
}


/* Private Functions ---------------------------------------------------------*/
__always_inline inline void AT21CS01_Delay(volatile uint32_t delayUs)
{
  System_DelayUs(delayUs + 1);
}

__always_inline inline void AT21CS01_Delay1us(void)
{
  System_DelayUs(1);
}

static void AT21CS01_CriticalSectionEnter(void)
{
  __disable_irq();

  return;
}

static void AT21CS01_CriticalSectionExit(void)
{
  __enable_irq();

  return;
}


static void AT21CS01_WriteBit(uint8_t bit)
{
  if (bit)
  {/* Writing 1 bit */
    //Set line low
    AT21CS01_SetOutputLow();

    AT21CS01_Delay1us();

    /* Set line high for rest of Time Slot Duration */
    AT21CS01_SetOutputHigh();

    AT21CS01_Delay(14);
  }

  else
  {/* Writting 0 bit */
    //Set line low
    AT21CS01_SetOutputLow();

    AT21CS01_Delay(8);

    /* Set line high for rest of Time Slot Duration */
    AT21CS01_SetOutputHigh();

    AT21CS01_Delay(4);
  }
}

static uint8_t AT21CS01_ReadBit()
{
  uint8_t bit = 0;

  AT21CS01_SetOutputLow();

  AT21CS01_Delay1us();

  /* Release line */
  AT21CS01_SetOutputHigh();

  AT21CS01_Delay1us();

  /* Read line value */
  if (AT21CS01_GetInputValue())
  {
    /* Bit is HIGH */
    bit = 1;
  }

  /* Keep line high for rest of Time Slot Duration */
  AT21CS01_Delay(13);

  /* Return bit value */
  return bit;
}


static Status_t AT21CS01_WriteOpCode(uint8_t address, uint8_t opCode, direction_t direction)
{
  Status_t status = STATUS_OK;

  uint8_t finalByte = ((address & AT21CS01_DEV_ADDR_PARAM_MASK) << AT21CS01_DEV_ADDR_OFFSET) | ((opCode & AT21CS01_OP_CODE_PARAM_MASK) << AT21CS01_OP_CODE_OFFSET);

  finalByte |= ((uint8_t)direction & 0x01);

  AckState_t ackState = AT21CS01_TransmitByte(finalByte);

  if(ackState == Nack)
    status = ERR_AT21CS01_DEV_ADDR_OPCODE_NACK;

  return status;
}


static AckState_t AT21CS01_TransmitByte(uint8_t byte)
{
  uint8_t i = 8;        ///< Iteration variable

  /* Write 8 bits */
  while (i--)
  {
    /* MSB bit is first */
    AT21CS01_WriteBit((byte >> i) & 0x01);
  }

  AckState_t ackState = AT21CS01_ReadBit();

  return ackState;
}

static uint8_t AT21CS01_ReceiveByte()
{
  uint8_t i = 8;        ///< Iteration variable
  uint8_t byte = 0;     ///< Read byte

  /* Read 8 bits */
  while (i--)
  {
    /* MSB bit is first */
    byte |= (AT21CS01_ReadBit() << i);
  }

  return byte;
}


/** @} */
