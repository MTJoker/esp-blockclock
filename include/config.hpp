#pragma once

// pins used for SPI communication with display
constexpr uint8_t SPI_CLK = D5;
constexpr uint8_t SPI_MOSI = D7;
constexpr uint8_t SPI_CS = D8;

// number of digits on 7-segment display
constexpr unsigned DISPLAY_DIGITS = 8;

// brightness setting of display (1-10)
constexpr uint8_t DISPLAY_BRIGHTNESS = 2;

// delay between updating each segment of display
constexpr unsigned DISPLAY_ANIMATION_DELAY = 150;

// update rate in milliseconds
constexpr unsigned BLOCKTIME_UPDATE_RATE_MS = 60000;

// WLAN credentials
inline const char *ssid = "your-wlan-ssid";
inline const char *password = "your-wlan-password";

// URL used for block time retrieval
// It can either deliver a plain text with the block height, or a JSON object which has a "height"
// member
// inline const char* apiUrl = "https://blockstream.info/api/blocks/tip/height";
// inline const char* apiUrl = "https://blockchain.info/q/getblockcount";
// inline const char* apiUrl = "https://api.blockcypher.com/v1/btc/main";
// inline const char* apiUrl = "https://api.bitaps.com/btc/v1/blockchain/block/last";
inline const char *apiUrl = "http://192.168.172.35:3020/api/blocks/tip";