//
// Created by Fabrizio Paino on 2024-02-15.
//

#ifndef CROSSPLAY_IMEMORYBANKCONTROLLER_H
#define CROSSPLAY_IMEMORYBANKCONTROLLER_H

#include <cstdint>

class IMemoryBankController {
public:
    virtual ~IMemoryBankController() = default;

    virtual uint8_t read(uint16_t address) = 0;

    virtual void write(uint16_t address, uint8_t data) = 0;
};


#endif //CROSSPLAY_IMEMORYBANKCONTROLLER_H
