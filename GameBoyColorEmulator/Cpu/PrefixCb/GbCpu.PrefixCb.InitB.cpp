//
// Created by Fabrizio Paino on 2024-02-24.
//

#include "../GbCpu.h"

bool CheckBit(const ui8 value, const ui8 bit)
{
    return (value & (1 << bit)) != 0;
}

ui8 GbCpu::OpCodeRl(ui8 value)
{
    const ui8 carry = this->registers.regF.carry;

    this->registers.regF.carry = CheckBit(value, 7);

    ui8 result = static_cast<ui8>(value << 1);
    result |= carry;

    this->registers.regF.zero = result == 0;

    this->registers.regF.subtract = false;
    this->registers.regF.halfCarry = false;

    return result;
}

void GbCpu::initBcodes()
{
    this->prefixCbOpcodes[PrefixCB::BIT_7_H] = [this]()
    {
        this->registers.regF.zero = !CheckBit(this->registers.regH, 7);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_A] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regA, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_B] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regB, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_C] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regC, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_D] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regD, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_E] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regE, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_H] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regH, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::BIT_1_L] = [this]()
    {
        this->registers.regF.zero = CheckBit(this->registers.regL, 1);
        this->registers.regF.subtract = false;
        this->registers.regF.halfCarry = true;
    };

    this->prefixCbOpcodes[PrefixCB::RL_A] = [this]()
    {
        this->registers.regA = this->OpCodeRl(this->registers.regA);
    };

    this->prefixCbOpcodes[PrefixCB::RL_B] = [this]()
    {
        this->registers.regB = this->OpCodeRl(this->registers.regB);
    };

    this->prefixCbOpcodes[PrefixCB::RL_C] = [this]()
    {
        this->registers.regC = this->OpCodeRl(this->registers.regC);
    };

    this->prefixCbOpcodes[PrefixCB::RL_D] = [this]()
    {
        this->registers.regD = this->OpCodeRl(this->registers.regD);
    };

    this->prefixCbOpcodes[PrefixCB::RL_E] = [this]()
    {
        this->registers.regE = this->OpCodeRl(this->registers.regE);
    };

    this->prefixCbOpcodes[PrefixCB::RL_H] = [this]()
    {
        this->registers.regH = this->OpCodeRl(this->registers.regH);
    };

    this->prefixCbOpcodes[PrefixCB::RL_L] = [this]()
    {
        this->registers.regL = this->OpCodeRl(this->registers.regL);
    };
}
