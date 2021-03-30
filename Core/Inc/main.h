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
void format_temp_hum(float temperature, float humidity);
void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);
void Response_callback();
void Struct_init();

typedef struct LoRa
{
	char f_data[100];
	uint8_t rx_index;
	uint8_t rx_data;
	uint8_t rx_buffer[100];
	uint8_t message;
	uint8_t joined;
	char x;
	char *p;
} LoRa;

typedef struct Callback
{
	char Tx_ok[9];
	char Joined[10];
	uint8_t cnt;
	uint8_t ok;
	uint8_t cj;
} Callback;

#ifdef __cplusplus
}
#endif

#endif
