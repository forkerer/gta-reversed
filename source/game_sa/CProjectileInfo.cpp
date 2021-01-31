#include "StdInc.h"

void CProjectileInfo::RemoveAllProjectiles()
{
    plugin::Call<0x7399B0>();
}

bool CProjectileInfo::RemoveIfThisIsAProjectile(CObject* object)
{
    return plugin::CallAndReturn<bool, 0x739A40, CObject*>(object);
}
