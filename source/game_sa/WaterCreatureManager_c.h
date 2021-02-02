#pragma once
#include "WaterCreature_c.h"

enum eWaterCreatureType : unsigned char {
    FISH1,
    FISH2,
    FISH3,
    JELLYFISH1,
    JELLYFISH2,
    TURTLE,
    DOLPHIN
};

struct WaterCreatureInfo
{
    uint16_t* m_pModelId;
    uint8_t m_nMinCreated;
    uint8_t m_nMaxCreated;
    uint16_t m_wUnkn;
    float m_fSpawnDepth;
    float m_fMinDistFromSameCreature;
    float m_fMinDistFromFollowed;
    float m_fMaxDistFromFollowed;
    float m_fMinAngle;
    float m_fMaxAngle;
    float m_fMaxHeadingChange;
    float m_fChanceToRandomizeRotation;
    float m_fSpeed;
};
VALIDATE_SIZE(WaterCreatureInfo, 0x2C);

class WaterCreatureManager_c
{
public:
    WaterCreatureManager_c();

public:
    static constexpr int NUM_WATER_CREATURES = 128;
    WaterCreature_c m_aCreatures[NUM_WATER_CREATURES];
    TList_c<WaterCreature_c> m_freeList;
    TList_c<WaterCreature_c> m_createdList;
    int m_nUnk4;

public:
    static void InjectHooks();

    bool Init();
    void Exit();
    int GetRandomWaterCreatureId();
    void TryToFreeUpWaterCreatures(int numToFree);
    bool CanAddWaterCreatureAtPos(int nCreatureType, CVector vecPos);
    void TryToExitGroup(WaterCreature_c* pCreature);
    void Update(float fTimestep);

public:
    static constexpr int NUM_WATER_CREATURE_INFOS = 7;
    static WaterCreatureInfo(&ms_waterCreatureInfos)[NUM_WATER_CREATURE_INFOS];
};

extern WaterCreatureManager_c& g_waterCreatureMan;
