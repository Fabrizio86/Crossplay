//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_INTERRUPTCONTROLLER_H
#define CROSSPLAY_INTERRUPTCONTROLLER_H

#include <unordered_map>
#include "InterruptType.h"

class InterruptController {
public:
    InterruptController();

    void requestInterrupt(InterruptType interruptType);

    bool isInterruptRequested(InterruptType interruptType) const;

    void clearInterrupt(InterruptType interruptType);

    bool isInterruptRequests();

    const std::unordered_map<InterruptType, bool> &getInterruptRequests() const;

private:
    std::unordered_map<InterruptType, bool> interruptRequests;
};


#endif //CROSSPLAY_INTERRUPTCONTROLLER_H
