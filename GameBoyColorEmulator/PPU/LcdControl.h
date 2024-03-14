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

    uint8_t toByte() const
    {
        return (LcdPpuEbabled << 7) |
            (WindowTileMapArea << 6) |
            (WindowEnabled << 5) |
            (BgWindowTileDataArea << 4) |
            (BgTileMapArea << 3) |
            (ObjSize << 2)
            | (ObjEnabled << 1)
            | BgWindowEnabled;
    }
};

#endif //LCDCONTROL_H
