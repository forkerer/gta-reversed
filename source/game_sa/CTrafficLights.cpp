#include "StdInc.h"

RwUInt8(&CTrafficLights::aTrafficLightColoursR)[3] = *(RwUInt8(*)[3])0x8A6214;
RwUInt8(&CTrafficLights::aTrafficLightColoursG)[3] = *(RwUInt8(*)[3])0x8A6218;
RwUInt8(&CTrafficLights::aTrafficLightColoursB)[3] = *(RwUInt8(*)[3])0xA9AD94;

void CTrafficLights::InjectHooks()
{
    ReversibleHooks::Install("CTrafficLights", "IsMITrafficLight", 0x49D5A0, &CTrafficLights::IsMITrafficLight);
}

bool CTrafficLights::ShouldCarStopForLight(CVehicle* pVehicle, bool bUnkn)
{
    return plugin::CallAndReturn<bool, 0x49D610, CVehicle*, bool>(pVehicle, bUnkn);
}

bool CTrafficLights::ShouldCarStopForBridge(CVehicle* pVehicle)
{
    return plugin::CallAndReturn<bool, 0x49D420, CVehicle*>(pVehicle);
}

int CTrafficLights::FindTrafficLightType(CEntity* pEntity)
{
    return plugin::CallAndReturn<int, 0x49D580, CEntity*>(pEntity);
}

void CTrafficLights::DisplayActualLight(CEntity* pEntity)
{
    plugin::Call<0x49DAB0, CEntity*>(pEntity);
}

bool CTrafficLights::IsMITrafficLight(int modelIndex)
{
    return modelIndex == ModelIndices::MI_TRAFFICLIGHTS
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_VERTICAL
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_MIAMI
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_VEGAS
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_TWOVERTICAL
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_3
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_4
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_5
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_GAY;
}

unsigned char CTrafficLights::LightForCars1()
{
    return plugin::CallAndReturn<unsigned char, 0x49D2D0>();
}

unsigned char CTrafficLights::LightForCars2()
{
    return plugin::CallAndReturn<unsigned char, 0x49D310>();
}

unsigned char CTrafficLights::LightForCars1_Visual()
{
    return plugin::CallAndReturn<unsigned char, 0x49D350>();
}

unsigned char CTrafficLights::LightForCars2_Visual()
{
    return plugin::CallAndReturn<unsigned char, 0x49D3A0>();
}

int CTrafficLights::FindTrafficLightTypeFromOrientation(float fOrientation)
{
    return plugin::CallAndReturn<int, 0x49D4D0, float>(fOrientation);
}
