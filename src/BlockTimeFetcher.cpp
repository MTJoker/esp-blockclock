#include "BlockTimeFetcher.hpp"
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

BlockTimeFetcher::BlockTimeFetcher(const String &url) : apiUrl(url)
{
    if (apiUrl.startsWith("https://"))
    {
        wifiClientSecure.setInsecure();
        wifi = std::make_unique<WiFiClientSecure>(wifiClientSecure);
    }
    else
    {
        wifi = std::make_unique<WiFiClient>(wifiClient);
    }
}

int BlockTimeFetcher::fetchBlockTime()
{
    HTTPClient http;
    int blockTime = -1;

    if (http.begin(*wifi, apiUrl))
    {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK)
        {
            blockTime = parseBlockTime(http.getString());
        }
        else
        {
            Serial.printf("HTTP Error: %d\n", httpCode);
        }
        http.end();
    }
    else
    {
        Serial.println("API Connection failed!");
    }
    return blockTime;
}

int BlockTimeFetcher::parseBlockTime(const String &payload)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error && doc.is<JsonObject>()) // check if payload was JSON
    {
        if (doc["height"].is<int>())
        {
            return doc["height"];
        }

        if (doc["data"]["height"].is<int>())
        {
            return doc["data"]["height"];
        }

        Serial.println("Unexpected JSON format.");
        return -1;
    }
    else
    {
        return payload.toInt();
    }
}