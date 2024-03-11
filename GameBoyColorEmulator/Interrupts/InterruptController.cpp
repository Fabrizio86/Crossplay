//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "InterruptController.h"
#include "InterruptType.h"

InterruptController::InterruptController()
{
    interruptRequests = {
        {InterruptType::VBlank, false}, // Vertical blank interrupt (VBlank)
        {InterruptType::HBlank, false}, // Vertical blank interrupt (VBlank)
        {InterruptType::LCD_STAT, false}, // LCD STAT interrupt
        {InterruptType::TimerOverflow, false}, // Timer overflow interrupt
        {InterruptType::Serial, false}, // Serial transfer complete interrupt
        {InterruptType::ButtonPress, false}, // Input (button press) interrupt
        {InterruptType::Unused, false}, // Unused (reserved) interrupt
        {InterruptType::DI, false}, // Used for Disable/Enable Interrupts
        {InterruptType::DMA, false},
    };
}

bool InterruptController::isInterruptRequested(InterruptType interruptType) const
{
    return interruptRequests.at(interruptType);
}

void InterruptController::clearInterrupt(InterruptType interruptType)
{
    interruptRequests[interruptType] = false;
}

void InterruptController::requestInterrupt(InterruptType interruptType)
{
    interruptRequests[interruptType] = true;
}

bool InterruptController::isInterruptRequests()
{
    for (const auto& entry : this->interruptRequests)
    {
        if (entry.second)
        {
            return true;
        }
    }
    return false;
}

const std::unordered_map<InterruptType, bool>& InterruptController::getInterruptRequests() const
{
    return interruptRequests;
}
