#pragma once

// number of digits on 7-segment display
#define DIGITS 8

// pins unsed for SPI communication
#define SPI_CLK D5
#define SPI_MOSI D7
#define SPI_CS D8

// brightness setting (1-10)
#define BRIGHTNESS 2

// update rate in milliseconds
#define UPDATE_RATE_MS 60000

// WLAN credentials
const char* ssid = "your-wlan-ssid";
const char* password = "your-wlan-password";

// URL used for block time retrieval. it is assumed it delivers a plain number
const char* apiUrl = "https://blockstream.info/api/blocks/tip/height";
