#include "LoRa.h"
#include "Init.h"

char *response;

char* Send(char *buffer)
{
	for(uint8_t i = 0; i < strlen(buffer); i++)
		HAL_UART_Transmit(&huart2, &buffer[i], 1, HAL_MAX_DELAY);

//	HAL_UART_Transmit(&huart2, buffer, strlen(buffer), HAL_MAX_DELAY);

	HAL_UART_Receive(&huart2, response, strlen(response), HAL_MAX_DELAY);
	return response;
}

//char* Receive()
//{
//	char *buffer;//velièina buffera?
//	HAL_UART_Receive(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);
//	return buffer;
//}

//Set OTAA keys

//<appKey>: 16-byte hexadecimal number representing the application key
//Response: ok if address is valid invalid_param if address is not valid
//This command sets the application key for the module. The application
//key is used to identify a grouping over module units which perform the
//same or similar task.
//Example: mac set appkey 00112233445566778899AABBCCDDEEFF

char*	SetAppkey		()
{
	return Send(appkey);
}
//char*	SetAppkey		(uint64_t key1, uint64_t key2)
//{
//	return Send((char*)(key1+key2));
//}

//<devEUI>: 8-byte hexadecimal number representing the device EUI
//Response: ok if address is valid invalid_param if address is not valid
//This command sets the globally unique device identifier for the module.
//The identifier must be set by the host MCU. The module contains a
//pre-programmed unique EUI and can be retrieved using the sys get hweui
//command (see Section 2.3.6.4) or user provided EUI can be configured
//using the mac set deveui command.
//Example: mac set deveui 0004A30B001A55ED

char*	SetDeveui		()
{
	return Send(deveui);
}
//char*	SetDeveui		(uint64_t key)
//{
//	return Send((char*)key);
//}

//<appEUI>: 8-byte hexadecimal number representing the application
//EUI Response: ok if address is valid invalid_param if address is not valid
//This command sets the application identifier for the module.
//The application identifier should be used to identify device types
//(sensor device, lighting device, etc.) within the network.
//Example: mac set appeui FEDCBA9876543210

char*	SetAppeui		()
{
	return Send(appeui);
}
//char*	SetAppeui		(uint64_t key)
//{
//	return Send((char*)key);
//}


//Set ABP keys

//<address>: 4-byte hexadecimal number representing the device address, from 00000000 – FFFFFFFF
//Response: ok if address is valid
//invalid_param if address is not valid
//This command configures the module with a 4-byte unique network device address
//<address>. The <address> MUST be UNIQUE to the current network. This must be directly set solely for activation by personalization devices. This parameter must not be set before attempting to join using over-the-air activation because it will be overwritten once the join process is over.
//Example: mac set devaddr ABCDEF01

char*	SetDevaddr		()
{
	return Send(devaddr);
}
//char*	SetDevaddr		(uint32_t key)
//{
//	return Send((char*)key);
//}

//<nwkSessKey>: 16-byte hexadecimal number representing the network session key Response: ok if address is valid
//invalid_param if address is not valid
//This command sets the network session key for the module. This key is 16 bytes in length, and should be modified with each session between the module and network. The key should remain the same until the communication session between devices is terminated.
//Example: mac set nwkskey 1029384756AFBECD5647382910DACFEB

char*	SetNwkskey		()
{
	return Send(nwkskey);
}
//char*	SetNwkskey		(uint64_t key1, uint64_t key2)
//{
//	return Send((char*)(key1+key2));
//}

//<appSessKey>: 16-byte hexadecimal number representing the application session key
//Response: ok if address is valid
//invalid_param if address is not valid
//This command sets the application session key for the module. This key is unique, created for each occurrence of communication, when the network requests an action taken by the application.
//Example: mac set appskey AFBECD56473829100192837465FAEBDC

char*	SetAppskey		()
{
	return Send(appskey);
}
//char*	SetAppskey		(uint64_t key1, uint64_t key2)
//{
//	return Send((char*)(key1+key2));
//}


//System funcions	/////////////////////////////////////////////

//<length>: decimal number representing the number of milliseconds the system is put to Sleep, from 100 to 4294967296.
//Response: ok after the system gets back from Sleep mode
//invalid_param if the length is not valid
//This command puts the system to Sleep for the specified number of milliseconds. The module can be forced to exit from Sleep by sending a break condition followed by a 0x55 character at the new baud rate. Note that the break condition needs to be long enough not to be interpreted as a valid character at the current baud rate.
//Example: sys sleep 120	// Puts the system to Sleep for 120 ms.

char*	Sleep			(int lenght)
{
	return Send(SLEEP);
}

//Response: RN2483 X.Y.Z MMM DD YYYY HH:MM:SS, where X.Y.Z is firmware version, MMM is month, DD is day, HH:MM:SS is hour, minutes, seconds (format: [HW] [FW] [Date] [Time]). [Date] and [Time] refer to the release of the firmware.
//This command resets and restarts the RN2483 module; stored internal configurations will be loaded automatically upon reboot.
//Example: sys reset	// Resets and restarts the RN2483 module.

char*	Reset			()
{
	return Send(RESET);
}

//Response: no response
//This command deletes the current RN2483 module application firmware and prepares it for firmware upgrade. The RN2483 module bootloader is ready to receive new firmware.
//Example: sys eraseFW	// Deletes the current RN2483 module
//application firmware.

char*	Erasefw			()
{
	return Send(ERASE_FW);
}

//Response: RN2483 X.Y.Z MMM DD YYYY HH:MM:SS, where X.Y.Z is firmware version, MMM is month, DD is day, HH:MM:SS is hour, minutes, seconds (format: [HW] [FW] [Date] [Time]). [Date] and [Time] refer to the release of the firmware.
//This command resets the module’s configuration data and user EEPPROM to factory default values and restarts the module. After factoryRESET, the RN2483 module will automatically reset and all configuration parameters are restored to factory default values.
//Example: sys factoryRESET	// Restores factory default values.

char*	FactoryReset	()
{
	return Send(FACTORY_RESET);
}

//<address>: hexadecimal number representing user EEPROM address, from 300 to 3FF
//<data>:	hexadecimal number representing data, from 00 to FF Response:	ok if the parameters (address and data) are valid
//invalid_param if the parameters (address and data) are not valid
//This command allows the user to modify the user EEPROM at <address> with the value supplied by <data>. Both <address> and <data> must be entered as hex values. The user EEPROM memory is located inside the MCU on the module.
//Example: sys set nvm 300 A5	// Stores the value 0xA5 at user EEPROM
//address 0x300.

char*	SetNvm			(uint16_t address, uint8_t data)
{
	return Send(SET_MEM);
}

//<pinname>:	string representing the pin. Parameter values can be:
//GPIO0 - GPIO14, UART_CTS, UART_RTS, TEST0, TEST1
//<pinstate>: decimal number representing the state. Parameter values can be: 0 or 1.
//Response: ok if the parameters (<pinname>, <pinstate>) are valid
//invalid_param if the parameters (<pinname>, <pinstate>) are not valid
//This command allows the user to modify the unused pins available for use by the module. The selected <pinname> is driven high or low depending on the desired
//<pinstate>.
//Default: GPIO0-GPIO14, UART_CTS, UART_RTS, TEST0 and TEST1 are driven low (value 0).
//Example: sys set pindig GPIO5 1	// Drives GPIO5 high 1, VDD.

char*	SetPins			(char* pinname, uint8_t pinstate)
{
	return Send(SET_PINS);
}

//Response: RN2483 X.Y.Z MMM DD YYYY HH:MM:SS, where X.Y.Z is firmware version, MMM is month, DD is day, HH:MM:SS is hour, minutes, seconds (format: [HW] [FW] [Date] [Time]). [Date] and [Time] refer to the release of the firmware.
//This command returns the information related to the hardware platform, firmware version, release date and time stamp on firmware creation.
//Example: sys get ver	// Returns version-related information.

char*	GetVersion		()
{
	return Send(GET_VER);
}

//<address>: hexadecimal number representing user EEPROM address, from 300 to 3FF
//Response:	00 – FF (hexadecimal value from 00 to FF) if the address is valid
//invalid_param if the address is not valid
//This command returns the data stored in the user EEPROM of the RN2483 module at the requested <address> location.
//Example: sys get nvm 300	// Returns the 8-bit hex value stored at 300.

char*	GetNvm			(uint16_t address)
{
	return Send(GET_NVM);
}

//Response: 0–3600 (decimal value from 0 to 3600)
//This command informs the RN2483 module to do an ADC conversion on the VDD. The measurement is converted and returned as a voltage (mV).
//Example: sys get vdd	// Returns mV measured on the VDD module.

char*	GetVdd			()
{
	return Send(GET_VDD);
}

//Response: hexadecimal number representing the preprogrammed EUI node address
//This command reads the preprogrammed EUI node address from the RN2483 module. The value returned by this command is a globally unique number provided by Microchip.
//Example: sys get hweui		// Reads the preprogrammed EUI node address.

char*	GetHweui		()
{
	return Send(GET_HWEUI);
}



//mac functions		////////////////////////////////////////////

//<band>: decimal number representing the frequency band, either 868 or 433 Response: ok if band is valid
//invalid_param if band is not valid
//This command will automatically reset the software LoRaWAN stack and initialize it with the parameters for the selected band.
//Example: mac reset 868	// Sets the default values and selects the 868
//default band.

char*	ResetFreq868	()
{
	return Send(RES868);
}
char*	ResetFreq433	()
{
	return Send(RES433);
}

//<type>:	string representing the uplink payload type, either cnf or uncnf
//(cnf – confirmed, uncnf – unconfirmed)
//<portno>: decimal number representing the port number, from 1 to 223
//<data>:	hexadecimal value. The length of <data> bytes capable of being transmitted are dependent upon the set data rate (please refer to the LoRaWAN™ Specification for further details).
//Response: this command may reply with two responses. The first response will be received immediately after entering the command. In case the command is valid (ok reply received), a second reply will be received after the end of the uplink transmission. Please refer to the LoRaWAN™ Specification for further details.
//Response after entering the command:
//•	ok – if parameters and configurations are valid and the packet was forwarded to the radio transceiver for transmission
//•	invalid_param – if parameters (<type> <portno> <data>) are not valid
//•	not_joined – if the network is not joined
//•	no_free_ch – if all channels are busy
//•	silent – if the module is in a Silent Immediately state
//•	frame_counter_err_rejoin_needed – if the frame counter rolled over
//•	busy – if MAC state is not in an Idle state
//•	mac_paused – if MAC was paused and not resumed back
//•	invalid_data_len if application payload length is greater than the maximum application payload length corresponding to the current data rate
//Response after the uplink transmission:
//•	mac_tx_ok if uplink transmission was successful and no downlink data was received back from the server;
//•	mac_rx <portno> <data> if transmission was successful, <portno>: port number, from 1 to 223; <data>: hexadecimal value that was received from the server;
//•	mac_err if transmission was unsuccessful, ACK not received back from the server
//•	invalid_data_len if application payload length is greater than the maximum application payload length corresponding to the current data rate
//A confirmed message will expect an acknowledgment from the server; otherwise, the message will be retransmitted by the number indicated by the command mac set retx <value>, whereas an unconfirmed message will not expect any acknowledgment back from the server. Please refer to the LoRaWAN™ Specification for further details.
//
//
//If the automatic reply feature is enabled and the server sets the Frame Pending bit or initiates downlink confirmed transmissions, multiple responses will be displayed after each downlink packet is received by the module. A typical scenario for this case would be (prerequisites: free LoRaWAN channels available and automatic reply enabled):
//•	The module sends a packet on port 4 with application payload 0xAB
//•	Radio transmission is successful and the module will display the first response:
//ok
//•	The server needs to send two separate downlink confirmed packets back on port 1 with the following data: 0xAC, then 0xAF. First it will transmit the first one (0xAC) and will set the Frame Pending bit. The module will display the second response mac_rx 1 AC
//•	The module will initiate an automatic uplink unconfirmed transmission with no application payload on the first free channel because the Frame Pending bit was set in the downlink transmission
//•	The server will send back the second confirmed packet (0xAF). The module will display a third response mac_rx 1 AF
//•	The module will initiate an automatic unconfirmed transmission with no application payload on the first free channel because the last downlink transmission was confirmed, so the server needs an ACK
//•	If no reply is received back from the server, the module will display the fourth response after the end of the second Receive window: mac_tx_ok
//•	After this scenario, the user is allowed to send packets when at least one enabled channel is free
//Based on this scenario, the following responses will be displayed by the module:
//•	mac tx cnf 4 AB
//•	ok
//•	mac_rx 1 AC
//•	mac_rx 1 AF
//•	mac_tx_ok
//Example: mac tx cnf 4 5A5B5B	// Sends a confirmed frame on port 4 with
//application payload 5A5B5B.

char*	Tx				(char* type, uint8_t portno, uint16_t data)
{
	return Send(TX);
}

//<mode>:		string representing the join procedure type (case-insensitive), either otaa or abp (otaa – over-the-air activation, abp – activation by personalization).
//Response: this command may reply with two responses. The first response will be received immediately after entering the command. In case the command is valid (ok reply received) a second reply will be received after the end of the join procedure.
//Please refer to the LoRaWAN™ Specification for further details. Response after entering the command:
//•	ok – if parameters and configurations are valid and the join request packet was forwarded to the radio transceiver for transmission
//•	invalid_param – if <mode> is not valid
//•	keys_not_init – if the keys corresponding to the Join mode (otaa or abp) were not configured
//•	no_free_ch – if all channels are busy
//•	silent – if the device is in a Silent Immediately state
//•	busy – if MAC state is not in an Idle state
//•	mac_paused – if MAC was paused and not resumed back Response after the join procedure:
//•	denied if the join procedure was unsuccessful (the module attempted to join the network, but was rejected);
//•	accepted if the join procedure was successful;
//This command informs the RN2483 module it should attempt to join the configured network. Module activation type is selected with <mode>. Parameter values can be otaa (over-the-air activation) or abp (activation by personalization). The <mode> parameter is not case sensitive. Before joining the network, the specific parameters for each activation type should be configured (for over the air activation: device EUI, application EUI, application key; for activation by personalization: device address, network session key, application session key).
//Example: mac join otaa	// Attempts to join the network using
//over-the-air activation.

char*	JoinOta			()
{
	return Send(JOIN_OTA);
}
char*	JoinAbp			()
{
	return Send(JOIN_ABP);
}

//Response: ok
//The mac save command must be issued after configuration parameters have been appropriately entered from the mac set <cmd> commands. This command will save LoRaWAN Class A protocol configuration parameters to the user EEPROM. When the next sys reset command is issued, the LoRaWAN Class A protocol configuration will be initialized with the last saved parameters.
//The LoRaWAN Class A protocol configuration savable parameters are:
//•	band: Band
//•	deveui: End-Device Identifier
//•	appeui: Application Identifier
//•	appkey: Application Key
//•	nwkskey: Network Session Key
//•	appskey: Application Session Key
//•	devaddr: End Device Address
//•	ch: All Channel Parameter
//-	freq: Frequency
//-	dcycle: Duty Cycle
//-	drrange: Data Rate Range
//-	status: Status
//Example: mac save		// Saves the LoRaWAN Class A protocol configuration parameters to the user EEPROM.

char*	Save			()
{
	return Send(SAVE);
}

//Response: ok
//The network can issue a certain command (Duty Cycle Request frame with parameter
//255) that would require the RN2483 module to go silent immediately. This mechanism disables any further communication of the module, effectively isolating it from the network. Using mac forceENABLE, after this network command has been received, restores the module’s connectivity by allowing it to send data.
//Example: mac forceENABLE	// Disables the Silent Immediately state.

char*	Force			()
{
	return Send(FORCE);
}

//Response: 0 – 4294967295 (decimal number representing the number of milliseconds the mac can be paused)
//This command pauses the LoRaWAN stack functionality to allow transceiver (radio) configuration. Through the use of mac pause, radio commands can be generated between a LoRaWAN Class A protocol uplink application (mac tx command), and the LoRaWAN Class A protocol Receive windows (second response for the mac tx command). This command will reply with the time interval in milliseconds that the transceiver can be used without affecting the LoRaWAN functionality. The maximum value (4294967295) is returned whenever the LoRaWAN stack functionality is in Idle state and the transceiver can be used without restrictions. ‘0’ is returned when the LoRaWAN stack functionality cannot be paused. After the radio configuration is complete, the mac resume command should be used to return to LoRaWAN Class A protocol commands.
//Example: mac pause	// Pauses the LoRaWAN stack functionality if the response is different from 0.

char*	Pause			()
{
	return Send(PAUSE);
}

//Response: ok
//This command resumes LoRaWAN stack functionality, in order to continue normal functionality after being paused.
//Example: mac resume	// Resumes the LoRaWAN stack functionality.

char*	Resume			()
{
	return Send(RESUME);
}

//<pwrIndex>: decimal number representing the index value for the output power, from 0 to 5 for 433 MHz frequency band and from 1 to 5 for 868 MHz frequency band.
//Response: ok if power index is valid
//invalid_param if power index is not valid
//This command sets the output power to be used on the next transmissions. Refer to the LoRaWAN™ Specification for the output power corresponding to the <pwrIndex> and also to the RN2483 Low-Power Long-Range LoRa™ Technology Transceiver Module Data Sheet (DS50002346) for the actual radio power capabilities.
//Example: mac set pwridx 1	// Sets the TX output power to 14 dBm on the
//next transmission for a 868 MHz EU module.

char*	SetPwridx		(uint8_t pwrindex)
{
	return Send(SET_PWRIDX);
}

//<dataRate>: decimal number representing the data rate, from 0 and 7, but within the limits of the data rate range for the defined channels.
//Response: ok if data rate is valid
//invalid_param if data rate is not valid
//This command sets the data rate to be used for the next transmission. Please refer to the LoRaWAN™ Specification for the description of data rates and the corresponding spreading factors.
//Example: mac set dr 5	// On EU863-870; SF7/125 kHz.

char*	SetDatarate		(uint8_t datarate)
{
	return Send(SET_DATARATE);
}

//<state>: string value representing the state, either on or off.
//Response: ok if state is valid
//invalid_param if state is not valid
//This command sets if the adaptive data rate (ADR) is to be enabled, or disabled. The server is informed about the status of the module’s ADR in every uplink frame it receives from the ADR field in uplink data packet. If ADR is enabled, the server will optimize the data rate and the transmission power of the module based on the information collected from the network.
//Example: mac set adr on	// This will enable the ADR mechanism.

char*	SetAdrOn		()
{
	return Send(SET_ADR_ON);
}
char*	SetAdrOff		()
{
	return Send(SET_ADR_OFF);
}

//<level>: decimal number representing the level of the battery, from 0 to 255. 0 means external power, 1 means low level, 254 means high level, 255 means the end device was not able to measure the battery level.
//Response: ok if the battery level is valid
//invalid_param if the battery level is not valid
//This command sets the battery level required for Device Status Answer frame in use with the LoRaWAN Class A protocol.
//Example: mac set bat 127	// Battery is set to ~50%.

char*	SetBat			(uint8_t level)
{
	return Send(SET_BAT);
}

//<reTxNb>: decimal number representing the number of retransmissions for an uplink confirmed packet, from 0 to 255.
//Response: ok if <retx> is valid
//invalid_param if <retx> is not valid
//This command sets the number of retransmissions to be used for an uplink confirmed packet, if no downlink acknowledgment is received from the server.
//Example: mac set retx 5	// The number of retransmissions made
//for an uplink confirmed packet is set to 5.

char*	SetRetx			(uint8_t retxnb)
{
	return Send(SET_RETX);
}

//<linkCheck>: decimal number that sets the time interval in seconds for the link check process, from 0 to 65535
//Response: ok if the time interval is valid
//invalid_param if the time interval is not valid
//This command sets the time interval for the link check process to be triggered periodically. A <value> of ‘0’ will disable the link check process. When the time interval expires, the next application packet that will be sent to the server will include also a link check MAC command. Please refer to the LoRaWAN™ Specification for more information on the Link Check MAC command.
//Example: mac set linkchk 600	// The module will attempt a link check
//process at 600-second intervals.

char*	SetLinkchk		(uint64_t linkcheck)
{
	return Send(SET_LINKCHK);
}

//<state>: string value representing the state, either on or off.
//Response: ok if state is valid
//invalid_param if state is not valid
//This command sets the state of the automatic reply. By enabling the automatic reply, the module will transmit a packet without a payload immediately after a confirmed downlink is received, or when the Frame Pending bit has been set by the server. If set to OFF, no automatic reply will be transmitted.
//Example: mac set ar on	// Enables the automatic reply process
//inside the module.

char*	SetArOn			()
{
	return Send(SET_AR_ON);
}
char*	SetArOff		()
{
	return Send(SET_AR_OFF);
}

//<dataRate>: decimal number representing the data rate, from 0 to 7.
//<frequency>: decimal number representing the frequency, from 863000000 to 870000000 or from 433050000 to 434790000, in Hz.
//Response: ok if parameters are valid
//invalid_param if parameters are not valid
//This command sets the data rate and frequency used for the second Receive window. The configuration of the Receive window parameters should be in concordance with the server configuration.
//Example: mac set rx2 3 865000000	// Receive window 2 is configured with
//SF9/125 kHz data rate with a center frequency of 865 MHz.

char*	SetRx2			(uint8_t datarate, float freq)//dodaj još 3 nule na freq
{
	return Send(SET_RX2);
}

//<channelId>: decimal number representing the channel number, from 3 to 15.
//<frequency>: decimal number representing the frequency, from 863000000 to 870000000 or from 433050000 to 434790000, in Hz.
//Response: ok if parameters are valid
//invalid_param if parameters are not valid
//This command sets the operational frequency on the given channel ID. The default channels (0-2) cannot be modified in terms of frequency.
//Example: mac set ch freq 13 864000000	// Define frequency for channel
//13 to be 864 MHz.

char*	SetChFreq		(uint8_t channelid, float freq)//dodaj još 3 nule na freq
{
	return Send(SET_FREQ);
}

//<channelId>: decimal number representing the channel number, from 0 to 15.
//<dutyCycle>: decimal number representing the duty cycle, from 0 to 65535.
//Response: ok if parameters are valid
//invalid_param if parameters are not valid
//This command sets the duty cycle used on the given channel ID on the module. The
//<dutyCycle> value that needs to be configured can be obtained from the actual duty cycle X (in percentage) using the following formula: <dutyCycle> = (100/X) – 1. The default settings consider only the three default channels (0-2), and their default duty cycle is 0.33%. If a new channel is created either by the server or by the user, all the channels (including the default ones) must be updated by the user in terms of duty cycle to comply with the ETSI regulations.
//Example: mac set ch dcycle 13 9	// Defines duty cycle for channel 13 to be
//10%. Since (100/10) – 1 = 9, the parameter that gets configured is 9.

char*	SetDcycle		(uint8_t channelid, uint64_t dutycycle)
{
	return Send(SET_DCYCLE);
}


//Response: 4-byte hexadecimal number representing the device address, from 00000000 to FFFFFFFF.
//This command will return the current end-device address of the module. Default: 00000000
//Example: mac get devaddr

char* 	GetDevaddr		()
{
	return Send(GET_DEVADDR);
}

//Response: 8-byte hexadecimal number representing the device EUI.
//This command returns the globally unique end-device identifier, as set in the module. Default: 0000000000000000
//Example: mac get deveui

char*	GetDeveui		()
{
	return Send(GET_DEVEUI);
}

//Response: 8-byte hexadecimal number representing the application EUI.
//This command will return the application identifier for the module. The application identifier is a value given to the device by the network.
//Default: 0000000000000000
//Example:	mac get appeui

char*	GetAppeui		()
{
	return Send(GET_APPEUI);
}

//Response: decimal number representing the current data rate. This command will return the current data rate.
//Default: 5
//Example: mac get dr

char*	GetDataRate		()
{
	return Send(GET_DATA_RATE);
}

//Response: decimal number representing the frequency band, either 868 or 433.
//This command returns the current frequency band of operation. The band reflects the module’s operation types.
//Default: 868
//Example: mac get band

char*	GetBand			()
{
	return Send(GET_BAND);
}

//Response: decimal number representing the current output power index value, from 0 to 5. This command returns the current output power index value.
//Default: 1
//Example: mac get pwridx

char*	GetPwridx		()
{
	return Send(GET_PWRIDX);
}

//Response: string representing the state of the adaptive data rate mechanism, either
//on or off.
//This command will return the state of the adaptive data rate mechanism. It will reflect if the ADR is on or off on the requested device.
//Default: off
//Example: mac get adr

char*	GetAdr			()
{
	return Send(GET_ADR);
}

//Response: decimal number representing the number of retransmissions, from 0 to 255.
//This command will return the currently configured number of retransmissions which are attempted for a confirmed uplink communication when no downlink response has been received.
//Default: 7
//Example: mac get retx

char*	GetRetx			()
{
	return Send(GET_RETX);
}

//Response: string representing the state of the automatic reply, either on or off.
//This command will return the current state for the automatic reply (AR) parameter. The response will indicate if the AR is on or off.
//Default: off
//Example: mac get ar

char*	GetAr			()
{
	return Send(GET_AR);
}

//<freqBand>: decimal number representing the frequency band, either 868 or 433.
//Response: decimal number representing the data rate configured for the second Receive window, from 0 to 7 and a decimal number for the frequency configured for the second Receive window, from 863000000 to 870000000 or from 433050000 to 434790000, in Hz.
//This command will return the current data rate and frequency configured to be used during the second Receive window.
//Default:	3 869525000	// for 868 band
//3 434665000	// for 433 band Example: mac get rx2 868

char*	GetRx2			()
{
	return Send(GET_RX2);
}

//Response: decimal number representing the prescaler value, from 0 to 65535.
//This command returns the duty cycle prescaler. The value of the prescaler can be configured ONLY by the SERVER through use of the Duty Cycle Request frame. Upon reception of this command from the server, the duty cycle prescaler is changed for all enabled channels.
//Default: 1
//Example: mac get dcycleps

char*	GetDcycleps		()
{
	return Send(GET_DCYCLEPS);
}

//Response: decimal number representing the demodulation margin, from 0 to 255.
//This command will return the demodulation margin as received in the last Link Check Answer frame. Please refer to the LoRaWAN™ Specification for the description of the values.
//Default: 255
//Example: mac get mrgn

char*	GetMrgn			()
{
	return Send(GET_MRGN);
}

//Response: decimal number representing the number of gateways, from 0 to 255.
//This command will return the number of gateways that successfully received the last Link Check Request frame command, as received in the last Link Check Answer.
//Default: 0
//Example: mac get gwnb

char*	GetGwnb			()
{
	return SendGET_GWNB(GET_GWNB);
}

//Response: 2-byte hexadecimal number representing the current status of the module.
//This command will return the current status of the module. The value returned is a bit mask represented in hexadecimal form. Please refer to Figure 2-1 for the significance of the bit mask.
//Default: 0000
//Example: mac get status

char*	GetStatus		()
{
	return Send(GET_STATUS);
}

//<channelId>: decimal number representing the channel number, from 0 to 15.
//Response: decimal number representing the frequency of the channel, from 863000000 to 870000000 or from 433050000 to 434790000, in Hz, depending on the frequency band selected.
//This command returns the frequency on the requested <channelId>, entered in decimal form.
//Default: see Table 2-10 Example: mac get ch freq 0

char*	GetChFreq		(uint8_t channelid)
{
	return Send(GET_CH_FREQ);
}

//<channelId>: decimal number representing the channel number, from 0 to 15.
//Response: decimal number representing the duty cycle of the channel, from 0 to 65535.
//This command returns the duty cycle on the requested <channelId>. The duty cycle is returned in decimal value. The actual duty cycle (in percentage) can be obtained using the returned value V as: percent = 100/(V + 1).
//Default: see Table 2-10
//Example: mac get ch dcycle 0	// Reads back duty cycle setting on Channel
//ID 0. If the value reported back is 99, the actual duty cycle on the channel (in percentage) is 100/(99 + 1) = 1.

char*	GetDcycle		(uint8_t channelid)
{
	return Send(GET_DCYCLE);
}









