//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_ICPU_H
#define CROSSPLAY_ICPU_H

class ICpu {
public:
    virtual ~ICpu() = default;

    virtual void reset() = 0;

    virtual void exec() = 0;
};

#endif //CROSSPLAY_ICPU_H
