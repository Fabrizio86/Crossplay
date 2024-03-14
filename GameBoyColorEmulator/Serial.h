//
// Created by Fabrizio Paino on 2024-03-13.
//

#ifndef SERIAL_H
#define SERIAL_H

#include "Consts.h"
#include "../Definitions.h"

class Serial
{
public:
    Serial() = default;
    ~Serial() = default;

    ui8 read() { return this->data; }
    void write(ui8 byte) { this->data = byte; }
    void writeControl(ui8 byte) { }

private:
    ui8 data;
};


#endif //SERIAL_H
