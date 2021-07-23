#include "main.h"
uint8_t cnt=0;

//retarded si
float temperature, humidity;
float *ptemp = &temperature, *phum = &humidity;

////////////////////////////////////////	MAIN	////////////////////////////////////////////////////////////////////

int main(void)
{
	Init_functions();

	//start receive interrupt
	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);

//Change parameters
//	LoRa_system(lora);

	while (1)
	{
//		Wake_UP_STM();
		Read_sensors();
//		Send_data();
//		Go_to_standby();

		HAL_Delay(5000);
	}
}

////////////////////////////////////////	CALLBACK	////////////////////////////////////////////////////////////////

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
			HAL_UART_Transmit(&huart1, lora.rx_buffer, lora.rx_index, HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, "\r\n", 2, HAL_MAX_DELAY);

			Response_callback(l, c);

			lora.rx_index = 0;
			lora.message = 1;
		}
	}
	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);
}

////////////////////////////////////////	INIT	////////////////////////////////////////////////////////////////////

void Init_functions()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_TIM14_Init();
	MX_RTC_Init();

	l = &lora;
	c = &callback;
	Struct_init(l, c);
	bmp280i();
	CO2();

	HAL_Delay(1000);
}

////////////////////////////////////////	LORA SET	/////////////////////////////////////////////////////////////////

void LoRa_system()
{
	SetDeveui();

	SetDevaddr();

	SetNwkskey();

	SetAppskey();

	Save();
}

////////////////////////////////////////	WAKE	////////////////////////////////////////////////////////////////////

void Wake_UP_STM()
{
	//STAND BY MODE//

	if( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		HAL_UART_Transmit(&huart1, "WAKE UP\r\n", strlen("WAKE UP\r\n"), HAL_MAX_DELAY);

		HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
		__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	}

	Response_timeout2(l);


}

////////////////////////////////////////	READ SENS	/////////////////////////////////////////////////////////////////

void Read_sensors()
{cnt++;
	//bmp is stupid so needs to read twice
//	bmp280_read_float(&bmp280, &bmetemp, &bmetlak, &bmehum);
//	HAL_Delay(100);
	bmp280_read_float(&bmp280, &bmetemp, &bmetlak, &bmehum);
	HAL_Delay(100);

	//Si
	r_both_Si7021(phum, ptemp);
	HAL_Delay(100);

	//CO2
	CO2_read();

	format(temperature, humidity, bmetlak, EPY.Channel1_CO2, l);


	HAL_UART_Transmit(&huart1, lora.f_data, strlen(lora.f_data), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, "\r\n", strlen("\r\n"), HAL_MAX_DELAY);
	//empty send buffer
	for(int i=0; i<100; i++)
		lora.f_data[i] = 0;
}

////////////////////////////////////////	SEND	////////////////////////////////////////////////////////////////////

void Send_data()
{
	//join abp
	JoinAbp();

	Response_timeout(l);
	Response_timeout(l);

	//send
	Tx("uncnf", "1", lora.f_data);

	Response_timeout(l);
	Response_timeout(l);

	//empty send buffer
	for(int i=0; i<100; i++)
		lora.f_data[i] = 0;

	HAL_Delay(100);
}

////////////////////////////////////////	STANDBY	////////////////////////////////////////////////////////////////////

void Go_to_standby()
{
//	Lora to sleep for 30 min
	Sleep();

//	STAND BY MODE//

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUFI);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF);
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);

	//set to 30 min
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 1759, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
//	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 290, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
//	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 5, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

	HAL_UART_Transmit(&huart1, "STAND BY\r\n", strlen("STAND BY\r\n"), HAL_MAX_DELAY);

	HAL_SuspendTick();

	HAL_PWR_EnterSTANDBYMode();

}


