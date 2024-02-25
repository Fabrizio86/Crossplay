//
// Created by Fabrizio Paino on 2024-02-24.
//

#include "../GbCpu.h"

void GbCpu::initScodes() {
    this->prefixCbOpcodes[PrefixCB::SLA_B] = [this]() {
        uint8_t value = this->registers.regB;
        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->registers.regB = value;
    };

    this->prefixCbOpcodes[PrefixCB::SLA_C] = [this]() {
        uint8_t value = this->registers.regC;

        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->registers.regC = value;
    };

    this->prefixCbOpcodes[PrefixCB::SLA_D] = [this]() {
        uint8_t value = this->registers.regD;

        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->registers.regD = value;
    };

    this->prefixCbOpcodes[PrefixCB::SLA_E] = [this]() {
        uint8_t value = this->registers.regE;

        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->registers.regE = value;
    };

    this->prefixCbOpcodes[PrefixCB::SLA_H] = [this]() {
        uint8_t value = this->registers.regH;

        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->registers.regH = value;
    };

    this->prefixCbOpcodes[PrefixCB::SLA_L] = [this]() {
        uint8_t value = this->registers.regL;
        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->registers.regL = value;
    };

    this->prefixCbOpcodes[PrefixCB::SLA_indHL] = [this]() {
        const uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(address);
        const bool carry = value >> 7;
        value <<= 1;

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->flags.carry = carry;

        this->memory->writeByte(address, value);
    };

    this->prefixCbOpcodes[PrefixCB::SET_3_L] = [this]() {
        uint8_t value = this->registers.regL;
        value |= (1 << 3);

        this->flags.zero = (value == 0);
        this->flags.subtract = false;
        this->flags.halfCarry = false;
        this->registers.regL = value;
    };

}
