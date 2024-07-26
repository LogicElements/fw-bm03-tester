/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_0_Pin GPIO_PIN_14
#define LED_0_GPIO_Port GPIOC
#define LED_1_Pin GPIO_PIN_15
#define LED_1_GPIO_Port GPIOC
#define GPIO0_Pin GPIO_PIN_0
#define GPIO0_GPIO_Port GPIOA
#define GPIO1_Pin GPIO_PIN_1
#define GPIO1_GPIO_Port GPIOA
#define GPIO10_Pin GPIO_PIN_2
#define GPIO10_GPIO_Port GPIOA
#define GPIO11_Pin GPIO_PIN_3
#define GPIO11_GPIO_Port GPIOA
#define DAC_Pin GPIO_PIN_4
#define DAC_GPIO_Port GPIOA
#define MUX_OE_Pin GPIO_PIN_5
#define MUX_OE_GPIO_Port GPIOA
#define MUX_S0_Pin GPIO_PIN_6
#define MUX_S0_GPIO_Port GPIOA
#define MUX_S1_Pin GPIO_PIN_7
#define MUX_S1_GPIO_Port GPIOA
#define MUX_S2_Pin GPIO_PIN_0
#define MUX_S2_GPIO_Port GPIOB
#define MUX_PU_Pin GPIO_PIN_1
#define MUX_PU_GPIO_Port GPIOB
#define MUX_A_Pin GPIO_PIN_8
#define MUX_A_GPIO_Port GPIOA
#define GPIO7_Pin GPIO_PIN_9
#define GPIO7_GPIO_Port GPIOA
#define GPIO8_Pin GPIO_PIN_10
#define GPIO8_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define GPIO9_Pin GPIO_PIN_15
#define GPIO9_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO2_Pin GPIO_PIN_4
#define GPIO2_GPIO_Port GPIOB
#define GPIO3_Pin GPIO_PIN_5
#define GPIO3_GPIO_Port GPIOB
#define GPIO4_Pin GPIO_PIN_6
#define GPIO4_GPIO_Port GPIOB
#define GPIO5_Pin GPIO_PIN_7
#define GPIO5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
