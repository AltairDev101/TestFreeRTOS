#include <string.h>
#include "main.h"
#include "can_task.h"
#include "ntc_steinhart.h"
#include "cmsis_os2.h"

// Include STM32 HAL header 
#include "stm32f4xx_hal.h"

// External CAN task handle
osThreadId_t canTaskHandle = NULL;

// External global buffer filled by ADC DMA 
extern uint16_t adc_dma_buf[];

// Forward declaration of the task function
static void CANTaskFunction(void *argument);

// Initialize (enable) the CAN peripheral
static void CAN_HardwareInit(void);

// Send data
static void CAN_Send(uint16_t id, const uint8_t *data, uint8_t len);

/**
  * @brief  Initializes the CAN module.
  * @param  None
  * @retval None
  */
void CAN_Task_Init(void)
{
    //Start the CAN
    if (HAL_CAN_Start(&hcan2) != HAL_OK)
    {
        // Start Error
        while (1);
    }

    // Create the task
    const osThreadAttr_t canTaskAttr = {
        .name       = "CAN_Task",
        .priority   = osPriorityNormal,
        .stack_size = 512
    };

    canTaskHandle = osThreadNew(CANTaskFunction, NULL, &canTaskAttr);
}

/**
  * @brief  Task function: waits for a notification (thread flag),
  *         reads raw ADC data from adc_dma_buf, converts to temperatures,
  *         and sends them over CAN via the static CAN_Send().
  * @param  argument: Not used
  * @retval None
  */
static void CANTaskFunction(void *argument)
{
    (void)argument;

    for (;;)
    {
        // Wait for a notification from adc_task
        osThreadFlagsWait(NEW_DATA_READY, osFlagsWaitAny, osWaitForever);

        // Read raw ADC data from the global DMA buffer
        uint16_t adcValue1 = adc_dma_buf[0];
        uint16_t adcValue2 = adc_dma_buf[1];

        // Convert to temperatures using external Steinhart-Hart function
        float temperature1 = NTC_GetTemperature(adcValue1);
        float temperature2 = NTC_GetTemperature(adcValue2);

        // Prepare CAN data: 2 floats = 8 bytes total
        uint8_t canData[8];
        memcpy(&canData[0], &temperature1, sizeof(float));
        memcpy(&canData[4], &temperature2, sizeof(float));

        // Transmit via CAN
        CAN_Send(CAN_ID_TEMP, canData, 8);
    }
}

/**
<<<<<<< HEAD
=======
  * @brief  Configure and start the CAN peripheral.
  */
static void CAN_HardwareInit(void)
{
    //Configure the CAN 
    //accept all IDs
    CAN_FilterTypeDef canFilterConfig;
    canFilterConfig.FilterActivation = ENABLE;
    canFilterConfig.FilterBank = 0;
    canFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    canFilterConfig.FilterIdHigh = 0x0000;
    canFilterConfig.FilterIdLow = 0x0000;
    canFilterConfig.FilterMaskIdHigh = 0x0000;
    canFilterConfig.FilterMaskIdLow = 0x0000;
    canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

    if (HAL_CAN_ConfigFilter(&hcan2, &canFilterConfig) != HAL_OK)
    {
        // Filter config error
        while (1);
    }


}

/**
>>>>>>> e078477 (Fix Readme)
  * @brief  send data over CAN using STM32 HAL.
  * @param  id   : CAN Standard ID
  * @param  data : pointer to payload
  * @param  len  : length of payload in bytes
  */
static void CAN_Send(uint16_t id, const uint8_t *data, uint8_t len)
{
    // Create a Tx header
    CAN_TxHeaderTypeDef txHeader;
    txHeader.StdId = id;
    txHeader.ExtId = 0;
    txHeader.IDE   = CAN_ID_STD;        // Standard ID
    txHeader.RTR   = CAN_RTR_DATA;      // Data frame
    txHeader.DLC   = len;               // Data length
    txHeader.TransmitGlobalTime = DISABLE;

    // A mailbox for tracking the Tx message
    uint32_t txMailbox = 0;

    // Attempt to add the message to the CAN Tx mailbox
    if (HAL_CAN_AddTxMessage(&hcan2, &txHeader, (uint8_t *)data, &txMailbox) != HAL_OK)
    {
        // Transmission request Error
    }
}
