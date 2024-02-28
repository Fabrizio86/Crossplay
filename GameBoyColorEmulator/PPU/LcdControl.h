//
// Created by Fabrizio Paino on 2024-02-25.
//

#ifndef LCDCONTROL_H
#define LCDCONTROL_H

struct LcdControl
{
    unsigned BgWindowEnabled : 1;
    unsigned ObjEnabled : 1;
    unsigned ObjSize : 1;
    unsigned BgTileMapArea : 1;
    unsigned BgWindowTileDataArea : 1;
    unsigned WindowEnabled : 1;
    unsigned WindowTileMapArea : 1;
    unsigned LcdPpuEbabled : 1;
};

#endif //LCDCONTROL_H
