//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_IGPU_H
#define CROSSPLAY_IGPU_H

#include <cstdint>

class IGpu {
public:
    virtual ~IGpu() = default;

    virtual void render() = 0;

    virtual void setPixel(int x, int y, uint32_t color) = 0;
};

#endif //CROSSPLAY_IGPU_H
