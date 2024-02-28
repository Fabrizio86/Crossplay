//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "InterruptVectorTable.h"
#include "../Consts.h"

#include <iostream>
using namespace std;

ISRFunctionPtr InterruptVectorTable::getInterruptHandler(InterruptType interruptType) const {
    return this->interruptHandlers.at(interruptType);
}

// todo: need to implement these
InterruptVectorTable::InterruptVectorTable(Bus *bus) : bus(bus) {
    this->interruptHandlers[InterruptType::VBlank] = []() { cout << "vblank" << endl; };
    this->interruptHandlers[InterruptType::LCD_STAT] = []() {cout << "LCD_STAT" << endl; };
    this->interruptHandlers[InterruptType::TimerOverflow] = []() {cout << "TimerOverflow" << endl; };
    this->interruptHandlers[InterruptType::Serial] = []() {cout << "SerialTransferComplete" << endl; };
    this->interruptHandlers[InterruptType::ButtonPress] = []() {cout << "ButtonPress" << endl; };
    this->interruptHandlers[InterruptType::Unused] = []() {cout << "Unused" << endl; };

    this->interruptHandlers[InterruptType::DMA] = [bus]() {
        std::cout << "DMA value: " << (int)DMA_VALUE << std::endl;
        bus->performDMA(DMA_VALUE);
    };
}

void InterruptVectorTable::setHandler(InterruptType interruptType, ISRFunctionPtr handler) {
    this->interruptHandlers[interruptType] = handler;
}