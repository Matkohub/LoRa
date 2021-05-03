#include "stm32g0xx_hal.h"
#include "stdlib.h"
#include <math.h>

#include "LoRa.h"

#include "BME280.h"
#include "EPY12231.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_I2C1_Init(void);
void bmp280i();
void CO2();
void MX_TIM14_Init(void);

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;
BMP280_HandleTypedef bmp280;
TIM_HandleTypeDef htim14;



//bmp280 variables
float bmetlak, bmetemp, bmehum;


