#include "main.h"

uint8_t rx_index = 0;
uint8_t rx_data;
uint8_t rx_buffer[100];
uint8_t message = 0;

float temperature, humidity;
float *ptemp = &temperature, *phum = &humidity;
char con[25] = "";
char f_data[100] = "";

uint8_t cnt=0, ok = 0;
char rs[9] = "mac_tx_ok";

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(rx_index == 0)
		for(uint8_t i = 0; i < 50; i++)
			rx_buffer[i] = 0;

	if(huart->Instance == USART2)
	{
		if((rx_data != 13) && (rx_data != 10) )
			rx_buffer[rx_index++] = rx_data;

		if(rx_data == 10)
		{
			for(int i = 0; i < 9; i++)
				if(rx_buffer[i] == rs[i])
					cnt++;
			if(cnt == 9)
				ok++;
			cnt = 0;

			HAL_UART_Transmit(&huart1, rx_buffer, strlen(rx_buffer), HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, "\r\n", strlen("\r\n"), HAL_MAX_DELAY);
			rx_index = 0;
			message = 1;
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
	MX_I2C1_Init();

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);

	//  GetVersion();
	//  while(message!=1);
	//  message = 0;

	JoinAbp();
	while(message!=1);
	message = 0;

	while (1)
	{
		if(message==0)
			r_both_Si7021(phum, ptemp);

		format_temp_hum(temperature, humidity);

		Tx("uncnf", "1", f_data);
		while(message!=1);
		message = 0;

		for(int i=0; i<100; i++)
			f_data[i] = 0;

		if(message == 0)
			HAL_Delay(60000);
	}
}

char* format_temp_hum(float temperature, float humidity)
{
	ftoa(temperature, con, 0);

	strncat(f_data, "A", 1);
	strncat(f_data, "1", 1);
	strncat(f_data, "B", 1);
	strncat(f_data, con, strlen(con));
	strncat(f_data, "A", 1);

	ftoa(humidity, con, 0);

	strncat(f_data, "2", 1);
	strncat(f_data, "B", 1);
	strncat(f_data, con, strlen(con));
	strncat(f_data, "C", 1);
	strncat(f_data, "\r\n", strlen("\r\n"));

	return f_data;
}

void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}















