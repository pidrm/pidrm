/*
Copyright (c) MagiCom Ltd. 2018
This software is licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
 
This example shows how to configure DRM:
-reset private key
-read out public key, save into a file
-enable read protection
This configuration only have to be done once, and be carefull, beacause the device's storage has a finite durability.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pidrm.h"
#include "callbackerrorcodes.h"

void DRMResetKeysCallback()
{
	printf("DRM keys regenerated\n");
}

void DRMGetPublicCallback(uint8_t* key, uint32_t length)
{
	FILE *fp = fopen("public_key.bin", "wb");
	fwrite(key, 1, length, fp);
	fclose(fp);
	printf("DRM public key is written to file.\n");
}

void DRMPublicKeyReadProtect()
{
	printf("DRM public key read protection enabled\n");
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

	pidrm_reset_keys(DRMResetKeysCallback, error_handler, -1);
	pidrm_get_public_key(DRMGetPublicCallback, error_handler, -1);
	pidrm_public_key_read_protect(DRMPublicKeyReadProtect, error_handler, -1);
	
	sleep(2);
	
	if(pidrm_deinit())
		printf("pidrm successfully deinitalized, all resources are freed up\n");
	else
		printf("Unable to deinitalize pidrm\n");
	return 0;
}