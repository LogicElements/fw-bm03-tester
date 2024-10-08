/**
 * @file       reg_map.h
 * @version    $(APP_VERSION)
 * @date       $(RELEASE_DATE)
 * @brief      Register map definition
 * @author     jan.bartovsky
 *
 * @copyright  Logic Elements Copyright
 *
 * @defgroup grRegMap Register map
 * @{
 * @brief Set of local register represented by 16-bit identifier
 *
 * This module contains definition and declaration of entire peripheral register map. It should
 * be generated from using excel sheet.
 *
 * @par Main features:
 * - Register map (set of register IDs)
 * - Storage memory structure for all registers
 * - Access macros to the registers according to its type
 * - Vectors of register that are to be logged, stored in EEPROM or synchronized
 * - Flash read and write API
 * - Callback for register change
 *
 * @par Example of initialization in the main().
 *
 * @code
 * //Configuration initial read out
 * Config_Init();
 * @endcode
 *
 *
 * @par Example of work with register.
 *
 * The snippet below shows how to set value of different registers. The call of ApplyConfig is here
 * to store the non-volatile register in the Flash memory.
 * @code
 * CONF_INT(REG_SYS_TIMESTAMP)++;
 * CONF_SHORT(REG_RF_ADDRESS_MY) = 0x01;
 * Config_ApplyConfig(REG_RF_ADDRESS_MY);
 * @endcode
 *
 */
#ifndef REG_MAP_H_
#define REG_MAP_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/


/** @defgroup grRegMapMap
 *  @ingroup grRegMap
 *  @brief Set of IDs of all registers in the map
 *  @{
 */

#define CONF_SYS_UPTIME                  0x00000112u  ///< Uptime
#define CONF_SYS_REGMAP_VERSION          0x00004132u  ///< Register map version
#define CONF_SYS_STATUS                  0x00008012u  ///< Status register
#define CONF_SYS_COMMAND                 0x0000C152u  ///< Command
#define CONF_SYS_TEST                    0x00010152u  ///< Testing register
#define CONF_SYS_IO_INPUT                0x00014111u  ///< Input signals
#define CONF_SYS_ERROR                   0x00016151u  ///< Error code
#define CONF_FACT_SERIAL_NUMBER          0x01000112u  ///< Serial number
#define CONF_FACT_DEVICE_ID              0x01004112u  ///< Product number
#define CONF_FACT_HW_REVISION            0x01008112u  ///< Hardware version
#define CONF_FACT_BOOT_REVISION          0x0100C112u  ///< Bootloader version
#define CONF_FIRM_REVISION               0x02000112u  ///< Firmware revision
#define CONF_FIRM_ASSEMBLY_INFO          0x02004112u  ///< Assembly date
#define CONF_FIRM_APP_CHECKSUM           0x02008112u  ///< CRC checksum
#define CONF_FIRM_APP_SIZE               0x0200C112u  ///< Firmware size
#define CONF_CALIB_MODE                  0x04000151u  ///< Calibration mode
#define CONF_CALIB_RESERVED              0x04002151u  ///< Reserved
#define CONF_BM_MUX                      0x05000150u  ///< Multiplexer select
#define CONF_BM_MUX_DEFAULT              0x05001170u  ///< Default multiplexer
#define CONF_BM_PADS                     0x05002150u  ///< Number of pads
#define CONF_BM_PAD_MODE                 0x05003150u  ///< Pad mode
#define CONF_BM_SIGNAL_PERIOD            0x05004151u  ///< Signal period
#define CONF_BM_SIGNAL_BASE              0x05006151u  ///< Signal DC value
#define CONF_BM_SIGNAL_AMPLITUDE         0x05008151u  ///< Signal AC amplitude
#define CONF_DBG_WRITES_CONF             0x06000112u  ///< Configuration writes
#define CONF_PAD_COMMAND                 0x07000550u  ///< Command for selected PAD memory
#define CONF_PAD_SELECT                  0x07001150u  ///< Multiplexer select
#define CONF_PAD_RESERVED                0x07002151u  ///< reserved
#define CONF_PAD_SERIAL_NUMBER           0x07004152u  ///< Serial number of PAD
#define CONF_PAD_LAST_CU_SN_WRITE_INDEX  0x07008152u  ///< Index of last written CU SN
#define CONF_PAD_CU_SN_INDEX0            0x0700C152u  ///< Control unit serial number 0
#define CONF_PAD_CU_SN_INDEX1            0x07010152u  ///< Control unit serial number 1
#define CONF_PAD_CU_SN_INDEX2            0x07014152u  ///< Control unit serial number 2
#define CONF_PAD_CU_SN_INDEX3            0x07018152u  ///< Control unit serial number 3
#define CONF_PAD_CU_SN_INDEX4            0x0701C152u  ///< Control unit serial number 4
#define CONF_PAD_CU_SN_INDEX5            0x07020152u  ///< Control unit serial number 5
#define CONF_PAD_CU_SN_INDEX6            0x07024152u  ///< Control unit serial number 6
#define CONF_PAD_CU_SN_INDEX7            0x07028152u  ///< Control unit serial number 7
#define CONF_PAD_CU_SN_INDEX8            0x0702C152u  ///< Control unit serial number 8
#define CONF_PAD_CU_SN_INDEX9            0x07030152u  ///< Control unit serial number 9
#define CONF_PAD_TIU_STANDBY             0x07034152u  ///< Time-in-use in standby mode
#define CONF_PAD_TIU_ACTIVE              0x07038152u  ///< Time-in-use in active mode
#define CONF_PAD_KID_PLACEMENT_NOTIF_CNT 0x0703C152u  ///< Number of kid placement notifications
#define CONF_PAD_ACTIVE_MODE_CNT         0x07040152u  ///< Number of active mode activation
#define CONF_PAD_CONN_CNT                0x07044152u  ///< Number of pad connections
#define CONF_PAD_ALARM_CNT               0x07048152u  ///< Number of alarms


/** @} */

/** @defgroup grRegMapBits
 *  @ingroup grRegMap
 *  @brief Set of definitions of bits 
 *  @{
 */

#define STAT_BIT_ERROR                (1 << (0))
#define STAT_BIT_TESTING              (1 << (1))
#define STAT_BIT_CALIBRATION          (1 << (2))
#define STAT_BIT_MB_TIMEOUT           (1 << (3))
#define STAT_BIT_CONFIG_FLASH         (1 << (16))
#define STAT_BIT_WATCHDOG             (1 << (17))


/** @} */


/** @defgroup grRegMapConstants
 *  @ingroup grRegMap
 *  @brief Constant parameters of the register map
 *  @{
 */

#define CONF_REG_BLOCK_NUMBER      (26)
#define CONF_REG_LOGGER_NUMBER     (0)
#define CONF_REG_CALIB_NUMBER      (1)
#define CONF_REG_SYNCED_NUMBER     (0)
#define CONF_REG_FLASH_NUMBER      (2)
#define CONF_REG_LOGGER_LENGTH     (0)
#define CONF_REG_CALIB_LENGTH      (8)
#define CONF_REG_SYNCED_LENGTH     (0)
#define CONF_REG_FLASH_LENGTH      (13)
#define CONF_REG_LOCAL_LENGTH      (0)

#define CONF_DIM_CONDITION ((sizeof(conf_reg_sys_t) != 24) || (sizeof(conf_reg_fact_t) != 16) || (sizeof(conf_reg_firm_t) != 16) || (sizeof(conf_reg_calib_t) != 4) || (sizeof(conf_reg_bm_t) != 12) || (sizeof(conf_reg_dbg_t) != 4) || (sizeof(conf_reg_pad_t) != 76) || 0)


/** @} */

/* Macros --------------------------------------------------------------------*/

/* Constants -------------------------------------------------------------------*/

/* Typedefs-------------------------------------------------------------------*/

/** @defgroup grRegMapTypedefs
 *  @ingroup grRegMap
 *  @brief Typedefs of structures that define register map
 *  @{
 */


typedef enum
{
  PAD_COM_IDLE = 0,
  PAD_COM_READ = 1,
  PAD_COM_WRITE = 2,
}pad_command_t ;

typedef struct __packed __aligned(4)
{
  uint32_t uptime;
  uint32_t regmap_version;
  uint32_t status;
  uint32_t command;
  uint32_t test;
  uint16_t io_input;
  uint16_t error;
}conf_reg_sys_t;

typedef struct __packed __aligned(4)
{
  uint32_t serial_number;
  uint32_t device_id;
  uint32_t hw_revision;
  uint32_t boot_revision;
}conf_reg_fact_t;

typedef struct __packed __aligned(4)
{
  uint32_t revision;
  uint32_t assembly_info;
  uint32_t app_checksum;
  uint32_t app_size;
}conf_reg_firm_t;

typedef struct __packed __aligned(4)
{
  uint16_t mode;
  uint16_t reserved;
}conf_reg_calib_t;

typedef struct __packed __aligned(4)
{
  uint8_t mux;
  uint8_t mux_default;
  uint8_t pads;
  uint8_t pad_mode;
  uint16_t signal_period;
  uint16_t signal_base;
  uint16_t signal_amplitude;
}conf_reg_bm_t;

typedef struct __packed __aligned(4)
{
  uint32_t writes_conf;
}conf_reg_dbg_t;

typedef struct __packed __aligned(4)
{
  pad_command_t command;
  uint8_t select;
  uint16_t reserved;
  uint32_t serial_number;
  uint32_t last_cu_sn_write_index;
  uint32_t cu_sn_index0;
  uint32_t cu_sn_index1;
  uint32_t cu_sn_index2;
  uint32_t cu_sn_index3;
  uint32_t cu_sn_index4;
  uint32_t cu_sn_index5;
  uint32_t cu_sn_index6;
  uint32_t cu_sn_index7;
  uint32_t cu_sn_index8;
  uint32_t cu_sn_index9;
  uint32_t tiu_standby;
  uint32_t tiu_active;
  uint32_t kid_placement_notif_cnt;
  uint32_t active_mode_cnt;
  uint32_t conn_cnt;
  uint32_t alarm_cnt;
}conf_reg_pad_t;


typedef struct 
{
  conf_reg_sys_t sys;
  conf_reg_fact_t fact;
  conf_reg_firm_t firm;
  uint32_t res4;
  conf_reg_calib_t calib;
  conf_reg_bm_t bm;
  conf_reg_dbg_t dbg;
  conf_reg_pad_t pad;
  uint32_t res9;
  uint32_t res10;
  uint32_t res11;
  uint32_t res12;
  uint32_t res13;
  uint32_t res14;
  uint32_t res15;
  uint32_t res16;
  uint32_t res17;
  uint32_t res18;
  uint32_t res19;
  uint32_t res20;
  uint32_t res21;
  uint32_t res22;
  uint32_t res23;
  uint32_t res24;
  uint32_t res25;
  uint32_t res26;
}
conf_reg_t;



/** @} */

/* Functions -----------------------------------------------------------------*/


/**
 * Set factory values for all registers. 
 *
 * @return Status
 */
Status_t RegMap_RestoreFactoryValues(void);

#endif /* REG_MAP_H_ */
/** @} */
