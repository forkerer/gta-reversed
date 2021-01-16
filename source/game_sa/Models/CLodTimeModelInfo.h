#pragma once
#include "PluginBase.h"
#include "CAtomicModelInfo.h"

class  CLodTimeModelInfo : public CAtomicModelInfo {
public:
    tTimeInfo m_timeInfo;
};

VALIDATE_SIZE(CLodTimeModelInfo, 0x24);
