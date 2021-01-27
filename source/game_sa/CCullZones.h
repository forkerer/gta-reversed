#pragma once
#include "CVector.h"

enum eZoneAttributes : std::uint16_t
{
    ZONE_ATTRIBUTE_CAMCLOSEIN_FORPLAYER = 0x1,
    ZONE_ATTRIBUTE_CAMSTAIRS_FORPLAYER = 0x2,
    ZONE_ATTRIBUTE_CAM1STPERSONS_FORPLAYER = 0x4,
    ZONE_ATTRIBUTE_CAMNORAIN = 0x8,
    ZONE_ATTRIBUTE_UNKNOWN_1 = 0x80,
    ZONE_ATTRIBUTE_POLICES_ABANDONCARS = 0x100,
    ZONE_ATTRIBUTE_UNKNOWN_2 = 0x800,
    ZONE_ATTRIBUTE_INROOMS_FORAUDIO = 0x200,
    ZONE_ATTRIBUTE_MILITARYZONE = 0x1000,
    ZONE_ATTRIBUTE_EXTRA_AIRRESISTANCE = 0x4000,
    ZONE_ATTRIBUTE_FEWERCARS = 0x8000,
};

class CCullZones
{
public:
    static void Init();
    static bool DoExtraAirResistanceForPlayer();
    static eZoneAttributes FindTunnelAttributesForCoors(CVector point);
};
