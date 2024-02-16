//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_CLOCK_H
#define CROSSPLAY_CLOCK_H

#include "Definitions.h"
#include "Interfaces/ICpu.h"

class Clock {
public:
    ~Clock() = default;

    Clock(double freqMHz, ICpu *cpu);

    void start();

    void stop();

private:
    bool running;
    uint freq;
    ICpu *cpu;

};

#endif //CROSSPLAY_CLOCK_H
