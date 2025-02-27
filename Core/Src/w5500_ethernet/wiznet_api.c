/*
 * wiznet_api.c
 *
 *  Created on: Aug 8, 2024
 *      Author: Kirill
 */

#include "wiznet_api.h"

static wiz_NetInfo gWIZNETINFO;

void W5500_Select(void)
{
	HAL_GPIO_WritePin(WIZNET_CS_PORT, WIZNET_CS_PIN, GPIO_PIN_RESET);
}

void W5500_Deselect(void)
{
	HAL_GPIO_WritePin(WIZNET_CS_PORT, WIZNET_CS_PIN, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len)
{
	HAL_SPI_Receive(WIZNET_SPI_INSTANCE, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len)
{
	HAL_SPI_Transmit(WIZNET_SPI_INSTANCE, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void)
{
	uint8_t byte;
	W5500_ReadBuff(&byte, sizeof(byte));
	return byte;
}

void W5500_WriteByte(uint8_t byte)
{
	W5500_WriteBuff(&byte, sizeof(byte));
}

void W5500_SetAddress(wiz_NetInfo info)
{
	gWIZNETINFO = info;
}
void W5500_Reboot()
{
	HAL_GPIO_WritePin(WIZNET_RST_PORT, WIZNET_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(WIZNET_RST_PORT, WIZNET_RST_PIN, GPIO_PIN_SET);
	HAL_Delay(5);
	reg_wizchip_cs_cbfunc(W5500_Select, W5500_Deselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
	uint8_t rx_tx_buff_sizes[] = {2, 2, 2, 2, 2, 2, 2, 2};
	wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
}


