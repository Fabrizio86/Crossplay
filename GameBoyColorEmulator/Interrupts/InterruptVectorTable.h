//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_INTERRUPTVECTORTABLE_H
#define CROSSPLAY_INTERRUPTVECTORTABLE_H

#include <unordered_map>
#include "../../Definitions.h"
#include "InterruptType.h"
#include "../Bus.h"

class InterruptVectorTable {
public:
    InterruptVectorTable(Bus* bus);

    ISRFunctionPtr getInterruptHandler(InterruptType interruptType) const;

    void setHandler(InterruptType interruptType, ISRFunctionPtr handler);

private:
    std::unordered_map<InterruptType, ISRFunctionPtr> interruptHandlers;
    Bus* bus;
};


#endif //CROSSPLAY_INTERRUPTVECTORTABLE_H
