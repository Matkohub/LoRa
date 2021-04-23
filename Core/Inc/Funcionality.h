#include "stm32g0xx_hal.h"
#include "ftoa.h"

typedef struct LoRa
{
	char f_data[100];
	uint8_t rx_index;
	uint8_t rx_data;
	uint8_t rx_buffer[100];
	uint8_t message;
	uint8_t joined;
} LoRa;

typedef struct Callback
{
	char Tx_ok[9];
	char Joined[10];
	uint8_t cnt;
	uint8_t ok;
	uint8_t cj;
} Callback;

void Struct_init(LoRa lora, Callback callback);
void LoRa_init(LoRa lora);
void Callback_init(Callback callback);
void Response_callback(LoRa lora, Callback callback);
void format(float temperature, float humidity, LoRa *lora);


