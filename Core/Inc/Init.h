#include "stm32g0xx_hal.h"
#include "BME280.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_I2C1_Init(void);
void bmp280i();

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;
BMP280_HandleTypedef bmp280;

