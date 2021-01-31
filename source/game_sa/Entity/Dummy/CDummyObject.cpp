#include "StdInc.h"

void CDummyObject::UpdateFromObject(CObject* pObject)
{
    plugin::CallMethod<0x59EB70, CDummyObject*, CObject*>(this, pObject);
}
