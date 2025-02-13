#include <Arduino.h>

#include "BlockTimeFetcher.hpp"
#include "DisplayManager.hpp"
#include <ESP8266WiFi.h>

BlockTimeFetcher blockTimeFetcher(apiUrl);
DisplayManager displayManager;

int lastBlockTime = 0;
unsigned long lastMillis = millis() - BLOCKTIME_UPDATE_RATE_MS;

void setupWifi()
{
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WLAN...");

    while (WiFi.status() != WL_CONNECTED)
    {
        displayManager.showConnectingAnimation();
        Serial.print(".");
    }

    displayManager.clear();

    Serial.println(" connected!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
}

void setup()
{
    Serial.begin(115200);
    setupWifi();
}

void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastMillis >= BLOCKTIME_UPDATE_RATE_MS)
    {
        lastMillis = currentMillis;

        if (WiFi.status() != WL_CONNECTED)
        {
            setupWifi();
        }

        auto currentBlockTime = blockTimeFetcher.fetchBlockTime();

        if ((currentBlockTime >= 0) && (currentBlockTime != lastBlockTime))
        {
            Serial.println("New BlockTime: " + String(currentBlockTime));
            displayManager.showBlockTime(currentBlockTime);
            lastBlockTime = currentBlockTime;
        }
    }
}
