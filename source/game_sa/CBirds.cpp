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
    CBirds::uiNumberOfBirds = 0;
    CBirds::bHasBirdBeenShot = false;
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
    CBirds::uiNumberOfBirds = 0;
#endif
}

void CBirds::Update()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__cdecl*)())0x712330)();
#else
    auto vecCamPos = TheCamera.GetPosition();

    if (!CGame::currArea
        && CBirds::uiNumberOfBirds < 6
        && CClock::ms_nGameClockHours < 22u
        && CClock::ms_nGameClockHours > 5u
        && (CTimer::m_FrameCounter & 0x1FF) == 6) {

        int iNumBirdsToCreate = CGeneral::GetRandomNumberInRange(1, 7 - CBirds::uiNumberOfBirds);
        eBirdsBiome eBiome = BIOME_WATER;

        if (TheCamera.m_fDistanceToWater > 30.0F){
            if (CWeather::WeatherRegion == eWeatherRegion::WEATHER_REGION_DESERT) {
                eBiome = BIOME_DESERT;
                if (iNumBirdsToCreate >= 3 - CBirds::uiNumberOfBirds)
                    iNumBirdsToCreate = 3 - CBirds::uiNumberOfBirds;
            }
            else {
                eBiome = BIOME_NORMAL;
            }
        }

        if (iNumBirdsToCreate > 0) {
            rand(); // Called 2 times for some reason
            rand();

            float fFlightHeight;
            float fSpawnDistance;

            switch (eBiome) {
            case BIOME_WATER:
                fFlightHeight = CGeneral::GetRandomNumberInRange(4.0F, 13.0F);
                fSpawnDistance = 45.0F;
                break;
            case BIOME_DESERT:
                fFlightHeight = CGeneral::GetRandomNumberInRange(15.0F, 25.0F);
                fSpawnDistance = 80.0F;
                break;
            case BIOME_NORMAL:
                fFlightHeight = CGeneral::GetRandomNumberInRange(2.0F, 10.0F);
                fSpawnDistance = 40.0F;
                break;
            }

            if (fFlightHeight > 5.0F) {
                float fBirdSpawnZ = fFlightHeight + vecCamPos.z;
                float fSpawnAngleCamRelative;

                if (rand() & 1)
                    fSpawnAngleCamRelative = (float)((char)rand()) * 0.024531251; // [0-255] mapped to [0 - 2π]
                else {
                    auto vecCamForward = TheCamera.m_mCameraMatrix.GetForward();
                    vecCamForward.z = 0;
                    if (vecCamForward.x == 0.0F)
                        vecCamForward.x = 0.01F;

                    fSpawnAngleCamRelative = (float)((char)rand() - 128) / 256.0F + atan2(vecCamForward.x, vecCamForward.y);
                }
            }
        }
    }


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
            CBirds::vecBirdShotAt = pBird.m_vecPosn;
            CBirds::bHasBirdBeenShot = true;
            CBirds::uiNumberOfBirds--;
            pBird.m_bCreated = false;
        }
    }
#endif
}
