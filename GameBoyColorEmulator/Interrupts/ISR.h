//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_ISR_H
#define CROSSPLAY_ISR_H

#include "InterruptController.h"
#include "InterruptVectorTable.h"

class ISR {
public:
    ISR(InterruptController *ic, InterruptVectorTable *ivt);

    // Implementation of handleInterrupt function
    void handleInterrupt();

private:
    InterruptController *ic;
    InterruptVectorTable *ivt;
};


#endif //CROSSPLAY_ISR_H
