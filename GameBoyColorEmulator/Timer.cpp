//
// Created by Fabrizio Paino on 2024-02-28.
//

#include "Timer.h"

uint8_t Timer::getDivider() const
{
    return divider;
}

uint8_t Timer::getTimerCounter() const
{
    return timerCounter;
}

uint8_t Timer::getTimerModulo() const
{
    return timerModulo;
}

uint8_t Timer::getTimerControl() const
{
    return timerControl;
}
