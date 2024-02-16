//
// Created by Fabrizio Paino on 2024-02-16.
//

#ifndef CROSSPLAY_MBC3_H
#define CROSSPLAY_MBC3_H

#include <cstdint>
#include <vector>
#include "../../Interfaces/IMBC.h"

class MBC3 : public IMBC {
public:
    MBC3(const std::vector<uint8_t> &romData, const std::vector<uint8_t> &ramData);

    uint8_t read(uint16_t address);

    void write(uint16_t address, uint8_t data);

private:
    // ROM and RAM data
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;

    // MBC3 registers
    uint8_t romBank;
    uint8_t ramBank;
    bool ramEnabled;
    bool rtcEnabled;
    bool rtcLatch;
    uint8_t rtcSeconds;
    uint8_t rtcMinutes;
    uint8_t rtcHours;
    uint16_t rtcDays;
    uint32_t rtcLatchValue;

    // RTC registers
    enum RTCRegister {
        RTC_REG_SECONDS = 0,
        RTC_REG_MINUTES,
        RTC_REG_HOURS,
        RTC_REG_DAYS_LOW,
        RTC_REG_DAYS_HIGH
    };
    uint8_t rtcRegisters[5];

    // Constants
    static constexpr uint32_t ROM_BANK_SIZE = 0x4000;
    static constexpr uint32_t RAM_BANK_SIZE = 0x2000;

    void latchClockData();
};

#endif //CROSSPLAY_MBC3_H
