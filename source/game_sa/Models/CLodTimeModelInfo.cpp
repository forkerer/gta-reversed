#include "StdInc.h"

void CLodTimeModelInfo::InjectHooks()
{
    ReversibleHooks::Install("CLodTimeModelInfo", "GetModelType", 0x4C5660, &CLodTimeModelInfo::GetModelType_Reversed);
    ReversibleHooks::Install("CLodTimeModelInfo", "GetTimeInfo", 0x4C5670, &CLodTimeModelInfo::GetTimeInfo_Reversed);
}

CLodTimeModelInfo::CLodTimeModelInfo() : CTimeModelInfo()
{
    m_timeInfo.m_nTimeOn = 0;
    m_timeInfo.m_nTimeOff = 0;
    m_timeInfo.m_wOtherTimeModel = 0;
    m_lodTimeInfo.m_wOtherTimeModel = -1;
}

ModelInfoType CLodTimeModelInfo::GetModelType()
{
    return CLodTimeModelInfo::GetModelType_Reversed();
}
ModelInfoType CLodTimeModelInfo::GetModelType_Reversed()
{
    return ModelInfoType::MODEL_INFO_TIME;
}

tTimeInfo* CLodTimeModelInfo::GetTimeInfo()
{
    return CLodTimeModelInfo::GetTimeInfo_Reversed();
}
tTimeInfo* CLodTimeModelInfo::GetTimeInfo_Reversed()
{
    return &m_timeInfo;
}
