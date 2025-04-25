/*
 * flash_data.c
 *
 *  Created on: Feb 19, 2025
 *      Author: Kirill
 */

#include "flash_data.h"
#include <string.h>


#include "stm32f4xx_hal.h"


// STM32F401CEU6
//#define FLASH_DATA_SECTOR FLASH_SECTOR_7 /* Sector 7 0x08060000 - 0x0807FFFF */
//#define FLASH_DATA_BASE_ADDR 0x08060000

// STM32F407VET6
//#define FLASH_DATA_SECTOR FLASH_SECTOR_8 /* Sector 8 0x08080000 - 0x0809FFFF */
//#define FLASH_DATA_BASE_ADDR 0x08080000


#define FLASH_DATA_SECTOR FLASH_SECTOR_11 /* Sector  0x080E0000 - 0x080FFFFF */
#define FLASH_DATA_BASE_ADDR 0x080E0000

static void flash_data_erase_sector()
{
	FLASH_EraseInitTypeDef FlashErase;
	uint32_t sectorError = 0;

	__disable_irq();
	HAL_FLASH_Unlock();

	FlashErase.TypeErase = FLASH_TYPEERASE_SECTORS;
	FlashErase.NbSectors = 1;
	FlashErase.Sector = FLASH_DATA_SECTOR;
	FlashErase.VoltageRange = VOLTAGE_RANGE_3;
	if (HAL_FLASHEx_Erase(&FlashErase, &sectorError) != HAL_OK)
	{
		HAL_FLASH_Lock();
        __enable_irq();
		//return HAL_ERROR;
	}
	HAL_FLASH_Lock();
    __enable_irq();
}

void flash_data_write(flash_data_t data)
{
	flash_data_erase_sector();

	HAL_StatusTypeDef status;
	__disable_irq();
	status = HAL_FLASH_Unlock();

	uint32_t addr = FLASH_DATA_BASE_ADDR;
	uint8_t* ptr = (uint8_t*)&data;
	int i;
	for(i = 0; i < sizeof(flash_data_t); i++)
	{
		status += HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr, ptr[i]);
		addr++;
	}
	__enable_irq();
	HAL_FLASH_Lock();
}

flash_data_t flash_data_read()
{
	flash_data_t data;
	memset((void*)&data, 0, sizeof(data));

	uint8_t* ptr = (uint8_t*)&data;
	uint32_t addr = FLASH_DATA_BASE_ADDR;
	int i;
	for(i = 0; i < sizeof(flash_data_t); i++)
	{
		*(ptr + i) = *(__IO uint8_t*)addr;
		addr++;
	}
	return data;
}


