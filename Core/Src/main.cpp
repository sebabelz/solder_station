/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

#include <AD7995.h>
#include "main.h"
#include "SolderingHandle.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdio>


#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "SSD1322_HW_Driver.h"
#include "SSD1322_API.h"
#include "SSD1322_GFX.h"
#include "Fonts/FreeMono7pt8b.h"
#include "Fonts/FreeMono18pt8b.h"


void SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

volatile uint8_t OLED_TX_completed_flag = 1;    //flag indicating finish of SPI transmission
uint8_t tx_buf[256 * 64 / 2];

SolderingHandle handle[2];
char debug[20] = "";

uint32_t buffer[8] = {0};

int setPoint[2] = {320, 320};
char char_setPoint[2][4] = {"", ""};
char actual_temp[2][4] = {"---", "---"};
uint16_t pwmOutput[2][2];

void WriteDisplay();

/**
  * @brief  The application entry point.
  * @retval int
  */
int main() {
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_SPI2_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM8_Init();
    MX_USART3_UART_Init();

    SSD1322_API_init();

    if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK) {
        Error_Handler();
    };

    HAL_ADC_Start_DMA(&hadc1, buffer, 8);

    set_buffer_size(256, 64);
    send_buffer_to_OLED(tx_buf, 0, 0);

    snprintf(char_setPoint[0], 4, "%d", setPoint[0]);
    snprintf(char_setPoint[1], 4, "%d", setPoint[1]);

    HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
    HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);


    handle[0].setI2CHandle(&hi2c1);
    handle[0].setSetPoint(setPoint[0]);
    handle[0].setOutput(&pwmOutput[0][0], &pwmOutput[0][1]);

    handle[1].setI2CHandle(&hi2c2);
    handle[1].setSetPoint(setPoint[1]);
    handle[1].setOutput(&pwmOutput[1][0], &pwmOutput[1][1]);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        snprintf(actual_temp[0], 4, "%d", handle[0].getTipTemperature());
        snprintf(actual_temp[1], 4, "%d", handle[1].getTipTemperature());

        snprintf(debug, 20, "%lu", buffer[3]);
        WriteDisplay();
    }
#pragma clang diagnostic pop

}

void WriteDisplay() {
    clear_pixel(tx_buf, 0, 0, 255, 63);
    select_font(&FreeMono18pt8b);
    draw_text(tx_buf, actual_temp[0], 20, 40, 15);
    draw_text(tx_buf, actual_temp[1], 168, 40, 15);

    select_font(&FreeMono7pt8b);
    draw_text(tx_buf, debug, 10, 10, 15);

    draw_text(tx_buf, "set:", 20, 52, 15);
    draw_text(tx_buf, "set:", 168, 52, 15);
    draw_text(tx_buf, char_setPoint[0], 56, 52, 15);
    draw_text(tx_buf, char_setPoint[1], 204, 52, 15);
    send_buffer_to_OLED(tx_buf, 0, 0);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_I2C1
                                         | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_ADC;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{


}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    SSD1322_HW_drive_CS_high();
    OLED_TX_completed_flag = 1;
}

/**
//  * @brief EXTI line detection callbacks
//  * @param GPIO_Pin: Specifies the pins connected EXTI line
//  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == ENC1_A_Pin) {
        if (HAL_GPIO_ReadPin(ENC1_B_GPIO_Port, ENC1_B_Pin)) {
            snprintf(char_setPoint[0], 4, "%d",handle[0].setSetPoint(--setPoint[0]));
        } else {
            snprintf(char_setPoint[0], 4, "%d", handle[0].setSetPoint(++setPoint[0]));
        }
    }

    if (GPIO_Pin == ENC1_SW_Pin) {
        handle[0].toggle();
    }

    if (GPIO_Pin == ENC2_A_Pin) {
        if (HAL_GPIO_ReadPin(ENC2_SW_GPIO_Port, ENC2_B_Pin)) {
            snprintf(char_setPoint[1], 4, "%d", handle[0].setSetPoint(--setPoint[1]));
        } else {
            snprintf(char_setPoint[1], 4, "%d", handle[0].setSetPoint(++setPoint[1]));
        }
    }

    if (GPIO_Pin == ENC2_SW_Pin) {
        handle[1].toggle();
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        handle[0].processControl();
        if (handle[0].getStatus() == ConnectionStatus::Disconnected) {
            HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
            HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_2);
        }

        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwmOutput[0][0]);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwmOutput[0][1]);
    }

    if (htim->Instance == TIM8) {
        handle[1].processControl();
        if (handle[1].getStatus() == ConnectionStatus::Disconnected) {
            HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);
            HAL_TIM_OC_Stop_IT(&htim8, TIM_CHANNEL_2);
        }

        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, pwmOutput[1][0]);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, pwmOutput[1][1]);
    }
    if (htim->Instance == TIM3) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            handle[0].checkConnection();
            if (handle[0].getStatus() != ConnectionStatus::Disconnected) {
                HAL_TIM_OC_Stop_IT(&htim3, TIM_CHANNEL_1);
                HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_2);
            }
        }
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
            handle[1].checkConnection();
            if (handle[1].getStatus() != ConnectionStatus::Disconnected) {
                HAL_TIM_OC_Stop_IT(&htim3, TIM_CHANNEL_2);
                HAL_TIM_OC_Start_IT(&htim8, TIM_CHANNEL_2);
            }
        }
    }

}

/**
  * @brief  This function is executed in case of _error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL _error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param _error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param _error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
