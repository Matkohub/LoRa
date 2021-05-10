#include "EPY12231.h"

void CO2_read()
{
	uint8_t rd;
	HAL_I2C_Mem_Read(EPY.dat, ADDRESS, WAKE_UP, 1, &rd, 1, I2C_TIMEOUT);


	HAL_I2C_Mem_Read(&hi2c1, ADDRESS, 0x04, 1, &EPY.dat, 1, HAL_MAX_DELAY);

	//read only if there if FIFO is full //if there is 14 values
	if((EPY.dat & 0x1C) == 0x1C)
	{
		EPY12231_FIFO_Read_Full(&EPY);
		EPY_EPY12231_FIFO_Clear(&EPY);
		EPY_EPY12231_FIFO_Reset(&EPY);
	}

	HAL_I2C_Mem_Read(EPY.dat, ADDRESS, GO_TO_SLEEP, 1, &rd, 1, I2C_TIMEOUT);
}

void EPY12231_Init(TypeDefHandleEPY12231 *Handle)
{
	EPY_EPY12231_Test(Handle);
	for (int i = 1; i <= 127; i++) {

		asm("NOP");
	}
}

/*
 * testr
 * @arg
 * 	TypeDefHandleEPY12231 *Handle
 * @return
 * 	01 - OK
 * 	02 - Error
 */
uint8_t EPY_EPY12231_Test(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[1];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 10, I2C_TIMEOUT)!= HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return (HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, TEST, 1, ReturnBuffer, 1, I2C_TIMEOUT));
	}
}

/*
 * version
 * @arg
 * 	TypeDefHandleEPY12231 *Handle
 * @return
 * version
 */
uint8_t EPY12231_Version(TypeDefHandleEPY12231 *Handle, uint8_t *ReturnBuffer)
{

	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		return (HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, VERSION, 1, ReturnBuffer, 1, I2C_TIMEOUT));
	}

}
/*
 * status
 * @arg
 * 	TypeDefHandleEPY12231 *Handle
 * @return
 * version
 */
uint8_t EPY12231_Status(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[1];

	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, FIFO_STATUS, 1, ReturnBuffer, 1, I2C_TIMEOUT);
	}
	Handle->InversedStatus = ReturnBuffer[0] & 0x01;
	Handle->FIFO_Count = (ReturnBuffer[0] & 0x1E) >> 1;
	Handle->Error = (ReturnBuffer[0] & 0x30) >> 5;
	Handle->WakeDetected = ReturnBuffer[0] & 0x80 >> 7;

	return 1;
}

/*
 * read full
 * @arg
 * 	TypeDefHandleEPY12231 *Handle
 * @return
 * version
 */
HAL_StatusTypeDef EPY12231_FIFO_Read_Full(TypeDefHandleEPY12231 *Handle)
{

	uint8_t ReturnBuffer[17];
	if(HAL_I2C_IsDeviceReady(Handle->I2C,ADDRESS,2,I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		 (HAL_I2C_Mem_Read(Handle->I2C,ADDRESS,FIFO_READ_FULL,1,ReturnBuffer,17,I2C_TIMEOUT));

		 Handle->Channel1_CO2 = 0x00 << 24 | ReturnBuffer[0] << 16 | ReturnBuffer[1] << 8 | ReturnBuffer[2];
		 Handle->Channel2_CO2 = 0x00 << 24 | ReturnBuffer[3] << 16 | ReturnBuffer[4] << 8 | ReturnBuffer[5];
		 Handle->Channel3_CO2 = 0x00 << 24 | ReturnBuffer[6] << 16 | ReturnBuffer[7] << 8 | ReturnBuffer[8];
		 Handle->Channel4_CO2 = 0x00 << 24 | ReturnBuffer[9] << 16 | ReturnBuffer[10] << 8 | ReturnBuffer[11];
		 Handle->Channel5_CO2 = 0x00 << 24 | ReturnBuffer[12] << 16 | ReturnBuffer[13] << 8 | ReturnBuffer[14];
		 Handle->Count = ReturnBuffer[15] << 8 | ReturnBuffer[16];
		 return HAL_OK;
	 }
	return HAL_OK;
}
//
///*
// * FIFO_Read_Active
// * @arg
// * 	TypeDefHandleEPY12231 *Handle
// * @return
// * version
// */
//uint8_t EPY_EPY12231_FIFO_Read_Active(TypeDefHandleEPY12231 *Handle){
//
//	uint8_t TxBuffer[17];
//	uint8_t RxBuffer[17];
//	TxBuffer[0] = FIFO_READ_ACTIVE;
//	HAL_I2C_Master_Transmit(&hi2c,ADDRESS,TxBuffer,1,I2C_TIMEOUT);
//	HAL_I2C_Master_Receive(&hi2c,ADDRESS,RxBuffer,17,I2C_TIMEOUT);
//	return (RxBuffer[0]);
//}
//
/*
 * FIFO_Clear
 * @arg
 * 	TypeDefHandleEPY12231 *Handle
 * @return
 * version
 */
uint8_t EPY_EPY12231_FIFO_Clear(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[1];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		(HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, FIFO_CLEAR, 1, ReturnBuffer, 1, I2C_TIMEOUT));
		return ReturnBuffer[0];
	}
}
//
/*
 * FIFO_Reset
 * @arg
 * 	TypeDefHandleEPY12231 *Handle
 * @return
 * version
 */
uint8_t EPY_EPY12231_FIFO_Reset(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[1];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, FIFO_RESET, 1, ReturnBuffer, 1, I2C_TIMEOUT);
		return ReturnBuffer[0];
	}
}

// * FIFO_CH_Read
// * @arg
// * 	TypeDefHandleEPY12231 *Handle
// * @return
// * version
//
uint8_t EPY12231_CH_Read(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[5];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		if ((HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, CH_READ, 1, ReturnBuffer, 5, I2C_TIMEOUT)) != HAL_OK)
		{
			return HAL_ERROR;
		}
		else
		{
			Handle->Channel_1_Status = (ReturnBuffer[0] & 0x01);
			Handle->Channel_1_FeedbackCapacitorSelection = (ReturnBuffer[0]
					& 0x0E) >> 1;
			Handle->Channel_1_HighPassSignalFilterFrequencySelection =
					(ReturnBuffer[0] & 0x30) >> 4;
			Handle->Channel_1_FrontEndTransconductanceSelection =
					(ReturnBuffer[0] & 0xC0) >> 6;

			Handle->Channel_2_Status = (ReturnBuffer[1] & 0x01);
			Handle->Channel_2_FeedbackCapacitorSelection = (ReturnBuffer[1]
					& 0x0E) >> 1;
			Handle->Channel_2_HighPassSignalFilterFrequencySelection =
					(ReturnBuffer[1] & 0x30) >> 4;
			Handle->Channel_2_FrontEndTransconductanceSelection =
					(ReturnBuffer[1] & 0xC0) >> 6;

			Handle->Channel_3_Status = (ReturnBuffer[2] & 0x01);
			Handle->Channel_3_FeedbackCapacitorSelection = (ReturnBuffer[2]
					& 0x0E) >> 1;
			Handle->Channel_3_HighPassSignalFilterFrequencySelection =
					(ReturnBuffer[2] & 0x30) >> 4;
			Handle->Channel_3_FrontEndTransconductanceSelection =
					(ReturnBuffer[2] & 0xC0) >> 6;

			Handle->Channel_4_Status = (ReturnBuffer[3] & 0x01);
			Handle->Channel_4_FeedbackCapacitorSelection = (ReturnBuffer[3]
					& 0x0E) >> 1;
			Handle->Channel_4_HighPassSignalFilterFrequencySelection =
					(ReturnBuffer[3] & 0x30) >> 4;
			Handle->Channel_4_FrontEndTransconductanceSelection =
					(ReturnBuffer[3] & 0xC0) >> 6;

			Handle->Channel_5_Status = (ReturnBuffer[4] & 0x01);
			Handle->Channel_5_FeedbackCapacitorSelection = (ReturnBuffer[4]
					& 0x0E) >> 1;
			Handle->Channel_5_HighPassSignalFilterFrequencySelection =
					(ReturnBuffer[4] & 0x30) >> 4;
			Handle->Channel_5_FrontEndTransconductanceSelection =
					(ReturnBuffer[4] & 0xC0) >> 6;
			return HAL_OK;
		}
	}
	return HAL_OK;
}

// * FIFO_CH_Write
// * @arg
// * 	TypeDefHandleEPY12231 *Handle
// * @return
// * version
//
uint8_t EPY12231_CH_Write(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[5];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		ReturnBuffer[0] = (Handle->Channel_1_FrontEndTransconductanceSelection) << 6
				| (Handle->Channel_1_HighPassSignalFilterFrequencySelection) << 4
				| (Handle->Channel_1_FeedbackCapacitorSelection) << 1
				| (Handle->Channel_1_Status);

		ReturnBuffer[1] = (Handle->Channel_2_FrontEndTransconductanceSelection) << 6
				| (Handle->Channel_2_HighPassSignalFilterFrequencySelection) << 4
				| (Handle->Channel_2_FeedbackCapacitorSelection) << 1
				| (Handle->Channel_2_Status);

		ReturnBuffer[2] = (Handle->Channel_3_FrontEndTransconductanceSelection) << 6
				| (Handle->Channel_3_HighPassSignalFilterFrequencySelection) << 4
				| (Handle->Channel_3_FeedbackCapacitorSelection) << 1
				| (Handle->Channel_3_Status);

		ReturnBuffer[3] = (Handle->Channel_4_FrontEndTransconductanceSelection) << 6
				| (Handle->Channel_4_HighPassSignalFilterFrequencySelection) << 4
				| (Handle->Channel_4_FeedbackCapacitorSelection) << 1
				| (Handle->Channel_4_Status);

		ReturnBuffer[4] = (Handle->Channel_5_FrontEndTransconductanceSelection) << 6
				| (Handle->Channel_5_HighPassSignalFilterFrequencySelection) << 4
				| (Handle->Channel_5_FeedbackCapacitorSelection) << 1
				| (Handle->Channel_5_Status);

		return (HAL_I2C_Mem_Write(Handle->I2C, ADDRESS, CH_WRITE, 1, ReturnBuffer, 5, I2C_TIMEOUT));
	}
	return HAL_OK;
}

uint8_t EPY12231_Ana_Read(TypeDefHandleEPY12231 *Handle)
{
	uint8_t ReturnBuffer[2];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		if ((HAL_I2C_Mem_Read(Handle->I2C, ADDRESS, ANA_READ, 1, ReturnBuffer, 2, I2C_TIMEOUT)) != HAL_OK) {
			return HAL_ERROR;
		}
		else
		{
			Handle->SamplingRate = (ReturnBuffer[0]);
			Handle->INTEnable = (ReturnBuffer[1] & 0x01);
			Handle->SYNC = (ReturnBuffer[1] & 0x04) >> 2;
			Handle->CLK_OUT = (ReturnBuffer[1] & 0x08) >> 3;
			Handle->LowPassSignalFilterFrequencySelection = (ReturnBuffer[1] & 0x30) >> 4;
			Handle->HP = (ReturnBuffer[1] & 0x40) >> 6;
			Handle->LP = (ReturnBuffer[1] & 0x80) >> 7;
		}
	}
	return HAL_OK;
}

uint8_t EPY12231_Ana_Write(TypeDefHandleEPY12231 *Handle)
{
	uint8_t WriteBuffer[2];
	if (HAL_I2C_IsDeviceReady(Handle->I2C, ADDRESS, 2, I2C_TIMEOUT) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
			WriteBuffer[0] = Handle->SamplingRate;
			WriteBuffer[1] = Handle->LP << 7 | Handle->HP << 6
					| Handle->LowPassSignalFilterFrequencySelection << 4
					| Handle->CLK_OUT << 3 | Handle->SYNC << 2
					| Handle->INTEnable;

			Handle->INTEnable = (WriteBuffer[1] & 0x01);
			Handle->SYNC = (WriteBuffer[1] & 0x04) >> 2;
			Handle->CLK_OUT = (WriteBuffer[1] & 0x08) >> 3;
			Handle->LowPassSignalFilterFrequencySelection = (WriteBuffer[1]
					& 0x30) >> 4;
			Handle->HP = (WriteBuffer[1] & 0x40) >> 6;
			Handle->LP = (WriteBuffer[1] & 0x80) >> 7;

			HAL_I2C_Mem_Write(Handle->I2C, ADDRESS, ANA_WRITE, 1, WriteBuffer,
						2, I2C_TIMEOUT);

	}
	return HAL_OK;
}
