//
// Created by Fabrizio Paino on 2024-02-28.
//

#include "Timer.h"

void Timer::resetDivider()
{
    this->divider = 0;
}

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

void Timer::setDivider(uint8_t divider)
{
    this->divider = divider;
}

void Timer::setTimerCounter(uint8_t timer_counter)
{
    timerCounter = timer_counter;
}

void Timer::setTimerModulo(uint8_t timer_modulo)
{
    timerModulo = timer_modulo;
}

void Timer::setTimerControl(uint8_t timer_control)
{
    timerControl = timer_control;
}
