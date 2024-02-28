//
// Created by Fabrizio Paino on 2024-02-25.
//

#ifndef CARTRIDGEHEADER_H
#define CARTRIDGEHEADER_H

struct CartridgeHeader
{
    char title[16];
    uint8_t cgbFlag;
    uint8_t sgbCode;
    uint8_t type;
    uint8_t romSize;
    uint8_t ramSize;
    uint16_t entryPoint;
};


#endif //CARTRIDGEHEADER_H
