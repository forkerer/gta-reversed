#pragma once
#include "PluginBase.h"
#include "RenderWare.h"

class CCustomBuildingDNPipeline {
public:
    static bool& m_bCameraChange;
    static RwUInt32& m_AtmDNWorkingIndex;
    static RxPipeline*& ObjPipeline;
    static RwUInt32& ms_extraVertColourPluginOffset;
    static float& m_fDNBalanceParam;

public:
    static void UnknSetup(RpAtomic* pAtomic, bool bUnkn); //Temporary function name, it doesn't exist in Android IDB, and it's not yet known what it does
};
