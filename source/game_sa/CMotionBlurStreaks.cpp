#include "StdInc.h"

CRegisteredMotionBlurStreak(&CMotionBlurStreaks::aStreaks)[MAX_NUM_MOTIONBLUR_STREAKS] = *(CRegisteredMotionBlurStreak(*)[MAX_NUM_MOTIONBLUR_STREAKS])0xC7CA08;

void CMotionBlurStreaks::InjectHooks()
{
    ReversibleHooks::Install("CMotionBlurStreaks", "Init", 0x721D90, &CMotionBlurStreaks::Init);
    ReversibleHooks::Install("CMotionBlurStreaks", "Update", 0x7240C0, &CMotionBlurStreaks::Update);
}

void CMotionBlurStreaks::Init()
{
    for (auto& pStreak : CMotionBlurStreaks::aStreaks)
        pStreak.m_nId = 0;
}

void CMotionBlurStreaks::Update()
{
    for (auto& pStreak : CMotionBlurStreaks::aStreaks) {
        if (pStreak.m_nId)
            pStreak.Update();
    }
}

void CMotionBlurStreaks::Render()
{
    plugin::Call<0x7240E0>();
}

void CMotionBlurStreaks::RegisterStreak(unsigned int id, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, CVector leftPoint, CVector rightPoint)
{
    plugin::Call<0x721DC0, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, CVector, CVector>(id, red, green, blue, alpha, leftPoint, rightPoint);
}
