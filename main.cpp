#include "Hardware.h"
#include "GameBoyColorEmulator/Cpu/GbCpu.h"
#include "GameBoyColorEmulator/Bus.h"
#include "GameBoyColorEmulator/PPU/GbPPU.h"

int main()
{
    Hardware* hardware;
    InterruptController ic;
    Bus bus(&ic);
    bus.loadRom("/Users/fabriziopaino/Crossplay/PR.gb");

    InterruptVectorTable ivt(&bus);
    ISR isr(&ic, &ivt);
    GbCpu cpu(&bus, &ic, &isr);
    GbPPU ppu(&bus, &ic);
    Clock clock(40.20, 80.40, &cpu, &ppu);

    cpu.reset();
    clock.start();
    clock.stop();
    return 0;
}
