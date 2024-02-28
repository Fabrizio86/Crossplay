//
// Created by Fabrizio Paino on 2024-02-16.
//

#include "MBC3.h"

MBC3::MBC3(const DataBuffer& romData, const DataBuffer& ramData)
        : rom(romData), ram(ramData), romBank(1), ramBank(0), ramEnabled(false), rtcEnabled(false), rtcLatch(false),
          rtcSeconds(0), rtcMinutes(0), rtcHours(0), rtcDays(0), rtcLatchValue(0)
{
    // Initialize RTC registers
    rtcRegisters[RTC_REG_SECONDS] = 0;
    rtcRegisters[RTC_REG_MINUTES] = 0;
    rtcRegisters[RTC_REG_HOURS] = 0;
    rtcRegisters[RTC_REG_DAYS_LOW] = 0;
    rtcRegisters[RTC_REG_DAYS_HIGH] = 0;
}

uint8_t MBC3::read(uint16_t address)
{
    if (address < 0x4000)
    {
        return rom[address]; // Read from ROM bank 0
    }
    else if (address < 0x8000)
    {
        uint32_t adjustedAddress = (romBank * ROM_BANK_SIZE) + (address - 0x4000);
        return rom[adjustedAddress]; // Read from switchable ROM bank
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        if (ramEnabled)
        {
            if (rtcEnabled && address >= 0xA000 && address < 0xA080)
            {
                return rtcRegisters[address - 0xA000]; // Read from RTC registers
            }
            else
            {
                uint32_t adjustedAddress = (ramBank * RAM_BANK_SIZE) + (address - 0xA000);
                return ram[adjustedAddress]; // Read from RAM bank
            }
        }
        else
        {
            return 0xFF; // RAM not enabled, return default value
        }
    }
    else
    {
        return 0xFF; // Invalid address, return default value
    }
}

void MBC3::write(uint16_t address, uint8_t data)
{
    if (address < 0x2000)
    {
        ramEnabled = ((data & 0x0F) == 0x0A); // Enable/disable RAM
    }
    else if (address < 0x4000)
    {
        // Write to ROM bank lower 5 bits
        romBank = (data & 0x7F) | ((data & 0x80) ? 0x80 : 0x00);
    }
    else if (address < 0x6000)
    {
        // Write to RAM bank or RTC register select
        if (data >= 0x08 && data <= 0x0C)
        {
            rtcRegisters[data - 0x08] = data;
        }
        else
        {
            ramBank = data & 0x0F;
        }
    }
    else if (address < 0x8000)
    {
        // Latch clock frameBuffer
        if (data == 0x01)
        {
            latchClockData();
        }
        else
        {
            rtcLatch = false;
        }
    }
}

void MBC3::latchClockData()
{
    // Latch current RTC values
    rtcLatchValue = 0xFF;
    rtcLatchValue |= (rtcSeconds & 0x3F);
    rtcLatchValue |= ((rtcMinutes & 0x3F) << 8);
    rtcLatchValue |= ((rtcHours & 0x1F) << 16);
    rtcLatchValue |= ((rtcDays & 0xFF) << 24);
}
