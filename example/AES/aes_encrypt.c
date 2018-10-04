/*
Copyright (c) MagiCom Ltd. 2018
This software is licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
 
This example shows how to configure encrypt AES:
-generate a new IV, and save it into file
-Encrypt buffer, and save the cipertext to file
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pidrm.h"
#include "callbackerrorcodes.h"

void AESIVGenerateCallback()
{
	printf("New IV generated.\n");
}

void AESGetIVCallback(uint8_t* key, uint32_t length)
{
	FILE *fp = fopen("iv.bin", "wb");
	fwrite(key, 1, length, fp);
	fclose(fp);
	printf("AES Channel's IV is written to file.\n");
}

void AESEncryptCallback(uint8_t* cipertext, uint32_t length)
{
	FILE *fp = fopen("cipertext.bin", "wb");
	fwrite(cipertext, 1, length, fp);
	fclose(fp);
	printf("AES encryption is successful.\n");
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
	uint8_t plaintext[64] = "That's one small step  for  man,  one giant  leap for  mankind.";
	if (!pidrm_init(NULL)) //initialize the API and the device
	{
		printf("unable to init pidrm\n");
		return 0;
	}
	else
		printf("pidrm initialized successfully\n");
	
	pidrm_aes_block_iv_generate(0,AESIVGenerateCallback,error_handler,-1);
	pidrm_aes_block_iv_get(0, AESGetIVCallback, error_handler, -1);
	pidrm_aes_block_encrypt(0, plaintext, 64, AESEncryptCallback, error_handler, -1);


	sleep(2);

	if (pidrm_deinit())
		printf("pidrm successfully deinitalized, all resources are freed up\n");
	else
		printf("Unable to deinitalize pidrm\n");
	return 0;
}