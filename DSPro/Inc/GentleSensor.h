/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GENTLESENSOR_H
#define __GENTLESENSOR_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
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
  uint8_t   GpioCurrentReadVal;
  uint8_t   GpioLastReadVal;
  uint8_t   GpioFilterCnt;
  uint8_t   GpioStatusVal;
  uint8_t   GpioCarFlag;
};

typedef struct sGpioStatusDetection  GPIOSTATUSDETECTION, *pGPIOSTATUSDETECTION;

#ifdef __cplusplus
}
#endif
#endif
/***************************************************************END OF FILE****/
