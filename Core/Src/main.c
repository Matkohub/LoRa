#include "main.h"

uint8_t rx_index = 0;
uint8_t rx_data;
uint8_t rx_buffer[50];
uint8_t message = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rx_index == 0)
		for(uint8_t i = 0; i < 50; i++)
			rx_buffer[i] = 0;

	if(huart->Instance == USART2)
	{
		if(rx_data == 10)
			message = 1;

		if((rx_data != 13) && (rx_data != 10) )
			rx_buffer[rx_index++] = rx_data;
		else
		{
			rx_index = 0;

			HAL_UART_Transmit(&huart1, rx_buffer, strlen(rx_buffer), HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, "\r\n", strlen("\r\n"), HAL_MAX_DELAY);
		}
	}

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  HAL_UART_Receive_IT(&huart2, &rx_data, 1);

  GetVersion();
  while(message!=1);
  message = 0;

//  SetAppkey();
//  while(message!=1);
//  message = 0;
//
//  SetDeveui();
//  while(message!=1);
//  message = 0;
//
//  SetAppeui();
//  while(message!=1);
//  message = 0;
//
//  SetDevaddr();
//  while(message!=1);
//  message = 0;
//
//  SetNwkskey();
//  while(message!=1);
//  message = 0;
//
//  SetAppskey();
//  while(message!=1);
//  message = 0;

  JoinAbp();
  while(message!=1);
  message = 0;

  HAL_Delay(100);

  Tx("uncnf", "1", "5D");
  while(message!=1);
  message = 0;

  while (1)
  {


	  HAL_Delay(3000);
  }
}


