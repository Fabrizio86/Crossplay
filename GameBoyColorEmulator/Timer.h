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
    uint8_t getDivider() const;
    uint8_t getTimerCounter() const;
    uint8_t getTimerModulo() const;
    uint8_t getTimerControl() const;
    void setDivider(uint8_t divider);
    void setTimerCounter(uint8_t timer_counter);
    void setTimerModulo(uint8_t timer_modulo);
    void setTimerControl(uint8_t timer_control);

private:
    uint8_t divider;
    uint8_t timerCounter;
    uint8_t timerModulo;
    uint8_t timerControl;
};


#endif //TIMER_H
