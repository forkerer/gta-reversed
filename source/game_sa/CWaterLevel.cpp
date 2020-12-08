#include "StdInc.h"

void CWaterLevel::InjectHooks()
{
    ReversibleHooks::Install("CWaterLevel", "GetWaterLevel", 0x6EB690, &CWaterLevel::GetWaterLevel);
    //ReversibleHooks::Install("CWaterLevel", "AddWaveToResult", 0x6E81E0, &CWaterLevel::AddWaveToResult);
    //ReversibleHooks::Install("CWaterLevel", "GetWaterLevelNoWaves", 0x6E8580, &CWaterLevel::GetWaterLevelNoWaves);
}

void CWaterLevel::AddWaveToResult(float x, float y, float* pfWaterLevel, float fUnkn1, float fUnkn2, CVector* pVecNormal)
{
    plugin::Call<0x6E81E0, float, float, float*, float, float, CVector*>(x, y, pfWaterLevel, fUnkn1, fUnkn2, pVecNormal);
    return;

    /*if (!ReversibleHooks::Hooked("CWaterLevel", "ProcessBuoyancy")) {
        plugin::Call<0x6E81E0, float, float, float*, float, float, CVector*>(x, y, pfWaterLevel, fUnkn1, fUnkn2, pVecNormal);
        return;
    }*/
}

bool CWaterLevel::GetWaterLevel(float x, float y, float z, float* pOutWaterLevel, unsigned char bTouchingWater, CVector* pVecNormals)
{
    if (!ReversibleHooks::Hooked("CWaterLevel", __func__)) {
        return plugin::CallAndReturn<bool, 0x6EB690, float, float, float, float*, unsigned char, CVector*>(x, y, z, pOutWaterLevel, bTouchingWater, pVecNormals);
    }

    float fUnkn1, fUnkn2;
    if (!CWaterLevel::GetWaterLevelNoWaves(x, y, z, pOutWaterLevel, &fUnkn1, &fUnkn2))
        return false;

    if ((*pOutWaterLevel - z > 3.0F) && !bTouchingWater) {
        *pOutWaterLevel = 0.0F;
        return false;
    }

    CWaterLevel::AddWaveToResult(x, y, pOutWaterLevel, fUnkn1, fUnkn2, pVecNormals);
    return true;
}

bool CWaterLevel::GetWaterLevelNoWaves(float x, float y, float z, float* pOutWaterLevel, float* fUnkn1, float* fUnkn2) {
    return plugin::CallAndReturn<bool, 0x6E8580, float, float, float, float*, float*, float*>(x, y, z, pOutWaterLevel, fUnkn1, fUnkn2);
}
