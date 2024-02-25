//
// Created by Fabrizio Paino on 2024-02-24.
//

#include "../GbCpu.h"

void GbCpu::initBcodes()
{
    this->prefixCbOpcodes[PrefixCB::BIT_7_H] = [this]()
    {
        const bool bitSet = (this->registers.regH & (1 << 7)) != 0;
        this->flags.zero = !bitSet;
        this->flags.subtract = false;
        this->flags.halfCarry = true;
    };
}
