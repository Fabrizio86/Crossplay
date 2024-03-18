//
// Created by Fabrizio Paino on 2024-03-16.
//

#include "DMA.h"
#include "../Consts.h"

DMA::DMA(IMemory* memory) : memory(memory)
{
}

void DMA::start(const ui8 value)
{
    this->active = true;
    this->byte = 0;
    this->delay = 2;
    this->value = value;
}

void DMA::exec()
{
    if (!this->active) return;

    if (this->delay)
    {
        this->delay--;
        return;
    }

    const ui8 newVal = this->memory->read(this->value * 0x100 + this->byte++);
    this->memory->writeByte(OAM_ADDR + this->byte, newVal);
    this->active = this->byte < 0xA0;

    printf("dma in progress\n");
}

bool DMA::inProgress() const
{
    return this->active;
}
