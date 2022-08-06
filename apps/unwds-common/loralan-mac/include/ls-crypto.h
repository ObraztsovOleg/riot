/*
 * Copyright (C) 2016-2018 Unwired Devices LLC <info@unwds.com>

 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * @defgroup
 * @ingroup
 * @brief
 * @{
 * @file        ls_crypto.h
 * @brief       Cryptographic routines definitions
 * @author      Eugene Ponomarev
 */
#ifndef LS_CRYPTO_H_
#define LS_CRYPTO_H_

#include "crypto/aes.h"
#include "ls-mac-types.h"

#define LS_MIC_KEY_LEN AES_KEY_SIZE

/**
 * @brief Cryptography settings for the device.
 */
typedef struct {
	uint8_t mic_key[LS_MIC_KEY_LEN];
	uint8_t aes_key[AES_KEY_SIZE];
	uint8_t join_key[AES_KEY_SIZE];
} ls_crypto_t;

/**
 * @brief Calculates Message Integrity Code for the specified frame
 *
 * @param	[IN]	ls		the ls stack state
 * @param	[IN]	frame	frame for which the MIC will be calculated
 *
 * @return MIC for the specified frame
 */
ls_mic_t ls_calculate_mic(uint8_t *key, ls_frame_t *frame, uint8_t payload_size);

/**
 * @brief Validates Message Integrity Code for the specified frame
 *
 * @param	[IN]	ls		the ls stack state including cryptographic keys
 * @param	[IN]	frame	frame for which the MIC will be validated
 *
 * @return true if MIC is valid, false otherwise
 */
bool ls_validate_frame_mic(uint8_t *key, ls_frame_t *frame);

/**
 * @brief Encrypts payload of the specified frame with specified key.
 *
 * @param	[IN]	*key		cryptographic key
 * @param	[IN]	*frame		pointer to the frame to encrypt it's payload
 *
 */
void ls_encrypt_frame_payload(uint8_t *key, ls_frame_t *frame);

/**
 * @brief Decrypts payload of the specified frame with specified key.
 *
 * @param	[IN]	*key		cryptographic key
 * @param	[IN]	*frame		pointer to the frame to decrypt it's payload
 *
 */
void ls_decrypt_frame_payload(uint8_t *key, ls_frame_t *frame);

/**
 * @brief Encrypts frame payload and calculates frame's MIC
 *
 * @param	[IN]	*lsc		the ls stack state including cryptographic keys
 * @param	[IN]	*frame		the frame to work with
 * @param	[OUT]	*newsize	new size of payload (resizes after encryption)
 */
void ls_encrypt_frame(uint8_t *key_mic, uint8_t *key_aes, ls_frame_t *frame, size_t *newsize);

/**
 * @brief Derives keys from the nonce numbers
 *
 * @param	[IN]	dev_nonce	the device nonce number
 * @param	[IN]	app_nonce	the application nonce number
 * @param	[IN]	addr		device address
 * @param	[OUT]	*key_mic	key for the MIC calculation
 * @param	[OUT]	*key_aes	key for the AES encryption
 */
void ls_derive_keys(uint32_t dev_nonce, uint32_t app_nonce, ls_addr_t addr, uint8_t *key_mic, uint8_t *key_aes);

#endif /* LS_CRYPTO_H_ */
