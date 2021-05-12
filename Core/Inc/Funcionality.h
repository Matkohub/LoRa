#include "ftoa.h"
#include "Init.h"

void Struct_init(LoRa* lora, Callback* callback);
void LoRa_init(LoRa* lora);
void Callback_init(Callback* callback);
void Response_callback(LoRa *lora, Callback *callback);
void format(float temperature, float humidity, float tlak, uint32_t co2, LoRa *lora);
void Response_timeout(LoRa *lora);

