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

#include "GameBoyColorEmulator/PPU//GbPPU.h"

using namespace std;

void Clock::start()
{
    sf::RenderWindow* const window = new sf::RenderWindow(sf::VideoMode(SCANLINE_WIDTH * 2, SCANLINE_HEIGHT * 2, 32), "Game Boy Color Emulator");
    ((GbPPU*)this->ppu)->window = window;

    this->running = true;

    thread clkT([&]()
    {
        while (this->running)
        {
            try
            {
                this->cpu->exec();
            }
            catch (StopCPUException& e)
            {
                cout << e.what() << endl;
                this->running = false;
            }

            std::this_thread::sleep_for(std::chrono::nanoseconds(this->cpuFreq));
        }
    });

    thread plkT([this]()
    {
        while (this->running)
        {
            this->ppu->exec();
            //std::this_thread::sleep_for(std::chrono::microseconds(this->ppuFreq));

        }
    });

    // Handle events
    sf::Event event;
    while (window->pollEvent(event) && this->running)
    {
        if (event.type == sf::Event::Closed)
            window->close();
    }

    clkT.join();
    plkT.join();
}

Clock::Clock(double cpuFreqMHz, double ppuFreqMHz, ICpu* cpu, IPPU* ppu) : cpu(cpu), ppu(ppu)
{
    this->cpuFreq = 1.0 / cpuFreqMHz * 1e6;
    this->ppuFreq = std::round(1000 * 1000 / 59.7);
}

void Clock::stop()
{
    this->running = false;
}
