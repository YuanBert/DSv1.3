/**
  ******************************************************************************
  * File Name          : GentleSensor.c
  * Description        : GentleSensor.c file
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
  /* Includes ------------------------------------------------------------------*/
#include "GentleSensor.h"

extern GPIOSTATUSDETECTION gGentleSensorStatusDetection;
extern PROTOCOLCMD  gCortexA9ProtocolCmd;
extern PROTOCOLCMD  gDoorBoardProtocolCmd;
DS_StatusTypeDef    DS_GentleSensorCheck(void)
{
  DS_StatusTypeDef state = DS_OK;
  if(0 == gGentleSensorStatusDetection.GpioSendDataFlag && gGentleSensorStatusDetection.GpioCarFlag)
  {
    /* if the vehcile is still ,carry out the release operation */
  }
  
  if(gGentleSensorStatusDetection.GpioStatusVal && gGentleSensorStatusDetection.GpioSendDataFlag)
  {
    gCortexA9ProtocolCmd.CmdType    = 0xB1;
    gCortexA9ProtocolCmd.CmdParam   = 0x01;
    gCortexA9ProtocolCmd.DataLength = 0x0000;
    gCortexA9ProtocolCmd.DataLengthLow    = 0x00;
    gCortexA9ProtocolCmd.DataLengthHight  = 0x00;
    state = DS_SendRequestCmdToCortexA9(&gCortexA9ProtocolCmd);
    
    gGentleSensorStatusDetection.GpioSendDataFlag = 0;
  }
  
  
  return state;
}

DS_StatusTypeDef    DS_GentleSensorReleaseCheck(void)
{
  DS_StatusTypeDef state = DS_OK;
  if(0 == gGentleSensorStatusDetection.GpioSendDataFlag && gGentleSensorStatusDetection.GpioCarFlag)
  {
    /* if the vehcile is still ,carry out the release operation */
    
    
  }
  else
  {
    /* if the vehicle is not in, give up the releases operation,and return the relevant code*/
    
    state = DS_ERROR;
  }
  
  return state;
}
  /**
    * @}
    */
  /**
    * @}
    */
  /*****************************END OF FILE**************************************/

