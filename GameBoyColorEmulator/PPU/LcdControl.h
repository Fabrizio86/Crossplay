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

    void fromByte(ui8 byte)
    {
        LcdPpuEbabled = (byte >> 7) & 1U;
        WindowTileMapArea = (byte >> 6) & 1U;
        WindowEnabled = (byte >> 5) & 1U;
        BgWindowTileDataArea = (byte >> 4) & 1U;
        BgTileMapArea = (byte >> 3) & 1U;
        ObjSize = (byte >> 2) & 1U;
        ObjEnabled = (byte >> 1) & 1U;
        BgWindowEnabled = byte & 1U;
    }
};

#endif //LCDCONTROL_H
