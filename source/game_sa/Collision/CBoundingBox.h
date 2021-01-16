/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"
#include "CBox.h"

class CBoundingBox : public CBox {
public:
    CBoundingBox();
};

VALIDATE_SIZE(CBoundingBox, 0x18);

extern int32_t& numBBFailed;
