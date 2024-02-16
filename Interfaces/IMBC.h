//
// Created by Fabrizio Paino on 2024-02-15.
//

#ifndef CROSSPLAY_IMBC_H
#define CROSSPLAY_IMBC_H

#include <cstdint>

class IMBC {
public:
    virtual ~IMBC() = default;

    virtual uint8_t read(uint16_t address) = 0;

    virtual void write(uint16_t address, uint8_t data) = 0;
};


#endif //CROSSPLAY_IMBC_H
