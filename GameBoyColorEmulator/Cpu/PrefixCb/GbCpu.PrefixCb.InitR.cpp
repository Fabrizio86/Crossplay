//
// Created by Fabrizio Paino on 2024-02-24.
//

#include "../GbCpu.h"

#include <iostream>

void GbCpu::initRcodes()
{
    // todo: to be deleted
    for (int i = 0; i < 256; ++i)
    {
        this->prefixCbOpcodes[i] = [i]()
        {
            std::cout << "cb instruction not implemented " << i << std::endl;
        };
    }

    this->prefixCbOpcodes[PrefixCB::RLC_B] = [this]()
    {
        this->registers.regF.carry = (this->registers.regB & 0x80) != 0;
        this->registers.regB = (this->registers.regB << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regB == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_C] = [this]()
    {
        this->registers.regF.carry = (this->registers.regC & 0x80) != 0;
        this->registers.regC = (this->registers.regC << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regC == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_D] = [this]()
    {
        this->registers.regF.carry = (this->registers.regD & 0x80) != 0;
        this->registers.regD = (this->registers.regD << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regD == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_E] = [this]()
    {
        this->registers.regF.carry = (this->registers.regE & 0x80) != 0;
        this->registers.regE = (this->registers.regE << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regE == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_H] = [this]()
    {
        this->registers.regF.carry = (this->registers.regH & 0x80) != 0;
        this->registers.regH = (this->registers.regH << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regH == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_L] = [this]()
    {
        this->registers.regF.carry = (this->registers.regL & 0x80) != 0;
        this->registers.regL = (this->registers.regL << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regL == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_indHL] = [this]()
    {
        const uint16_t hl = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(hl);
        this->registers.regF.carry = (value & 0x80) != 0;
        value = (value << 1) | this->registers.regF.carry;
        this->memory->writeByte(hl, value);
        this->registers.regF.zero = value == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RLC_A] = [this]()
    {
        this->registers.regF.carry = (this->registers.regA & 0x80) != 0;
        this->registers.regA = (this->registers.regA << 1) | this->registers.regF.carry;
        this->registers.regF.zero = this->registers.regA == 0;
        this->registers.regF.subtract = false;
    };

    this->prefixCbOpcodes[PrefixCB::RRC_B] = [this]()
    {
        // Get the value from register B
        uint8_t value = this->registers.regB;

        // Perform RRC operation
        const bool carry = value & 0x01; // Store the carry flag
        value >>= 1; // Right shift
        value |= (carry << 7); // Set MSB with previous carry

        // Update flags
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        // Write back to register B
        this->registers.regB = value;
    };

    // Implement the rest of the opcodes in a similar manner...
    this->prefixCbOpcodes[PrefixCB::RRC_C] = [this]()
    {
        uint8_t value = this->registers.regC;
        const bool carry = value & 0x01;
        value >>= 1;
        value |= (carry << 7);
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;
        this->registers.regC = value;
    };

    this->prefixCbOpcodes[PrefixCB::RRC_D] = [this]()
    {
        uint8_t value = this->registers.regD;
        const bool carry = value & 0x01;
        value >>= 1;
        value |= (carry << 7);
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;
        this->registers.regD = value;
    };

    this->prefixCbOpcodes[PrefixCB::RRC_E] = [this]()
    {
        uint8_t value = this->registers.regE;
        const bool carry = value & 0x01;
        value >>= 1;
        value |= (carry << 7);
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;
        this->registers.regE = value;
    };

    this->prefixCbOpcodes[PrefixCB::RRC_H] = [this]()
    {
        uint8_t value = this->registers.regH;
        const bool carry = value & 0x01;
        value >>= 1;
        value |= (carry << 7);
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;
        this->registers.regH = value;
    };

    this->prefixCbOpcodes[PrefixCB::RRC_L] = [this]()
    {
        uint8_t value = this->registers.regL;
        const bool carry = value & 0x01;
        value >>= 1;
        value |= (carry << 7);
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;
        this->registers.regL = value;
    };

    this->prefixCbOpcodes[PrefixCB::RRC_indHL] = [this]()
    {
        const uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(address);
        const bool carry = value & 0x01;
        value >>= 1;
        value |= (carry << 7);
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;
        this->memory->writeByte(address, value);
    };

    this->prefixCbOpcodes[PrefixCB::RRC_A] = [this]()
    {
        uint8_t value = this->registers.regA;

        const bool carry = value & 0x01; // Store the carry flag
        value = (value >> 1) | (carry << 7); // Right rotate

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regA = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_B] = [this]()
    {
        uint8_t value = this->registers.regB;

        const bool carry = value >> 7; // Store the carry flag
        value = (value << 1) | this->registers.regF.carry; // Left rotate

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regB = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_C] = [this]()
    {
        // Get the value from register C
        uint8_t value = this->registers.regC;

        // Perform RL operation
        const bool carry = value >> 7; // Store the carry flag
        value = (value << 1) | this->registers.regF.carry; // Left rotate

        // Update flags
        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        // Write back to register C
        this->registers.regC = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_D] = [this]()
    {
        uint8_t value = this->registers.regD;

        const bool carry = value >> 7;
        value = (value << 1) | this->registers.regF.carry;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regD = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_E] = [this]()
    {
        uint8_t value = this->registers.regE;

        const bool carry = value >> 7;
        value = (value << 1) | this->registers.regF.carry;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regE = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_H] = [this]()
    {
        uint8_t value = this->registers.regH;

        const bool carry = value >> 7;
        value = (value << 1) | this->registers.regF.carry;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regH = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_L] = [this]()
    {
        uint8_t value = this->registers.regL;

        const bool carry = value >> 7;
        value = (value << 1) | this->registers.regF.carry;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regL = value;
    };

    this->prefixCbOpcodes[PrefixCB::RL_indHL] = [this]()
    {
        const uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(address);
        const bool carry = value >> 7;
        value = (value << 1) | this->registers.regF.carry;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->memory->writeByte(address, value);
    };

    this->prefixCbOpcodes[PrefixCB::RL_A] = [this]()
    {
        uint8_t value = this->registers.regA;

        const bool carry = value >> 7;
        value = (value << 1) | this->registers.regF.carry;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regA = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_B] = [this]()
    {
        uint8_t value = this->registers.regB;
        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regB = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_C] = [this]()
    {
        uint8_t value = this->registers.regC;
        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regC = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_D] = [this]()
    {
        uint8_t value = this->registers.regD;
        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regD = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_E] = [this]()
    {
        uint8_t value = this->registers.regE;
        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regE = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_H] = [this]()
    {
        uint8_t value = this->registers.regH;
        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regH = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_L] = [this]()
    {
        uint8_t value = this->registers.regL;
        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->registers.regL = value;
    };

    this->prefixCbOpcodes[PrefixCB::RR_indHL] = [this]()
    {
        const uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(address);

        const bool carry = value & 0x01;
        value = (value >> 1) | (this->registers.regF.carry << 7);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->memory->writeByte(address, value);
    };
}
