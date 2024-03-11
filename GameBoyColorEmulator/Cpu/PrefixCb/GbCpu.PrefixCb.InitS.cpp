//
// Created by Fabrizio Paino on 2024-02-24.
//

#include "../GbCpu.h"

ui8 GbCpu::slaOp(ui8 value)
{
    this->registers.regF.carry = CheckBit(value, 7);
    const ui8 result = static_cast<ui8>(value << 1);

    this->registers.regF.zero == result == 0;
    this->registers.regF.halfCarry = false;
    this->registers.regF.subtract = false;

    return result;
}

void GbCpu::initScodes()
{
    this->prefixCbOpcodes[PrefixCB::SLA_B] = [this]()
    {
        this->registers.regB = this->slaOp(this->registers.regB);
    };

    this->prefixCbOpcodes[PrefixCB::SLA_C] = [this]()
    {
        this->registers.regC = this->slaOp(this->registers.regC);
    };

    this->prefixCbOpcodes[PrefixCB::SLA_D] = [this]()
    {
        this->registers.regD = this->slaOp(this->registers.regD);
    };

    this->prefixCbOpcodes[PrefixCB::SLA_E] = [this]()
    {
        this->registers.regE = this->slaOp(this->registers.regE);
    };

    this->prefixCbOpcodes[PrefixCB::SLA_H] = [this]()
    {
        this->registers.regH = this->slaOp(this->registers.regH);
    };

    this->prefixCbOpcodes[PrefixCB::SLA_L] = [this]()
    {
        this->registers.regL = this->slaOp(this->registers.regL);
    };

    this->prefixCbOpcodes[PrefixCB::SLA_indHL] = [this]()
    {
        const uint16_t address = (this->registers.regH << 8) | this->registers.regL;
        uint8_t value = this->memory->read(address);
        const bool carry = value >> 7;
        value <<= 1;

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regF.carry = carry;

        this->memory->writeByte(address, value);
    };

    this->prefixCbOpcodes[PrefixCB::SET_3_L] = [this]()
    {
        uint8_t value = this->registers.regL;
        value |= (1 << 3);

        this->registers.regF.zero = (value == 0);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = false;
        this->registers.regL = value;
    };
}
