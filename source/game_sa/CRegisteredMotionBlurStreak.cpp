#include "StdInc.h"

void CRegisteredMotionBlurStreak::Update()
{
    plugin::CallMethod<0x721EE0, CRegisteredMotionBlurStreak*>(this);
}

void CRegisteredMotionBlurStreak::Render()
{
    plugin::CallMethod<0x721F70, CRegisteredMotionBlurStreak*>(this);
}
