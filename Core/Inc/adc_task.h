#ifndef __ADC_SAMPLING_H
#define __ADC_SAMPLING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"
#include "adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "can_task.h"

/**
  * @brief  Size of the DMA buffer for NTC reading.
  *         We have 2 sensors: NTC1 and NTC2.
  */
#define ADC_BUF_SIZE 2

/**
  * @brief  Global DMA buffer holding raw ADC readings for the two NTC sensors.
  *         Index 0 -> NTC 1
  *         Index 1 -> NTC 2
  */
extern uint16_t adc_dma_buf[ADC_BUF_SIZE];

/**
  * @brief  Creates the FreeRTOS task that handles
  *         ADC sampling with DMA. 
  */
void ADC_Task_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __ADC_SAMPLING_H */
