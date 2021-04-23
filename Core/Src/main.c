#include "main.h"

////////////////////////////	DEFINES			///////////////////////////
//structure defines
LoRa lora, *p;
Callback callback;

//dht variables
uint8_t dht1, dht2;

//Si7021 variables
float temperature, humidity;
float *ptemp = &temperature, *phum = &humidity;

//bmp280 variables
float bmetlak, bmetemp, bmehum;

//port variables
char port[9] = "123456789";
uint8_t q=0;

int main(void)
{
////////////////////////////	INITS			///////////////////////////

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	Struct_init(lora, callback);
	bmp280i();

	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);

	LoRa_system(lora);

	while (1)
	{

////////////////////////////	JOIN ABP		///////////////////////////

		if(lora.joined == 0)
		{
			JoinAbp();
			while(lora.message!=1);
			lora.message = 0;
			lora.joined = 1;
		}

		//empty send buffer
		for(int i=0; i<100; i++)
			lora.f_data[i] = 0;

////////////////////////////	READ SENSORS	///////////////////////////

		bmp280_read_float(&bmp280, &bmetemp, &bmetlak, &bmehum);


		r_both_Si7021(phum, ptemp);

		p = &lora;
		format(temperature, humidity, p);

////////////////////////////	SEND IT			///////////////////////////

		if(q==9)
			q=0;

		//send
		Tx("uncnf", &port[q], lora.f_data);
		while(lora.message!=1);
		lora.message = 0;
		q++;

		//30 min delay
		for(int i=0; i<30; i++)
			HAL_Delay(60000);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//empty receive buffer for new data
	if(lora.rx_index == 0)
		for(uint8_t i = 0; i < 50; i++)
			lora.rx_buffer[i] = 0;

	if(huart->Instance == USART2)
	{
		//fill buffer with new data if it's not NL or CR
		if((lora.rx_data != 13) && (lora.rx_data != 10) )
			lora.rx_buffer[lora.rx_index++] = lora.rx_data;

		//end of received data
		if(lora.rx_data == 10)
		{
			Response_callback(lora, callback);

			HAL_UART_Transmit(&huart1, lora.rx_buffer, lora.rx_index, HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, "\r\n", 2, HAL_MAX_DELAY);

			lora.rx_index = 0;
			lora.message = 1;
		}
	}
	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);
}

void LoRa_system()
{
	GetVersion();
	while(lora.message!=1);
		lora.message = 0;

//	SetDeveui();
//	while(lora.message!=1);
//		lora.message = 0;
//
//	SetDevaddr();
//	while(lora.message!=1);
//		lora.message = 0;
//
//	SetNwkskey();
//	while(lora.message!=1);
//		lora.message = 0;
//
//	SetAppskey();
//	while(lora.message!=1);
//		lora.message = 0;
}


















