#include "Funcionality.h"

char response[10];
char con[25] = "";

//Send to LoRa
char* Send(char *buffer)
{
	HAL_UART_Transmit(&huart1, buffer, strlen(buffer), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffer, strlen(buffer), HAL_MAX_DELAY);

	lora.message = 0;
	return response;
}

void Response_callback(LoRa *lora, Callback *callback)
{
	//set flag if ok response
	for(int i = 0; i < 2; i++)
		if(lora->rx_buffer[i] == callback->ok[i])
			callback->cnt++;
	if(callback->cnt == 2)
		lora->flag = 1;

	callback->cnt = 0;

	//setting flag joined based on response
	for(int i = 0; i < 8; i++)
		if(lora->rx_buffer[i] == callback->Joined[i])
			callback->cnt++;
	if(callback->cnt==8)
		lora->flag = 1;

	callback->cnt = 0;

	//setting flag if the message is sent
	for(int i = 0; i < 9; i++)
		if(lora->rx_buffer[i] == callback->Tx_ok[i])
			callback->cnt++;
	if(callback->cnt==9)
		lora->flag = 1;

	callback->cnt = 0;
}

//init structures
void Struct_init(LoRa* lora, Callback* callback)
{
	LoRa_init(lora);
	Callback_init(callback);
}

void LoRa_init(LoRa* lora)
{
	lora->rx_index = 0;
	lora->message = 0;
	lora->flag = 0;
}

void Callback_init(Callback *callback)
{
	callback->cnt = 0;

	strncat(callback->ok, "ok", strlen("ok"));
	strncat(callback->Joined, "accepted", strlen("accepted"));
	strncat(callback->Tx_ok, "mac_tx_ok", strlen("mac_tx_ok"));
}

//make format for sending data
void format(float temperature, float humidity, float tlak, uint32_t co2, LoRa *lora)
{
	ftoa(temperature, con, 2);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "1", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	ftoa(humidity, con, 2);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "2", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	ftoa(tlak, con, 2);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "3", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	ftoa((float)co2, con, 2);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "4", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	strncat(lora->f_data, "C", 1);
}

void Response_timeout(LoRa *lora)
{
	HAL_TIM_Base_Start(&htim14);

	while((__HAL_TIM_GET_COUNTER(&htim14)<=5000) && (lora->flag == 0));

	__HAL_TIM_SET_COUNTER(&htim14, 0);
	HAL_TIM_Base_Stop(&htim14);

	if(lora->flag == 1)
		lora->flag=0;
}

void Response_timeout2(LoRa *lora)
{
	HAL_TIM_Base_Start(&htim14);

	while((__HAL_TIM_GET_COUNTER(&htim14)<=50000) && (lora->flag == 0));

	__HAL_TIM_SET_COUNTER(&htim14, 0);
	HAL_TIM_Base_Stop(&htim14);

	if(lora->flag == 1)
		lora->flag=0;
}









