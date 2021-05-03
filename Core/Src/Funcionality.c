#include "Funcionality.h"

char response[100];
char con[25] = "";
uint8_t cnt=0;

//Send to LoRa
char* Send(char *buffer)
{
	HAL_UART_Transmit(&huart1, buffer, strlen(buffer), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, buffer, strlen(buffer), HAL_MAX_DELAY);

	//timer for message timeout
	HAL_TIM_Base_Start(&htim14);

	//message timeout	//will resend once if timeout expires
	while(lora.message!=1)
		if(__HAL_TIM_GET_COUNTER(&htim14)>=1000)
		{
			HAL_TIM_Base_Stop(&htim14);
			if(cnt == 0)
			{
				cnt++;
				Send(buffer);
			}
			else
			{
				cnt = 0;
				break;
			}
		}

	lora.message = 0;

	return response;
}

void Response_callback(LoRa lora, Callback callback)
{
	//counting tx ok responses in ok variable
	for(int i = 0; i < 9; i++)
		if(lora.rx_buffer[i] == callback.Tx_ok[i])
			callback.cnt++;
	if(callback.cnt == 9)
		callback.ok++;
	callback.cnt = 0;

	//setting flag joined based on response
	for(int i = 0; i < 10; i++)
		if(lora.rx_buffer[i] == callback.Joined[i])
			callback.cj++;
	if(callback.cj==10)
		lora.joined = 0;
	else
		lora.joined = 1;
}

//init structures
void Struct_init(LoRa lora, Callback callback)
{
	LoRa_init(lora);
	Callback_init(callback);
}

void LoRa_init(LoRa lora)
{
	lora.rx_index = 0;
	lora.message = 0;
	lora.joined = 0;
}

void Callback_init(Callback callback)
{
	strncat(callback.Tx_ok, "mac_tx_ok", strlen("mac_tx_ok"));
	strncat(callback.Joined, "not_joined", strlen("not_joined"));
	callback.cj=0;
	callback.cnt=0;
	callback.ok=0;
}

//make format for sending data
void format(float temperature, float humidity, float tlak, uint32_t co2, LoRa *lora)
{
	ftoa(temperature, con, 0);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "1", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	ftoa(humidity, con, 0);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "2", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	ftoa(tlak, con, 0);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "3", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	ftoa((float)co2, con, 0);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "3", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));

	strncat(lora->f_data, "C", 1);
}













