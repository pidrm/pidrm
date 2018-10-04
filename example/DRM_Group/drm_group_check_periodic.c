/*
Copyright (c) MagiCom Ltd. 2018
This software is licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
 
This example shows how to perform a periodic DRM Group check:
-set public key in the API, in this case we read it from a file
-start the periodic DRM Group check with 2sec period and wait for the results
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pidrm.h"
#include "callbackerrorcodes.h"

void DRMGroupSetPublicCallback()
{
	printf("DRM Group Public key set in API\n");
}

void DRMGroupPeriodicCheckStartCallback()
{
	printf("Periodic check started successfully\n");
}
void DRMGroupPeriodicCheckStopCallback()
{
	printf("Periodic check stopped successfully\n");
}


void DRMGroupGenunieCallback(uint32_t genuinecounter, uint32_t fakecounter, uint32_t errorcount)
{
	printf("GENUINE device detected, stats\n");
	printf("\tGenunie counter:%d\n", genuinecounter);
	printf("\tFake counter:%d\n", fakecounter);
	printf("\tError counter:%d\n", errorcount);
}
void DRMGroupFakeCallback(uint32_t genuinecounter, uint32_t fakecounter, uint32_t errorcount)
{
	printf("FAKE device detected, stats\n");
	printf("\tGenunie counter:%d\n", genuinecounter);
	printf("\tFake counter:%d\n", fakecounter);
	printf("\tError counter:%d\n", errorcount);
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
	uint8_t PublicKey[32];
	if (!pidrm_init(NULL)) //initialize the API and the device
	{
		printf("unable to init pidrm\n");
		return 0;
	}
	else
		printf("pidrm initialized successfully\n");
	FILE *fp = fopen("public_key.bin", "rb");
	if (fp != 0)
	{
		fread(PublicKey, 1, 32, fp);
		fclose(fp);

		pidrm_group_set_public_key(DRMGroupSetPublicCallback, error_handler, PublicKey, -1);
		pidrm_group_check_periodic_start(DRMGroupPeriodicCheckStartCallback, error_handler, DRMGroupGenunieCallback, DRMGroupFakeCallback, 2, -1);

		sleep(100);
		pidrm_check_periodic_stop(DRMGroupPeriodicCheckStopCallback, error_handler, -1);
	}
	else
		printf("Unable to open key file\n");
	if (pidrm_deinit())
		printf("pidrm successfully deinitalized, all resources are freed up\n");
	else
		printf("Unable to deinitalize pidrm\n");
	return 0;
}