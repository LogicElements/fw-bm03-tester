/**
  * @file       AT21CS01.h
  * @version    $(APP_VERSION)
  * @date       $(RELEASE_DATE)
  * @brief      AT21CS01 Single-Wire EEPROM driver
  * @author     Miroslav Sima
  *
  * @copyright  Logic Elements Copyright
  *
  * @defgroup gr AT21CS01 Single-WIRE EEPROM
  * @{
  * @brief AT21CS01 Single-WIRE EEPROM driver
  *
  * This module contains set of functions for EEPROM Read/Write operations.
  *
  * @par Main features
  *
  * - EEPROM reset and presence detection
  * - EEPROM read/write functions
  *
  * @par Driver porting to another platform
  *
  * To port the module to another platform modify HAL functions in AT21CS01.c:
  * - AT21CS01_Delay()
  * - AT21CS01_Delay1us()
  * - AT21CS01_CriticalSectionEnter()
  * - AT21CS01_CriticalSectionExit()
  * - AT21CS01_SetOutputLow()
  * - AT21CS01_SetOutputHigh()
  * - AT21CS01_GetInputValue()
  *
  *
  * @par Example of module initialization
  * @code
  * Status_t status;
  *
  * status = AT21CS01_Init();
  * if(status == STATUS_OK)
  * {
  *   status = AT21CS01_DetectPresentDevices();
  *   if(status == STATUS_OK)
  *   {
  *     status = AT21CS01_GetNumberOfDevices(&eepromDetectedDevices);
  *     if(status == STATUS_OK)
  *     {
  *       // AT21CS01 EEPROM driver is initialized
  *     }
  *   }
  * }
  * @endcode
  *
  * @par Example of module test
  *
  * EEPROM Read Write test:
  *
  * @code
  * Status_t status;
  * status = AT21CS01_EepromTest();
  * if(status == STATUS_OK)
  * {
  *   // EEPROM Read/Write Test successful
  * }
  * @endcode
  *
  * Time in Use Read/Write algorithm test:
  *
  * To test Time in Use R/W algorithm, it is necessary to compile driver with
  * Virtual EEPROM Emulation enabled (#define AT21CS01_VIRTUAL_EEPROM  1).
  * Time in Use R/W test is not allowed on physical AT21CS01 EEPROM memory,
  * because all segments are written with maximum EEPROM cycles and after test
  * the real EEPROM memory should be damaged (its lifetime expired).
  * Test run with Virtual EEPROM Emulation disabled causes return of
  * ERR_AT21CS01_TEST_ERROR (test is not performed).
  *
  *
  * @code
  * Status_t status;
  * status = AT21CS01_TimeInUseTest();
  * if(status == STATUS_OK)
  * {
  *   // Time in Use Read/Write Test successful
  * }
  * @endcode
  *
  */
#ifndef AT21CS01_H_
#define AT21CS01_H_

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "main.h"

/* Definitions----------------------------------------------------------------*/

#define AT21CS01_DEV_ADDR         0x0

#define AT21CS01_VIRTUAL_EEPROM   0u        ///< AT21CS01 Virtual EEPROM emulation enable

/* AT21CS01 op-codes */
#define AT21CS01_EEPROM_ACCESS          0xA      ///< AT21CS01 Write Memory command
#define AT21CS01_SECURITY_REG_ACCESS    0xB
#define AT21CS01_LOCK_SECURITY_REG      0x2
#define AT21CS01_ROM_ZONE_REG_ACCESS    0x7
#define AT21CS01_FREEZE_ROM_ZONE_STATE  0x1
#define AT21CS01_MANUFACTURER_ID_READ   0xC
#define AT21CS01_STANDARD_SPEED_MODE    0xD
#define AT21CS01_HIGH_SPEED_MODE        0xE

#define AT21CS01_ZONE0_MEM_ADDR     0x00
#define AT21CS01_ZONE0_REG_ADDR     0x01
#define AT21CS01_ZONE1_MEM_ADDR     0x20
#define AT21CS01_ZONE1_REG_ADDR     0x02
#define AT21CS01_ZONE2_MEM_ADDR     0x40
#define AT21CS01_ZONE2_REG_ADDR     0x04
#define AT21CS01_ZONE3_MEM_ADDR     0x60
#define AT21CS01_ZONE3_REG_ADDR     0x08


#define AT21CS01_EEPROM_PROG_TIME       5u    // Self-timed write cycle (ms)
#define AT21CS01_OP_CODE_SIZE           4u    //Device address size (bits)
#define AT21CS01_OP_CODE_PARAM_MASK     0xF   //Device address size (bits)
#define AT21CS01_OP_CODE_OFFSET         4u    //Device address size (bits)
#define AT21CS01_DEV_ADDR_SIZE          3u    //Device address size (bits)
#define AT21CS01_DEV_ADDR_PARAM_MASK    0x07  //Device address size (bits)
#define AT21CS01_DEV_ADDR_OFFSET        1u    //Device address size (bits)
#define AT21CS01_EEPROM_PAGE_SIZE       8u    // AT21CS01 page size [B]
#define AT21CS01_EEPROM_SEG_SIZE        1u    // AT21CS01 segment size size [B]
#define AT21CS01_EEPROM_PAGE_TOTAL      16u   // AT21CS01 number of writable pages
#define AT21CS01_EEPROM_SIZE            (AT21CS01_EEPROM_PAGE_SIZE * AT21CS01_EEPROM_PAGE_TOTAL)  // AT21CS01 EEPROM writable area size [B]
#define AT21CS01_EEPROM_START_ADDR      0x00  // AT21CS01 EEPROM writable area start address
#define AT21CS01_EEPROM_END_ADDR        0x6F  // AT21CS01 EEPROM writable area end address
#define AT21CS01_EEPROM_PAGE_MASK       0x78  // AT21CS01 EEPROM page position mask within address value
#define AT21CS01_EEPROM_PAGE_SHIFT      3u    // AT21CS01 EEPROM page position shift within address value
#define AT21CS01_EEPROM_SEG_MASK        0x07  // AT21CS01 EEPROM segment position mask within address value
#define AT21CS01_EEPROM_SEG_SHIFT       0u    // AT21CS01 EEPROM segment position shift within address value
#define AT21CS01_SERIAL_NUM_SIZE        8u    // AT21CS01 Device ID size [B]
#define AT21CS01_MANUFACTURER_ID_SIZE   3u    // AT21CS01 Factory ID size [B]

#define AT21CS01_SER_NUM_CRC_OFFSET     7u    // AT21CS01 CRC value offset within Serial number
#define AT21CS01_SER_NUM_CRC_SIZE       1u    // AT21CS01 CRC value size [B]
#define AT21CS01_UNIQUE_NUM_OFFSET      1u    // AT21CS01 Device Serial Number value offset within Device ID
#define AT21CS01_UNIQUE_NUM_SIZE        6u    // AT21CS01 Device Serial Number value size [B]
#define AT21CS01_PROD_ID_OFFSET         0u    // AT21CS01 Family number value offset within Device ID
#define AT21CS01_PROD_ID_SIZE           1u    // AT21CS01 Family number value size [B]


#define AT21CS01_TS_TIU_MAX_WRITE       1000000u  // Maximum number of writes into one Time in Use block
#define AT21CS01_CU_CONN_HIST_REC_CNT   10        // Number of maximum count of central units serial numbers stored in pad EEPROM

/* Addresses */
#define AT21CS01_SN_ADDR                          0u
#define AT21CS01_CU_CONN_NEWEST_CU_SN_IDX_ADDR    8u
#define AT21CS01_CU_CONN_HIST_START_ADDR          12u
#define AT21CS01_TIU_IN_STANDBY_ADDR              56u
#define AT21CS01_TIU_IN_ACTIVE_ADDR               72u
#define AT21CS01_KID_PLACEMENT_CNT_ADDR           88u
#define AT21CS01_PAD_ACTIVE_CNT_ADDR              96u
#define AT21CS01_PAD_CONN_CNT_ADDR                104u
#define AT21CS01_PAD_ALARM_CNT                    112u

#define STATUS_MEM_INVALID_DATA                 0x0600u  ///< Memory has invalid data or factory default data (0xFF...)
#define ERR_AT21CS01_NO_DEVICE                  0x0601u  ///< No EEPROM device responded to bus reset
#define ERR_AT21CS01_WRONG_DEVICE               0x0602u  ///< Device not present on bus
#define ERR_AT21CS01_WRONG_ADDRESS              0x0603u  ///< Address is out of memory boundaries
#define ERR_AT21CS01_WRONG_PAGE                 0x0604u  ///< Page number is out of memory boundaries
#define ERR_AT21CS01_WRONG_DATA_LENGTH          0x0605u  ///< Wrong length of data to read/write
#define ERR_AT21CS01_EEPROM_WRITE_PROTECT       0x0606u  ///< Device is write protected
#define ERR_AT21CS01_EEPROM_WRITE_ERROR         0x0607u  ///< Write operation failed
#define ERR_AT21CS01_TIU_ADDR_ERROR             0x0608u  ///< Invalid TIU address value
#define ERR_AT21CS01_TIU_LIFETIME_ERROR         0x0609u  ///< Maximum EEPROM write cycles reached
#define ERR_AT21CS01_TEST_ERROR                 0x060Au  ///< Testing sequence error
#define ERR_AT21CS01_WRONG_SEGMENT              0x060Bu  ///< Memory segment is out of memory boundaries
#define ERR_AT21CS01_TIU_NOT_LOADED             0x060Cu  ///< Time in use values were not loaded
#define ERR_AT21CS01_DEV_ADDR_OPCODE_NACK       0x060Du  ///< Device address with opcode not acknowledged
#define ERR_AT21CS01_MEM_ADDR_NACK              0x060Eu  ///< Memory address not acknowledged
#define ERR_AT21CS01_BYTE_WRITE_NACK            0x060Fu  ///< Memory address not acknowledged
 #define ERR_AT21CS01_UNALIGNED_ACCESS          0x0610u  ///< Unaligned access

/** @} */
/* Typedefs-------------------------------------------------------------------*/

typedef enum{
  Direction_Write = 0,//!< Direction_Write
  Direction_Read,     //!< Direction_Read
}direction_t;

typedef enum{
  Ack = 0,
  Nack
}AckState_t;

/* Functions -----------------------------------------------------------------*/


/**
 * Read single-wire logic level
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @return Single-wire bus logic level
 */
__inline uint8_t AT21CS01_GetInputValue()
{
#if DBG_1WIRE_SAMPLING
  LD_R_GPIO_Port->BSRR = LD_R_Pin;
  LD_R_GPIO_Port->BRR = LD_R_Pin;
#endif

  if((MUX_A_GPIO_Port->IDR & MUX_A_Pin) != 0)
    return 1;
  else
    return 0;
}


/**
 * Reset AT21CS01 EEPROM
 * @param[in] padPtr Pointer on pad structure to be used
 * @return Status_t State if some device is present
 */
Status_t AT21CS01_ResetAndPresenceDet();

/**
 * AT21CS01 Factory default EEPROM erase
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] address Address of word to be read (must be word aligned)
 * @param[in] word Pointer to byte destination
 * @return Status_t
 */
Status_t AT21CS01_ReadWord(uint8_t address, uint32_t* word);

/**
 * AT21CS01 read word and validate it against local storage
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] address Address of word to be read (must be word aligned)
 * @param[in] word Pointer to byte destination
 * @param[in] storage Pointer to local storage
 * @return Status_t
 */
Status_t AT21CS01_ReadWordValidate(uint8_t address, uint32_t* word, uint32_t* storage);

/**
 * AT21CS01 Write word to EEPROM, read it back and retry if they differ
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] address Address of word destination (must be word aligned)
 * @param[in] word Word (32 bits) value to write
 * @return Status_t
 */
Status_t AT21CS01_WriteWord(uint8_t address, uint32_t word);



/**
 * AT21CS01 Factory default EEPROM erase
 *
 * @param[in] padPtr Pointer on pad structure to be used
 * @param[in] address Address of byte destination
 * @param[in] byte Byte value to write
 * @return Status_t
 */
Status_t AT21CS01_WriteByte(uint8_t address, uint8_t byte);





#endif /* AT21CS01_H_ */
/** @} */
