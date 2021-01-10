#include "StdInc.h"

void CPtrListDoubleLink::InjectHooks()
{
    ReversibleHooks::Install("CPtrListDoubleLink", "AddItem", 0x533670, &CPtrListDoubleLink::AddItem);
}

void CPtrListDoubleLink::Flush()
{
    plugin::CallMethod<0x552470, CPtrListDoubleLink*>(this);
}

CPtrNodeDoubleLink* CPtrListDoubleLink::AddItem(void* item)
{
    auto pNewDoubleLink = new CPtrNodeDoubleLink(item);
    pNewDoubleLink->AddToList(this);
    return pNewDoubleLink;

}

void CPtrListDoubleLink::DeleteItem(void* item)
{
    plugin::CallMethod<0x5336B0, CPtrListDoubleLink*, void*>(this, item);
}
