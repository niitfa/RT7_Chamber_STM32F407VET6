/*
 * flash_data.h
 *
 *  Created on: Feb 19, 2025
 *      Author: Kirill
 */

#ifndef SRC_FLASH_MANAGER_FLASH_DATA_H_
#define SRC_FLASH_MANAGER_FLASH_DATA_H_

#include <stdint.h>

typedef struct
{
	uint8_t ip[4];
	uint16_t input_port;
	uint16_t output_port;
} flash_data_t;

// методы
void flash_data_write(flash_data_t data);
flash_data_t flash_data_read();




#endif /* SRC_FLASH_MANAGER_FLASH_DATA_H_ */
