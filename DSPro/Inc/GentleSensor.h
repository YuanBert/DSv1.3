/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GENTLESENSOR_H
#define __GENTLESENSOR_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DS_Protocol.h"
#include "stm32f1xx_hal.h"

/*******************************************************************************
** struct: sGpioStatusDetection
**
** DESCRIPTION:
**  --gpio status 
**
** CREATED: 2017/12/7, by bert
**
** FILE: GentleSensor.h
**
** AUTHOR: Bert.Zhang
********************************************************************************
*/
struct sGpioStatusDetection
{
  uint8_t   GpioCurrentReadVal;                 //Current GPIO value
  uint8_t   GpioLastReadVal;                    //Last GPIO value
  uint8_t   GpioFilterCnt;                      //Filter times
  uint8_t   GpioStatusVal;                      //GPIO true logic state
  uint8_t   GpioCarFlag;                        //vehicle is stilled Flag
  uint8_t   GpioSendDataFlag;                   //Send data flag,it is zero if data been sent within asingle logic ,
                                                //and set to one in the next logic
  uint32_t  GpioValidLogicTimeCnt;              //Gpio vaild logic time counter
};                                                                                                                      

typedef struct sGpioStatusDetection  GPIOSTATUSDETECTION, *pGPIOSTATUSDETECTION;


DS_StatusTypeDef    DS_GentleSensorInit(void);
DS_StatusTypeDef    DS_GentleSensorCheck(void);
DS_StatusTypeDef    DS_GentleSensorReleaseCheck(void);


#ifdef __cplusplus
}
#endif
#endif
/***************************************************************END OF FILE****/
