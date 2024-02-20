//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "InterruptVectorTable.h"

ISRFunctionPtr InterruptVectorTable::getInterruptHandler(InterruptType interruptType) const {
    return this->interruptHandlers.at(interruptType);
}

// todo: need to implement these
InterruptVectorTable::InterruptVectorTable() {
    this->interruptHandlers[InterruptType::VBlank] = []() {};
    this->interruptHandlers[InterruptType::LCD_STAT] = []() {};
    this->interruptHandlers[InterruptType::TimerOverflow] = []() {};
    this->interruptHandlers[InterruptType::SerialTransferComplete] = []() {};
    this->interruptHandlers[InterruptType::ButtonPress] = []() {};
    this->interruptHandlers[InterruptType::Unused] = []() {};
}

void InterruptVectorTable::setHandler(InterruptType interruptType, ISRFunctionPtr handler) {
    this->interruptHandlers[interruptType] = handler;
}