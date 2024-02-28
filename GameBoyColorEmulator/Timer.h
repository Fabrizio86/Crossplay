//
// Created by Fabrizio Paino on 2024-02-28.
//

#ifndef TIMER_H
#define TIMER_H
#include <cstdint>

class Timer
{
public:
    void resetDivider();

    uint8_t divider1() const;

    uint8_t timer_counter() const;

    uint8_t timer_modulo() const;

    uint8_t timer_control() const;

private:
    uint8_t divider;
    uint8_t timerCounter;

    uint8_t timerModulo;
    uint8_t timerControl;
};


#endif //TIMER_H
