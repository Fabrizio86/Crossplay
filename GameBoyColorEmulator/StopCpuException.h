//
// Created by Fabrizio Paino on 2024-02-24.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_STOPCPUEXCEPTION_H
#define CROSSPLAY_STOPCPUEXCEPTION_H

#include <exception>
#include <string>

class StopCPUException : public std::exception {
public:
    StopCPUException() = default;

    const char *what() const noexcept {
        return msg.data();
    }

private:
    const std::string msg = "CPU stopped";
};

#endif //CROSSPLAY_STOPCPUEXCEPTION_H
