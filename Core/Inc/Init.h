#include "stm32g0xx_hal.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void MX_DMA_Init(void);

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
