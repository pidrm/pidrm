/*
Copyright (c) MagiCom Ltd. 2018
This software is licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
 
This example shows how to configure DRM Group:
-reset private key (this disabled all protections so we can edit a private key)
-set the private key from file
-read out public key, save into a file
-enable read protection
-enable write protection
This configuration only have to be done once, and be carefull, beacause the device's storage has a finite durability.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pidrm.h"
#include "callbackerrorcodes.h"

void DRMGroupResetKeysCallback()
{
	printf("DRM Group keys regenerated\n");
}

void DRMGroupSetPrivateCallback()
{
	printf("DRM Group private key successfully set\n");
}

void DRMGroupGetPublicCallback(uint8_t* key, uint32_t length)
{
	FILE *fp = fopen("public_key.bin", "wb");
	fwrite(key, 1, length, fp);
	fclose(fp);
	printf("DRM Group public key is written to file.\n");
}

void DRMGroupPublicKeyReadProtect()
{
	printf("DRM Group public key read protection enabled\n");
}

void DRMGroupPrivateKeyReadProtect()
{
	printf("DRM Group private key read protection enabled\n");
}

void DRMGroupPrivateKeyWriteProtect()
{
	printf("DRM Group private key write protection enabled\n");
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
	uint8_t PrivateKey[32];
	if (!pidrm_init(NULL)) //initialize the API and the device
	{
		printf("unable to init pidrm\n");
		return 0;
	}
	else
		printf("pidrm initialized successfully\n");

	FILE *fp = fopen("private_key.bin", "rb");
	if (fp != NULL)
	{
		fread(PrivateKey, 1, 32, fp);
		fclose(fp);

		pidrm_group_reset_keys(DRMGroupResetKeysCallback, error_handler, -1);
		pidrm_group_set_private_key(DRMGroupSetPrivateCallback, error_handler, PrivateKey, -1);
		pidrm_group_get_public_key(DRMGroupGetPublicCallback, error_handler, -1);
		pidrm_group_public_key_read_protect(DRMGroupPublicKeyReadProtect, error_handler, -1);
		pidrm_group_private_key_read_protect(DRMGroupPrivateKeyReadProtect, error_handler, -1);
		pidrm_group_private_key_write_protect(DRMGroupPrivateKeyWriteProtect, error_handler, -1);

		sleep(2);
	}
	else
		printf("Unable to open private_key.bin\n");
	if (pidrm_deinit())
		printf("pidrm successfully deinitalized, all resources are freed up\n");
	else
		printf("Unable to deinitalize pidrm\n");
	return 0;
}