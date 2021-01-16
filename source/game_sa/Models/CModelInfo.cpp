/*
Plugin-SDK (Grand Theft Auto San Andreas) source file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

CBaseModelInfo *(&CModelInfo::ms_modelInfoPtrs)[NUM_MODEL_INFOS] = *(CBaseModelInfo*(*)[NUM_MODEL_INFOS])0xA9B0C8;

CStore<CAtomicModelInfo, CModelInfo::NUM_ATOMIC_MODEL_INFOS>& CModelInfo::ms_atomicModelInfoStore = *(CStore<CAtomicModelInfo, NUM_ATOMIC_MODEL_INFOS>*)0xAAE950;
CStore<CDamageAtomicModelInfo, CModelInfo::NUM_DAMAGE_ATOMIC_MODEL_INFOS>& CModelInfo::ms_damageAtomicModelInfoStore = *(CStore<CDamageAtomicModelInfo, NUM_DAMAGE_ATOMIC_MODEL_INFOS>*)0xB1BF58;
CStore<CLodAtomicModelInfo, CModelInfo::NUM_LOD_ATOMIC_MODEL_INFOS>& CModelInfo::ms_lodAtomicModelInfoStore = *(CStore<CLodAtomicModelInfo, NUM_LOD_ATOMIC_MODEL_INFOS>*)0xB1C934;
CStore<CTimeModelInfo, CModelInfo::NUM_TIME_MODEL_INFOS>& CModelInfo::ms_timeModelInfoStore = *(CStore<CTimeModelInfo, NUM_TIME_MODEL_INFOS>*)0xB1C960;
CStore<CLodTimeModelInfo, CModelInfo::NUM_LOD_TIME_MODEL_INFOS>& CModelInfo::ms_lodTimeModelInfoStore = *(CStore<CLodTimeModelInfo, NUM_LOD_TIME_MODEL_INFOS>*)0xB1E128;
CStore<CWeaponModelInfo, CModelInfo::NUM_WEAPON_MODEL_INFOS>& CModelInfo::ms_weaponModelInfoStore = *(CStore<CWeaponModelInfo, NUM_WEAPON_MODEL_INFOS>*)0xB1E158;
CStore<CClumpModelInfo, CModelInfo::NUM_CLUMP_MODEL_INFOS>& CModelInfo::ms_clumpModelInfoStore = *(CStore<CClumpModelInfo, NUM_CLUMP_MODEL_INFOS>*)0xB1E958;
CStore<CVehicleModelInfo, CModelInfo::NUM_VEHICLE_MODEL_INFOS>& CModelInfo::ms_vehicleModelInfoStore = *(CStore<CVehicleModelInfo, NUM_VEHICLE_MODEL_INFOS>*)0xB1F650;
CStore<CPedModelInfo, CModelInfo::NUM_PED_MODEL_INFOS>& CModelInfo::ms_pedModelInfoStore = *(CStore<CPedModelInfo, NUM_PED_MODEL_INFOS>*)0xB478F8;
CStore<C2dEffect, CModelInfo::NUM_2DFX_INFOS>& CModelInfo::ms_2dFXInfoStore = *(CStore<C2dEffect, NUM_2DFX_INFOS>*)0xB4C2D8;

void CModelInfo::InjectHooks()
{
    ReversibleHooks::Install("CModelInfo", "Initialise", 0x4C6810, &CModelInfo::Initialise);
    ReversibleHooks::Install("CModelInfo", "ShutDown", 0x4C63E0, &CModelInfo::ShutDown);

    ReversibleHooks::Install("CModelInfo", "AddAtomicModel", 0x4C6620, &CModelInfo::AddAtomicModel);
    ReversibleHooks::Install("CModelInfo", "AddDamageAtomicModel", 0x4C6650, &CModelInfo::AddDamageAtomicModel);
    ReversibleHooks::Install("CModelInfo", "AddLodAtomicModel", 0x4C6680, &CModelInfo::AddLodAtomicModel);
    ReversibleHooks::Install("CModelInfo", "AddTimeModel", 0x4C66B0, &CModelInfo::AddTimeModel);
    ReversibleHooks::Install("CModelInfo", "AddLodTimeModel", 0x4C66E0, &CModelInfo::AddLodTimeModel);
    ReversibleHooks::Install("CModelInfo", "AddWeaponModel", 0x4C6710, &CModelInfo::AddWeaponModel);
    ReversibleHooks::Install("CModelInfo", "AddClumpModel", 0x4C6740, &CModelInfo::AddClumpModel);
    ReversibleHooks::Install("CModelInfo", "AddVehicleModel", 0x4C6770, &CModelInfo::AddVehicleModel);
    ReversibleHooks::Install("CModelInfo", "AddPedModel", 0x4C67A0, &CModelInfo::AddPedModel);
}

// Converted from stdcall void CModelInfo::ReInit2dEffects(void) 0x4C63B0
void CModelInfo::ReInit2dEffects()
{
    ((void(__cdecl *)())0x4C63B0)();
}

// Converted from stdcall void CModelInfo::ShutDown(void) 0x4C63E0
void CModelInfo::ShutDown()
{
    for (int32_t i = 0; i < ms_atomicModelInfoStore.m_nCount; ++i)
        ms_atomicModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_damageAtomicModelInfoStore.m_nCount; ++i)
        ms_damageAtomicModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_lodAtomicModelInfoStore.m_nCount; ++i)
        ms_lodAtomicModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_timeModelInfoStore.m_nCount; ++i)
        ms_timeModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_lodTimeModelInfoStore.m_nCount; ++i)
        ms_lodTimeModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_weaponModelInfoStore.m_nCount; ++i)
        ms_weaponModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_clumpModelInfoStore.m_nCount; ++i)
        ms_clumpModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_vehicleModelInfoStore.m_nCount; ++i)
        ms_vehicleModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_pedModelInfoStore.m_nCount; ++i)
        ms_pedModelInfoStore.GetItemAtIndex(i).Shutdown();

    for (int32_t i = 0; i < ms_2dFXInfoStore.m_nCount; ++i) {
        auto& pEffect = ms_2dFXInfoStore.GetItemAtIndex(i);
        if (pEffect.m_nType == e2dEffectType::EFFECT_ROADSIGN) {
            if (pEffect.roadsign.m_pText) {
                CMemoryMgr::Free(pEffect.roadsign.m_pText);
                pEffect.roadsign.m_pText = nullptr;
            }

            if (pEffect.roadsign.m_pAtomic) {
                auto pFrame = RpAtomicGetFrame(pEffect.roadsign.m_pAtomic);
                if (pFrame) {
                    RpAtomicSetFrame(pEffect.roadsign.m_pAtomic, nullptr);
                    RwFrameDestroy(pFrame);
                }
                RpAtomicDestroy(pEffect.roadsign.m_pAtomic);
                pEffect.roadsign.m_pAtomic = nullptr;
            }
        }
        else if (pEffect.m_nType == e2dEffectType::EFFECT_LIGHT) {
            if (pEffect.light.m_pCoronaTex) {
                RwTextureDestroy(pEffect.light.m_pCoronaTex);
                pEffect.light.m_pCoronaTex = nullptr;
            }

            if (pEffect.light.m_pShadowTex) {
                RwTextureDestroy(pEffect.light.m_pShadowTex);
                pEffect.light.m_pShadowTex = nullptr;
            }
        }
    }

    ms_atomicModelInfoStore.m_nCount = 0;
    ms_damageAtomicModelInfoStore.m_nCount = 0;
    ms_lodAtomicModelInfoStore.m_nCount = 0;
    ms_timeModelInfoStore.m_nCount = 0;
    ms_lodTimeModelInfoStore.m_nCount = 0;
    ms_weaponModelInfoStore.m_nCount = 0;
    ms_clumpModelInfoStore.m_nCount = 0;
    ms_vehicleModelInfoStore.m_nCount = 0;
    ms_pedModelInfoStore.m_nCount = 0;
    ms_2dFXInfoStore.m_nCount = 0;
}

// Converted from stdcall CAtomicModelInfo* CModelInfo::AddAtomicModel(int index) 0x4C6620
CAtomicModelInfo* CModelInfo::AddAtomicModel(int index)
{
    auto& pInfo = ms_atomicModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CDamageAtomicModelInfo* CModelInfo::AddDamageAtomicModel(int index) 0x4C6650
CDamageAtomicModelInfo* CModelInfo::AddDamageAtomicModel(int index)
{
    auto& pInfo = ms_damageAtomicModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CLodAtomicModelInfo* CModelInfo::AddLodAtomicModel(int index) 0x4C6680
CLodAtomicModelInfo* CModelInfo::AddLodAtomicModel(int index)
{
    auto& pInfo =  ms_lodAtomicModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CTimeModelInfo* CModelInfo::AddTimeModel(int index) 0x4C66B0
CTimeModelInfo* CModelInfo::AddTimeModel(int index)
{
    auto& pInfo = ms_timeModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CLodTimeModelInfo* CModelInfo::AddLodTimeModel(int index) 0x4C66E0
CLodTimeModelInfo* CModelInfo::AddLodTimeModel(int index)
{
    auto& pInfo = ms_lodTimeModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CWeaponModelInfo* CModelInfo::AddWeaponModel(int index) 0x4C6710
CWeaponModelInfo* CModelInfo::AddWeaponModel(int index)
{
    auto& pInfo = ms_weaponModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CClumpModelInfo* CModelInfo::AddClumpModel(int) index 0x4C6740
CClumpModelInfo* CModelInfo::AddClumpModel(int index)
{
    auto& pInfo = ms_clumpModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CVehicleModelInfo* CModelInfo::AddVehicleModel(int index) 0x4C6770
CVehicleModelInfo* CModelInfo::AddVehicleModel(int index)
{
    auto& pInfo = ms_vehicleModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall CPedModelInfo* CModelInfo::AddPedModel(int index) 0x4C67A0
CPedModelInfo* CModelInfo::AddPedModel(int index)
{
    auto& pInfo = ms_pedModelInfoStore.AddItem();
    pInfo.Init();
    CModelInfo::SetModelInfo(index, &pInfo);
    return &pInfo;
}

// Converted from stdcall void CModelInfo::Initialise(void) 0x4C6810
void CModelInfo::Initialise()
{
    memset(ms_modelInfoPtrs, 0, sizeof(ms_modelInfoPtrs));
    ms_damageAtomicModelInfoStore.m_nCount = 0;
    ms_lodAtomicModelInfoStore.m_nCount = 0;
    ms_timeModelInfoStore.m_nCount = 0;
    ms_lodTimeModelInfoStore.m_nCount = 0;
    ms_weaponModelInfoStore.m_nCount = 0;
    ms_clumpModelInfoStore.m_nCount = 0;
    ms_vehicleModelInfoStore.m_nCount = 0;
    ms_pedModelInfoStore.m_nCount = 0;
    ms_2dFXInfoStore.m_nCount = 0;
    ms_atomicModelInfoStore.m_nCount = 0;

    auto pDoor1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_DOOR1);
    pDoor1->SetColModel(&CTempColModels::ms_colModelDoor1, false);
    pDoor1->SetTexDictionary("generic");
    pDoor1->m_fDrawDistance = 80.0F;

    auto pBumper1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_BUMPER1);
    pBumper1->SetColModel(&CTempColModels::ms_colModelBumper1, false);
    pBumper1->SetTexDictionary("generic");
    pBumper1->m_fDrawDistance = 80.0F;

    auto pModelPanel1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_PANEL1);
    pModelPanel1->SetColModel(&CTempColModels::ms_colModelPanel1, false);
    pModelPanel1->SetTexDictionary("generic");
    pModelPanel1->m_fDrawDistance = 80.0F;

    auto pBonnet1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_BONNET1);
    pBonnet1->SetColModel(&CTempColModels::ms_colModelBonnet1, false);
    pBonnet1->SetTexDictionary("generic");
    pBonnet1->m_fDrawDistance = 80.0F;

    auto pBoot1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_BOOT1);
    pBoot1->SetColModel(&CTempColModels::ms_colModelBoot1, false);
    pBoot1->SetTexDictionary("generic");
    pBoot1->m_fDrawDistance = 80.0F;

    auto pWheel1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_WHEEL1);
    pWheel1->SetColModel(&CTempColModels::ms_colModelWheel1, false);
    pWheel1->SetTexDictionary("generic");
    pWheel1->m_fDrawDistance = 80.0F;

    auto pBodyPart1 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_BODYPART1);
    pBodyPart1->SetColModel(&CTempColModels::ms_colModelBodyPart1, false);
    pBodyPart1->SetTexDictionary("generic");
    pBodyPart1->m_fDrawDistance = 80.0F;

    auto pBodyPart2 = CModelInfo::AddAtomicModel(eModelID::MODEL_TEMPCOL_BODYPART2);
    pBodyPart2->SetColModel(&CTempColModels::ms_colModelBodyPart2, false);
    pBodyPart2->SetTexDictionary("generic");
    pBodyPart2->m_fDrawDistance = 80.0F;
}

// Converted from stdcall CBaseModelInfo* CModelInfo::GetModelInfo(char* name,int *index) 0x4C5940
CBaseModelInfo* CModelInfo::GetModelInfo(char const* name, int* index)
{
    return ((CBaseModelInfo* (__cdecl *)(char const*, int*))0x4C5940)(name, index);
}

// Converted from stdcall CBaseModelInfo* CModelInfo::GetModelInfoFromHashKey(uint,int *index) 0x4C59B0
CBaseModelInfo* CModelInfo::GetModelInfoFromHashKey(unsigned int arg0, int* index)
{
    return ((CBaseModelInfo* (__cdecl *)(unsigned int, int*))0x4C59B0)(arg0, index);
}

// Converted from stdcall CBaseModelInfo* CModelInfo::GetModelInfouint16_t(char *name,ushort *int16index) 0x4C59F0
CBaseModelInfo* CModelInfo::GetModelInfouint16_t(char const* name, unsigned short* int16index)
{
    return ((CBaseModelInfo* (__cdecl *)(char const*, unsigned short*))0x4C59F0)(name, int16index);
}

// Converted from stdcall CBaseModelInfo* CModelInfo::GetModelInfo(char* name,int minIndex,int maxInedx) 0x4C5A20
CBaseModelInfo* CModelInfo::GetModelInfo(char const* name, int minIndex, int maxInedx)
{
    return ((CBaseModelInfo* (__cdecl *)(char const*, int, int))0x4C5A20)(name, minIndex, maxInedx);
}

// Converted from stdcall void* CModelInfo::Get2dEffectStore(void) 0x4C5A60
void* CModelInfo::Get2dEffectStore()
{
    return ((void* (__cdecl *)())0x4C5A60)();
}

// Converted from stdcall bool CModelInfo::IsBoatModel(int index) 0x4C5A70
bool CModelInfo::IsBoatModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5A70)(index);
}

// Converted from stdcall bool CModelInfo::IsCarModel(int index) 0x4C5AA0
bool CModelInfo::IsCarModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5AA0)(index);
}

// Converted from stdcall bool CModelInfo::IsTrainModel(int index) 0x4C5AD0
bool CModelInfo::IsTrainModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5AD0)(index);
}

// Converted from stdcall bool CModelInfo::IsHeliModel(int index) 0x4C5B00
bool CModelInfo::IsHeliModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5B00)(index);
}

// Converted from stdcall bool CModelInfo::IsPlaneModel(int index) 0x4C5B30
bool CModelInfo::IsPlaneModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5B30)(index);
}

// Converted from stdcall bool CModelInfo::IsBikeModel(int index) 0x4C5B60
bool CModelInfo::IsBikeModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5B60)(index);
}

// Converted from stdcall bool CModelInfo::IsFakePlaneModel(int index) 0x4C5B90
bool CModelInfo::IsFakePlaneModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5B90)(index);
}

// Converted from stdcall bool CModelInfo::IsMonsterTruckModel(int index) 0x4C5BC0
bool CModelInfo::IsMonsterTruckModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5BC0)(index);
}

// Converted from stdcall bool CModelInfo::IsQuadBikeModel(int index) 0x4C5BF0
bool CModelInfo::IsQuadBikeModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5BF0)(index);
}

// Converted from stdcall bool CModelInfo::IsBmxModel(int index) 0x4C5C20
bool CModelInfo::IsBmxModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5C20)(index);
}

// Converted from stdcall bool CModelInfo::IsTrailerModel(int index) 0x4C5C50
bool CModelInfo::IsTrailerModel(int index)
{
    return ((bool(__cdecl *)(int))0x4C5C50)(index);
}

// Converted from stdcall int CModelInfo::IsVehicleModelType(int index) 0x4C5C80
int CModelInfo::IsVehicleModelType(int index)
{
    return ((int(__cdecl *)(int))0x4C5C80)(index);
}
