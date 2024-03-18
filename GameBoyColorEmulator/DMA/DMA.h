//
// Created by Fabrizio Paino on 2024-03-16.
//

#ifndef DMA_H
#define DMA_H

#include "../../Definitions.h"
#include "../../Interfaces/IMemory.h"

class DMA {
public:
    DMA(IMemory* memory);

    void start(ui8 value);
    void exec();
    bool inProgress() const;

private:
    IMemory* memory;
    bool active;
    ui8 byte;
    ui8 value;
    ui8 delay;
};

#endif //DMA_H
