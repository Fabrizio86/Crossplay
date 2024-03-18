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
#include "../Interrupts/InterruptController.h"
#include "../Interrupts/ISR.h"
#include "../Instruction.h"

class GbCpu : public ICpu
{
public:
    GbCpu(IMemory* memory, InterruptController* ic, ISR* isr, DMA* dma);

    ~GbCpu() override = default;

    void reset() override;

    void exec() override;

private:
    bool stopped;
    bool halted;

    DMA* dma;
    Instruction opcode;
    std::array<OpcodeHandler, 256> opCodes;
    std::array<OpcodeHandler, 256> prefixCbOpcodes;

    IMemory* memory;
    Registers registers;
    bool interruptEnabled = true;

    Registers savedRegisters;
    InterruptController* interruptController;
    ISR* isr;

private:
    void saveState();

    void restoreState();

    void updateFlagsAfterSubtraction(uint8_t result, uint8_t operand1, uint8_t operand2);

    void updateFlagsAfterAddition(uint8_t result, uint8_t operand1, uint8_t operand2);

    void updateFlagsAfterLogicalOperation(uint8_t value, LogicalOperation operation);

    void resetRegistersAndDisableInterrupts(uint16_t currentPc, uint16_t newPcValue);

    void pushToStack(uint8_t value);

    void performSubtraction(uint8_t operandB, bool includeCarry);

    void performXor(uint8_t value);

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

    void initSub();

    void initR();

    bool MeetCondition(Condition condition) const;

    void initJ();

    void initC();

    void initPushPop();

    void initRcodes();

    ui8 slaOp(ui8 value);

    void initScodes();

    ui8 OpCodeRl(ui8 value);

    void initBcodes();

    uint16_t popFromStack();
};

#endif //CROSSPLAY_GBCPU_H
