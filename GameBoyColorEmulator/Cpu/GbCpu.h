//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_GBCPU_H
#define CROSSPLAY_GBCPU_H

#include "../../Interfaces/ICpu.h"
#include "../../Definitions.h"
#include "../Registers.h"
#include "../../Interfaces/IMemory.h"
#include "../InterruptController.h"
#include "../ISR.h"
#include "../Instruction.h"
#include <unordered_map>
#include <string>

class GbCpu : public ICpu {
public:
    GbCpu(IMemory *memory, InterruptController *ic, ISR *isr);

    ~GbCpu() override = default;

    void reset() override;

    void exec() override;

private:
    bool stopped;
    bool halted;

    Instruction opcode;
    std::array<OpcodeHandler, 256> opCodes;

    IMemory *memory;
    Registers registers;
    Flags flags;
    bool interruptEnabled;

    Registers savedRegisters;
    Flags savedFlags;
    InterruptController *interruptController;
    ISR *isr;

private:
    void saveState();

    void restoreState();

    void updateFlagsAfterSubtraction(uint8_t result, uint8_t operand1, uint8_t operand2);

    void updateFlagsAfterAddition(uint8_t result, uint8_t operand1, uint8_t operand2);

    void updateFlagsAfterLogicalOperation(uint8_t value, bool isAndOperation);

    void performBitwiseXorAndUpdateFlags(uint8_t &registerA, uint8_t value);

    void resetRegistersAndDisableInterrupts(uint16_t currentPc, uint16_t newPcValue);

    void pushToStack(uint8_t value);

    void initXor();

    void initRst();

    void initLdb();

    void initLda();

    void initInc();

    void initDec();

    void initLdd();

    void initLdh();

    void initLdc();

    void initLde();

    void initLdl();

    void initAdd();

    void initAnd();
};

#endif //CROSSPLAY_GBCPU_H
