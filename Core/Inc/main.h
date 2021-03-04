/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define CH0_TIP1_VOL_Pin GPIO_PIN_0
#define CH0_TIP1_VOL_GPIO_Port GPIOC
#define CH0_TIP1_CUR_Pin GPIO_PIN_1
#define CH0_TIP1_CUR_GPIO_Port GPIOC
#define CH0_TIP0_VOL_Pin GPIO_PIN_2
#define CH0_TIP0_VOL_GPIO_Port GPIOC
#define CH0_TIP0_CUR_Pin GPIO_PIN_3
#define CH0_TIP0_CUR_GPIO_Port GPIOC
#define CH1_TIP1_CUR_Pin GPIO_PIN_0
#define CH1_TIP1_CUR_GPIO_Port GPIOA
#define CH1_TIP1_VOL_Pin GPIO_PIN_1
#define CH1_TIP1_VOL_GPIO_Port GPIOA
#define CH1_TIP0_VOL_Pin GPIO_PIN_2
#define CH1_TIP0_VOL_GPIO_Port GPIOA
#define CH1_TIP0_CUR_Pin GPIO_PIN_3
#define CH1_TIP0_CUR_GPIO_Port GPIOA
#define ENC2_B_Pin GPIO_PIN_5
#define ENC2_B_GPIO_Port GPIOA
#define ENC2_SW_Pin GPIO_PIN_6
#define ENC2_SW_GPIO_Port GPIOA
#define ENC2_SW_EXTI_IRQn EXTI9_5_IRQn
#define ENC2_A_Pin GPIO_PIN_7
#define ENC2_A_GPIO_Port GPIOA
#define ENC2_A_EXTI_IRQn EXTI9_5_IRQn
#define USART_TX_Pin GPIO_PIN_4
#define USART_TX_GPIO_Port GPIOC
#define USART_RX_Pin GPIO_PIN_5
#define USART_RX_GPIO_Port GPIOC
#define CH1_SCL_Pin GPIO_PIN_10
#define CH1_SCL_GPIO_Port GPIOB
#define CH1_SDA_Pin GPIO_PIN_11
#define CH1_SDA_GPIO_Port GPIOB
#define OLED_nCS_Pin GPIO_PIN_12
#define OLED_nCS_GPIO_Port GPIOB
#define OLED_SDCLK_Pin GPIO_PIN_13
#define OLED_SDCLK_GPIO_Port GPIOB
#define OLED_nRST_Pin GPIO_PIN_14
#define OLED_nRST_GPIO_Port GPIOB
#define OLED_SDIN_Pin GPIO_PIN_15
#define OLED_SDIN_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOC
#define CH1_TIP_1_Pin GPIO_PIN_8
#define CH1_TIP_1_GPIO_Port GPIOC
#define CH1_TIP_0_Pin GPIO_PIN_9
#define CH1_TIP_0_GPIO_Port GPIOC
#define CH0_TIP_1_Pin GPIO_PIN_10
#define CH0_TIP_1_GPIO_Port GPIOA
#define CH0_TIP_0_Pin GPIO_PIN_11
#define CH0_TIP_0_GPIO_Port GPIOA
#define ENC1_B_Pin GPIO_PIN_15
#define ENC1_B_GPIO_Port GPIOA
#define ENC1_SW_Pin GPIO_PIN_10
#define ENC1_SW_GPIO_Port GPIOC
#define ENC1_SW_EXTI_IRQn EXTI15_10_IRQn
#define ENC1_A_Pin GPIO_PIN_11
#define ENC1_A_GPIO_Port GPIOC
#define ENC1_A_EXTI_IRQn EXTI15_10_IRQn
#define CH0_SCL_Pin GPIO_PIN_6
#define CH0_SCL_GPIO_Port GPIOB
#define CH0_SDA_Pin GPIO_PIN_7
#define CH0_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
