//
// Created by Fabrizio Paino on 2024-02-12.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_INTERRUPTTYPE_H
#define CROSSPLAY_INTERRUPTTYPE_H

enum class InterruptType {
    VBlank = 0,
    LCD_STAT = 1,
    TimerOverflow = 2,
    SerialTransferComplete = 3,
    ButtonPress = 4,
    Unused = 5,
    DI = 6,
    DMA = 7
};

#endif //CROSSPLAY_INTERRUPTTYPE_H
