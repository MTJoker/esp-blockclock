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
      displayBlockTimeWithAnimation(currentBlockTime);
      lastBlockTime = currentBlockTime;
    }
  } else {
    Serial.println("Error getting BlockTime: " + String(httpCode));
  }

  http.end();
}

void displayBlockTimeWithAnimation(int blockTime) {
  auto blockStr = String(blockTime);
  const auto len = blockStr.length();

  const auto rightIndex = (DIGITS - len) / 2;
  const auto leftIndex = rightIndex + len - 1;

  for (int i = rightIndex; i <= leftIndex; i++) {
    lc.setChar(0, i, '-', false); 
    delay(150);
  }

  for (int i = leftIndex; i >= rightIndex; i--) {
    auto value = blockStr[len - i];
    lc.setChar(0, i, value, false);
    delay(150);
  }
}
