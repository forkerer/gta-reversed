#include "StdInc.h"

void CPtrListDoubleLink::Flush()
{
    plugin::CallMethod<0x552470, CPtrListDoubleLink*>(this);
}

void CPtrListDoubleLink::AddItem(void* item)
{
    plugin::CallMethod<0x533670, CPtrListDoubleLink*, void*>(this, item);
}

void CPtrListDoubleLink::DeleteItem(void* item)
{
    plugin::CallMethod<0x5336B0, CPtrListDoubleLink*, void*>(this, item);
}
