/*
Copyright (c) MagiCom Ltd. 2018
This software is licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
 
This example shows how to configure AES:
-reset key and IV
-save key and IV into file
-enable read protection
This configuration only have to be done once, and be carefull, beacause the device's storage has a finite durability.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pidrm.h"
#include "callbackerrorcodes.h"

void AESResetCallback()
{
	printf("AES channel reset successfully\n");
}

void AESGetKeyCallback(uint8_t* key, uint32_t length)
{
	FILE *fp = fopen("key.bin", "wb");
	fwrite(key, 1, length, fp);
	fclose(fp);
	printf("AES Channel's key is written to file.\n");
}

void AESGetIVCallback(uint8_t* key, uint32_t length)
{
	FILE *fp = fopen("iv.bin", "wb");
	fwrite(key, 1, length, fp);
	fclose(fp);
	printf("AES Channel's IV is written to file.\n");
}

void AESKeyReadProtectCallack()
{
	printf("AES Channel's key read protection enabled\n");
}
void AESKeyWriteProtectCallack()
{
	printf("AES Channel's key write protection enabled\n");
}


void error_handler(uint32_t errorcode)
{
	switch (errorcode)
	{
	case CALLBACK_ERROR_UNKNOWN:
		printf("Error: Unkown error occured\n");
		break;
	case CALLBACK_ERROR_TIMEOUT_EXPIRED:
		printf("Error: Timeout Expired\n");
		break;
	case CALLBACK_ERROR_COMMUNICATION_ERROR:
		printf("Error: Communication error\n");
		break;
	case CALLBACK_ERROR_READ_PROTECT:
		printf("Error: Trying to read protected data\n");
		break;
	case CALLBACK_ERROR_WRITE_PROTECT:
		printf("Error: Trying to write protected data\n");
		break;
	case CALLBACK_ERROR_NOT_PROPER_LENGTH:
		printf("Error: Error with data length\n");
		break;
	case CALLBACK_ERROR_FLASH_STORAGE_ERROR:
		printf("Error: Flash storage error on device\n");
		break;
	case CALLBACK_DH_ERROR:
		printf("Error: Cannot establish DH\n");
		break;
	case CALLBACK_ERROR_AES_ERROR:
		printf("Error: AES error\n");
		break;
	case CALLBACK_ERROR_CANNOT_MALLOC:
		printf("Error: The API cannot allocate memory\n");
		break;
	case CALLBACK_ERROR_COMMUNICATION_BUFFER_FULL:
		printf("Error: Communication buffer is full\n");
		break;
	default:
		printf("Unknown error, errorcode: %d\n", errorcode);
		break;
	}
}

int main()
{
	if (!pidrm_init(NULL)) //initialize the API and the device
	{
		printf("unable to init pidrm\n");
		return 0;
	}
	else
		printf("pidrm initialized successfully\n");

	pidrm_aes_block_generate(0,AESResetCallback, error_handler, -1);
	pidrm_aes_block_key_get(0,AESGetKeyCallback, error_handler, -1);
	pidrm_aes_block_iv_get(0, AESGetIVCallback, error_handler, -1);
	pidrm_aes_block_key_disable_read(0, AESKeyReadProtectCallack, error_handler, -1);
	pidrm_aes_block_key_disable_write(0, AESKeyWriteProtectCallack, error_handler, -1);

	sleep(2);

	if (pidrm_deinit())
		printf("pidrm successfully deinitalized, all resources are freed up\n");
	else
		printf("Unable to deinitalize pidrm\n");
	return 0;
}