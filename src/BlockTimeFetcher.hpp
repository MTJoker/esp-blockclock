#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

class BlockTimeFetcher
{
  public:
    explicit BlockTimeFetcher(const String &url);
    int fetchBlockTime();

  private:
    String apiUrl;
    WiFiClient wifiClient;
    WiFiClientSecure wifiClientSecure;

    std::unique_ptr<WiFiClient> wifi;

    int parseBlockTime(const String &payload);
};