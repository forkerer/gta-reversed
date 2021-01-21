/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "StdInc.h"

CVehicleModelInfo::CLinkedUpgradeList& CVehicleModelInfo::ms_linkedUpgrades = *(CVehicleModelInfo::CLinkedUpgradeList*)0xB4E6D8;
RwTexture* &CVehicleModelInfo::ms_pRemapTexture = *(RwTexture**)0xB4E47C;
RwTexture* &CVehicleModelInfo::ms_pLightsTexture = *(RwTexture**)0xB4E68C;
RwTexture* &CVehicleModelInfo::ms_pLightsOnTexture = *(RwTexture**)0xB4E690;
unsigned char (&CVehicleModelInfo::ms_currentCol)[NUM_CURRENT_COLORS] = *(unsigned char(*)[NUM_CURRENT_COLORS])0xB4E3F0;
CRGBA (&CVehicleModelInfo::ms_vehicleColourTable)[NUM_VEHICLE_COLORS] = *(CRGBA(*)[NUM_VEHICLE_COLORS])0xB4E480;
char (&CVehicleModelInfo::ms_compsUsed)[NUM_COMPS_USAGE] = *(char(*)[NUM_COMPS_USAGE])0xB4E478;
char (&CVehicleModelInfo::ms_compsToUse)[NUM_COMPS_USAGE] = *(char(*)[NUM_COMPS_USAGE])0x8A6458;
short(&CVehicleModelInfo::ms_numWheelUpgrades)[NUM_WHEELS] = *(short(*)[NUM_WHEELS])0xB4E470;
short(&CVehicleModelInfo::ms_upgradeWheels)[NUM_WHEEL_UPGRADES][NUM_WHEELS] = *(short(*)[NUM_WHEEL_UPGRADES][NUM_WHEELS])0xB4E3F8;

typedef CVehicleModelInfo::CVehicleStructure CVehicleStructure;
RwObjectNameIdAssocation* (&CVehicleModelInfo::ms_vehicleDescs)[NUM_VEHICLE_MODEL_DESCS] = *(RwObjectNameIdAssocation*(*)[NUM_VEHICLE_MODEL_DESCS])0x8A7740;

void CVehicleModelInfo::InjectHooks()
{
// ClinkedUpgradeList
    ReversibleHooks::Install("CLinkedUpgradeList", "AddUpgradeLink", 0x4C74B0, &CLinkedUpgradeList::AddUpgradeLink);
    ReversibleHooks::Install("CLinkedUpgradeList", "FindOtherUpgrade", 0x4C74D0, &CLinkedUpgradeList::FindOtherUpgrade);

// VTable
    ReversibleHooks::Install("CVehicleModelInfo", "GetModelType", 0x4C7650, &CVehicleModelInfo::GetModelType_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "Init", 0x4C7630, &CVehicleModelInfo::Init_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "DeleteRwObject", 0x4C9890, &CVehicleModelInfo::DeleteRwObject_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "CreateInstance", 0x4C9680, &CVehicleModelInfo::CreateInstance_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "SetAnimFile", 0x4C7670, &CVehicleModelInfo::SetAnimFile_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "ConvertAnimFileIndex", 0x4C76D0, &CVehicleModelInfo::ConvertAnimFileIndex_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "GetAnimFileIndex", 0x4C7660, &CVehicleModelInfo::GetAnimFileIndex_Reversed);
    ReversibleHooks::Install("CVehicleModelInfo", "SetClump", 0x4C95C0, &CVehicleModelInfo::SetClump_Reversed);
}

ModelInfoType CVehicleModelInfo::GetModelType()
{
    return CVehicleModelInfo::GetModelType_Reversed();
}
ModelInfoType CVehicleModelInfo::GetModelType_Reversed()
{
    return ModelInfoType::MODEL_INFO_VEHICLE;
}

void CVehicleModelInfo::Init()
{
    CVehicleModelInfo::Init_Reversed();
}
void CVehicleModelInfo::Init_Reversed()
{
    CClumpModelInfo::Init();
    m_nVehicleType = eVehicleType::VEHICLE_NONE;
    m_nWheelModelIndex = 01;
    m_fBikeSteerAngle = 999.99F;
}

void CVehicleModelInfo::DeleteRwObject()
{
    CVehicleModelInfo::DeleteRwObject_Reversed();
}
void CVehicleModelInfo::DeleteRwObject_Reversed()
{
    if (m_pVehicleStruct)
        delete m_pVehicleStruct;

    m_pVehicleStruct = nullptr;
    CClumpModelInfo::DeleteRwObject();
}

RwObject* CVehicleModelInfo::CreateInstance()
{
    return CVehicleModelInfo::CreateInstance_Reversed();
}
RwObject* CVehicleModelInfo::CreateInstance_Reversed()
{
    auto pClump = reinterpret_cast<RpClump*>(CClumpModelInfo::CreateInstance());
    if (m_pVehicleStruct->m_nNumExtras) {
        RwFrame* pFrame;
        if (m_nVehicleType != eVehicleType::VEHICLE_BIKE
            && m_nVehicleType != eVehicleType::VEHICLE_BMX
            && m_nVehicleType >= eVehicleType::VEHICLE_BOAT) {

            pFrame = RpClumpGetFrame(pClump);
        }
        else {
            pFrame = CClumpModelInfo::GetFrameFromId(pClump, 1);
            if (!pFrame)
                pFrame = RpClumpGetFrame(pClump);
        }

        auto iFirstComp = CVehicleModelInfo::ChooseComponent();
        if (iFirstComp != -1 && m_pVehicleStruct->m_apExtras[iFirstComp]) {
            auto& pExtra = m_pVehicleStruct->m_apExtras[iFirstComp];
            auto pAtomicClone = RpAtomicClone(pExtra);
            auto pNewFrame = RwFrameCreate();
            RwFrameTransform(pNewFrame, RwFrameGetMatrix(RpAtomicGetFrame(pExtra)), RwOpCombineType::rwCOMBINEREPLACE);
            RpAtomicSetFrame(pAtomicClone, pNewFrame);
            RpClumpAddAtomic(pClump, pAtomicClone);
            RwFrameAddChild(pFrame, pNewFrame);
        }
        CVehicleModelInfo::ms_compsUsed[0] = iFirstComp;

        auto iSecondComp = CVehicleModelInfo::ChooseSecondComponent();
        if (iSecondComp != -1 && m_pVehicleStruct->m_apExtras[iSecondComp]) {
            auto& pExtra = m_pVehicleStruct->m_apExtras[iSecondComp];
            auto pAtomicClone = RpAtomicClone(pExtra);
            auto pNewFrame = RwFrameCreate();
            RwFrameTransform(pNewFrame, RwFrameGetMatrix(RpAtomicGetFrame(pExtra)), RwOpCombineType::rwCOMBINEREPLACE);
            RpAtomicSetFrame(pAtomicClone, pNewFrame);
            RpClumpAddAtomic(pClump, pAtomicClone);
            RwFrameAddChild(pFrame, pNewFrame);
        }
        CVehicleModelInfo::ms_compsUsed[1] = iSecondComp;
    }
    else {
        CVehicleModelInfo::ms_compsUsed[0] = -1;
        CVehicleModelInfo::ms_compsUsed[1] = -1;
    }

    RpClumpForAllAtomics(pClump, CVehicleModelInfo::SetEnvironmentMapAtomicCB, nullptr);
    return reinterpret_cast<RwObject*>(pClump);
}

void CVehicleModelInfo::SetAnimFile(char const* filename)
{
    return CVehicleModelInfo::SetAnimFile_Reversed(filename);
}
void CVehicleModelInfo::SetAnimFile_Reversed(char const* filename)
{
    if (!strcmp(filename, "null")) {
        m_dwAnimBlockIndex = -1;
        return;
    }

    auto pName = new char[strlen(filename) + 1];
    m_animBlockFileName = pName;
    strcpy(pName, filename);
}

void CVehicleModelInfo::ConvertAnimFileIndex()
{
    CVehicleModelInfo::ConvertAnimFileIndex_Reversed();
}
void CVehicleModelInfo::ConvertAnimFileIndex_Reversed()
{
    if (m_dwAnimBlockIndex == -1)
        return;

    auto iIndex = CAnimManager::GetAnimationBlockIndex(m_animBlockFileName);
    delete[] m_animBlockFileName;
    m_dwAnimBlockIndex = iIndex;
}

signed int CVehicleModelInfo::GetAnimFileIndex()
{
    return CVehicleModelInfo::GetAnimFileIndex_Reversed();
}
signed int CVehicleModelInfo::GetAnimFileIndex_Reversed()
{
    return m_dwAnimBlockIndex;
}

void CVehicleModelInfo::SetClump(RpClump* clump)
{
    CVehicleModelInfo::SetClump_Reversed(clump);
}
void CVehicleModelInfo::SetClump_Reversed(RpClump* clump)
{
    m_pVehicleStruct = new CVehicleStructure();
    CClumpModelInfo::SetClump(clump);
    CVehicleModelInfo::SetAtomicRenderCallbacks();
    CClumpModelInfo::SetFrameIds(ms_vehicleDescs[m_nVehicleType]);
    CVehicleModelInfo::SetRenderPipelines();
    CVehicleModelInfo::PreprocessHierarchy();
    CVehicleModelInfo::ReduceMaterialsInVehicle();
    m_nCurrentPrimaryColor = 255;
    m_nCurrentSecondaryColor = 255;
    m_nCurrentTertiaryColor = 255;
    m_nCurrentQuaternaryColor = 255;
    CVehicleModelInfo::SetCarCustomPlate();
}

void CVehicleModelInfo::ShutdownLightTexture()
{
    ((void(__cdecl*)())0x4C7470)();
}

RwTexture* CVehicleModelInfo::FindTextureCB(char const* name)
{
    return ((RwTexture * (__cdecl*)(char const*))0x4C7510)(name);
}

void CVehicleModelInfo::UseCommonVehicleTexDicationary()
{
    ((void(__cdecl*)())0x4C75A0)();
}

void CVehicleModelInfo::StopUsingCommonVehicleTexDicationary()
{
    ((void(__cdecl*)())0x4C75C0)();
}

RpAtomic* CVehicleModelInfo::MoveObjectsCB(RwObject* object, void* data)
{
    return ((RpAtomic * (__cdecl*)(RwObject*, void*))0x4C7700)(object, data);
}

RpAtomic* CVehicleModelInfo::HideDamagedAtomicCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C7720)(atomic, data);
}

RpAtomic* CVehicleModelInfo::HideAllComponentsAtomicCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C7790)(atomic, data);
}

RpMaterial* CVehicleModelInfo::HasAlphaMaterialCB(RpMaterial* material, void* data)
{
    return ((RpMaterial * (__cdecl*)(RpMaterial*, void*))0x4C77C0)(material, data);
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C77E0)(atomic, data);
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_RealHeli(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C7870)(atomic, data);
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Plane(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C7930)(atomic, data);
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Boat(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C79A0)(atomic, data);
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Heli(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C7A30)(atomic, data);
}

RpAtomic* CVehicleModelInfo::SetAtomicRendererCB_Train(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C7AA0)(atomic, data);
}

void CVehicleModelInfo::SetAtomicRenderCallbacks()
{
    ((void(__thiscall*)(CVehicleModelInfo*))0x4C7B10)(this);
}

RwObject* CVehicleModelInfo::SetAtomicFlagCB(RwObject* object, void* data)
{
    return ((RwObject * (__cdecl*)(RwObject*, void*))0x4C7B90)(object, data);
}

// Converted from stdcall RwObject* CVehicleModelInfo::ClearAtomicFlagCB(RwObject *object, void *data) 0x4C7BB0
RwObject* CVehicleModelInfo::ClearAtomicFlagCB(RwObject* object, void* data)
{
    return ((RwObject * (__cdecl*)(RwObject*, void*))0x4C7BB0)(object, data);
}

// Converted from thiscall void CVehicleModelInfo::SetVehicleComponentFlags(RwFrame *component, unsigned int flags) 0x4C7C10
void CVehicleModelInfo::SetVehicleComponentFlags(RwFrame* component, unsigned int flags)
{
    ((void(__thiscall*)(CVehicleModelInfo*, RwFrame*, unsigned int))0x4C7C10)(this, component, flags);
}

// Converted from thiscall void CVehicleModelInfo::GetWheelPosn(int wheel, CVector &outVec, bool local) 0x4C7D20
void CVehicleModelInfo::GetWheelPosn(int wheel, CVector& outVec, bool local)
{
    ((void(__thiscall*)(CVehicleModelInfo*, int, CVector&, bool))0x4C7D20)(this, wheel, outVec, local);
}

// Converted from thiscall bool CVehicleModelInfo::GetOriginalCompPosition(CVector &outVec, int component) 0x4C7DD0
bool CVehicleModelInfo::GetOriginalCompPosition(CVector& outVec, int component)
{
    return ((bool(__thiscall*)(CVehicleModelInfo*, CVector&, int))0x4C7DD0)(this, outVec, component);
}

// Converted from thiscall int CVehicleModelInfo::ChooseComponent(void) 0x4C8040
int CVehicleModelInfo::ChooseComponent()
{
    return ((int(__thiscall*)(CVehicleModelInfo*))0x4C8040)(this);
}

// Converted from thiscall int CVehicleModelInfo::ChooseSecondComponent(void) 0x4C8120
int CVehicleModelInfo::ChooseSecondComponent()
{
    return ((int(__thiscall*)(CVehicleModelInfo*))0x4C8120)(this);
}

// Converted from thiscall bool CVehicleModelInfo::IsUpgradeAvailable(VehicleUpgradePosn upgrade) 0x4C8200
bool CVehicleModelInfo::IsUpgradeAvailable(VehicleUpgradePosn upgrade)
{
    return ((bool(__thiscall*)(CVehicleModelInfo*, VehicleUpgradePosn))0x4C8200)(this, upgrade);
}

RpMaterial* CVehicleModelInfo::SetEditableMaterialsCB(RpMaterial* material, void* data)
{
    return ((RpMaterial * (__cdecl*)(RpMaterial*, void*))0x4C8220)(material, data);
}

RpAtomic* CVehicleModelInfo::SetEditableMaterialsCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C83E0)(atomic, data);
}

// Converted from stdcall void CVehicleModelInfo::SetEditableMaterials(RpClump *clump) 0x4C8430
void CVehicleModelInfo::SetEditableMaterials(RpClump* clump)
{
    ((void(__cdecl*)(RpClump*))0x4C8430)(clump);
}

// Converted from stdcall void CVehicleModelInfo::ResetEditableMaterials(RpClump *clump) 0x4C8460
void CVehicleModelInfo::ResetEditableMaterials(RpClump* clump)
{
    ((void(__cdecl*)(RpClump*))0x4C8460)(clump);
}

// Converted from thiscall void CVehicleModelInfo::SetVehicleColour(unsigned char prim, unsigned char sec, unsigned char tert, unsigned char quat) 0x4C84B0
void CVehicleModelInfo::SetVehicleColour(unsigned char prim, unsigned char sec, unsigned char tert, unsigned char quat)
{
    ((void(__thiscall*)(CVehicleModelInfo*, unsigned char, unsigned char, unsigned char, unsigned char))0x4C84B0)(this, prim, sec, tert, quat);
}

// Converted from thiscall void CVehicleModelInfo::ChooseVehicleColour(unsigned char &prim, unsigned char &sec, unsigned char &tert, unsigned char &quat, int variationShift) 0x4C8500
void CVehicleModelInfo::ChooseVehicleColour(unsigned char& prim, unsigned char& sec, unsigned char& tert, unsigned char& quat, int variationShift)
{
    ((void(__thiscall*)(CVehicleModelInfo*, unsigned char&, unsigned char&, unsigned char&, unsigned char&, int))0x4C8500)(this, prim, sec, tert, quat, variationShift);
}

// Converted from thiscall int CVehicleModelInfo::GetNumRemaps(void) 0x4C86B0
int CVehicleModelInfo::GetNumRemaps()
{
    return ((int(__thiscall*)(CVehicleModelInfo*))0x4C86B0)(this);
}

// Converted from thiscall void CVehicleModelInfo::AddRemap(int txd) 0x4C86D0
void CVehicleModelInfo::AddRemap(int txd)
{
    ((void(__thiscall*)(CVehicleModelInfo*, int))0x4C86D0)(this, txd);
}

// Converted from stdcall void CVehicleModelInfo::AddWheelUpgrade(int wheelSetNumber, int modelId) 0x4C8700
void CVehicleModelInfo::AddWheelUpgrade(int wheelSetNumber, int modelId)
{
    ((void(__cdecl*)(int, int))0x4C8700)(wheelSetNumber, modelId);
}

int CVehicleModelInfo::GetNumWheelUpgrades(int wheelSetNumber)
{
    return ((int(__cdecl*)(int))0x4C8740)(wheelSetNumber);
}

void CVehicleModelInfo::GetWheelUpgrade(int wheelSetNumber, int wheelUpgradeNumber)
{
    ((void(__cdecl*)(int, int))0x4C8750)(wheelSetNumber, wheelUpgradeNumber);
}

void CVehicleModelInfo::DeleteVehicleColourTextures()
{
    ((void(__cdecl*)())0x4C8770)();
}

void CVehicleModelInfo::LoadEnvironmentMaps()
{
    ((void(__cdecl*)())0x4C8780)();
}

void CVehicleModelInfo::ShutdownEnvironmentMaps()
{
    ((void(__cdecl*)())0x4C87D0)();
}

RpMaterial* CVehicleModelInfo::GetMatFXEffectMaterialCB(RpMaterial* material, void* data)
{
    return ((RpMaterial * (__cdecl*)(RpMaterial*, void*))0x4C8810)(material, data);
}

RpMaterial* CVehicleModelInfo::SetEnvironmentMapCB(RpMaterial* material, void* data)
{
    return ((RpMaterial * (__cdecl*)(RpMaterial*, void*))0x4C8840)(material, data);
}

RpMaterial* CVehicleModelInfo::SetEnvMapCoeffCB(RpMaterial* material, void* data)
{
    return ((RpMaterial * (__cdecl*)(RpMaterial*, void*))0x4C88B0)(material, data);
}

RpAtomic* CVehicleModelInfo::SetRenderPipelinesCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C88F4)(atomic, data);
}

void CVehicleModelInfo::SetRenderPipelines()
{
    ((void(__thiscall*)(CVehicleModelInfo*))0x4C8900)(this);
}

char* CVehicleModelInfo::GetCustomCarPlateText()
{
    return ((char* (__thiscall*)(CVehicleModelInfo*))0x4C8970)(this);
}

void CVehicleModelInfo::SetCustomCarPlateText(char* text)
{
    ((void(__thiscall*)(CVehicleModelInfo*, char*))0x4C8980)(this, text);
}

int CVehicleModelInfo::GetMaximumNumberOfPassengersFromNumberOfDoors(int modelId)
{
    return ((int(__cdecl*)(int))0x4C89B0)(modelId);
}

void CVehicleModelInfo::ReduceMaterialsInVehicle()
{
    ((void(__thiscall*)(CVehicleModelInfo*))0x4C8BD0)(this);
}

void CVehicleModelInfo::SetupLightFlags(CVehicle* vehicle)
{
    ((void(__thiscall*)(CVehicleModelInfo*, CVehicle*))0x4C8C90)(this, vehicle);
}

RwFrame* CVehicleModelInfo::CollapseFramesCB(RwFrame* frame, void* data)
{
    return ((RwFrame * (__cdecl*)(RwFrame*, void*))0x4C8E30)(frame, data);
}

void CVehicleModelInfo::PreprocessHierarchy()
{
    ((void(__thiscall*)(CVehicleModelInfo*))0x4C8E60)(this);
}

RpAtomic* CVehicleModelInfo::SetEnvironmentMapAtomicCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C9410)(atomic, data);
}

RpAtomic* CVehicleModelInfo::SetEnvMapCoeffAtomicCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C9430)(atomic, data);
}

void CVehicleModelInfo::SetCarCustomPlate()
{
    ((void(__thiscall*)(CVehicleModelInfo*))0x4C9450)(this);
}

void CVehicleModelInfo::DisableEnvMap()
{
    ((void(__thiscall*)(CVehicleModelInfo*))0x4C97E0)(this);
}

void CVehicleModelInfo::SetEnvMapCoeff(float coeff)
{
    ((void(__thiscall*)(CVehicleModelInfo*, float))0x4C9800)(this, coeff);
}

int CVehicleModelInfo::GetNumDoors()
{
    return ((int(__thiscall*)(CVehicleModelInfo*))0x4C73C0)(this);
}

void CVehicleModelInfo::AssignRemapTxd(const char* name, std::int16_t txdSlot)
{
    plugin::Call<0x4C9360, const char*, std::int16_t>(name, txdSlot);
}

void CVehicleModelInfo::CLinkedUpgradeList::AddUpgradeLink(std::int16_t upgrade1, std::int16_t upgrade2)
{
    m_anUpgrade1[m_nLinksCount] = upgrade1;
    m_anUpgrade2[m_nLinksCount] = upgrade2;
    ++m_nLinksCount;
}

std::int16_t CVehicleModelInfo::CLinkedUpgradeList::FindOtherUpgrade(std::int16_t upgrade)
{
    if (!m_nLinksCount)
        return -1;

    for (int32_t i = m_nLinksCount - 1; i >= 0; --i) {
        if (m_anUpgrade1[i] == upgrade)
            return m_anUpgrade2[i];

        if (m_anUpgrade2[i] == upgrade)
            return m_anUpgrade1[i];
    }

    return -1;
}

CVehicleModelInfo::CVehicleStructure::CVehicleStructure() : m_aUpgrades()
{
    for (auto& pVecPos : m_avDummyPos)
        pVecPos.Set(0.0F, 0.0F, 0.0F);

    for (auto& pUpgrade : m_aUpgrades)
        pUpgrade.m_nParentComponentId = -1;

    memset(m_apExtras, 0, sizeof(m_apExtras));
    m_nNumExtras = 0;
    m_nMaskComponentsDamagable = 0;
}

CVehicleModelInfo::CVehicleStructure::~CVehicleStructure()
{
    for (int32_t i = 0; i < m_nNumExtras; ++i) {
        auto pAtomic = m_apExtras[i];
        auto pFrame = RpAtomicGetFrame(pAtomic);
        RpAtomicDestroy(pAtomic);
        RwFrameDestroy(pFrame);
    }
}

void* CVehicleModelInfo::CVehicleStructure::operator new(unsigned int size)
{
    return CPools::m_pInfoPool->New();
}

void CVehicleModelInfo::CVehicleStructure::operator delete(void* data)
{
    CPools::m_pInfoPool->Delete(reinterpret_cast<CVehicleStructure*>(data));
}
