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

void Error_Handler(void);
char* format_temp_hum(float temperature, float humidity);
void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);



#ifdef __cplusplus
}
#endif

#endif
