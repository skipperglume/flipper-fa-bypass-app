# FA Bypass App

A Flipper Zero TOTP + HID application for safe two-factor authentication bypass with explicit user confirmation before sending codes via USB HID.

## Features

- **TOTP Code Generation**: Generates 6-digit TOTP codes from Base32-encoded secrets
- **HID Support**: Can send TOTP codes via USB HID keyboard emulation
- **Safe by Design**: Requires explicit user confirmation (OK button) before sending any codes
- **Real-time Updates**: Displays remaining time until code refresh
- **Educational Focus**: Well-structured, modular code suitable for learning

## File Structure

```
fa_bypass_app/
├── main.c                     # Entry point
├── application.fam            # Flipper app manifest
├── fa_bypass_app.h           # Main app header
├── fa_bypass_app.c           # Main app implementation
├── fa_bypass_main_view.h     # Main view header
├── fa_bypass_main_view.c     # Main view (GUI drawing/input)
├── fa_bypass_crypto.h        # Crypto utilities header
├── fa_bypass_crypto.c        # Crypto implementation
├── fa_bypass_config.h        # Configuration constants
└── README.md                 # This file
```

## Implementation Details

### Modules

#### `fa_bypass_config.h`
Configuration constants including TOTP period, number of digits, and storage keys.

#### `fa_bypass_crypto.c/h`
Cryptographic functions:
- **`base32_decode()`**: Decodes RFC4648 Base32-encoded secrets to bytes
- **`totp_calculate()`**: Generates TOTP codes using HMAC-SHA1 and dynamic truncation (RFC 4226)
- **`crypto_hmac_sha1()`**: Stub for HMAC-SHA1 (requires SDK adaptation)

#### `fa_bypass_main_view.c/h`
GUI view for displaying:
- Current TOTP code in large digits
- Time remaining until code expiration
- Instructions for sending code via HID

Handles input events (OK button to send, Back to exit).

#### `fa_bypass_app.c/h`
Main application logic:
- Initializes GUI and notification systems
- Spawns background thread for TOTP code generation
- Manages app lifecycle and cleanup

## Building

1. Place this directory in your Flipper firmware's `applications_user/` folder
2. Build with your normal Furi build system:
   ```bash
   ./fbt build
   ```

## Security Notes

⚠️ **IMPORTANT**: This is an educational example. For production use:

1. **Secret Storage**: Secrets should be encrypted on-disk and never stored in plaintext
2. **PIN Protection**: Implement secure PIN entry (hash-based, not hardcoded)
3. **USB HID**: Only enable USB HID after explicit user confirmation and PIN verification
4. **Permissions**: Check your organization's policies before using USB HID features
5. **Crypto Adaptation**: The `crypto_hmac_sha1()` function is a stub and must be adapted to your SDK version

## TODO Items

- [ ] Implement secure PIN entry and verification
- [ ] Add encrypted storage for TOTP secrets
- [ ] Adapt `crypto_hmac_sha1()` to your Flipper SDK version
- [ ] Implement USB HID keyboard emulation for sending codes
- [ ] Add confirmation dialog before HID transmission
- [ ] Add multiple secret management (add/edit/delete)
- [ ] Implement time sync with NTP server

## References

- [RFC 4226 - HOTP](https://tools.ietf.org/html/rfc4226)
- [RFC 6238 - TOTP](https://tools.ietf.org/html/rfc6238)
- [RFC 4648 - Base Encoding Data Formats](https://tools.ietf.org/html/rfc4648)
- [Flipper Zero Developer Documentation](https://flipper.readthedocs.io/)

## License

Educational/Example code - use at your own risk.
## Behind the app:


flipper-fa-bypass-app

## Compiling and Flashing: 

```bash
python3 -m venv ufbtVenv
. ufbtVenv/bin/activate
python3 -m pip install --upgrade ufbt
ufbt vscode_dist # Set vscode paths
ufbt launch # Flash the new app
ufbt build

ufbt launch APPID=fa_bypass
fa_bypass

UFBT_APP_DIR:


fap_{APPID}

ufbt launch VERBOSE=True

ufbt --help VERBOSE=True

ufbt --help VERBOSE=True UFBT_APP_DIR='/home/lofu/flipper/fa_bypass_app'

ufbt build VERBOSE=True UFBT_APP_DIR='/home/lofu/flipper/fa_bypass_app'

ufbt build  UFBT_APP_DIR='/home/lofu/flipper/fa_bypass_app'


ufbt launch

```

## TO seeFURI Logs:
```bash
ufbt cli
```



### Usefull links:
 - https://developer.flipper.net/flipperzero/doxygen/index.html
 - https://etyp.dev/posts/flipper-into-hardware-p1/
 - https://github.com/jamisonderek/flipper-zero-tutorials/wiki/User-Interface







### Check the connected serial ports:
```bash
ls /dev/serial/by-id/
screen /dev/serial/by-id/[...]
```


