/*
 * EPY12231.h
 *
 *  Created on: May 21, 2020
 *      Author: Josip
 */
#include"stdio.h"
#include"stdlib.h"
#include "stm32g0xx_hal.h"

#ifndef EPY_EPY12231_H_
#define EPY_EPY12231_H_

extern I2C_HandleTypeDef hi2c1;

#define ADDRESS 			0x65 << 1

#define TEST				0x00
#define VERSION				0x02
#define FIFO_STATUS			0x04
#define FIFO_READ_FULL		0x06
#define FIFO_READ_ACTIVE	0x08
#define FIFO_CLEAR			0x0A
#define FIFO_RESET			0x0C
#define CH_READ				0x0E
#define CH_WRITE			0x10
#define ANA_READ			0x12
#define ANA_WRITE			0x14
#define WAKE_READ			0x16
#define WAKE_WRITE			0x18
#define ADDR_WRITE			0x1E
#define GO_TO_SLEEP			0x20
#define WAKE_UP				0x22
#define RESET_SOFT			0x24
#define RESET_FULL 			0x26

#define I2C_TIMEOUT			1000

typedef struct {
	I2C_HandleTypeDef *I2C;

	uint8_t dat;

	uint8_t Channel_Cx_ST[4];

	uint8_t InversedStatus;
	uint8_t FIFO_Count;
	uint8_t Error;
	uint8_t WakeDetected;

	uint8_t Channel_1_Status;
	uint8_t Channel_1_FeedbackCapacitorSelection;
	uint8_t Channel_1_HighPassSignalFilterFrequencySelection;
	uint8_t Channel_1_FrontEndTransconductanceSelection;

	uint8_t Channel_2_Status;
	uint8_t Channel_2_FeedbackCapacitorSelection;
	uint8_t Channel_2_HighPassSignalFilterFrequencySelection;
	uint8_t Channel_2_FrontEndTransconductanceSelection;

	uint8_t Channel_3_Status;
	uint8_t Channel_3_FeedbackCapacitorSelection;
	uint8_t Channel_3_HighPassSignalFilterFrequencySelection;
	uint8_t Channel_3_FrontEndTransconductanceSelection;

	uint8_t Channel_4_Status;
	uint8_t Channel_4_FeedbackCapacitorSelection;
	uint8_t Channel_4_HighPassSignalFilterFrequencySelection;
	uint8_t Channel_4_FrontEndTransconductanceSelection;

	uint8_t Channel_5_Status;
	uint8_t Channel_5_FeedbackCapacitorSelection;
	uint8_t Channel_5_HighPassSignalFilterFrequencySelection;
	uint8_t Channel_5_FrontEndTransconductanceSelection;

	uint8_t SamplingRate;
	uint8_t INTEnable;
	uint8_t SYNC;
	uint8_t CLK_OUT;
	uint8_t LowPassSignalFilterFrequencySelection;
	uint8_t HP;
	uint8_t LP;

	uint32_t Channel1_CO2;
	uint32_t Channel2_CO2;
	uint32_t Channel3_CO2;
	uint32_t Channel4_CO2;
	uint32_t Channel5_CO2;
	uint16_t Count;
}TypeDefHandleEPY12231;

typedef enum{
	Channel1,
	Channel2,
	Channel3,
	Channel4,
	Channel5,
	ChannelAll

}ChannelConfigSelection;

typedef enum{
	ChannelDisable,
	ChannelEnable

}ChannelStatusSelection;

typedef enum{
	FC_50_RG_64x,
	FC_100_RG_32x,
	FC_200_RG_16x,
	FC_400_RG_8x,
	FC_800_RG_4x,
	FC_1600_RG_2x,
	FC_3200_RG_1x,

}FeedbackCapacitorSelection;


void EPY12231_Init(TypeDefHandleEPY12231 *Handle);
uint8_t EPY_EPY12231_Test(TypeDefHandleEPY12231 *Handle);
uint8_t EPY12231_Version(TypeDefHandleEPY12231 *Handle, uint8_t *ReturnBuffer);
uint8_t EPY12231_Status(TypeDefHandleEPY12231 *Handle);
HAL_StatusTypeDef EPY12231_FIFO_Read_Full(TypeDefHandleEPY12231 *Handle);
uint8_t EPY12231_FIFO_Read_Active(TypeDefHandleEPY12231 *Handle, uint8_t *ReturnBuffer);
uint8_t EPY_EPY12231_FIFO_Clear(TypeDefHandleEPY12231 *Handle);
uint8_t EPY_EPY12231_FIFO_Reset(TypeDefHandleEPY12231 *Handle);
uint8_t EPY12231_CH_Read(TypeDefHandleEPY12231 *Handle);
uint8_t EPY12231_CH_Write(TypeDefHandleEPY12231 *Handle);
uint8_t EPY12231_Ana_Read(TypeDefHandleEPY12231 *Handle);
uint8_t EPY12231_Ana_Write(TypeDefHandleEPY12231 *Handle);
void CO2_read();

TypeDefHandleEPY12231 EPY;

#endif /* EPY_EPY12231_H_ */
