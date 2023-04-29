/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

  extern TIM_HandleTypeDef htim2;
  extern TIM_HandleTypeDef htim21;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define LEVEL1_Pin GPIO_PIN_5
#define LEVEL1_GPIO_Port GPIOA
#define LEVEL2_Pin GPIO_PIN_6
#define LEVEL2_GPIO_Port GPIOA
#define LEVEL3_Pin GPIO_PIN_7
#define LEVEL3_GPIO_Port GPIOA
#define SONO_ECHO_Pin GPIO_PIN_0
#define SONO_ECHO_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_8
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_9
#define LED_G_GPIO_Port GPIOA
#define SONO_TRIG_Pin GPIO_PIN_10
#define SONO_TRIG_GPIO_Port GPIOA
#define BTN_1_Pin GPIO_PIN_11
#define BTN_1_GPIO_Port GPIOA
#define BTN_2_Pin GPIO_PIN_12
#define BTN_2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LED_Life_Pin GPIO_PIN_3
#define LED_Life_GPIO_Port GPIOB
#define REL1_Pin GPIO_PIN_4
#define REL1_GPIO_Port GPIOB
#define REL2_Pin GPIO_PIN_5
#define REL2_GPIO_Port GPIOB
#define REL3_Pin GPIO_PIN_6
#define REL3_GPIO_Port GPIOB
#define REL4_Pin GPIO_PIN_7
#define REL4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
