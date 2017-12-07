/**
  ******************************************************************************
  * File Name          : DS_Protocol.h
  * Description        : DS_Protocol.h file
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
  #ifndef __DS_PROTOCOL_H
  #define __DS_PROTOCOL_H
  #ifdef __cplusplus
   extern "C" {
  #endif
  /* Includes ------------------------------------------------------------------*/
  #include "main.h"
  #include "usart.h"
  #include "stm32f1xx_hal.h"  
  
  /*******************************************************************************
  ** struct: sProtocolCmd
  **
  ** DESCRIPTION:
  **  -- new ProtocolCmd 
  **
  ** CREATED: 2017/12/7, by Bert
  **
  ** FILE: DS_Protocol.h
  **
  ** AUTHOR: Bert.Zhang
  *****************************************************************************
  */
  struct sProtocolCmd
  {
    uint8_t     CmdType;
    uint8_t     CmdParam;
    uint8_t     DataLengthLow;
    uint8_t     DataLengthHight;
    uint8_t     DataCRC8;
    uint8_t*    pDataBuffer;
    
    uint8_t     HandingFlag;
    uint8_t     AckCmdCode;
    uint8_t     AckCode;
    
    uint8_t     RevRequestFlag;
    uint8_t     RevEchoFlag;
    uint8_t     SendTimesCnt;
  };
  
  
  #ifdef __cplusplus
  }
  #endif
  #endif
  /***************************************************************END OF FILE****/

