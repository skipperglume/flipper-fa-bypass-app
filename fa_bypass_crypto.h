#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Decode base32 string to bytes (RFC4648)
 * @param src Base32 encoded string
 * @param dest Destination buffer for decoded bytes
 * @param dest_len Maximum size of destination buffer
 * @return Number of decoded bytes, or -1 on error
 */
int base32_decode(const char* src, uint8_t* dest, size_t dest_len);

/**
 * @brief Calculate TOTP code from secret
 * @param secret Secret bytes
 * @param secret_len Length of secret
 * @param timestamp Current timestamp (seconds since epoch)
 * @param code Output buffer for 6-digit code (must be at least 7 bytes for null terminator)
 * @return true on success, false on failure
 */
bool totp_calculate(const uint8_t* secret, size_t secret_len, uint32_t timestamp, char* code);

/**
 * @brief Generate HMAC-SHA1 digest
 * @param key Secret key
 * @param key_len Length of key
 * @param message Message to digest
 * @param msg_len Length of message
 * @param digest Output buffer (must be at least 20 bytes)
 * @return true on success, false on failure
 */
bool crypto_hmac_sha1(const uint8_t* key, size_t key_len, const uint8_t* message,
                      size_t msg_len, uint8_t* digest);
