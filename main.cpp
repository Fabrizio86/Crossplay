#include <iostream>
#include "Hardware.h"
#include "GameBoyColorEmulator/GbCpu.h"
#include "GameBoyColorEmulator/GbMemory.h"

int main()
{
    Hardware* hardware;
    auto* memory = new GbMemory();
    InterruptController ic;
    InterruptVectorTable ivt;
    ISR isr(&ic, &ivt);
    ICpu* cpu = new GbCpu(memory, &ic, &isr);
    Clock clock(4.20, cpu);

    memory->loadRom("/Users/fabriziopaino/CrossPlay/PR.gb");

    clock.start();

    int i;
    std::cin >> i;

    clock.stop();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
