#pragma once

#include "LedController.hpp"
#include "config.hpp"
#include <memory>

class DisplayManager
{
  public:
    DisplayManager();

    void showConnectingAnimation();
    void showBlockTime(int blockTime);
    void clear();

  private:
    LedController<DISPLAY_DIGITS, 1> ledController = LedController<DISPLAY_DIGITS, 1>(SPI_MOSI, SPI_CLK, SPI_CS);
    int animationDigit = 0;
};