#include "StdInc.h"

void CPtrListSingleLink::AddItem(void* item)
{
    plugin::CallMethod<0x5335E0, CPtrListSingleLink*, void*>(this, item);
}

void CPtrListSingleLink::DeleteItem(void* item)
{
    plugin::CallMethod<0x533610, CPtrListSingleLink*, void*>(this, item);
}
