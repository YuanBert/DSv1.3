/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define CommunicationLED_Pin GPIO_PIN_13
#define CommunicationLED_GPIO_Port GPIOC
#define CTR485_EN_Pin GPIO_PIN_1
#define CTR485_EN_GPIO_Port GPIOC
#define PhotosensitiveSensor_Pin GPIO_PIN_2
#define PhotosensitiveSensor_GPIO_Port GPIOC
#define MCU_KEY_IN_Pin GPIO_PIN_1
#define MCU_KEY_IN_GPIO_Port GPIOA
#define MCU_KEY_IN_EXTI_IRQn EXTI1_IRQn
#define BSP_TX_Pin GPIO_PIN_2
#define BSP_TX_GPIO_Port GPIOA
#define BSP_RX_Pin GPIO_PIN_3
#define BSP_RX_GPIO_Port GPIOA
#define W25Q64_SCK_Pin GPIO_PIN_5
#define W25Q64_SCK_GPIO_Port GPIOA
#define W25Q64_MISO_Pin GPIO_PIN_6
#define W25Q64_MISO_GPIO_Port GPIOA
#define W25Q64_MOSI_Pin GPIO_PIN_7
#define W25Q64_MOSI_GPIO_Port GPIOA
#define GentleSensor_Pin GPIO_PIN_4
#define GentleSensor_GPIO_Port GPIOC
#define MCUAtmosphereLED2_Pin GPIO_PIN_12
#define MCUAtmosphereLED2_GPIO_Port GPIOB
#define MCUAtmosphereLED1_Pin GPIO_PIN_13
#define MCUAtmosphereLED1_GPIO_Port GPIOB
#define MCU_DS18B20_Pin GPIO_PIN_6
#define MCU_DS18B20_GPIO_Port GPIOC
#define CortexA9_TX_Pin GPIO_PIN_9
#define CortexA9_TX_GPIO_Port GPIOA
#define CortexA9_RX_Pin GPIO_PIN_10
#define CortexA9_RX_GPIO_Port GPIOA
#define Speaker_TX_Pin GPIO_PIN_10
#define Speaker_TX_GPIO_Port GPIOC
#define Speaker_RX_Pin GPIO_PIN_11
#define Speaker_RX_GPIO_Port GPIOC
#define MCU_FAN_OUT_Pin GPIO_PIN_5
#define MCU_FAN_OUT_GPIO_Port GPIOB
#define MCU_LED_OUT_Pin GPIO_PIN_6
#define MCU_LED_OUT_GPIO_Port GPIOB
#define RunningLED_Pin GPIO_PIN_9
#define RunningLED_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define MCU_LED_ON                      GPIO_PIN_SET
#define MCU_LED_OFF                     GPIO_PIN_RESET
#define MCU_FAN_ON                      GPIO_PIN_SET
#define MCU_FAN_OFF                     GPIO_PIN_RESET
#define RunningLED_ON                   GPIO_PIN_SET
#define RunningLED_OFF                  GPIO_PIN_RESET
#define CommunicationLED_ON             GPIO_PIN_SET
#define CommunicationLED_OFF            GPIO_PIN_RESET
#define MCUAtmosphereLED1_ON            GPIO_PIN_SET
#define MCUAtmosphereLED1_OFF           GPIO_PIN_RESET
#define MCUAtmosphereLED2_ON            GPIO_PIN_SET
#define MCUAtmosphereLED2_OFF           GPIO_PIN_RESET

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
