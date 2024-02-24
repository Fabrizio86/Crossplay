//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "InterruptVectorTable.h"
#include "Consts.h"

#include <iostream>

ISRFunctionPtr InterruptVectorTable::getInterruptHandler(InterruptType interruptType) const {
    return this->interruptHandlers.at(interruptType);
}

// todo: need to implement these
InterruptVectorTable::InterruptVectorTable(Bus *bus) : bus(bus) {
    this->interruptHandlers[InterruptType::VBlank] = []() {};
    this->interruptHandlers[InterruptType::LCD_STAT] = []() {};
    this->interruptHandlers[InterruptType::TimerOverflow] = []() {};
    this->interruptHandlers[InterruptType::SerialTransferComplete] = []() {};
    this->interruptHandlers[InterruptType::ButtonPress] = []() {};
    this->interruptHandlers[InterruptType::Unused] = []() {};

    this->interruptHandlers[InterruptType::DMA] = [bus]() {
        std::cout << "DMA value: " << (int)DMA_VALUE << std::endl;
        bus->performDMA(DMA_VALUE);
    };
}

void InterruptVectorTable::setHandler(InterruptType interruptType, ISRFunctionPtr handler) {
    this->interruptHandlers[interruptType] = handler;
}