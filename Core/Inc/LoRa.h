#include "stm32g0xx_hal.h"

#define OTA            "otaa"
#define ABP            "abp"

//OTAA keys
#define appkey 			"8A465B4FC57F03AA95FFCC662D9D1C44"
#define deveui 			"BE7A00000000301C"
#define appeui 			"BE7A0000000023CE"

//ABP keys
#define devaddr 		"30C5A958"
#define nwkskey 		"EA9F51B388F7509101C870CEABAB44CA"
#define appskey 		"6DCE01412167FF3EF0DF822910232630"

//OTAA defines
#define SET_APPKEY 		"mac set appkey"
#define SET_DEVEUI 		"mac set deveui"
#define SET_APPEUI 		"mac set appeui"

//ABP defines
#define SET_DEVADDR 	"mac set devaddr"
#define SET_NWKSKEY 	"mac set nwkskey"
#define SET_APPSKEY 	"mac set appskey"

// SYS Commands
#define SLEEP           "sys sleep "		//sleep for x miliseconds
#define RESET           "sys reset"			//reset device
#define ERASE_FW		"sys eraseFW"		//erases firmware and prepares for upgrades
#define FACTORY_RESET   "sys factoryRESET"	//reset to factory settings

#define SET_MEM         "sys set nvm"		//modify EEPROM at x address
#define SET_PINS		"sys set pindig"	//set pin high or low

#define GET_VER         "sys get ver\r\n"	//get system version
#define GET_NVM         "sys get nvm"		//data from EEPROM
#define GET_VDD         "sys get vdd"		//voltage on vdd pin
#define GET_HWEUI       "sys get hweui"		//preprogrammed EUI node address

// MAC Commands
#define RES868			"mac reset 868"		//sets 868 freq
#define RES433			"mac reset 433"		//sets 433 freq
#define TX              "mac tx"			//send data
#define JOIN_OTA        "mac join otaa"		//joins OTAA network
#define JOIN_ABP        "mac join abp"		//joins ABP network
#define SAVE            "mac save"			//save changes
#define FORCE			"mac forceENABLE"	//enables connectivity from sleep
#define PAUSE           "mac pause"			//pauses operations
#define RESUME          "mac resume"		//resumes operations

#define SET_PWRIDX		"mac set pwridx"	//sets output power
#define SET_DATARATE    "mac set dr"		//sets data rate/SF
#define SET_ADR_ON     "mac set adr on"	//adaptive data rate on
#define SET_ADR_OFF     "mac set adr off"	//adaptive data rate off
#define SET_BAT			"mac set bat"		//sets battery level
#define SET_RETX		"mac set retx"		//number of retransmisions
#define SET_LINKCHK		"mac set linkchk" 	//link check process
#define SET_AR_ON       "mac set ar on"		//automatic response on
#define SET_AR_OFF      "mac set ar off"	//automatic response off
#define SET_RX2			"mac set rx2"		//sets rx2 SF & freq
#define SET_FREQ		"mac set ch freq"	//sets channel freq
#define SET_DCYCLE		"mac set ch dcycle"	//sets channel duty cycle

#define GET_DEVADDR		"mac get devaddr"	//gets current address
#define GET_DEVEUI		"mac get deveui"	//gets deveui
#define GET_APPEUI		"mac get appeui"	//gets appeui
#define GET_DATA_RATE	"mac get dr"		//gets data rate
#define GET_BAND		"mac get band"		//gets current freq band
#define GET_PWRIDX		"mac get pwridx"	//gets pwr index
#define GET_ADR			"mac get adr"		//gets adaptive data rate
#define GET_RETX		"mac get retx"		//gets retransmisions
#define GET_AR			"mac get ar"		//gets automatic reply
#define GET_RX2			"mac get rx2"		//gets rx2 SF & freq
#define GET_DCYCLEPS	"mac get dcycleps"	//gets duty cycle prescaler
#define GET_MRGN		"mac get mrgn"		//gets demodulation margin
#define GET_GWNB		"mac get gwnb"		//gets number of gateways that received last link check request frame
#define GET_STATUS		"mac get status"	//gets status
#define GET_CH_FREQ		"mac get freq"		//freq of channel
#define GET_DCYCLE		"mac get dcycle"	//gets channel duty cycle

// Radio commands
#define RRX				"radio rx"
#define	RTX				"radio tx"
#define RCW				"radio cw"
#define RSET			"radio set"
#define RGET			"radio get"

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
char*	Tx				(char* type, uint8_t portno, uint16_t data);
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


































