#include "StdInc.h"

int& snNumActiveModifiers = *(int*)0xC81450;
CWindModifier(&CWindModifiers::saModifiers)[MAX_NUM_MODIFIERS] = *(CWindModifier(*)[MAX_NUM_MODIFIERS])0xC81458;

void CWindModifiers::FindWindModifier(CVector vecPos, float* pOutX, float* pOutY)
{
    plugin::Call<0x72C950, CVector, float*, float*>(vecPos, pOutX, pOutY);
}

void CWindModifiers::RegisterOne(CVector vecPos, int iActive, float fPower)
{
    plugin::Call<0x72C8B0, CVector, int, float>(vecPos, iActive, fPower);
}
