#pragma once

#define TOTP_PERIOD_SECONDS 30
#define TOTP_DIGITS 6
#define REFRESH_MS 500

#define STORAGE_KEY "totp_hid_secret"
#define DEMO_PIN "1234"  // TODO: Implement secure PIN entry

#define MAX_SECRET_LEN 256
#define MAX_PIN_LEN 32
