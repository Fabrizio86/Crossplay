//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_CLOCK_H
#define CROSSPLAY_CLOCK_H

#include <condition_variable>
#include <mutex>

#include "Definitions.h"
#include "Interfaces/ICpu.h"
#include "Interfaces/IPPU.h"

class Clock {
public:
    ~Clock() = default;

    Clock(double freqMHz, ICpu *cpu, IPPU *ppu);

    void start();

    void stop();

private:
    bool running;
    uint freq;
    ICpu *cpu;
    IPPU *ppu;

    bool cpuCycleCompleted = false;
    std::mutex m;
    std::condition_variable cv;

};

#endif //CROSSPLAY_CLOCK_H
