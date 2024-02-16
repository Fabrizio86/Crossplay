//
// Created by Fabrizio Paino on 2024-02-11.
// Copyright (c) 2024 Aedifex Solutions Inc. All rights reserved.
//

#ifndef CROSSPLAY_IINPUTDEVICE_H
#define CROSSPLAY_IINPUTDEVICE_H

class IInputDevice {
public:
    virtual ~IInputDevice() = default;

    virtual bool isButtonPressed(int button) const = 0;

    virtual void setButtonState(int button, bool pressed) = 0;
};

#endif //CROSSPLAY_IINPUTDEVICE_H
