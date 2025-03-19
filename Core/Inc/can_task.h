#ifndef TEMP_CAN_H
#define TEMP_CAN_H

#include <stdint.h>
#include "can.h"
#include "cmsis_os2.h"

// CAN ID for temperature data
#define CAN_ID_TEMP 0x123

// CAN task handle
extern osThreadId_t canTaskHandle;

// Initialization function to create and start the CAN task
void CAN_Task_Init(void);

#endif /* TEMP_CAN_H */