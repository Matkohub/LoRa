#include "Funcionality.h"

char con[25] = "";

void Struct_init(LoRa lora, Callback callback)
{
	LoRa_init(lora);
	Callback_init(callback);
}

void LoRa_init(LoRa lora)
{
	lora.rx_index = 0;
	lora.message = 0;
	lora.joined = 0;
}

void Callback_init(Callback callback)
{
	strncat(callback.Tx_ok, "mac_tx_ok", strlen("mac_tx_ok"));
	strncat(callback.Joined, "not_joined", strlen("not_joined"));
	callback.cj=0;
	callback.cnt=0;
	callback.ok=0;
}

void Response_callback(LoRa lora, Callback callback)
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

void format(float temperature, float humidity, LoRa *lora)
{
	ftoa(temperature, con, 0);

	strncat(lora->f_data, "A", 1);
	strncat(lora->f_data, "1", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));
	strncat(lora->f_data, "A", 1);

	ftoa(humidity, con, 0);

	strncat(lora->f_data, "2", 1);
	strncat(lora->f_data, "B", 1);
	strncat(lora->f_data, con, strlen(con));
	strncat(lora->f_data, "C", 1);
}













