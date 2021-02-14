#include "StdInc.h"

void CCullZones::Init()
{
    plugin::Call<0x72D6B0>();
}

bool CCullZones::DoExtraAirResistanceForPlayer()
{
    return plugin::CallAndReturn<bool, 0x72DDD0>();
}

eZoneAttributes CCullZones::FindTunnelAttributesForCoors(CVector point)
{
    return plugin::CallAndReturn<eZoneAttributes, 0x72D9F0, CVector>(point);
}
