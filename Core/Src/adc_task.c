#include "adc_task.h"
#include "stm32f4xx_hal.h"

/* Internal ADC task handle */
static osThreadId_t adcTaskHandle = NULL;

/* Global DMA buffer for two NTC channels */
uint16_t adc_dma_buf[ADC_BUF_SIZE] = {0, 0};

/* Thread flag for DMA completion notification */
#define ADC_FLAG_DMA_COMPLETE  (1UL << 0)

/* Private function prototypes */
static void ADC_SamplingTask(void *argument);

/**
  * @brief  Initializes ADC and DMA, then creates the FreeRTOS task that handles
  *         ADC sampling with DMA. 
  */
void ADC_Task_Init(void)
{
    const osThreadAttr_t adcTaskAttributes = {
        .name       = "ADC_Task",
        .priority   = osPriorityHigh,  
        .stack_size = 128
    };

    /* Create the ADC sampling thread */
    adcTaskHandle = osThreadNew(ADC_SamplingTask, NULL, &adcTaskAttributes);
    if (adcTaskHandle == NULL)
    {
        //Failed to create the thread
    }
}

/**
  * @brief FreeRTOS task function that handles ADC sampling via DMA.
  * Starts ADC+DMA to fill adc_dma_buf
  * Waits for notification from DMA ISR
  * Notify another task
  * Delays for next sampling interval
  */
static void ADC_SamplingTask(void *argument)
{
    (void)argument;
    
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(100);

    /*Initialize xLastWakeTime with the current tick count*/
    xLastWakeTime = xTaskGetTickCount();


	for (;;)
    {
        /*Start ADC + DMA transfer to adc_dma_buf */
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buf, ADC_BUF_SIZE);

        /*Wait until DMA completes, signaled by ADC_FLAG_DMA_COMPLETE. */
        osThreadFlagsWait(ADC_FLAG_DMA_COMPLETE, osFlagsWaitAny, osWaitForever);

        /* DMA transfer complete. Notify processing task to handle new data */
        osThreadFlagsSet(canTaskHandle, NEW_DATA_READY);
		
        /*Delay until 100 ms have elapsed since the previous 
         *time this task unblock.
         */
        vTaskDelayUntil(&xLastWakeTime, xPeriod);        
    }
}

/**
  * @brief  Set a thread flag to wake the ADC sampling thread.
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        /* Signal the ADC thread that DMA is complete */
        osThreadFlagsSet(adcTaskHandle, ADC_FLAG_DMA_COMPLETE);
    }
}
