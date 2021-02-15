#include "StdInc.h"

uint8_t* CAnimBlendSequence::GetFrame(int iFrame)
{
    return plugin::CallMethodAndReturn<uint8_t*, 0x4CF1F0, CAnimBlendSequence*, int>(this, iFrame);
}
