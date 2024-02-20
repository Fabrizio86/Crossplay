//
// Created by Fabrizio Paino on 2024-02-19.
//

#ifndef CROSSPLAY_IPPU_H
#define CROSSPLAY_IPPU_H

class IPPU {
public:
    virtual ~IPPU() = default;

    virtual void exec() = 0;
};

#endif //CROSSPLAY_IPPU_H
