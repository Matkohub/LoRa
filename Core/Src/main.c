#include "main.h"

//port variables
char port[9] = "123456789";
uint8_t q=0;

//retarded si
float temperature, humidity;
float *ptemp = &temperature, *phum = &humidity;

int main(void)
{
	Init_functions();

	//start receive interrupt
	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);

//	LoRa_system(lora);

	while (1)
	{
		Wake_UP_STM();
		Read_sensors();
		Send_data();
		Go_to_standby();
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
	Struct_init(lora, callback);
	bmp280i();
	CO2();
}

void LoRa_system()
{
	SetDeveui();

	SetDevaddr();

	SetNwkskey();

	SetAppskey();

	Save();
}

void Wake_UP_STM()
{

	if( __HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		HAL_UART_Transmit(&huart1, "WAKE UP\r\n", strlen("WAKE UP\r\n"), HAL_MAX_DELAY);

		HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
		HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	}
}

void Read_sensors()
{
	//bmp is stupid so needs to read twice
	bmp280_read_float(&bmp280, &bmetemp, &bmetlak, &bmehum);
	HAL_Delay(100);
	bmp280_read_float(&bmp280, &bmetemp, &bmetlak, &bmehum);

	//Si
	r_both_Si7021(phum, ptemp);

	//CO2
	CO2_read();

	p = &lora;
	format(temperature, humidity, bmetlak, EPY.Channel1_CO2, p);
}

void Send_data()
{
	//timer for wakeup
	HAL_TIM_Base_Start(&htim14);

	if(lora.wake_up == 0)
	{
		while((__HAL_TIM_GET_COUNTER(&htim14)<=10000) && (lora.wake_up == 0));

		__HAL_TIM_SET_COUNTER(&htim14, 0);
		HAL_TIM_Base_Stop(&htim14);

		if(lora.wake_up == 1)
			lora.wake_up=0;
	}

	//join abp
//	if(lora.joined == 0)
	{
		JoinAbp();
		HAL_Delay(500);
	}

	//send
	Tx("uncnf", &port[q], lora.f_data);
	//switch channel
	q++;

	//reset channel count
	if(q==9)
		q=0;

	//empty send buffer
	for(int i=0; i<100; i++)
		lora.f_data[i] = 0;

	HAL_Delay(5000);

	//go to sleep for 30 min
	Sleep();
}

void Go_to_standby()
{
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUFI);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF);
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);

	//set to 30 min
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 1798, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_UART_Transmit(&huart1, "STAND BY\r\n", strlen("STAND BY\r\n"), HAL_MAX_DELAY);

	HAL_SuspendTick();
	HAL_PWR_EnterSTANDBYMode();

}



