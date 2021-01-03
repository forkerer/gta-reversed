#include "StdInc.h"

void CCustomBuildingDNPipeline::UnknSetup(RpAtomic* pAtomic, bool bUnkn)
{
    plugin::Call<0x5D7200, RpAtomic*, bool>(pAtomic, bUnkn);
}
