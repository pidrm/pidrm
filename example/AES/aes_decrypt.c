/*
Copyright (c) MagiCom Ltd. 2018
This software is licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
 
This example shows how to configure encrypt AES:
-In order to decrypt, we have to set the IV 
-Decrypt file, and save the plaintext to file
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pidrm.h"
#include "callbackerrorcodes.h"


void AESSetIVCallback()
{
	printf("AES channel's IV set successfully\n");
}

void AESDecryptCallback(uint8_t* plaintext, uint32_t length)
{
	FILE *fp = fopen("plaintext.bin", "wb");
	fwrite(plaintext, 1, length, fp);
	fclose(fp);
	printf("AES decryption is successful.\n");
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
	uint8_t cipertext[1024];
	uint8_t IV[16];
	if (!pidrm_init(NULL)) //initialize the API and the device
	{
		printf("unable to init pidrm\n");
		return 0;
	}
	else
		printf("pidrm initialized successfully\n");
	
	FILE *fp = fopen("cipertext.bin", "rb");
	uint16_t filelen = 0;
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		filelen = ftell(fp);
		rewind(fp);
		fread(cipertext, filelen, 1, fp);
		fclose(fp);
		fp = fopen("iv.bin", "rb");
		if (fp != NULL)
		{
			fread(IV, 1, 16, fp);
			fclose(fp);
			if (filelen <= 1024)
			{
				pidrm_aes_block_iv_set(0,IV,AESSetIVCallback,error_handler,-1);
				pidrm_aes_block_decrypt(0, cipertext, 64, AESDecryptCallback, error_handler, -1);
				sleep(2);
			}
			else
				printf("Cipertext is too big for this example.\n");
		}
		else
			printf("Cannot open IV file\n");
	}
	else
		printf("Unable to open cipertext file\n");

	if (pidrm_deinit())
		printf("pidrm successfully deinitalized, all resources are freed up\n");
	else
		printf("Unable to deinitalize pidrm\n");
	return 0;
}