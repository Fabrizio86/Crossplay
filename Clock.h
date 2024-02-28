//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_CLOCK_H
#define CROSSPLAY_CLOCK_H

#include "Definitions.h"
#include "Interfaces/ICpu.h"
#include "Interfaces/IPPU.h"

class Clock
{
public:
    ~Clock() = default;

    Clock(double cpuFreqMHz, double ppuFreqMHz, ICpu* cpu, IPPU* ppu);

    void start();

    void stop();

private:
    bool running;
    uint cpuFreq;
    uint ppuFreq;
    ICpu* cpu;
    IPPU* ppu;
};

#endif //CROSSPLAY_CLOCK_H
