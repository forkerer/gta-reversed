#pragma once
#include "CAtomicModelInfo.h"

class CLodAtomicModelInfo : public CAtomicModelInfo
{
public:
    __int16 field_20;
    __int16 field_22;

public:
    static bool& ms_bCreateDamagedVersion;
};

VALIDATE_SIZE(CLodAtomicModelInfo, 0x24);
