#include "StdInc.h"

WaterCreatureManager_c& g_waterCreatureMan = *(WaterCreatureManager_c*)0xC1DF30;
WaterCreatureInfo(&WaterCreatureManager_c::ms_waterCreatureInfos)[NUM_WATER_CREATURE_INFOS] = *(WaterCreatureInfo(*)[NUM_WATER_CREATURE_INFOS])0x8D3698;

void WaterCreatureManager_c::InjectHooks()
{
}


WaterCreatureManager_c::WaterCreatureManager_c() : m_aCreatures(), m_freeList(), m_createdList()
{}

bool WaterCreatureManager_c::Init()
{
    for (auto i = 0; i < NUM_WATER_CREATURES; ++i)
        m_freeList.AddItem(&m_aCreatures[i]);

    m_nUnk4 = 0;
    return true;
}

void WaterCreatureManager_c::Exit()
{
    auto* pCur = m_createdList.GetTail();
    while (pCur)
    {
        auto pPrev = m_createdList.GetPrev(pCur);
        m_createdList.RemoveItem(pCur);
        m_
    }
}

int WaterCreatureManager_c::GetRandomWaterCreatureId()
{
}

void WaterCreatureManager_c::TryToFreeUpWaterCreatures(int numToFree)
{
}

bool WaterCreatureManager_c::CanAddWaterCreatureAtPos(int nCreatureType, CVector vecPos)
{
}

void WaterCreatureManager_c::TryToExitGroup(WaterCreature_c* pCreature)
{
}

void WaterCreatureManager_c::Update(float fTimestep)
{
}
