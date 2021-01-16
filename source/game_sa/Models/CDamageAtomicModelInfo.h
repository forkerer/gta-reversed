#pragma once
#include "CAtomicModelInfo.h"

class CDamageAtomicModelInfo : public CAtomicModelInfo
{
public:
    RpAtomic* m_pDamagedAtomic;

public:
    static bool& ms_bCreateDamagedVersion;
};

VALIDATE_SIZE(CDamageAtomicModelInfo, 0x24);
