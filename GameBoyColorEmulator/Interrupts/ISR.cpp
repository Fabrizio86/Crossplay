//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#include "ISR.h"

void ISR::handleInterrupt() {
    for (const auto &[interruptType, requested]: this->ic->getInterruptRequests()) {
        if (requested) {
            auto handler = this->ivt->getInterruptHandler(interruptType);

            if (handler != 0) {
                handler();
            }

            this->ic->clearInterrupt(interruptType);
        }
    }
}

ISR::ISR(InterruptController *ic, InterruptVectorTable *ivt) : ic(ic), ivt(ivt) {}
