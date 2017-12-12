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

/* CRC16-CCITT */
static unsigned short ccitt_table[256] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
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


static uint16_t CRC16BIT(uint8_t *q, uint16_t len)
{
  uint16_t crc = 0;
  
  while (len-- > 0)
    crc = ccitt_table[(crc >> 8 ^ *q++) & 0xff] ^ (crc << 8);
  return ~crc;
}

static DS_StatusTypeDef DS_SendRequestCmd(pPROTOCOLCMD pRequestCmd,uint8_t *pCmdDataBuffer)
{
    uint16_t dataLength = 0;
    uint16_t tempCRC    = 0;
    DS_StatusTypeDef state = DS_OK;
    if(0 == pRequestCmd->HandingFlag)
    {
      pRequestCmd->HandingFlag = 0;
      *(pCmdDataBuffer + 1) = pRequestCmd->CmdType;
      *(pCmdDataBuffer + 2)= pRequestCmd->CmdParam;
      *(pCmdDataBuffer + 3)= pRequestCmd->DataLengthHight;
      *(pCmdDataBuffer + 4)= pRequestCmd->DataLengthLow;
      dataLength = ((pRequestCmd->DataLengthHight) << 8) + pRequestCmd->DataLengthLow;
      *(pCmdDataBuffer + REQUESTFIXEDCOMMANDLEN + dataLength - 1) = 0x5D;
      
      /* Calculate CRC */
      tempCRC = CRC16BIT(pCmdDataBuffer + 1, 4 + dataLength);
      *(pCmdDataBuffer + dataLength + REQUESTFIXEDCOMMANDLEN - 3) = (uint8_t)(tempCRC >> 8);
      *(pCmdDataBuffer + dataLength + REQUESTFIXEDCOMMANDLEN - 2) = (uint8_t)tempCRC;
      
      pRequestCmd->TotalLength = dataLength + REQUESTFIXEDCOMMANDLEN;
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
        /* Handle the data in the command - Part2 */
        if(gCortexA9ProtocolCmd.RevDataCnt < gCortexA9ProtocolCmd.DataLength && 0 != gCortexA9ProtocolCmd.RevDataCnt)
        {
          for(i = 0; i < CortexA9UsartType.RX_Size; i++)
          {
            CortexA9CmdDataBuffer[5 + gCortexA9ProtocolCmd.RevDataCnt + i] = *(CortexA9UsartType.RX_pData + i);
            gCortexA9ProtocolCmd.RevDataCnt++;
            if(gCortexA9ProtocolCmd.DataLength == gCortexA9ProtocolCmd.RevDataCnt)
            {
              if(0x5D != *(CortexA9UsartType.RX_pData + i + 3))
              {
                gCortexA9ProtocolCmd.RevDataCnt     = 0;
                gCortexA9ProtocolCmd.DataLength     = 0;
                gCortexA9ProtocolCmd.TotalLength    = 0;
                return state;
              }
              gCortexA9ProtocolCmd.DataCRC16      = *(CortexA9UsartType.RX_pData + i + 1) << 8 + *(CortexA9UsartType.RX_pData + i + 2);
              /* Here to add CRC16-CCITT inspection code */
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
          /* Process the replay command */
          if(0xA0 ==(*(CortexA9UsartType.RX_pData + 1) & 0xF0) && 0x5D == *(CortexA9UsartType.RX_pData + ACKFIXEDCOMMANDLEN - 1))
          {
              gCortexA9ProtocolCmd.AckCmdCode   = *(CortexA9UsartType.RX_pData + 1);
              gCortexA9ProtocolCmd.AckCode      = *(CortexA9UsartType.RX_pData + 2);
              gCortexA9ProtocolCmd.AckCRC16CITT = (*(CortexA9UsartType.RX_pData + 3) << 8) + *(CortexA9UsartType.RX_pData + 4);
              /* Here to add CRC16-CCITT inspection code */
              gCortexA9ProtocolCmd.RevEchoFlag  = 1;
              return state;
          }
          
          gCortexA9ProtocolCmd.CmdType          = *(CortexA9UsartType.RX_pData + 1);
          gCortexA9ProtocolCmd.CmdParam         = *(CortexA9UsartType.RX_pData + 2);
          
          gCortexA9ProtocolCmd.DataLengthLow    = *(CortexA9UsartType.RX_pData + 4);    //Low byte in the back
          gCortexA9ProtocolCmd.DataLengthHight  = *(CortexA9UsartType.RX_pData + 3);    //high byte first
          
          CortexA9CmdDataBuffer[1]              = gCortexA9ProtocolCmd.CmdType;
          CortexA9CmdDataBuffer[2]              = gCortexA9ProtocolCmd.CmdParam;
          CortexA9CmdDataBuffer[3]              = gCortexA9ProtocolCmd.DataLengthHight;
          CortexA9CmdDataBuffer[4]              = gCortexA9ProtocolCmd.DataLengthLow;
          
          gCortexA9ProtocolCmd.DataLength       = (gCortexA9ProtocolCmd.DataLengthHight << 8) +\
                                                   gCortexA9ProtocolCmd.DataLengthLow;
          /* No data command */
          if(0 == gCortexA9ProtocolCmd.DataLength)
          {
            if(0x5D != *(CortexA9UsartType.RX_pData + REQUESTFIXEDCOMMANDLEN - 1))
            {
              return state;
            }
            gCortexA9ProtocolCmd.DataCRC16      = *(CortexA9UsartType.RX_pData + 5) << 8 + *(CortexA9UsartType.RX_pData + 6);
            /* Here to add CRC16-CCITT inspection code */
            gCortexA9ProtocolCmd.RevRequestFlag = 1;
            gCortexA9ProtocolCmd.TotalLength    = REQUESTFIXEDCOMMANDLEN ;
            return state;
          }
          
          /* Handle the data in the command - Part1 */
          for(i = 5; i < CortexA9UsartType.RX_Size; i++)
          {
            CortexA9CmdDataBuffer[i] = *(CortexA9UsartType.RX_pData + i);
            gCortexA9ProtocolCmd.RevDataCnt++;
            if(gCortexA9ProtocolCmd.DataLength == gCortexA9ProtocolCmd.RevDataCnt)
            {
              if(0x5D != *(CortexA9UsartType.RX_pData + REQUESTFIXEDCOMMANDLEN + gCortexA9ProtocolCmd.RevDataCnt - 1))
              {
                gCortexA9ProtocolCmd.RevDataCnt = 0;
                gCortexA9ProtocolCmd.DataLength = 0;
                gCortexA9ProtocolCmd.TotalLength = 0;
                return state;
              }
              gCortexA9ProtocolCmd.DataCRC16      = *(CortexA9UsartType.RX_pData + i + 1) << 8 + *(CortexA9UsartType.RX_pData + i + 2);
              gCortexA9ProtocolCmd.RevRequestFlag = 1;
              gCortexA9ProtocolCmd.TotalLength = REQUESTFIXEDCOMMANDLEN + gCortexA9ProtocolCmd.DataLength;
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
            DoorBoardCmdDataBuffer[5 + gDoorBoardProtocolCmd.RevDataCnt + i] = *(DoorBoardUsartType.RX_pData + i);
            gDoorBoardProtocolCmd.RevDataCnt++;
            if(gDoorBoardProtocolCmd.DataLength == gDoorBoardProtocolCmd.RevDataCnt)
            {
              if(0x5D != *(DoorBoardUsartType.RX_pData + i + 3))
              {
                gDoorBoardProtocolCmd.RevDataCnt    = 0;
                gDoorBoardProtocolCmd.DataLength    = 0;
                gDoorBoardProtocolCmd.TotalLength   = 0;
                return state;
              }
              gDoorBoardProtocolCmd.DataCRC16       = *(DoorBoardUsartType.RX_pData + i + 1)<<8 + *(DoorBoardUsartType.RX_pData + i + 2);
              /* Here add to CRC16-CCITT inspection code */
              gDoorBoardProtocolCmd.RevRequestFlag  = 1;
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
          
          if(0xA0 ==(*(DoorBoardUsartType.RX_pData + 1) & 0xAF) && 0x5D == *(DoorBoardUsartType.RX_pData + ACKFIXEDCOMMANDLEN - 1))
          {
              gDoorBoardProtocolCmd.AckCmdCode   = *(DoorBoardUsartType.RX_pData + 1);
              gDoorBoardProtocolCmd.AckCode      = *(DoorBoardUsartType.RX_pData + 2);
              gDoorBoardProtocolCmd.AckCRC16CITT = (*(DoorBoardUsartType.RX_pData + 3) << 8) + *(DoorBoardUsartType.RX_pData + 4);
              /*here to add CRC16-CCITT inspection code */
              
              gDoorBoardProtocolCmd.RevEchoFlag  = 1;
              return state;
          }
          
          gDoorBoardProtocolCmd.CmdType          = *(DoorBoardUsartType.RX_pData + 1);
          gDoorBoardProtocolCmd.CmdParam         = *(DoorBoardUsartType.RX_pData + 2);
          gDoorBoardProtocolCmd.DataLengthLow    = *(DoorBoardUsartType.RX_pData + 4);
          gDoorBoardProtocolCmd.DataLengthHight  = *(DoorBoardUsartType.RX_pData + 3);
          
          DoorBoardCmdDataBuffer[1]              = gDoorBoardProtocolCmd.CmdType;
          DoorBoardCmdDataBuffer[2]              = gDoorBoardProtocolCmd.CmdParam;
          DoorBoardCmdDataBuffer[3]              = gDoorBoardProtocolCmd.DataLengthHight;
          DoorBoardCmdDataBuffer[4]              = gDoorBoardProtocolCmd.DataLengthLow;
          
          
          gDoorBoardProtocolCmd.DataLength       = (gDoorBoardProtocolCmd.DataLengthHight << 8) +\
                                                    gDoorBoardProtocolCmd.DataLengthLow;
          
          if(0 == gDoorBoardProtocolCmd.DataLength)
          {
            if(0x5D != *(DoorBoardUsartType.RX_pData + REQUESTFIXEDCOMMANDLEN -1))
            {
              return state;
            }
            gDoorBoardProtocolCmd.DataCRC16     =  (*(DoorBoardUsartType.RX_pData + 5) << 8) + *(DoorBoardUsartType.RX_pData + 6);
            
            /*Here add to CRC16-CCITT inspection code */
            
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
              if(0x5D != *(DoorBoardUsartType.RX_pData + REQUESTFIXEDCOMMANDLEN + gDoorBoardProtocolCmd.RevDataCnt - 1))
              {
                gDoorBoardProtocolCmd.RevDataCnt = 0;
                gDoorBoardProtocolCmd.DataLength = 0;
                gDoorBoardProtocolCmd.TotalLength = 0;
                return state;
              }
              gDoorBoardProtocolCmd.DataCRC16      = *(DoorBoardUsartType.RX_pData + i + 1)<<8 + *(DoorBoardUsartType.RX_pData + i + 2);
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
    
    AckCmdBuffer[3] = (uint8_t)(tCRC16 >> 8);
    AckCmdBuffer[4] = (uint8_t)tCRC16;
    
    AckCmdBuffer[1] = pRequestCmd->AckCmdCode;
    AckCmdBuffer[2] = pRequestCmd->AckCode;
    
    state = DS_SendDataToCortexA9(AckCmdBuffer,6,0xFFFF);
    
    return state;
  }

  DS_StatusTypeDef DS_AckRequestCmdFromDoorBoard(pPROTOCOLCMD pRequestCmd)
  {
    
    DS_StatusTypeDef state = DS_OK;
    uint16_t    tCRC16;
    uint8_t    CRCDataBuffer[2];
    CRCDataBuffer[0] = pRequestCmd->AckCmdCode;
    CRCDataBuffer[1] = pRequestCmd->AckCode;
    
    tCRC16 = CRC16BIT(CRCDataBuffer,2);
    
    AckCmdBuffer[3] = (uint8_t)(tCRC16 >> 8);
    AckCmdBuffer[4] = (uint8_t)tCRC16;
    
    AckCmdBuffer[1] = pRequestCmd->AckCmdCode;
    AckCmdBuffer[2] = pRequestCmd->AckCode;
    state = DS_SendDataToDoorBoard(AckCmdBuffer,6,0xFFFF);
    
    return state;
  }

  /**
    * @}
    */
  /**
    * @}
    */
  /*****************************END OF FILE**************************************/

