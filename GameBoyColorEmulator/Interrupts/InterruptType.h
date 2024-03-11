//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_INTERRUPTTYPE_H
#define CROSSPLAY_INTERRUPTTYPE_H

enum class InterruptType
{
    VBlank = 0,
    HBlank,
    LCD_STAT,
    TimerOverflow,
    Serial,
    ButtonPress,
    Unused,
    DI,
    DMA
};

#endif //CROSSPLAY_INTERRUPTTYPE_H
