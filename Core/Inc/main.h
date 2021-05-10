#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Funcionality.h"
#include "Si7021.h"

void Error_Handler(void);
HAL_StatusTypeDef EPY12231_FIFO_Read_Full();
void LoRa_system();

void Init_functions();
void Wake_UP_STM();
void Read_sensors();
void Send_data();
void Go_to_standby();

#ifdef __cplusplus
}
#endif

#endif
