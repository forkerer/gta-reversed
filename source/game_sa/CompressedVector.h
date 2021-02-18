/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"

class  CompressedVector {
public:
    signed short x;
    signed short y;
    signed short z;
};

void UncompressVector(CVector* outVec, CompressedVector* compressedVec);
CVector UncompressVector(CompressedVector* compressedVec);

VALIDATE_SIZE(CompressedVector, 6);
