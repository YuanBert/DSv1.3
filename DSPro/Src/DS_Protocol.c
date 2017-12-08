/**
  ******************************************************************************
  * File Name          : DS_Protocol.c
  * Description        : DS_Protocol.c file
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
#include "DS_Protocol.h"
#include "usart.h"

uint8_t AckCmdBuffer[4];
uint8_t CortexA9CmdDataBuffer[DS_CMD_LEN + DS_DATA_LEN];
uint8_t DoorBoardCmdDataBuffer[DS_CMD_LEN + DS_DATA_LEN];

extern PROTOCOLCMD  gCortexA9ProtocolCmd;
extern PROTOCOLCMD  gDoorBoardProtocolCmd;
extern UART_HandleTypeDef   huart1;
extern UART_HandleTypeDef   huart2;

USARTRECIVETYPE     CortexA9UsartType;
USARTRECIVETYPE     DoorBoardUsartType;

static DS_StatusTypeDef DS_SendRequestCmd(pPROTOCOLCMD pRequestCmd,uint8_t *pCmdDataBuffer)
{
    uint16_t dataLength = 0;
    DS_StatusTypeDef state = DS_OK;
    if(0 == pRequestCmd->HandingFlag)
    {
      pRequestCmd->HandingFlag = 0;
      *(pCmdDataBuffer + 1) = pRequestCmd->CmdType;
      *(pCmdDataBuffer + 2)= pRequestCmd->CmdParam;
      *(pCmdDataBuffer + 3)= pRequestCmd->DataLengthLow;
      *(pCmdDataBuffer + 4)= pRequestCmd->DataLengthHight;
      dataLength = ((pRequestCmd->DataLengthHight) << 8) + pRequestCmd->DataLengthLow;
      *(pCmdDataBuffer + 5 + dataLength) = 0x53;
      pRequestCmd->TotalLength = dataLength;
      pRequestCmd->RevOrSendFlag = 1;
      pRequestCmd->RevEchoFlag = 0;
    }
    return state;  
}
void DS_CortexA9UsartReceive_IDLE(UART_HandleTypeDef *huart)
{
  uint32_t  temp;
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    HAL_UART_DMAStop(&huart1);
    temp = huart1.hdmarx->Instance->CNDTR;
    CortexA9UsartType.RX_Size = DS_RX_LEN - temp;
    CortexA9UsartType.RX_Flag = 1;
    HAL_UART_Receive_DMA(&huart1,CortexA9UsartType.RX_pData,DS_RX_LEN);
  }
}

void DS_DoorBoardUsartReceive_IDLE(UART_HandleTypeDef *huart)
{
  uint32_t  temp;
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    HAL_UART_DMAStop(&huart2);
    temp = huart2.hdmarx->Instance->CNDTR;
    DoorBoardUsartType.RX_Size = DS_RX_LEN - temp;
    DoorBoardUsartType.RX_Flag = 1;
    HAL_UART_Receive_DMA(&huart2,DoorBoardUsartType.RX_pData,DS_RX_LEN);    
  }
}
  DS_StatusTypeDef DS_ProtocolInit(void)
  {
    DS_StatusTypeDef state = DS_OK;
    CortexA9CmdDataBuffer[0] = 0x53;
    DoorBoardCmdDataBuffer[0] = 0x53;
    
    
    return state;
  }

  DS_StatusTypeDef DS_SendDataToCortexA9(uint8_t *pData, uint16_t size,uint32_t Timeout)
  {
    DS_StatusTypeDef state = DS_OK;
    state = (DS_StatusTypeDef)HAL_UART_Transmit_DMA(&huart1,pData,size);
    return state;
  }

  DS_StatusTypeDef DS_SendDataToDoorBoard(uint8_t *pData, uint16_t size,uint32_t Timeout)
  {
    DS_StatusTypeDef state = DS_OK;
    state = (DS_StatusTypeDef)HAL_UART_Transmit_DMA(&huart2,pData,size);
    return state;
  }

  DS_StatusTypeDef DS_SendRequestCmdToCortexA9(pPROTOCOLCMD pRequestCmd)
  {
      DS_StatusTypeDef state = DS_OK;
      state = DS_SendRequestCmd(pRequestCmd,CortexA9CmdDataBuffer);
      return state;
  }

  DS_StatusTypeDef DS_SendRequestCmdToDoorBoard(pPROTOCOLCMD pRequestCmd)
  {
      DS_StatusTypeDef state = DS_OK;
      state = DS_SendRequestCmd(pRequestCmd,DoorBoardCmdDataBuffer);
      return state;
  }

  DS_StatusTypeDef DS_TrySend5TimesCmdToCortexA9(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
    if(1 == pRequestCmd->RevOrSendFlag)
    {
      if(1 == pRequestCmd->RevEchoFlag)
      {
        pRequestCmd->HandingFlag    = 0;
        pRequestCmd->RevOrSendFlag  = 0;
        pRequestCmd->SendTimesCnt   = 0;
        pRequestCmd->RevEchoFlag    = 0;
      }
      else
      {
        if(pRequestCmd->SendTimesCnt > 4)
        {
          //³¬Ê±´íÎó
          pRequestCmd->HandingFlag = 0;
          pRequestCmd->RevOrSendFlag = 0;
          pRequestCmd->SendTimesCnt = 0;
          pRequestCmd->RevEchoFlag = 0;
        }
        else
        {
          state = DS_SendDataToCortexA9(CortexA9CmdDataBuffer,pRequestCmd->TotalLength,0xFFFF);
          (pRequestCmd->SendTimesCnt)++;
        }
      }
    }
    return state;
  }

  DS_StatusTypeDef DS_TrySend5TimesCmdToDoorBoard(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
    if(1 == pRequestCmd->RevOrSendFlag)
    {
      if(1 == pRequestCmd->RevEchoFlag)
      {
        pRequestCmd->HandingFlag    = 0;
        pRequestCmd->RevOrSendFlag  = 0;
        pRequestCmd->SendTimesCnt   = 0;
        pRequestCmd->RevEchoFlag    = 0;
      }
      else
      {
        if(pRequestCmd->SendTimesCnt > 4)
        {
          //³¬Ê±´íÎó
          pRequestCmd->HandingFlag = 0;
          pRequestCmd->RevOrSendFlag = 0;
          pRequestCmd->SendTimesCnt = 0;
          pRequestCmd->RevEchoFlag = 0;
        }
        else
        {
          state = DS_SendDataToDoorBoard(DoorBoardCmdDataBuffer,pRequestCmd->TotalLength,0xFFFF);
          (pRequestCmd->SendTimesCnt)++;
        }
      }
    }   
    return state;
  }
  
  DS_StatusTypeDef DS_HandingUartDataFromCortexA9(void)
  {
    DS_StatusTypeDef state = DS_OK;
    //uint8_t temp;
    uint16_t i;
    if(CortexA9UsartType.RX_Flag)
    {
      CortexA9UsartType.RX_Flag = 0;
      if(gCortexA9ProtocolCmd.RevRequestFlag)
      {
        //
        return state;
      }
      else
      {
        if(gCortexA9ProtocolCmd.RevDataCnt < gCortexA9ProtocolCmd.DataLength && 0 != gCortexA9ProtocolCmd.RevDataCnt)
        {
          for(i = 0; i < CortexA9UsartType.RX_Size; i++)
          {
            CortexA9CmdDataBuffer[gCortexA9ProtocolCmd.RevDataCnt + i] = *(CortexA9UsartType.RX_pData + i);
            gCortexA9ProtocolCmd.RevDataCnt++;
            if(gCortexA9ProtocolCmd.DataLength == gCortexA9ProtocolCmd.RevDataCnt)
            {
              if(0x53 != *(CortexA9UsartType.RX_pData + 6 + gCortexA9ProtocolCmd.RevDataCnt))
              {
                gCortexA9ProtocolCmd.RevDataCnt = 0;
                gCortexA9ProtocolCmd.DataLength = 0;
                gCortexA9ProtocolCmd.TotalLength = 0;
                return state;
              }
              gCortexA9ProtocolCmd.RevRequestFlag = 1;
              return state;
            }             
          }
          return state;
        }
        
        if(0 == gCortexA9ProtocolCmd.TotalLength)
        {
          if(0x53 != *(CortexA9UsartType.RX_pData))
          {
            return state;
          }
          
          if(0xA0 ==(*(CortexA9UsartType.RX_pData + 1) & 0xF0) && 0x53 == *(CortexA9UsartType.RX_pData + 4))
          {
              gCortexA9ProtocolCmd.AckCmdCode   = *(CortexA9UsartType.RX_pData + 1);
              gCortexA9ProtocolCmd.AckCode      = *(CortexA9UsartType.RX_pData + 2);
              gCortexA9ProtocolCmd.RevEchoFlag  = 1;
              return state;
          }
          
          gCortexA9ProtocolCmd.CmdType          = *(CortexA9UsartType.RX_pData + 1);
          gCortexA9ProtocolCmd.CmdParam         = *(CortexA9UsartType.RX_pData + 2);
          gCortexA9ProtocolCmd.DataLengthLow    = *(CortexA9UsartType.RX_pData + 3);
          gCortexA9ProtocolCmd.DataLengthHight  = *(CortexA9UsartType.RX_pData + 4);
          gCortexA9ProtocolCmd.DataLength       = (gCortexA9ProtocolCmd.DataLengthHight << 8) +\
                                                   gCortexA9ProtocolCmd.DataLengthLow;
          
          if(0 == gCortexA9ProtocolCmd.DataLength)
          {
            if(0x53 != *(CortexA9UsartType.RX_pData + 6))
            {
              return state;
            }
            gCortexA9ProtocolCmd.RevRequestFlag = 1;
            gCortexA9ProtocolCmd.TotalLength = 6;
            return state;
          }
          
          for(i = 5; i < CortexA9UsartType.RX_Size; i++)
          {
            CortexA9CmdDataBuffer[i] = *(CortexA9UsartType.RX_pData + i);
            gCortexA9ProtocolCmd.RevDataCnt++;
            if(gCortexA9ProtocolCmd.DataLength == gCortexA9ProtocolCmd.RevDataCnt)
            {
              if(0x53 != *(CortexA9UsartType.RX_pData + 6 + gCortexA9ProtocolCmd.RevDataCnt))
              {
                gCortexA9ProtocolCmd.RevDataCnt = 0;
                gCortexA9ProtocolCmd.DataLength = 0;
                gCortexA9ProtocolCmd.TotalLength = 0;
                return state;
              }
              gCortexA9ProtocolCmd.RevRequestFlag = 1;
              gCortexA9ProtocolCmd.TotalLength = 6 + gCortexA9ProtocolCmd.DataLength;
              return state;
            } 
          }
          
        }
      }
    }
    return state;
  }
  
  DS_StatusTypeDef DS_HandingUartDataFromDoorBoard(void)
  {
    DS_StatusTypeDef state = DS_OK;
    //uint8_t temp;
    uint16_t i;
    if(DoorBoardUsartType.RX_Flag)
    {
      DoorBoardUsartType.RX_Flag = 0;
      if(gDoorBoardProtocolCmd.RevRequestFlag)
      {
        //
        return state;
      }
      else
      {
        if(gDoorBoardProtocolCmd.RevDataCnt < gDoorBoardProtocolCmd.DataLength && 0 != gDoorBoardProtocolCmd.RevDataCnt)
        {
          for(i = 0; i < DoorBoardUsartType.RX_Size; i++)
          {
            DoorBoardCmdDataBuffer[gDoorBoardProtocolCmd.RevDataCnt + i] = *(DoorBoardUsartType.RX_pData + i);
            gDoorBoardProtocolCmd.RevDataCnt++;
            if(gDoorBoardProtocolCmd.DataLength == gDoorBoardProtocolCmd.RevDataCnt)
            {
              if(0x53 != *(DoorBoardUsartType.RX_pData + 6 + gDoorBoardProtocolCmd.RevDataCnt))
              {
                gDoorBoardProtocolCmd.RevDataCnt = 0;
                gDoorBoardProtocolCmd.DataLength = 0;
                gDoorBoardProtocolCmd.TotalLength = 0;
                return state;
              }
              gDoorBoardProtocolCmd.RevRequestFlag = 1;
              return state;
            }             
          }
          return state;
        }
        
        if(0 == gDoorBoardProtocolCmd.TotalLength)
        {
          if(0x53 != *(DoorBoardUsartType.RX_pData))
          {
            return state;
          }
          
          if(0xA0 ==(*(DoorBoardUsartType.RX_pData + 1) & 0xAF) && 0x53 == *(DoorBoardUsartType.RX_pData + 4))
          {
              gDoorBoardProtocolCmd.AckCmdCode   = *(DoorBoardUsartType.RX_pData + 1);
              gDoorBoardProtocolCmd.AckCode      = *(DoorBoardUsartType.RX_pData + 2);
              gDoorBoardProtocolCmd.RevEchoFlag  = 1;
              return state;
          }
          
          gDoorBoardProtocolCmd.CmdType          = *(DoorBoardUsartType.RX_pData + 1);
          gDoorBoardProtocolCmd.CmdParam         = *(DoorBoardUsartType.RX_pData + 2);
          gDoorBoardProtocolCmd.DataLengthLow    = *(DoorBoardUsartType.RX_pData + 3);
          gDoorBoardProtocolCmd.DataLengthHight  = *(DoorBoardUsartType.RX_pData + 4);
          gDoorBoardProtocolCmd.DataLength       = (gDoorBoardProtocolCmd.DataLengthHight << 8) +\
                                                    gDoorBoardProtocolCmd.DataLengthLow;
          
          if(0 == gDoorBoardProtocolCmd.DataLength)
          {
            if(0x53 != *(DoorBoardUsartType.RX_pData + 6))
            {
              return state;
            }
            gDoorBoardProtocolCmd.RevRequestFlag = 1;
            gDoorBoardProtocolCmd.TotalLength = 6;
            return state;
          }
          
          for(i = 5; i < DoorBoardUsartType.RX_Size; i++)
          {
            DoorBoardCmdDataBuffer[i] = *(DoorBoardUsartType.RX_pData + i);
            gDoorBoardProtocolCmd.RevDataCnt++;
            if(gDoorBoardProtocolCmd.DataLength == gDoorBoardProtocolCmd.RevDataCnt)
            {
              if(0x53 != *(DoorBoardUsartType.RX_pData + 6 + gDoorBoardProtocolCmd.RevDataCnt))
              {
                gDoorBoardProtocolCmd.RevDataCnt = 0;
                gDoorBoardProtocolCmd.DataLength = 0;
                gDoorBoardProtocolCmd.TotalLength = 0;
                return state;
              }
              gDoorBoardProtocolCmd.RevRequestFlag = 1;
              gDoorBoardProtocolCmd.TotalLength = 6 + gDoorBoardProtocolCmd.DataLength;
              return state;
            } 
          }
          
        }
      }
    }
    return state;
  }

  DS_StatusTypeDef DS_HandingCmdFromCortexA9(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
    if(1 == pRequestCmd->RevRequestFlag )
    {
      switch(pRequestCmd->CmdType & 0xF0)
      {
      case 0xB0:;break;
      case 0xC0:;break;
      case 0xD0:;break;
      case 0xE0:;break;
      case 0xF0:;break;
      default: state = DS_NOCMD; break;
      }
      
      pRequestCmd->HandingFlag      = 0;
      pRequestCmd->RevRequestFlag   = 0;
      pRequestCmd->DataLength       = 0;
      pRequestCmd->DataLengthHight  = 0;
      pRequestCmd->DataLengthLow    = 0;
      pRequestCmd->RevDataCnt       = 0;
      pRequestCmd->TotalLength      = 0;
    }
    return state;
  }

  DS_StatusTypeDef DS_HandingCmdFromDoorBoard(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
     if(1 == pRequestCmd->RevRequestFlag )
    {
      switch(pRequestCmd->CmdType & 0xF0)
      {
      case 0xB0:;break;
      case 0xC0:;break;
      case 0xD0:;break;
      case 0xE0:;break;
      case 0xF0:;break;
      default: state = DS_NOCMD; break;
      }
      
      pRequestCmd->HandingFlag      = 0;
      pRequestCmd->RevRequestFlag   = 0;
      pRequestCmd->DataLength       = 0;
      pRequestCmd->DataLengthHight  = 0;
      pRequestCmd->DataLengthLow    = 0;
      pRequestCmd->RevDataCnt       = 0;
      pRequestCmd->TotalLength      = 0;
    }   
    return state;
  }

  DS_StatusTypeDef DS_AckRequestCmdFromCortexA9(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
    
    return state;
  }

  DS_StatusTypeDef DS_AckRequestCmdFromDoorBoard(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
    
    return state;
  }

  /**
    * @}
    */
  /**
    * @}
    */
  /*****************************END OF FILE**************************************/

