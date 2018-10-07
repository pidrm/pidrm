
PiDRM
=====

Easy to use DRM for your Raspberry Pi.
The library at the moment is available as a static library, so it can be used in C and C++ projects.

## Main functionality 

 - DRM: Uses 256bit **Elliptic-curve cryptography**, it supports key generation with true random number generator. The private key can not be modified, thus it supports one to one relationship between software and hardware.
 - Group DRM: Similar to the simple DRM, but the private key can be set. This enables to use one copy of the software together with multiple PiDRM devices.
 - 4 General purpose AES channels: 256bit **Advanced Encryption Standard** in **Cipher Block Chaining** mode with 32byte long key and 16byte long initialization vector.

## Installation
As default all RPis use the UART interface as serial console, you have to disable it in order to use PiDRM. 
First open a terminal and enter:
```
sudo raspi-config
```
And select ***Interfacing options***, then  ***Serial*** and disable it.

Now the serial console is turned off, but the UART interface is also disabled. Enable it with editing ***/boot/config.txt***: add ***enable_uart=1*** at the end of the file.

If you have Raspberry Pi 2 or earlier you are done, but if you own a Pi 3 keep reading.
On Pi 3 the bluetooth uses the Serial0, and the console uses Serial1, we need to swap it in order to use the PiDRM correctly. Open  ***/boot/config.txt*** again and add ***`dtoverlay=pi3-miniuart-bt`*** at the end of the file.
Reboot your RPi and you are good to go.

The hardware has been set up, now you can focus on the software. Clone this git repository and take a look at the example codes. When you understand the structure of the library, you can use the library in your own code. 
You need to specify the following packages for the linker:
 - pthread
 - stdc++
 - pidrm

 For example:
```
 gcc pidrm_config.c -o pidrm_config.o -L. -lpidrm -lpthread -lstdc++
```
## Files
The ***example*** folder contains the example codes of the basic functionality.
The ***include*** folder has 2 files:

 - pidrm.h: This contains all the available functions
 - callbackerrorcodes.h: The error codes defined here
 
 The last is ***bin*** where you can find the static library (libpidrm.a).

## Available Functions
```
uint32_t pidrm_get_api_version();
```
Get the API version.
|Parameter   |Description                                   |
|------------|----------------------------------------------|
|Return value|Version of the API as an unsigned 32bit value.|

------
```
uint8_t pidrm_init(uint8_t *com_name);
```
Initialize PiDRM software and hardware.
|Parameter   |Description                                                                                                                              |
|------------|-----------------------------------------------------------------------------------------------------------------------------------------|
|com_name    |ASCII string that contains the name of the used serial port. If you use the default gpio header's UART, you can pass *NULL* as parameter.|
|Return value|In case of something fails return 0, otherwise 1                                                                                         |

------
```
uint8_t pidrm_deinit();
```
Deinitialize PiDRM software and hardware.
|Parameter   |Description                                      |
|------------|-------------------------------------------------|
|Return value|In case of something fails returns 0, otherwise 1|

------
```
uint8_t pidrm_get_id(CALLBACKGETID _callbackgetid, CALLBACKERROR _callbackerror, int32_t timeout);
```
Get the ID of the used PiDRM device.
|Parameter     |Description                                                                                                                                                      |                                                                                                                                                    |
|--------------|-------------                                                                                                                                                    |
|_callbackgetid|This callback is called when the ID query is successful, and the parameters that are passed to the callback contain the IDs                                      |
|_callbackerror|This callback is called when an error occured                                                                                                                    |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the ID query is started, 0 if not                                                                                                                           |

------
```
uint8_t pidrm_reset_keys(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Regenerate DRM keys(private,public), clears the read out protection bit.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_public_key_read_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Enable DRM public key read out protection.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|                                                                                                                                                 |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_get_public_key(CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
Read DRM public key if read out is not disabled.
|Parameter     |Description                                                                                                                                                      |
|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbackdata |This callback called when the public key query is successful, and the parameters that this callback have contains the public key                                 |
|_callbackerror|Called when error occured                                                                                                                                        |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_set_public_key(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, uint8_t *public_key, int32_t timeout);
```
Set DRM public key in the API. You have to set the key with this function before running checks.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|public_key      |Array of the 32 byte long public key                                                                                                                             |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_check(CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, CALLBACKERROR _callbackerror, int32_t timeout);
```
Run single DRM check.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbackgenuine|Called when genuine device is detected                                                                                                                           |
|_callbackfake   |Called when fake/not genuine device detected                                                                                                                     |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |
------
```
uint8_t pidrm_check_periodic_start(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, uint16_t period, int32_t timeout);
```
Start periodic DRM check.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the periodic check started successfully                                                                                                              |
|_callbackerror  |Called when error occured                                                                                                                                        |
|_callbackgenuine|Called when genuine device is detected                                                                                                                           |
|_callbackfake   |Called when fake device detected                                                                                                                                 |
|period          |Period of the DRM check in seconds                                                                                                                               |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_check_periodic_stop(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Stop the period DRM check.
|Parameter       |Description                                                                                                                                                      |
|----------------|-------------                                                                                                                                                    |
|_callbacksuccess|Called when the periodic check stopped successfully                                                                                                              |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_group_reset_keys(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Regenerate Group DRM keys(private,public), clears the read out protection bit.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_group_get_private_key(CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
Read out Group DRM private key if read out protection is not enabled.
|Parameter     |                                                                                                                                                                 |
|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbackdata |This callback is called when the private key query is successful, and the parameters of the callback contains the private key                                    |
|_callbackerror|Called when error occured                                                                                                                                        |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the operation has started, 0 if not                                                                                                                         |
------
```
uint8_t pidrm_group_set_private_key(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, uint8_t *private_key, int32_t timeout);
```
Set Group DRM private key in the hardware.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|private_key     |Array of the 32 byte long private key                                                                                                                            |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |


------
```
uint8_t pidrm_group_private_key_read_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Enable read out protection for Group DRM private key.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_group_private_key_write_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Enable write protection for Group DRM private key.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_group_get_public_key(CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
Read Group DRM public key if read out protection is not enabled.
|Parameter     |Description                                                                                                                                                      |
|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbackdata |This callback is called when the public key query is successful, and the parameters of the callback contains the public key                                      |
|_callbackerror|Called when error occured                                                                                                                                        |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_group_public_key_read_protect(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Enable read out protection for Group DRM public key.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |
------
```
uint8_t pidrm_group_set_public_key(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, uint8_t * public_key, int32_t timeout);
```
Set DRM Group public key in the API. You have to set the key using this function before running any checks.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|public_key      |Array of the 32 byte long public key                                                                                                                             |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_group_check(CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, CALLBACKERROR _callbackerror, int32_t timeout);
```
Run single Group DRM check.
|Parameter |Description                                                                                                                                                            |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbackgenuine|Called when genuine device is detected                                                                                                                           |
|_callbackfake   |Called when fake device detected                                                                                                                                 |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not																														   |
------
```
uint8_t pidrm_group_check_periodic_start(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, CALLBACKGENUINE _callbackgenuine, CALLBACKFAKE _callbackfake, uint16_t period, int32_t timeout);
```
Start periodic Group DRM checks.
|Parameter| Description                                                                                                                                                            |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the periodic checks started successfully                                                                                                             |
|_callbackerror  |Called when error occured                                                                                                                                        |
|_callbackgenuine|Called when genuine device is detected                                                                                                                           |
|_callbackfake   |Called when fake/not genuine device detected                                                                                                                     |
|period          |Period of the Group DRM checks in seconds                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |
------
```
uint8_t pidrm_group_check_periodic_stop(CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Stop periodic Group DRM checks.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|_callbacksuccess|Called when the periodic checks stopped successfully                                                                                                             |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |
------
```
uint8_t pidrm_aes_block_generate(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Reset AES channel(new key, new IV, protections disabled).
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch              |Channel number: 0-3                                                                                                                                              |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_aes_block_key_set(uint8_t ch, uint8_t *key, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Set the key of an AES channel.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch              |Channel number: 0-3                                                                                                                                              |
|key             |Public key to be set                                                                                                                                             |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_aes_block_key_get(uint8_t ch, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
Read the key of an AES channel.
|Parameter     |Description                                                                                                                                                        |
|--------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch            |Channel number: 0-3                                                                                                                                                |
|_callbackdata |This callback called when the key query is successful, and the parameters that this callback have contains the key                                                 |
|_callbackerror|Called when error occured                                                                                                                                          |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds  |
|Return value  |1 if the operation has started, 0 if not                                                                                                                           |

------
```
uint8_t pidrm_aes_block_key_disable_read(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Enable read out protection of an AES channel key.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch              |Channel number: 0-3                                                                                                                                              |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |


------
```
uint8_t pidrm_aes_block_key_disable_write(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Enable the write protection of an AES channel key.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch              |Channel number: 0-3                                                                                                                                              |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
Generate new new IV for an AES Channel.
```
uint8_t pidrm_aes_block_iv_generate(uint8_t ch, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch              |Channel number: 0-3                                                                                                                                              |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_aes_block_iv_set(uint8_t ch, uint8_t *iv, CALLBACKSUCCESS _callbacksuccess, CALLBACKERROR _callbackerror, int32_t timeout);
```
Set the IV of an AES channel.
|Parameter       |Description                                                                                                                                                      |
|----------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch              |Channel number: 0-3                                                                                                                                              |
|iv              |16 byte long array of IV                                                                                                                                         |
|_callbacksuccess|Called when the operation is successful                                                                                                                          |
|_callbackerror  |Called when error occured                                                                                                                                        |
|timeout         |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value    |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_aes_block_iv_get(uint8_t ch, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
Read the IV of an AES channel.
|Parameter     |Description                                                                                                                                                      |
|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch            |Channel number: 0-3                                                                                                                                              |
|_callbackdata |This callback called when the IV query is successful, the parameter of the callback contains the IV                                                              |
|_callbackerror|Called when error occured                                                                                                                                        |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the operation has started, 0 if not                                                                                                                         |

------
```
uint8_t pidrm_aes_block_encrypt(uint8_t ch, uint8_t *plain_text, uint16_t length, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
Encrypt data with AES Channel. It is strongly recommended to use 16 byte aligned data due to AES block size. The maximum input size is 32 kByte.
|Parameter     |Description                                                                                                                                                      |
|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch            |Channel number: 0-3                                                                                                                                              |
|plain_text    |Array of the data that will be encrypted                                                                                                                         |
|length        |Length of the data that will be encrypted                                                                                                                        |
|_callbackdata |This callback called when encryption is successful, the callback contains the encrypted data                                                                     |
|_callbackerror|Called when error occured                                                                                                                                        |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the operation has started, 0 if not                                                                                                                         |
------
```
uint8_t pidrm_aes_block_decrypt(uint8_t ch, uint8_t *ciper_text, uint16_t length, CALLBACKDATA _callbackdata, CALLBACKERROR _callbackerror, int32_t timeout);
```
|Decrypt data with AES Channel. You can only decrypt data that is 16 byte aligned. The maximum input size is 32 kByte.
|Parameter     |Description                                                                                                                                                      |
|--------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ch            |Channel number: 0-3                                                                                                                                              |
|ciper_text    |Array of the data that will be decrypted                                                                                                                         |
|length        |Length of the data that will be decrypted                                                                                                                        |
|_callbackdata |This callback called when decryption is successful, the callback returns the decrypted data                                                                      |
|_callbackerror|Called when error occured                                                                                                                                        |
|timeout       |This timeout controls how long the API should wait for the start of the transaction before it gives up. -1:forever,  0:try,greater than 0 : given in milliseconds|
|Return value  |1 if the operation has started, 0 if not                                                                                                                         |
------
## Licensing
Copyright (c) MagiCom Ltd. 2018

All source codes are licensed under the MIT license.
You may obtain a copy of the MIT license here: https://opensource.org/licenses/MIT

Binaries are free to use without modification.
