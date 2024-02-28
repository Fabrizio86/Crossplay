#include <iostream>
#include "Hardware.h"
#include "GameBoyColorEmulator/Cpu/GbCpu.h"
#include "GameBoyColorEmulator/Bus.h"
#include "GameBoyColorEmulator/PPU/GbPPU.h"
#include <fstream>
#include <vector>

int main()
{
    Hardware* hardware;
    InterruptController ic;
    Bus bus(&ic);
    bus.loadRom("/Users/fabriziopaino/CLionProjects/gbemu/scripts/test_roms/02-interrupts.gb");

    InterruptVectorTable ivt(&bus);
    ISR isr(&ic, &ivt);
    GbCpu cpu(&bus, &ic, &isr);
    GbPPU ppu(&bus, &ic);
    Clock clock(4.19, 8.39, &cpu, &ppu);

    cpu.reset();

    clock.start();

    int i;
    std::cin >> i;

    clock.stop();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
