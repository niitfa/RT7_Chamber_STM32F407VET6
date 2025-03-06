/*
 * user_sd.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Kirill
 */

#ifndef APP_USER_SD_H_
#define APP_USER_SD_H_

#include "fatfs_sd.h"
#include "fatfs.h"
#include "stm32f4xx_hal.h"

#define SD_BUFFER_SIZE 1024

typedef struct
{
	FATFS fs;
	FIL fil;
	FRESULT fresult;
	char buffer[SD_BUFFER_SIZE];
	UINT br, bw;
	FATFS *pfs;
	DWORD fre_clust;
	uint32_t total, free_space;
} user_sd_t;

int user_sd_init(
		user_sd_t* self,
		SPI_HandleTypeDef* spi,
		GPIO_TypeDef* csPort,
		uint16_t csPin
		);

int user_sd_mount(user_sd_t* self, BYTE opt);
int user_sd_getfree(user_sd_t* self);
int user_sd_fopen(user_sd_t* self, const TCHAR* path, BYTE mode);
int user_sd_fputs(user_sd_t* self, const TCHAR* str);
char* user_sd_fgets(user_sd_t* self, char* buff, int size);
int user_sd_fclose(user_sd_t* self);




#endif /* APP_USER_SD_H_ */
