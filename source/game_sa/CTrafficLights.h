#pragma once
#include "PluginBase.h"
#include "CVehicle.h"

class CTrafficLights {
public:
    static RwUInt8(&aTrafficLightColoursR)[3];
    static RwUInt8(&aTrafficLightColoursG)[3];
    static RwUInt8(&aTrafficLightColoursB)[3];

public:
    static void InjectHooks();

public:
    static bool ShouldCarStopForLight(CVehicle* pVehicle, bool bUnkn);
    static bool ShouldCarStopForBridge(CVehicle* pVehicle);
    static void DisplayActualLight(CEntity* pEntity);
    static bool IsMITrafficLight(int modelIndex);

private:
    static unsigned char LightForCars1(); //Are red lights on
    static unsigned char LightForCars2(); //Are orangle lights on
    static unsigned char LightForCars1_Visual(); // Switch lights to tellow
    static unsigned char LightForCars2_Visual(); // Switch lights to green
    static int FindTrafficLightType(CEntity* pEntity);
    static int FindTrafficLightTypeFromOrientation(float fOrientation);
};
