#include "config.hpp"
#include <LedController.hpp>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

int lastBlockTime = 0;
unsigned long lastMillis = millis() - UPDATE_RATE_MS;
LedController<DIGITS, 1> lc(SPI_MOSI, SPI_CLK, SPI_CS);

void setup() {
  Serial.begin(115200);

  setupDisplay();
  setupWifi();
}

void setupDisplay() {
  lc.setIntensity(BRIGHTNESS);
  lc.clearMatrix();
}

void setupWifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WLAN...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= UPDATE_RATE_MS) {
    lastMillis = currentMillis;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconnecting to WLAN...");
      setupWifi();
      return;
    }

    auto currentBlockTime = getBlockTime();
    if ((currentBlockTime >= 0) && (currentBlockTime != lastBlockTime)) {
      Serial.println("New BlockTime: " + currentBlockTime);
      displayBlockTimeWithAnimation(currentBlockTime);
      lastBlockTime = currentBlockTime;
    }
  }
}

int getBlockTime() {
  WiFiClientSecure wifiClient;
  wifiClient.setInsecure();

  HTTPClient http;
  int blockTime = -1;

  if (http.begin(wifiClient, apiUrl)) {
    const int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      const auto payload = http.getString();
      if (payload.length() > 0) {
        blockTime = payload.toInt();
      } else {
        Serial.println("Received empty payload.");
      }
    } else {
      Serial.println("HTTP error: " + String(httpCode));
    }
    http.end();
  } else {
    Serial.println("Failed to connect to API.");
  }

  return blockTime;
}

void displayBlockTimeWithAnimation(int blockTime) {
  auto blockStr = String(blockTime);
  auto len = blockStr.length() <= DIGITS ? blockStr.length() : DIGITS;

  const auto rightIndex = (DIGITS - len) / 2;
  const auto leftIndex = rightIndex + len - 1;

  for (int i = rightIndex; i <= leftIndex; i++) {
    lc.setChar(0, i, '-', false);
    delay(150);
  }

  for (int i = leftIndex, digitIndex = 0; i >= rightIndex; i--, digitIndex++) {
    auto value = blockStr[digitIndex];
    lc.setChar(0, i, value, false);
    delay(150);
  }
}
