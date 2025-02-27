/*
 * w5500_api.h
 *
 *  Created on: Aug 8, 2024
 *      Author: Kirill
 */

#ifndef SRC_W5500_ETHERNET_WIZNET_API_H_
#define SRC_W5500_ETHERNET_WIZNET_API_H_

#include "wizchip_conf.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;

#define WIZNET_SPI_INSTANCE 	&hspi2
#define WIZNET_CS_PORT 			ETH_SPI_CS_GPIO_Port
#define WIZNET_CS_PIN			ETH_SPI_CS_Pin
#define WIZNET_RST_PORT			ETH_RESET_GPIO_Port
#define WIZNET_RST_PIN			ETH_RESET_Pin
#define WIZNET_INT_PORT			GPIOB
#define WIZNET_INT_PIN			GPIO_PIN_9

void 	W5500_Select(void);
void 	W5500_Deselect(void);
void 	W5500_ReadBuff(uint8_t* buff, uint16_t len);
void 	W5500_WriteBuff(uint8_t* buff, uint16_t len);
uint8_t W5500_ReadByte(void);
void 	W5500_WriteByte(uint8_t byte);

// UPD by AKG
void 	W5500_SetAddress(wiz_NetInfo info);
void	W5500_Reboot();

#endif /* SRC_W5500_ETHERNET_WIZNET_API_H_ */
