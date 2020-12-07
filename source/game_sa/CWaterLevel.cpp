#include "StdInc.h"

bool CWaterLevel::GetWaterLevel(float x, float y, float z, float * pOutWaterLevel, char bUnderWater, CVector* pVector)
{
    return plugin::CallAndReturn<bool, 0x6EB690, float, float, float, float*, char, CVector*>(x, y, z, pOutWaterLevel, bUnderWater, pVector);
}

bool CWaterLevel::GetWaterLevelNoWaves(float x, float y, float z, float* pOutWaterLevel, float* fUnkn1, float* fUnkn2) {
    return ((bool(__cdecl*)(float, float, float, float*, float*, float*))0x6E8580)(x, y, z, pOutWaterLevel, fUnkn1, fUnkn2);
}
