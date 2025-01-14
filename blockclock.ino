#include "config.hpp"
#include <LedController.hpp>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>

int lastBlockTime = 0;
unsigned long lastMillis = millis() - UPDATA_RATE_MS;
LedController<DIGITS, 1> lc;
WiFiClientSecure wifiClient;

void setup() {
  Serial.begin(115200);

  setupDisplay();
  setupWifi();
  setupOTA();
}

void setupDisplay() {
  lc = LedController<DIGITS, 1>(SPI_MOSI, SPI_CLK, SPI_CS);
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

  wifiClient.setInsecure();
}

void setupOTA() {
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
  });

  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();

  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= UPDATA_RATE_MS) {
    lastMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      updateBlockTime();
    } else {
      Serial.println("No WLAN connection");
    }
  }
}

void updateBlockTime() {
  HTTPClient http;

  if (!http.begin(wifiClient, apiUrl)) {
    Serial.println("Failed to connect to API.");
    return;
  }

  const int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    const auto payload = http.getString();
    const auto currentBlockTime = payload.toInt();

    Serial.println("BlockTime: " + payload);

    if (currentBlockTime != lastBlockTime) {
      displayAnimation();

      displayBlockTime(currentBlockTime);
      lastBlockTime = currentBlockTime;
    }
  } else {
    Serial.println("Error getting BlockTime: " + String(httpCode));
  }

  http.end();
}

void displayBlockTime(int blockTime) {
  const auto blockStr = String(blockTime);
  const auto len = blockStr.length();
  const auto maxDigits = DIGITS - 1;  // max allowed len (first position stays empty)

  lc.setChar(0, maxDigits, ' ', false);

  for (auto i = 0; i < len && i < maxDigits; i++) {
    auto displayPos = maxDigits - i - 1;
    auto digit = blockStr[i];
    lc.setChar(0, displayPos, digit, false);
    delay(150);
  }

  if (len < maxDigits) {
    lc.setChar(0, 0, ' ', false);
  }
}

void displayAnimation() {
  // Move the "-" character from right to left
  for (int i = 0; i < DIGITS; i++) {
    lc.setChar(0, i, '-', false);
    delay(150);
  }
}
