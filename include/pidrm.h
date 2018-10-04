/*
Copyright (c) MagiCom Ltd. 2018
 This software is licensed under the MIT license.
 You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT
 
*/

#ifndef PIDRM_H
#define PIDRM_H

#include <stdint.h>
typedef void(*CALLBACKGETSTATE)(uint8_t busy);
typedef void(*CALLBACKGETID)(uint32_t unique_id, uint16_t manufacturer_id, uint16_t communication_id, uint16_t product_id);
typedef void(*CALLBACKDATA)(uint8_t *outbuffer, uint32_t len);
typedef void(*CALLBACKSUCCESS)(void);
typedef void(*CALLBACKERROR)(uint32_t errorcode);
typedef void(*CALLBACKGENUINE)(uint32_t genuinecounter, uint32_t fakecounter, uint32_t errorcount);
typedef void(*CALLBACKFAKE)(uint32_t genuinecounter, uint32_t fakecounter, uint32_t errorcount);

#ifdef __cplusplus
extern "C" {
#endif

	//Get API version
	uint32_t pidrm_get_api_version();

	//Initialize the hardware and the software
	uint8_t pidrm_init(uint8_t *com_name);
	//Free up all reserved resources by the API
	uint8_t pidrm_deinit();

	//Read the ID of the hardware
	uint8_t pidrm_get_id(CALLBACKGETID _callbackgetid, CALLBACKERROR _callbackerror, int32_t timeout);

	//Regenerate ECC keys(private,public), clears resets the read out protection bit
	uint8_t pidrm_reset_keys(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Enable ECC public key read out protection
	uint8_t pidrm_public_key_read_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Read ECC public key if read out is not disabled
	uint8_t pidrm_get_public_key(CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
	//Set ECC public key in the API
	uint8_t pidrm_set_public_key(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, uint8_t *public_key, int32_t timeout);
	//Run ECC check
	uint8_t pidrm_check(CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, CALLBACKERROR _callbackerror, int32_t timeout);
	//Starts periodic ECC check (period is in sec)
	uint8_t pidrm_check_periodic_start(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, uint16_t period, int32_t timeout);
	//Stops the period ECC check
	uint8_t pidrm_check_periodic_stop(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);

	//Regenerate Group ECC keys(private,public), clears resets the read out protection bit
	uint8_t pidrm_group_reset_keys(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Read out Group ECC private key if read out protection is not enabled
	uint8_t pidrm_group_get_private_key(CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
	//Set Group ECC private key in the hardware
	uint8_t pidrm_group_set_private_key(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, uint8_t *private_key, int32_t timeout);
	//Enable read protectiond for Group ECC private key
	uint8_t pidrm_group_private_key_read_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Enable write protection for Group ECC private key
	uint8_t pidrm_group_private_key_write_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Read out Group ECC public key if read out protection is not enabled
	uint8_t pidrm_group_get_public_key(CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
	//Enable read protectiond for Group ECC public key
	uint8_t pidrm_group_public_key_read_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Set Group ECC public key in the API
	uint8_t pidrm_group_set_public_key(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, uint8_t * public_key, int32_t timeout);
	//Run Group ECC check
	uint8_t pidrm_group_check(CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, CALLBACKERROR _callbackerror, int32_t timeout);
	//Start periodic Group ECC check (period is in sec)
	uint8_t pidrm_group_check_periodic_start(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, uint16_t period, int32_t timeout);
	//Stop periodic Group ECC check
	uint8_t pidrm_group_check_periodic_stop(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);



	//Reset AES channel(new key, new IV, protections disabled)
	uint8_t pidrm_aes_block_generate(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Set AES channel's key
	uint8_t pidrm_aes_block_key_set(uint8_t ch, uint8_t *key, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Read AES channel's key
	uint8_t pidrm_aes_block_key_get(uint8_t ch, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
	//Enable AES channel key's read protection
	uint8_t pidrm_aes_block_key_disable_read(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Enable AES channel key's write protection
	uint8_t pidrm_aes_block_key_disable_write(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Generate new new IV for AES Channel
	uint8_t pidrm_aes_block_iv_generate(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Set AES channel's IV
	uint8_t pidrm_aes_block_iv_set(uint8_t ch, uint8_t *iv, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
	//Read AES channel's IV
	uint8_t pidrm_aes_block_iv_get(uint8_t ch, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
	//Encrypt data with AES Channel
	uint8_t pidrm_aes_block_encrypt(uint8_t ch, uint8_t *plain_text, uint16_t lenght, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
	//Decrypt data with AES Channel
	uint8_t pidrm_aes_block_decrypt(uint8_t ch, uint8_t *ciper_text, uint16_t lenght, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);

	
#ifdef __cplusplus
}
#endif

#endif /*PIDRM_H*/