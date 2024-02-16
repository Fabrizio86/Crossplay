//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_IAPU_H
#define CROSSPLAY_IAPU_H

class IApu {
public:
    virtual ~IApu() = default;

    virtual void process() = 0;
};

#endif //CROSSPLAY_IAPU_H
