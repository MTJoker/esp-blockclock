#include "DisplayManager.hpp"

DisplayManager::DisplayManager()
{
    ledController.setIntensity(DISPLAY_BRIGHTNESS);
    ledController.clearMatrix();
}

void DisplayManager::showConnectingAnimation()
{
    ledController.setChar(0, animationDigit, '.', false);
    animationDigit = (animationDigit + 1) % DISPLAY_DIGITS;

    if (animationDigit == 0)
    {
        clear();
    }

    delay(DISPLAY_ANIMATION_DELAY);
}

void DisplayManager::showBlockTime(int blockTime)
{
    auto blockStr = String(blockTime);
    int len = std::min(blockStr.length(), DISPLAY_DIGITS);
    int rightIndex = (DISPLAY_DIGITS - len) / 2;
    int leftIndex = rightIndex + len - 1;

    for (int i = rightIndex; i <= leftIndex; i++)
    {
        ledController.setChar(0, i, '-', false);
        delay(DISPLAY_ANIMATION_DELAY);
    }

    for (int i = leftIndex, j = 0; i >= rightIndex; i--, j++)
    {
        ledController.setChar(0, i, blockStr[j], false);
        delay(DISPLAY_ANIMATION_DELAY);
    }
}

void DisplayManager::clear()
{
    animationDigit = 0;
    ledController.clearMatrix();
}