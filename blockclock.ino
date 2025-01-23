#include "config.hpp"
#include <LedController.hpp>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

int lastBlockTime = 0;
unsigned long lastMillis = millis() - UPDATE_RATE_MS;
LedController<DIGITS, 1> lc(SPI_MOSI, SPI_CLK, SPI_CS);
int connectingAnimationDigit = 0;
WiFiClient wifiClient;
WiFiClientSecure wifiClientSecure;

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
    displayConnectingAnimation();
    Serial.print(".");
  }
  connectingAnimationDigit = 0;
  lc.clearMatrix();

  Serial.println(" connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  wifiClientSecure.setInsecure();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= UPDATE_RATE_MS) {
    lastMillis = currentMillis;

    if (WiFi.status() != WL_CONNECTED) {
      setupWifi();
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
  HTTPClient http;
  int blockTime = -1;

  if (http.begin(*getWifiClient(), apiUrl)) {
    const int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      const auto payload = http.getString();
      if (payload.length() > 0) {
        blockTime = getBlockTimeFromPayload(payload);
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

WiFiClient* getWifiClient() {
  if (String(apiUrl).startsWith("https://")) {
    return &wifiClientSecure;
  } else {
    return &wifiClient;
  }
}

int getBlockTimeFromPayload(String payload) {
  int blockTime = -1;

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);

  if (!error && doc.is<JsonObject>())  // check if payload was JSON
  {
    if (doc.containsKey("height")) {
      blockTime = doc["height"];
    } else if (doc.containsKey("data") && doc["data"].containsKey("height")) {
      blockTime = doc["data"]["height"];
    } else {
      Serial.println("Unknown JSON structure");
    }
  } else {
    blockTime = payload.toInt();
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
    delay(animationDelay);
  }

  for (int i = leftIndex, digitIndex = 0; i >= rightIndex; i--, digitIndex++) {
    auto value = blockStr[digitIndex];
    lc.setChar(0, i, value, false);
    delay(animationDelay);
  }
}

void displayConnectingAnimation() {
  lc.setChar(0, connectingAnimationDigit, '.', false);
  connectingAnimationDigit++;

  if (connectingAnimationDigit >= DIGITS) {
    connectingAnimationDigit = 0;
    lc.clearMatrix();
  }

  delay(animationDelay);
}
