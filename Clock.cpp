//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "Clock.h"
#include "SFML/Graphics.hpp"
#include "GameBoyColorEmulator/Consts.h"
#include "GameBoyColorEmulator/StopCpuException.h"

#include <iostream>
#include <thread>

using namespace std;

constexpr double MICROSECONDS_IN_SECOND = 1000000.0;

void Clock::start() {
    this->running = true;

    thread clkT([this]() {
        while (this->running) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock);

            try {
                this->cpu->exec();
            }
            catch (StopCPUException &e) {
                cout << e.what() << endl;
                this->running = false;
            }
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

    // Handle events
    sf::Event event;

    while (this->running) {

        while (window->pollEvent(event) && this->running) {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(238));
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
