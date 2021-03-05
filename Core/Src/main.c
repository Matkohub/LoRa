#include "main.h"

int tim;
char *rec;
char a = 0, b, c;
uint8_t rx_index = 0;
uint8_t rx_data;
uint8_t rx_buffer[50];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rx_index == 0)
		for(uint8_t i = 0; i < 50; i++)
			rx_buffer[i] = 0;

	if(huart->Instance == USART2)
	{
		if((rx_data != 13) && (rx_data != 10) )
			rx_buffer[rx_index++] = rx_data;
		else
		{
			HAL_UART_Transmit(&huart2, rx_buffer, strlen(rx_buffer), HAL_MAX_DELAY);
			rx_index = 0;
		}
	}

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();

//  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);

  while (1)
  {
//	  GetVersion();


	  HAL_Delay(1000);
  }
}


