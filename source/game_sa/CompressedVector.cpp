#include "StdInc.h"

// 0x411590
void UncompressVector(CVector* outVec, CompressedVector* compressedVec)
{
    outVec->x = static_cast<float>(compressedVec->x) / 128.0F;
    outVec->y = static_cast<float>(compressedVec->y) / 128.0F;
    outVec->z = static_cast<float>(compressedVec->z) / 128.0F;
}

CVector UncompressVector(CompressedVector* compressedVec)
{
    CVector outVec;
    UncompressVector(&outVec, compressedVec);
    return outVec;
}
