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

static uint16_t crc_ta_8[256] = 
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,  
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,  
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,  
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,  
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,  
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,  
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,  
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,  
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,  
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,  
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,  
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,  
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,  
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,  
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,  
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,  
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,  
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,  
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,  
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,  
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,  
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,  
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,  
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,  
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,  
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,  
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,  
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,  
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,  
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,  
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,  
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0    
};

uint8_t AckCmdBuffer[6];                                    //return Request Cmd buffer
uint8_t CortexA9CmdDataBuffer[DS_CMD_LEN + DS_DATA_LEN];
uint8_t DoorBoardCmdDataBuffer[DS_CMD_LEN + DS_DATA_LEN];

extern PROTOCOLCMD  gCortexA9ProtocolCmd;
extern PROTOCOLCMD  gDoorBoardProtocolCmd;
extern UART_HandleTypeDef   huart1;
extern UART_HandleTypeDef   huart2;

USARTRECIVETYPE     CortexA9UsartType;
USARTRECIVETYPE     DoorBoardUsartType;

static uint16_t CRC16BIT(uint8_t *pDataBuffer, uint16_t dataLength)
{
  uint16_t count;
  uint16_t crc = 0x1d0f;
  uint16_t temp;
  
  for(count = 0; count < dataLength; ++count)
  {
    temp = (*pDataBuffer++ ^(crc >> 8)) & 0xFF;
    crc = crc_ta_8[temp] ^ (crc << 8);
  }
  return (uint16_t)(crc ^ 0xFFFF);
}

static uint16_t crc_cal_by_byte(uint8_t* pData, uint16_t len)
{
  uint16_t crc = 0;
  while(len-- != 0)
  {
    uint16_t high = (uint16_t)(crc/256);
    crc <<= 8;
    crc ^= crc_ta_8[high^(*pData)];
    pData++;
  }
  return crc;
}

static DS_StatusTypeDef DS_SendRequestCmd(pPROTOCOLCMD pRequestCmd,uint8_t *pCmdDataBuffer)
{
    uint16_t dataLength = 0;
    DS_StatusTypeDef state = DS_OK;
    if(0 == pRequestCmd->HandingFlag)
    {
      pRequestCmd->HandingFlag = 0;
      *(pCmdDataBuffer + 1) = pRequestCmd->CmdType;
      *(pCmdDataBuffer + 2)= pRequestCmd->CmdParam;
      *(pCmdDataBuffer + 3)= pRequestCmd->DataLengthHight;
      *(pCmdDataBuffer + 4)= pRequestCmd->DataLengthLow;
      dataLength = ((pRequestCmd->DataLengthHight) << 8) + pRequestCmd->DataLengthLow;
      *(pCmdDataBuffer + 5 + dataLength) = 0x5D;
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
    CortexA9CmdDataBuffer[0] = 0x5B;
    DoorBoardCmdDataBuffer[0] = 0x5B;
    AckCmdBuffer[0] = 0x5B;
    AckCmdBuffer[5] = 0x5D;
    
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
              if(0x5D != *(CortexA9UsartType.RX_pData + 6 + gCortexA9ProtocolCmd.RevDataCnt))
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
          if(0x5B != *(CortexA9UsartType.RX_pData))
          {
            return state;
          }
          
          if(0xA0 ==(*(CortexA9UsartType.RX_pData + 1) & 0xF0) && 0x5D == *(CortexA9UsartType.RX_pData + 4))
          {
              gCortexA9ProtocolCmd.AckCmdCode   = *(CortexA9UsartType.RX_pData + 1);
              gCortexA9ProtocolCmd.AckCode      = *(CortexA9UsartType.RX_pData + 2);
              gCortexA9ProtocolCmd.RevEchoFlag  = 1;
              return state;
          }
          
          gCortexA9ProtocolCmd.CmdType          = *(CortexA9UsartType.RX_pData + 1);
          gCortexA9ProtocolCmd.CmdParam         = *(CortexA9UsartType.RX_pData + 2);
          gCortexA9ProtocolCmd.DataLengthLow    = *(CortexA9UsartType.RX_pData + 4);
          gCortexA9ProtocolCmd.DataLengthHight  = *(CortexA9UsartType.RX_pData + 3);
          gCortexA9ProtocolCmd.DataLength       = (gCortexA9ProtocolCmd.DataLengthHight << 8) +\
                                                   gCortexA9ProtocolCmd.DataLengthLow;
          
          if(0 == gCortexA9ProtocolCmd.DataLength)
          {
            if(0x5D != *(CortexA9UsartType.RX_pData + 6))
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
              if(0x5D != *(CortexA9UsartType.RX_pData + 6 + gCortexA9ProtocolCmd.RevDataCnt))
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
              if(0x5D != *(DoorBoardUsartType.RX_pData + 6 + gDoorBoardProtocolCmd.RevDataCnt))
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
          if(0x5B != *(DoorBoardUsartType.RX_pData))
          {
            return state;
          }
          
          if(0xA0 ==(*(DoorBoardUsartType.RX_pData + 1) & 0xAF) && 0x5D == *(DoorBoardUsartType.RX_pData + 4))
          {
              gDoorBoardProtocolCmd.AckCmdCode   = *(DoorBoardUsartType.RX_pData + 1);
              gDoorBoardProtocolCmd.AckCode      = *(DoorBoardUsartType.RX_pData + 2);
              gDoorBoardProtocolCmd.RevEchoFlag  = 1;
              return state;
          }
          
          gDoorBoardProtocolCmd.CmdType          = *(DoorBoardUsartType.RX_pData + 1);
          gDoorBoardProtocolCmd.CmdParam         = *(DoorBoardUsartType.RX_pData + 2);
          gDoorBoardProtocolCmd.DataLengthLow    = *(DoorBoardUsartType.RX_pData + 4);
          gDoorBoardProtocolCmd.DataLengthHight  = *(DoorBoardUsartType.RX_pData + 3);
          gDoorBoardProtocolCmd.DataLength       = (gDoorBoardProtocolCmd.DataLengthHight << 8) +\
                                                    gDoorBoardProtocolCmd.DataLengthLow;
          
          if(0 == gDoorBoardProtocolCmd.DataLength)
          {
            if(0x5D != *(DoorBoardUsartType.RX_pData + 6))
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
              if(0x5D != *(DoorBoardUsartType.RX_pData + 6 + gDoorBoardProtocolCmd.RevDataCnt))
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
      case 0xB0:pRequestCmd->AckCmdCode = 0xAB;
                pRequestCmd->AckCode = 0x01;
                DS_AckRequestCmdFromCortexA9(pRequestCmd);
                break;
                
      case 0xC0:pRequestCmd->AckCmdCode = 0xAC;
                pRequestCmd->AckCode = 0x01;
                DS_AckRequestCmdFromCortexA9(pRequestCmd);
                break;
                
      case 0xD0:pRequestCmd->AckCmdCode = 0xAD;
                pRequestCmd->AckCode = 0x01;
                DS_AckRequestCmdFromCortexA9(pRequestCmd);
                break;
                
      case 0xE0:pRequestCmd->AckCmdCode = 0xAE;
                pRequestCmd->AckCode = 0x01;
                DS_AckRequestCmdFromCortexA9(pRequestCmd);
                break;
                
      case 0xF0:pRequestCmd->AckCmdCode = 0xAF;
                pRequestCmd->AckCode = 0x01;
                DS_AckRequestCmdFromCortexA9(pRequestCmd);
                break;
                
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
    uint16_t    tCRC16;
    uint8_t    CRCDataBuffer[2];
    CRCDataBuffer[0] = pRequestCmd->AckCmdCode;
    CRCDataBuffer[1] = pRequestCmd->AckCode;
    
    tCRC16 = CRC16BIT(CRCDataBuffer,2);
    
    AckCmdBuffer[3] = (uint8_t)tCRC16 >> 8;
    AckCmdBuffer[4] = (uint8_t)tCRC16;
    
    AckCmdBuffer[1] = pRequestCmd->AckCmdCode;
    AckCmdBuffer[2] = pRequestCmd->AckCode;
    
    state = DS_SendDataToCortexA9(AckCmdBuffer,6,0xFFFF);
    
    return state;
  }

  DS_StatusTypeDef DS_AckRequestCmdFromDoorBoard(pPROTOCOLCMD pRequestCmd)
  {
    DS_StatusTypeDef state = DS_OK;
    AckCmdBuffer[1] = pRequestCmd->AckCmdCode;
    AckCmdBuffer[2] = pRequestCmd->AckCode;
    state = DS_SendDataToDoorBoard(AckCmdBuffer,5,0xFFFF);
    
    return state;
  }

  /**
    * @}
    */
  /**
    * @}
    */
  /*****************************END OF FILE**************************************/

