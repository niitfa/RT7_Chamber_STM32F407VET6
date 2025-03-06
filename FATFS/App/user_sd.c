/*
 * user_sd.c
 *
 *  Created on: Mar 6, 2025
 *      Author: Kirill
 */

#include "user_sd.h"
#include <string.h>

//static int bufsize(user_sd_t* self);
//static void bufclear(user_sd_t* self);

int user_sd_init(
		user_sd_t* self,
		SPI_HandleTypeDef* spi,
		GPIO_TypeDef* csPort,
		uint16_t csPin
		)
{
	memset(self, 0, sizeof(*self));
	SD_set_spi_and_cs(spi, csPort, csPin);
	return 0;
}

int user_sd_mount(user_sd_t* self, BYTE opt)
{
	return f_mount(&self->fs, "", opt);
}

int user_sd_getfree(user_sd_t* self)
{
	f_getfree("", &self->fre_clust, &self->pfs);
	self->total = (uint32_t)((self->pfs->n_fatent - 2) * self->pfs->csize * 0.5);
	self->free_space = (uint32_t)(self->fre_clust * self->pfs->csize * 0.5);
	return self->free_space;
}

int user_sd_fopen(user_sd_t* self, const TCHAR* path, BYTE mode)
{
	return f_open(&self->fil, path, mode);
}

int user_sd_fputs(user_sd_t* self, const TCHAR* str)
{
	return f_puts(str, &self->fil);
}

char* user_sd_fgets(user_sd_t* self, char* buff, int size)
{
	return f_gets(buff, size, &self->fil);
}

int user_sd_fclose(user_sd_t* self)
{
	return f_close(&self->fil);
}





/*static int bufsize(user_sd_t* self)
{
	int i = 0;
	char* p = self->buffer;
	while(*p++ != '\0')
	{
		i++;
	}
	return i;
}

static void bufclear(user_sd_t* self)
{
	memset(self->buffer, 0, sizeof(*(self->buffer)) * SD_BUFFER_SIZE);
}
*/
