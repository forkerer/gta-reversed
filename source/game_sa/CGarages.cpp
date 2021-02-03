#include "StdInc.h"

void CGarages::Init()
{
    plugin::Call<0x447120>();
}

void CGarages::PrintMessages()
{
    plugin::Call<0x447790>();
}

void CGarages::TriggerMessage(char* cTagMsg, short wMsgMin, unsigned short ucTime, short wMsgMax)
{
    plugin::Call<0x447B80, char*, short, unsigned short, short>(cTagMsg, wMsgMin, ucTime, wMsgMax);
}

bool CGarages::IsModelIndexADoor(int nModelIndex)
{
    return plugin::CallAndReturn<bool, 0x448AF0, int>(nModelIndex);
}

int CGarages::FindGarageForObject(CObject* pObject)
{
    return plugin::CallAndReturn<int, 0x44A240, CObject*>(pObject);
}
