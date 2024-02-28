//
// Created by Fabrizio Paino on 2024-02-28.
//

#include "Timer.h"

uint8_t Timer::divider1() const
{
    return divider;
}

uint8_t Timer::timer_counter() const
{
    return timerCounter;
}

uint8_t Timer::timer_modulo() const
{
    return timerModulo;
}

uint8_t Timer::timer_control() const
{
    return timerControl;
}
