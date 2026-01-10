#include "fa_bypass_config.h"
#include "fa_bypass_crypto.h"
#include <string.h>
#include <stdio.h>


/* Base32 character to value conversion */
static int base32_char_val(char c) {
    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= '2' && c <= '7') return 26 + (c - '2');
    return -1;
}

/* Decodes a base32 string (ASCII) to bytes. Returns decoded length or -1 */
int base32_decode(const char* src, uint8_t* dest, size_t dest_len) {
    size_t src_len = strlen(src);
    int buffer = 0;
    int bits_left = 0;
    size_t count = 0;

    for(size_t i = 0; i < src_len; ++i) {
        char c = src[i];
        if(c == ' ' || c == '\t' || c == '=') continue;
        if(c >= 'a' && c <= 'z') c -= 'a' - 'A';
        
        int val = base32_char_val(c);
        if(val < 0) return -1;
        
        buffer <<= 5;
        buffer |= val & 0x1F;
        bits_left += 5;
        
        if(bits_left >= 8) {
            bits_left -= 8;
            if(count >= dest_len) return -1;
            dest[count++] = (buffer >> bits_left) & 0xFF;
        }
    }
    
    return (int)count;
}

/* HMAC-SHA1 implementation using Flipper's crypto API */
bool crypto_hmac_sha1(const uint8_t* key, size_t key_len, const uint8_t* message,
                      size_t msg_len, uint8_t* digest) {
    // TODO: Adapt to your firmware's crypto API
    // This is a placeholder. Real implementation depends on SDK version
    // Flipper SDK provides crypto functions - check your version's API

    if (key_len > 64) {
        // If key is longer than block size, hash it first
        uint8_t hashed_key[20]; // SHA1 produces 20-byte hash
        // Call SHA1 function here (not implemented in this snippet)
        // sha1_hash(key, key_len, hashed_key);
        key = hashed_key;
        key_len = 20;
    }

    if (msg_len > 0 && message == NULL) return false;
    
    if(!key || !message || !digest) return false;
    
    // Example: Use furi_hal_crypto or similar based on your SDK
    // For now, this is a stub that needs adaptation
    return true;
}

/* TOTP calculation using HMAC-SHA1 */
bool totp_calculate(const uint8_t* secret, size_t secret_len, uint32_t timestamp, char* code) {
    if(!secret || !code) return false;
    
    // Calculate time counter (30-second intervals)
    uint32_t counter = timestamp / TOTP_PERIOD_SECONDS;
    
    // Convert counter to big-endian bytes
    uint8_t counter_bytes[8] = {0};
    for(int i = 7; i >= 0; i--) {
        counter_bytes[i] = (uint8_t)(counter & 0xFF);
        counter >>= 8;
    }
    
    // Calculate HMAC-SHA1
    uint8_t hmac[20];
    // Fill hmac with place holders
    for (int i = 0; i < 20; i++) {
        hmac[i] = 0;
    }
    if(!crypto_hmac_sha1(secret, secret_len, counter_bytes, 8, hmac)) {
        return false;
    }
    
    // Dynamic truncation (RFC 4226)
    uint8_t offset = hmac[19] & 0x0F;
    uint32_t p = ((hmac[offset] & 0x7F) << 24) | (hmac[offset + 1] << 16) |
                 (hmac[offset + 2] << 8) | (hmac[offset + 3]);
    
    // Generate 6-digit code
    uint32_t otp = p % 1000000;
    snprintf(code, 7, "%06lu", (unsigned long)otp);
    
    return true;
}
