#include <iostream>
#include "Hardware.h"
#include "GameBoyColorEmulator/GbCpu.h"
#include "GameBoyColorEmulator/GbMemory.h"
#include <fstream>
#include <vector>

void loadGbcRom(const std::string& filePath, GbMemory* memory) {
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
    romFile.read(reinterpret_cast<char*>(romData.data()), fileSize);
    romFile.close();

    // Pass the ROM data to the memory object
    memory->loadRom(romData);
}

int main()
{
    Hardware* hardware;
    auto* memory = new GbMemory();
    InterruptController ic;
    InterruptVectorTable ivt;
    ISR isr(&ic, &ivt);
    ICpu* cpu = new GbCpu(memory, &ic, &isr);
    Clock clock(4.20, cpu);

    loadGbcRom("/Users/fabriziopaino/CrossPlay/PR.gb", memory);

    clock.start();

    int i;
    std::cin >> i;

    clock.stop();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
