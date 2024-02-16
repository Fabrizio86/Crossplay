//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "Clock.h"

#include <thread>

using namespace std;

constexpr double MICROSECONDS_IN_SECOND = 1000000.0;

void Clock::start() {
    this->running = true;

    thread clkT([this]() {
        while (this->running) {
            this->cpu->exec();
            std::this_thread::sleep_for(std::chrono::microseconds(this->freq));

        }
    });

    clkT.detach();
}

Clock::Clock(double freqMHz, ICpu *cpu) : cpu(cpu) {
    this->freq = static_cast<unsigned int>(1.0 / freqMHz * MICROSECONDS_IN_SECOND);
}

void Clock::stop() {
    this->running = false;
}
