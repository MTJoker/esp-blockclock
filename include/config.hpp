#pragma once

// number of digits on 7-segment display
#define DIGITS 8

// pins used for SPI communication
#define SPI_CLK D5
#define SPI_MOSI D7
#define SPI_CS D8

// brightness setting (1-10)
#define BRIGHTNESS 2

// update rate in milliseconds
#define UPDATE_RATE_MS 60000

// delay between updating each segment
constexpr int animationDelay = 150;

// WLAN credentials
const char* ssid = "your-wlan-ssid";
const char* password = "your-wlan-password";

// URL used for block time retrieval
// It can either deliver a plain text with the block height, or a JSON object which has a "height" member
const char* apiUrl = "https://blockstream.info/api/blocks/tip/height";
//const char* apiUrl = "https://blockchain.info/q/getblockcount";
//const char* apiUrl = "https://api.blockcypher.com/v1/btc/main";
//const char* apiUrl = "https://api.bitaps.com/btc/v1/blockchain/block/last";
//const char* apiUrl = "http://192.168.172.35:3020/api/blocks/tip";