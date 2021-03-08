#include "stm32g0xx_hal.h"

#define OTA            "otaa"
#define ABP            "abp"

//OTAA keys
#define appkey 			"8A465B4FC57F03AA95FFCC662D9D1C44\r\n"
#define deveui 			"BE7A00000000301C\r\n"
#define appeui 			"BE7A0000000023CE\r\n"

//ABP keys
#define devaddr 		"30C5A958\r\n"
#define nwkskey 		"EA9F51B388F7509101C870CEABAB44CA\r\n"
#define appskey 		"6DCE01412167FF3EF0DF822910232630\r\n"

//OTAA defines
#define SET_APPKEY 		"mac set appkey 8A465B4FC57F03AA95FFCC662D9D1C44\r\n"
#define SET_DEVEUI 		"mac set deveui BE7A00000000301C\r\n"
#define SET_APPEUI 		"mac set appeui BE7A0000000023CE\r\n"

//ABP defines
#define SET_DEVADDR 	"mac set devaddr 30C5A958\r\n"
#define SET_NWKSKEY 	"mac set nwkskey EA9F51B388F7509101C870CEABAB44CA\r\n"
#define SET_APPSKEY 	"mac set appskey 6DCE01412167FF3EF0DF822910232630\r\n"

// SYS Commands
#define SLEEP           "sys sleep\r\n"			//sleep for x miliseconds
#define RESET           "sys reset\r\n"			//reset device
#define ERASE_FW		"sys eraseFW\r\n"		//erases firmware and prepares for upgrades
#define FACTORY_RESET   "sys factoryRESET\r\n"	//reset to factory settings

#define SET_MEM         "sys set nvm\r\n"		//modify EEPROM at x address
#define SET_PINS		"sys set pindig\r\n"	//set pin high or low

#define GET_VER         "sys get ver\r\n"		//get system version
#define GET_NVM         "sys get nvm\r\n"		//data from EEPROM
#define GET_VDD         "sys get vdd\r\n"		//voltage on vdd pin
#define GET_HWEUI       "sys get hweui\r\n"		//preprogrammed EUI node address

// MAC Commands
#define RES868			"mac reset 868\r\n"		//sets 868 freq
#define RES433			"mac reset 433\r\n"		//sets 433 freq
#define TX              "mac tx "				//send data
#define JOIN_OTA        "mac join otaa\r\n"		//joins OTAA network
#define JOIN_ABP        "mac join abp\r\n"		//joins ABP network
#define SAVE            "mac save\r\n"			//save changes
#define FORCE			"mac forceENABLE\r\n"	//enables connectivity from sleep
#define PAUSE           "mac pause\r\n"			//pauses operations
#define RESUME          "mac resume\r\n"		//resumes operations

#define SET_PWRIDX		"mac set pwridx\r\n"	//sets output power
#define SET_DATARATE    "mac set dr\r\n"		//sets data rate/SF
#define SET_ADR_ON     	"mac set adr on\r\n"		//adaptive data rate on
#define SET_ADR_OFF     "mac set adr off\r\n"	//adaptive data rate off
#define SET_BAT			"mac set bat\r\n"		//sets battery level
#define SET_RETX		"mac set retx\r\n"		//number of retransmisions
#define SET_LINKCHK		"mac set linkchk\r\n" 	//link check process
#define SET_AR_ON       "mac set ar on\r\n"		//automatic response on
#define SET_AR_OFF      "mac set ar off\r\n"	//automatic response off
#define SET_RX2			"mac set rx2\r\n"		//sets rx2 SF & freq
#define SET_FREQ		"mac set ch freq\r\n"	//sets channel freq
#define SET_DCYCLE		"mac set ch dcycle\r\n"	//sets channel duty cycle

#define GET_DEVADDR		"mac get devaddr\r\n"	//gets current address
#define GET_DEVEUI		"mac get deveui\r\n"	//gets deveui
#define GET_APPEUI		"mac get appeui\r\n"	//gets appeui
#define GET_DATA_RATE	"mac get dr\r\n"		//gets data rate
#define GET_BAND		"mac get band\r\n"		//gets current freq band
#define GET_PWRIDX		"mac get pwridx\r\n"	//gets pwr index
#define GET_ADR			"mac get adr\r\n"		//gets adaptive data rate
#define GET_RETX		"mac get retx\r\n"		//gets retransmisions
#define GET_AR			"mac get ar\r\n"		//gets automatic reply
#define GET_RX2			"mac get rx2\r\n"		//gets rx2 SF & freq
#define GET_DCYCLEPS	"mac get dcycleps\r\n"	//gets duty cycle prescaler
#define GET_MRGN		"mac get mrgn\r\n"		//gets demodulation margin
#define GET_GWNB		"mac get gwnb\r\n"		//gets number of gateways that received last link check request frame
#define GET_STATUS		"mac get status\r\n"	//gets status
#define GET_CH_FREQ		"mac get freq\r\n"		//freq of channel
#define GET_DCYCLE		"mac get dcycle\r\n"	//gets channel duty cycle

// Radio commands
#define RRX				"radio rx\r\n"
#define	RTX				"radio tx\r\n"
#define RCW				"radio cw\r\n"
#define RSET			"radio set\r\n"
#define RGET			"radio get\r\n"

// FUNCTION DEFINES ///////////////////////////////////////////////

char*	Send			(char *buffer);
char*	Receive			();

//Set OTAA keys
char*	SetAppkey		();
char*	SetDeveui		();
char*	SetAppeui		();
//char*	SetAppkey		(uint64_t key1, uint64_t key2);
//char*	SetDeveui		(uint64_t key);
//char*	SetAppeui		(uint64_t key);

//Set ABP keys
char*	SetDevaddr		();
char*	SetNwkskey		();
char*	SetAppskey		();
//char*	SetDevaddr		(uint32_t key);
//char*	SetNwkskey		(uint64_t key1, uint64_t key2);
//char*	SetAppskey		(uint64_t key1, uint64_t key2);

//System funcions
char*	Sleep			(int lenght);
char*	Reset			();
char*	Erasefw			();
char*	FactoryReset	();

char*	SetNvm			(uint16_t address, uint8_t data);
char*	SetPins			(char* pinname, uint8_t pinstate);

char*	GetVersion		();
char*	GetNvm			(uint16_t address);
char*	GetVdd			();
char*	GetHweui		();

//mac functions
char*	ResetFreq868	();
char*	ResetFreq433	();
char*	Tx				(char* type, char* portno, char* data);
char*	JoinOta			();
char*	JoinAbp			();
char*	Save			();
char*	Force			();
char*	Pause			();
char*	Resume			();

char*	SetPwridx		(uint8_t pwrindex);
char*	SetDatarate		(uint8_t datarate);
char*	SetAdrOn		();
char*	SetAdrOff		();
char*	SetBat			(uint8_t level);
char*	SetRetx			(uint8_t retxnb);
char*	SetLinkchk		(uint64_t linkcheck);
char*	SetArOn			();
char*	SetArOff		();
char*	SetRx2			(uint8_t datarate, float freq);//dodaj još 3 nule na freq
char*	SetChFreq		(uint8_t channelid, float freq);//dodaj još 3 nule na freq
char*	SetDcycle		(uint8_t channelid, uint64_t dutycycle);

char*	GetDevaddr		();
char*	GetDeveui		();
char*	GetAppeui		();
char*	GetDataRate		();
char*	GetBand			();
char*	GetPwridx		();
char*	GetAdr			();
char*	GetRetx			();
char*	GetAr			();
char*	GetRx2			();
char*	GetDcycleps		();
char*	GetMrgn			();
char*	GetGwnb			();
char*	GetStatus		();
char*	GetChFreq		(uint8_t channelid);
char*	GetDcycle		(uint8_t channelid);


































