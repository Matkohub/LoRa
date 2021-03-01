#include "main.h"

char* a;
int tim;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();



  while (1)
  {
	  a = GetVersion();
	  tim++;
	  HAL_Delay(2000);
  }
}

