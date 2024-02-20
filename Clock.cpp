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
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock);

            this->cpu->exec();

        }
    });

    thread plkT([this]() {
        while (this->running) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock);

            this->ppu->exec();
        }
    });

    clkT.detach();
    plkT.detach();

    while (this->running){
        std::this_thread::sleep_for(std::chrono::microseconds(this->freq));
        std::lock_guard<std::mutex> lock(m);
        cv.notify_all();
    }
}

Clock::Clock(double freqMHz, ICpu *cpu, IPPU *ppu) : cpu(cpu), ppu(ppu) {
    this->freq = static_cast<unsigned int>(1.0 / freqMHz * MICROSECONDS_IN_SECOND);
}

void Clock::stop() {
    this->running = false;
}
