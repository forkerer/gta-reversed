#include "StdInc.h"

bool& CBirds::bHasBirdBeenShot = *(bool*)0xC6A8A0;
unsigned int& CBirds::uiNumberOfBirds = *(unsigned int*)0xC6A8A4;
CBird* CBirds::aBirds = (CBird*)0xC6A8B0;
CVector& CBirds::vecBirdShotAt = *(CVector*)0xC6AA48;

float* BIRD_CREATION_COORS_X = (float*)0x8D5250; // Size: 6
float* BIRD_CREATION_COORS_Y = (float*)0x8D5268; // Size: 6
float* BIRD_CREATION_COORS_Z = (float*)0x8D5280; // Size: 6
float* BIRD_CREATION_COORS_U = (float*)0x8D5298; // Size: 8
float* BIRD_CREATION_COORS_V = (float*)0x8D52B8; // Size: 8

void CBirds::InjectHooks()
{
    HookInstall(0x711EC0, &CBirds::Init);
    HookInstall(0x712300, &CBirds::Shutdown);
    HookInstall(0x712E40, &CBirds::HandleGunShot);
}

void CBirds::Init()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__cdecl *)())0x711EC0)();
#else
    for (int32_t i = 0; i < MAX_BIRDS; ++i) {
        auto& pBird = aBirds[i];
        pBird.m_bCreated = false;
    }
    uiNumberOfBirds = 0;
    bHasBirdBeenShot = false;
#endif
}

void CBirds::Shutdown()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__cdecl*)())0x712300)();
#else
    for (int32_t i = 0; i < MAX_BIRDS; ++i) {
        auto& pBird = aBirds[i];
        if (pBird.m_bCreated)
            pBird.m_bCreated = false;
    }
    uiNumberOfBirds = 0;
#endif
}

void CBirds::HandleGunShot(CVector const* pointA, CVector const* pointB)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__cdecl*)(CVector const*, CVector const*))0x712E40)(pointA, pointB);
#else
    auto colLine = CColLine();
    colLine.Set(*pointA, *pointB);

    for (int32_t i = 0; i < MAX_BIRDS; ++i) {
        auto& pBird = aBirds[i];
        if (!pBird.m_bCreated)
            continue;

        auto birdSphere = CColSphere();
        birdSphere.m_fRadius = 0.5F;
        birdSphere.m_vecCenter = pBird.m_vecPosn;

        if (CCollision::TestLineSphere(colLine, birdSphere))
        {
            vecBirdShotAt = pBird.m_vecPosn;
            bHasBirdBeenShot = true;
            pBird.m_bCreated = false;
            uiNumberOfBirds--;
        }
    }
#endif
}
