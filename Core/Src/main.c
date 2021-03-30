#include "main.h"

LoRa lora;
Callback callback;

//Si7021 variables
float temperature, humidity;
float *ptemp = &temperature, *phum = &humidity;
char con[25] = "";

void Struct_init()
{
	lora.rx_index = 0;
	lora.message = 0;
	lora.joined = 0;
	lora.x='1';
	*lora.p=&lora.x;

	strncat(callback.Tx_ok, "mac_tx_ok", strlen("mac_tx_ok"));
	strncat(callback.Joined, "not_joined", strlen("not_joined"));
	callback.cj=0;
	callback.cnt=0;
	callback.ok=0;
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	Struct_init();

	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);

//	GetVersion();
//	while(lora.message!=1);
//	lora.message = 0;

//	SetDevaddr();
//	SetNwkskey();
//	SetAppskey();


	while (1)
	{
		//joining network
		if(lora.joined == 0)
		{
			JoinAbp();
			while(lora.message!=1);
			lora.message = 0;
			lora.joined = 1;
			HAL_Delay(1000);
		}

		//empty send buffer
		for(int i=0; i<100; i++)
			lora.f_data[i] = 0;

//		r_both_Si7021(phum, ptemp);

		format_temp_hum(temperature, humidity);

		//switch channel
		if(lora.x=='9')
			lora.x='1';

		//send data
		Tx("uncnf", lora.p, lora.f_data);
		while(lora.message!=1);
		lora.message = 0;
		lora.x++;




		for(int i=0; i<30; i++)
			HAL_Delay(60000);
	}
}

void Response_callback()
{
	//counting tx ok responses in ok variable
	for(int i = 0; i < 9; i++)
		if(lora.rx_buffer[i] == callback.Tx_ok[i])
			callback.cnt++;
	if(callback.cnt == 9)
		callback.ok++;
	callback.cnt = 0;

	//setting flag joined to 0 if not connected response
	for(int i = 0; i < 10; i++)
		if(lora.rx_buffer[i] == callback.Joined[i])
			callback.cj++;
	if(callback.cj==10)
		lora.joined = 0;
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
			Response_callback();

			HAL_UART_Transmit(&huart1, lora.rx_buffer, strlen(lora.rx_buffer), HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart1, "\r\n", strlen("\r\n"), HAL_MAX_DELAY);

			lora.rx_index = 0;
			lora.message = 1;
		}
	}

	HAL_UART_Receive_IT(&huart2, &lora.rx_data, 1);
}



void format_temp_hum(float temperature, float humidity)
{
	ftoa(temperature, con, 0);

	strncat(lora.f_data, "A", 1);
	strncat(lora.f_data, "1", 1);
	strncat(lora.f_data, "B", 1);
	strncat(lora.f_data, con, strlen(con));
	strncat(lora.f_data, "A", 1);

	ftoa(humidity, con, 0);

	strncat(lora.f_data, "2", 1);
	strncat(lora.f_data, "B", 1);
	strncat(lora.f_data, con, strlen(con));
	strncat(lora.f_data, "C", 1);
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
    if (afterpoint != 0)
    {
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
    while (x)
    {
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
    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}















