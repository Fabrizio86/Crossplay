#include <iostream>
#include "Hardware.h"
#include "GameBoyColorEmulator/Cpu/GbCpu.h"
#include "GameBoyColorEmulator/Bus.h"
#include "GameBoyColorEmulator/GbPPU.h"
#include <fstream>
#include <vector>

void loadGbcRom(const std::string &filePath, Bus *memory) {
    std::ifstream romFile(filePath, std::ios::binary);
    if (!romFile.is_open()) {
        // Handle error: unable to open file
        return;
    }

    // Determine file size
    romFile.seekg(0, std::ios::end);
    std::streampos fileSize = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    // Read ROM data into a vector
    std::vector<uint8_t> romData(fileSize);
    romFile.read(reinterpret_cast<char *>(romData.data()), fileSize);
    romFile.close();

    // Pass the ROM data to the memory object
    memory->loadRom(romData);
}

int main() {
    Hardware *hardware;
    InterruptController ic;
    auto *bus = new Bus(&ic);
    InterruptVectorTable ivt(bus);
    ISR isr(&ic, &ivt);
    ICpu *cpu = new GbCpu(bus, &ic, &isr);
    IPPU *ppu = new GbPPU(bus, &ic);
    Clock clock(14.20, cpu, ppu);

    loadGbcRom("/Users/fabriziopaino/CrossPlay/PR.gb", bus);

    clock.start();

    int i;
    std::cin >> i;

    clock.stop();

    std::cout << "Hello, World!" << std::endl;

    delete bus;
    delete cpu;
    delete ppu;
    return 0;
}
