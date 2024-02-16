//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_HARDWARE_H
#define CROSSPLAY_HARDWARE_H

#include "Clock.h"
#include "Interfaces/IApu.h"
#include "Interfaces/ICpu.h"
#include "Interfaces/IGpu.h"
#include "Interfaces/IMemory.h"
#include "Interfaces/IInputDevice.h"
#include "Interfaces/IStorage.h"

struct Hardware {
    Clock clock;
    IApu *apu;
    ICpu *cpu;
    IGpu *gpu;
    IMemory *memory;
    IInputDevice *inputDevice;
    IStorage *storage;
};

#endif //CROSSPLAY_HARDWARE_H
