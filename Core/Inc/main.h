#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g0xx_hal.h"
#include "stdlib.h"
#include "Init.h"
#include "LoRa.h"
#include "Si7021.h"
#include "BME280.h"
#include <math.h>
#include "Funcionality.h"

void Error_Handler(void);

void LoRa_system();

#ifdef __cplusplus
}
#endif

#endif
