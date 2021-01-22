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
int (&CVehicleModelInfo::ms_wheelFrameIDs)[NUM_WHEELS] = *(int(*)[NUM_WHEELS])0x8A7770;
short(&CVehicleModelInfo::ms_upgradeWheels)[NUM_WHEEL_UPGRADES][NUM_WHEELS] = *(short(*)[NUM_WHEEL_UPGRADES][NUM_WHEELS])0xB4E3F8;
char(&CVehicleModelInfo::ms_lightsOn)[NUM_LIGHTS] = *(char(*)[NUM_LIGHTS])0xB4E3E8;
RwObjectNameIdAssocation* (&CVehicleModelInfo::ms_vehicleDescs)[NUM_VEHICLE_MODEL_DESCS] = *(RwObjectNameIdAssocation*(*)[NUM_VEHICLE_MODEL_DESCS])0x8A7740;

RwTextureCallBackFind & CVehicleModelInfo::SavedTextureFindCallback = *(RwTextureCallBackFind*)0xB4E6A0;
RwTexDictionary* &vehicleTxd = *(RwTexDictionary**)0xB4E688;
RwFrame* &carFrame = *(RwFrame**)0xB4E6B8;

typedef CVehicleModelInfo::CVehicleStructure CVehicleStructure;

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

// Class methods
    ReversibleHooks::Install("CVehicleModelInfo", "SetAtomicRenderCallbacks", 0x4C7B10, &CVehicleModelInfo::SetAtomicRenderCallbacks);
    ReversibleHooks::Install("CVehicleModelInfo", "SetVehicleComponentFlags", 0x4C7C10, &CVehicleModelInfo::SetVehicleComponentFlags);
    ReversibleHooks::Install("CVehicleModelInfo", "GetWheelPosn", 0x4C7D20, &CVehicleModelInfo::GetWheelPosn);
    ReversibleHooks::Install("CVehicleModelInfo", "GetOriginalCompPosition", 0x4C7DD0, &CVehicleModelInfo::GetOriginalCompPosition);
    ReversibleHooks::Install("CVehicleModelInfo", "ChooseComponent", 0x4C8040, &CVehicleModelInfo::ChooseComponent);
    ReversibleHooks::Install("CVehicleModelInfo", "ChooseSecondComponent", 0x4C8120, &CVehicleModelInfo::ChooseSecondComponent);
    ReversibleHooks::Install("CVehicleModelInfo", "IsUpgradeAvailable", 0x4C8200, &CVehicleModelInfo::IsUpgradeAvailable);
    ReversibleHooks::Install("CVehicleModelInfo", "SetVehicleColour", 0x4C84B0, &CVehicleModelInfo::SetVehicleColour);
    ReversibleHooks::Install("CVehicleModelInfo", "ChooseVehicleColour", 0x4C8500, &CVehicleModelInfo::ChooseVehicleColour);
    ReversibleHooks::Install("CVehicleModelInfo", "GetNumRemaps", 0x4C86B0, &CVehicleModelInfo::GetNumRemaps);
    ReversibleHooks::Install("CVehicleModelInfo", "AddRemap", 0x4C86D0, &CVehicleModelInfo::AddRemap);
    ReversibleHooks::Install("CVehicleModelInfo", "SetRenderPipelines", 0x4C8900, &CVehicleModelInfo::SetRenderPipelines);
    ReversibleHooks::Install("CVehicleModelInfo", "GetCustomCarPlateText", 0x4C8970, &CVehicleModelInfo::GetCustomCarPlateText);
    ReversibleHooks::Install("CVehicleModelInfo", "SetCustomCarPlateText", 0x4C8980, &CVehicleModelInfo::SetCustomCarPlateText);
    ReversibleHooks::Install("CVehicleModelInfo", "ReduceMaterialsInVehicle", 0x4C8BD0, &CVehicleModelInfo::ReduceMaterialsInVehicle);
    ReversibleHooks::Install("CVehicleModelInfo", "SetupLightFlags", 0x4C8C90, &CVehicleModelInfo::SetupLightFlags);
    ReversibleHooks::Install("CVehicleModelInfo", "SetCarCustomPlate", 0x4C9450, &CVehicleModelInfo::SetCarCustomPlate);
    ReversibleHooks::Install("CVehicleModelInfo", "DisableEnvMap", 0x4C97E0, &CVehicleModelInfo::DisableEnvMap);
    ReversibleHooks::Install("CVehicleModelInfo", "SetEnvMapCoeff", 0x4C9800, &CVehicleModelInfo::SetEnvMapCoeff);
    ReversibleHooks::Install("CVehicleModelInfo", "GetNumDoors", 0x4C73C0, &CVehicleModelInfo::GetNumDoors);

// Static methods
    ReversibleHooks::Install("CVehicleModelInfo", "ShutdownLightTexture", 0x4C7470, &CVehicleModelInfo::ShutdownLightTexture);
    ReversibleHooks::Install("CVehicleModelInfo", "FindTextureCB", 0x4C7510, &CVehicleModelInfo::FindTextureCB);
    ReversibleHooks::Install("CVehicleModelInfo", "UseCommonVehicleTexDicationary", 0x4C75A0, &CVehicleModelInfo::UseCommonVehicleTexDicationary);
    ReversibleHooks::Install("CVehicleModelInfo", "StopUsingCommonVehicleTexDicationary", 0x4C75C0, &CVehicleModelInfo::StopUsingCommonVehicleTexDicationary);
    ReversibleHooks::Install("CVehicleModelInfo", "StoreAtomicUsedMaterialsCB", 0x4C8B60, &CVehicleModelInfo::StoreAtomicUsedMaterialsCB);

// Other
    ReversibleHooks::Install("CVehicleModelInfo", "HELP_IsValidCompRule", 0x4C7E10, &IsValidCompRule);
    ReversibleHooks::Install("CVehicleModelInfo", "HELP_ChooseComponent", 0x4C7FB0, &::ChooseComponent);
    ReversibleHooks::Install("CVehicleModelInfo", "HELP_CountCompsInRule", 0x4C7F80, &CountCompsInRule);
    ReversibleHooks::Install("CVehicleModelInfo", "HELP_GetListOfComponentsNotUsedByRules", 0x4C7E50, &GetListOfComponentsNotUsedByRules);
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

void CVehicleModelInfo::SetAtomicRenderCallbacks()
{
    if (m_nVehicleType == eVehicleType::VEHICLE_TRAIN)
        RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetAtomicRendererCB_Train, nullptr);
    else if (m_nVehicleType == eVehicleType::VEHICLE_PLANE || m_nVehicleType == eVehicleType::VEHICLE_FPLANE)
        RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetAtomicRendererCB_Plane, nullptr);
    else if (m_nVehicleType == eVehicleType::VEHICLE_BOAT)
        RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetAtomicRendererCB_Boat, nullptr);
    else if (m_nVehicleType == eVehicleType::VEHICLE_HELI)
        RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetAtomicRendererCB_Heli, nullptr);
    else
        RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetAtomicRendererCB, nullptr);
}

void CVehicleModelInfo::SetVehicleComponentFlags(RwFrame* component, unsigned int flags)
{
    tVehicleComponentFlagsUnion flagsUnion;
    flagsUnion.m_nFlags = flags;

    auto pHandling = &gHandlingDataMgr.m_aVehicleHandling[m_nHandlingId];
    if (flagsUnion.bCull)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_CULL);

    if (flagsUnion.bRenderAlways)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_RENDER_ALWAYS);

    if (flagsUnion.bDisableReflections)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_DISABLE_REFLECTIONS);

    if (flagsUnion.bIsFront)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_IS_FRONT);
    else if (flagsUnion.bIsRear && (pHandling->m_bIsVan || !(flagsUnion.bIsLeft || flagsUnion.bIsRight)))
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_IS_REAR);
    else if (flagsUnion.bIsLeft)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_IS_LEFT);
    else if (flagsUnion.bIsRight)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_IS_RIGHT);

    if (flagsUnion.bUnknown && (pHandling->m_bIsHatchback || flagsUnion.bIsFrontDoor || flagsUnion.bIsRearDoor))
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_VEHCOMP_15);

    if (flagsUnion.bIsRearDoor)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_IS_REAR_DOOR);
    else if (flagsUnion.bIsFrontDoor)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_IS_FRONT_DOOR);

    if (flagsUnion.bHasAlpha)
        RwFrameForAllObjects(component, CVehicleModelInfo::SetAtomicFlagCB, (void*)eAtomicComponentFlag::ATOMIC_HAS_ALPHA);
}

void CVehicleModelInfo::GetWheelPosn(int wheel, CVector& outVec, bool local)
{
    auto pFrame = CClumpModelInfo::GetFrameFromId(m_pRwClump, CVehicleModelInfo::ms_wheelFrameIDs[wheel]);

    if (m_nVehicleType != eVehicleType::VEHICLE_PLANE || local)
        outVec = *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
    else {
        auto pMatrix = RwMatrixCreate();
        memcpy(pMatrix, RwFrameGetMatrix(pFrame), sizeof(CMatrix));
        auto pParent = RwFrameGetParent(pFrame);
        while (pParent) {
            RwMatrixTransform(pMatrix, RwFrameGetMatrix(pParent), RwOpCombineType::rwCOMBINEPOSTCONCAT);
            pParent = RwFrameGetParent(pParent);
        }

        outVec = *RwMatrixGetPos(pMatrix);
        RwMatrixDestroy(pMatrix);
    }
}

bool CVehicleModelInfo::GetOriginalCompPosition(CVector& outVec, int component)
{
    auto pFrame = CClumpModelInfo::GetFrameFromId(m_pRwClump, component);
    if (!pFrame)
        return false;

    outVec = *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
    return true;
}

int CVehicleModelInfo::ChooseComponent()
{
    auto result = CVehicleModelInfo::ms_compsToUse[0];
    if (result != -2) {
        CVehicleModelInfo::ms_compsToUse[0] = -2;
        return result;
    }

    if (m_extraComps.nExtraARule && IsValidCompRule(m_extraComps.nExtraARule)) {
        return ::ChooseComponent(m_extraComps.nExtraARule, m_extraComps.nExtraAComp);
    }
    else if (CGeneral::GetRandomNumberInRange(0, 3) < 2) {
        int anVariations[6];
        auto numComps = GetListOfComponentsNotUsedByRules(m_extraComps.m_nComps, m_pVehicleStruct->m_nNumExtras, anVariations);
        if (numComps)
            return anVariations[CGeneral::GetRandomNumberInRange(0, numComps)];
    }

    return -1;
}

int CVehicleModelInfo::ChooseSecondComponent()
{
    auto result = CVehicleModelInfo::ms_compsToUse[1];
    if (result != -2) {
        CVehicleModelInfo::ms_compsToUse[1] = -2;
        return result;
    }

    if (m_extraComps.nExtraARule && IsValidCompRule(m_extraComps.nExtraARule)) {
        return ::ChooseComponent(m_extraComps.nExtraARule, m_extraComps.nExtraAComp);
    }
    else if (CGeneral::GetRandomNumberInRange(0, 3) < 2) {
        int anVariations[6];
        auto numComps = GetListOfComponentsNotUsedByRules(m_extraComps.m_nComps, m_pVehicleStruct->m_nNumExtras, anVariations);
        if (numComps)
            return anVariations[CGeneral::GetRandomNumberInRange(0, numComps)];
    }

    return -1;
}

bool CVehicleModelInfo::IsUpgradeAvailable(VehicleUpgradePosn upgrade)
{
    return m_pVehicleStruct->m_aUpgrades[upgrade].m_nParentComponentId >= 0;
}

void CVehicleModelInfo::SetVehicleColour(unsigned char prim, unsigned char sec, unsigned char tert, unsigned char quat)
{
    CVehicleModelInfo::ms_currentCol[0] = prim;
    CVehicleModelInfo::ms_currentCol[1] = sec;
    CVehicleModelInfo::ms_currentCol[2] = tert;
    CVehicleModelInfo::ms_currentCol[3] = quat;

    m_nCurrentPrimaryColor = prim;
    m_nCurrentSecondaryColor = sec;
    m_nCurrentTertiaryColor = tert;
    m_nCurrentQuaternaryColor = quat;
}

// Converted from thiscall void CVehicleModelInfo::ChooseVehicleColour(unsigned char &prim, unsigned char &sec, unsigned char &tert, unsigned char &quat, int variationShift) 0x4C8500
void CVehicleModelInfo::ChooseVehicleColour(unsigned char& prim, unsigned char& sec, unsigned char& tert, unsigned char& quat, int variationShift)
{
    if (!m_nNumColorVariations || CCheat::m_aCheatsActive[eCheats::CHEAT_BLACK_TRAFFIC]) {
        prim = 0;
        sec = 0;
        tert = 0;
        quat = 0;
        return;
    }

    if (CCheat::m_aCheatsActive[eCheats::CHEAT_PINK_TRAFFIC]) {
        prim = 126;
        sec = 126;
        tert = 126;
        quat = 126;
        return;
    }

    auto iVariation = (variationShift + m_nLastColorVariation) % m_nNumColorVariations;
    m_nLastColorVariation = iVariation;
    prim = m_anPrimaryColors[iVariation];
    sec = m_anSecondaryColors[iVariation];
    tert = m_anTertiaryColors[iVariation];
    quat = m_anQuaternaryColors[iVariation];

    if (m_nNumColorVariations <= 1)
        return;

    auto pVeh = FindPlayerVehicle(-1, false);
    if (!pVeh)
        return;

    if (CModelInfo::GetModelInfo(pVeh->m_nModelIndex) == this
        && pVeh->m_nPrimaryColor == prim
        && pVeh->m_nSecondaryColor == sec
        && pVeh->m_nTertiaryColor == tert
        && pVeh->m_nQuaternaryColor == quat) {

        iVariation = (variationShift + m_nLastColorVariation) % m_nNumColorVariations;
        m_nLastColorVariation = iVariation;
        prim = m_anPrimaryColors[iVariation];
        sec = m_anSecondaryColors[iVariation];
        tert = m_anTertiaryColors[iVariation];
        quat = m_anQuaternaryColors[iVariation];
    }
}

// Converted from thiscall int CVehicleModelInfo::GetNumRemaps(void) 0x4C86B0
int CVehicleModelInfo::GetNumRemaps()
{
    auto iCount = 0;
    for (int i = 0; i < 4; ++i) {
        if (m_anRemapTxds[i] == -1)
            break;

        ++iCount;
    }

    return iCount;
}

// Converted from thiscall void CVehicleModelInfo::AddRemap(int txd) 0x4C86D0
void CVehicleModelInfo::AddRemap(int txd)
{
    auto numRemaps = CVehicleModelInfo::GetNumRemaps();
    m_anRemapTxds[numRemaps] = txd;

}

void CVehicleModelInfo::SetRenderPipelines()
{
    CCarFXRenderer::CustomCarPipeClumpSetup(m_pRwClump);
    if (carFrame)
        return;

    carFrame = RwFrameCreate();
    RwMatrixRotate(RwFrameGetMatrix(carFrame), &CVector(1.0F, 0.0f, 0.0f), 60.0F, RwOpCombineType::rwCOMBINEREPLACE);
    RwFrameUpdateObjects(carFrame);
    RwFrameGetLTM(carFrame);
}

char* CVehicleModelInfo::GetCustomCarPlateText()
{
    if (!m_szPlateText[0])
        return nullptr;

    return m_szPlateText;
}

void CVehicleModelInfo::SetCustomCarPlateText(char* text)
{
    if (!text) {
        m_szPlateText[0] = '\0';
        return;
    }

    strncpy(m_szPlateText, text, 8);
}

void CVehicleModelInfo::ReduceMaterialsInVehicle()
{
    CMemoryMgr::LockScratchPad();
    CMemoryMgr::InitScratchPad();

    RpMaterialList matList;
    matList.space = 20;
    matList.numMaterials = 0;
    matList.materials = new RpMaterial*[matList.space];

    // CTimer::GetCurrentTimeInCycles(); // unused code used for performance diagnostics i guess
    RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::StoreAtomicUsedMaterialsCB, &matList);
    for (int32_t i = 0; i < m_pVehicleStruct->m_nNumExtras; ++i)
        StoreAtomicUsedMaterialsCB(m_pVehicleStruct->m_apExtras[i], &matList);

    // CTimer::GetCurrentTimeInCycles();
    // CTimer::GetCyclesPerMillisecond();
    _rpMaterialListDeinitialize(&matList);
    CMemoryMgr::ReleaseScratchPad();
    CVisibilityPlugins::ClearClumpForAllAtomicsFlag(m_pRwClump, eAtomicComponentFlag::ATOMIC_DISABLE_REFLECTIONS);
}

void CVehicleModelInfo::SetupLightFlags(CVehicle* vehicle)
{
    CVehicleModelInfo::ms_lightsOn[eLights::LIGHT_FRONT_LEFT] = vehicle->m_renderLights.m_bLeftFront;
    CVehicleModelInfo::ms_lightsOn[eLights::LIGHT_FRONT_RIGHT] = vehicle->m_renderLights.m_bRightFront;
    CVehicleModelInfo::ms_lightsOn[eLights::LIGHT_REAR_RIGHT] = vehicle->m_renderLights.m_bRightRear;
    CVehicleModelInfo::ms_lightsOn[eLights::LIGHT_REAR_LEFT] = vehicle->m_renderLights.m_bLeftRear;
}

void CVehicleModelInfo::SetCarCustomPlate()
{
    m_pPlateMaterial = nullptr;
    CVehicleModelInfo::SetCustomCarPlateText(nullptr);

    char plateBuffer[8];
    CCustomCarPlateMgr::GeneratePlateText(plateBuffer, 8);
    auto pMaterial = CCustomCarPlateMgr::SetupClump(m_pRwClump, plateBuffer, m_nPlateType);
    if (pMaterial)
        m_pPlateMaterial = pMaterial;
}

void CVehicleModelInfo::DisableEnvMap()
{
    if (!m_pRwObject)
        return;

    RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetEnvironmentMapAtomicCB, (void*)0xFFFF);
}

void CVehicleModelInfo::SetEnvMapCoeff(float coeff)
{
    auto iUsedCoeff = static_cast<int>(floor(coeff * 1000.0F));
    if (!m_pRwObject)
        return;

    RpClumpForAllAtomics(m_pRwClump, CVehicleModelInfo::SetEnvMapCoeffAtomicCB, (void*)iUsedCoeff);
}

int CVehicleModelInfo::GetNumDoors()
{
    return m_nNumDoors;
}

int CVehicleModelInfo::GetMaximumNumberOfPassengersFromNumberOfDoors(int modelId)
{
    return ((int(__cdecl*)(int))0x4C89B0)(modelId);
}

RpAtomic* CVehicleModelInfo::MoveObjectsCB(RwObject* object, void* data)
{
    return ((RpAtomic * (__cdecl*)(RwObject*, void*))0x4C7700)(object, data);
}

RpMaterial* CVehicleModelInfo::SetEditableMaterialsCB(RpMaterial* material, void* data)
{
    return ((RpMaterial * (__cdecl*)(RpMaterial*, void*))0x4C8220)(material, data);
}

RpAtomic* CVehicleModelInfo::SetEditableMaterialsCB(RpAtomic* atomic, void* data)
{
    return ((RpAtomic * (__cdecl*)(RpAtomic*, void*))0x4C83E0)(atomic, data);
}

void CVehicleModelInfo::ShutdownLightTexture()
{
    if (CVehicleModelInfo::ms_pLightsTexture) {
        RwTextureDestroy(CVehicleModelInfo::ms_pLightsTexture);
        CVehicleModelInfo::ms_pLightsTexture = nullptr;
    }

    if (CVehicleModelInfo::ms_pLightsOnTexture) {
        RwTextureDestroy(CVehicleModelInfo::ms_pLightsOnTexture);
        CVehicleModelInfo::ms_pLightsOnTexture = nullptr;
    }
}

RwTexture* CVehicleModelInfo::FindTextureCB(char const* name)
{
    auto dictTex = RwTexDictionaryFindNamedTexture(vehicleTxd, name);
    if (dictTex)
        return dictTex;

    auto pCurrent = RwTexDictionaryGetCurrent();
    auto pNamed = RwTexDictionaryFindNamedTexture(pCurrent, name);
    if (strncmp(name, "remap", 5))
        return pNamed;

    // Remap below this point
    if (pNamed) {
        pNamed->name[0] = '#';
        return pNamed;
    }

    char buffer[32];
    strcpy(buffer, name);
    buffer[0] = '#';
    return RwTexDictionaryFindNamedTexture(pCurrent, buffer);
}

void CVehicleModelInfo::UseCommonVehicleTexDicationary()
{
    CVehicleModelInfo::SavedTextureFindCallback = RwTextureGetFindCallBack();
    RwTextureSetFindCallBack(CVehicleModelInfo::FindTextureCB);
}

void CVehicleModelInfo::StopUsingCommonVehicleTexDicationary()
{
    RwTextureSetFindCallBack(CVehicleModelInfo::SavedTextureFindCallback);
    CVehicleModelInfo::SavedTextureFindCallback = nullptr;
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

RwObject* CVehicleModelInfo::SetAtomicFlagCB(RwObject* object, void* data)
{
    return ((RwObject * (__cdecl*)(RwObject*, void*))0x4C7B90)(object, data);
}

// Converted from stdcall RwObject* CVehicleModelInfo::ClearAtomicFlagCB(RwObject *object, void *data) 0x4C7BB0
RwObject* CVehicleModelInfo::ClearAtomicFlagCB(RwObject* object, void* data)
{
    return ((RwObject * (__cdecl*)(RwObject*, void*))0x4C7BB0)(object, data);
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

void CVehicleModelInfo::AssignRemapTxd(const char* name, std::int16_t txdSlot)
{
    plugin::Call<0x4C9360, const char*, std::int16_t>(name, txdSlot);
}

RpAtomic* CVehicleModelInfo::StoreAtomicUsedMaterialsCB(RpAtomic* atomic, void* data)
{
    auto matList = reinterpret_cast<RpMaterialList*>(data);
    auto pGeometry = RpAtomicGetGeometry(atomic);
    auto pMeshHeader = pGeometry->mesh;
    if (CVisibilityPlugins::GetAtomicId(atomic) & eAtomicComponentFlag::ATOMIC_DISABLE_REFLECTIONS)
        return atomic;

    for (int32_t i = 0; i < pMeshHeader->numMeshes; ++i) {
        auto pMesh = RpGeometryGetMesh(pGeometry, i);
        // auto pMat = matList->materials; // Unused code i guess
        // for (int32_t iMat = 0; iMat < matList->numMaterials; ++iMat)
        //     pMat++;

        _rpMaterialListAppendMaterial(matList, pMesh->material);
    }
    return atomic;
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

bool IsValidCompRule(int nRule)
{
    return nRule != eComponentsRules::ONLY_WHEN_RAINING
        || CWeather::OldWeatherType == eWeatherType::WEATHER_RAINY_COUNTRYSIDE
        || CWeather::NewWeatherType == eWeatherType::WEATHER_RAINY_COUNTRYSIDE
        || CWeather::OldWeatherType == eWeatherType::WEATHER_RAINY_SF
        || CWeather::NewWeatherType == eWeatherType::WEATHER_RAINY_SF;
}

int ChooseComponent(int rule, int comps)
{
    if (rule == eComponentsRules::ALLOW_ALWAYS || rule == eComponentsRules::ONLY_WHEN_RAINING) {
        auto iNumComps = CountCompsInRule(comps);
        auto rand = CGeneral::GetRandomNumberInRange(0, iNumComps);
        return (comps >> (4 * rand)) & 0xF;
    }
    else if (rule == eComponentsRules::MAYBE_HIDE) {
        auto iNumComps = CountCompsInRule(comps);
        auto rand = CGeneral::GetRandomNumberInRange(-1, iNumComps);
        if (rand == -1)
            return -1;

        return (comps >> (4 * rand)) & 0xF;
    }
    else if (rule == eComponentsRules::FULL_RANDOM) {
        return CGeneral::GetRandomNumberInRange(0, 5);
    }
    else
        return -1;
}

int CountCompsInRule(int comps)
{
    int result = 0;
    while (comps) {
        if ((comps & 0xF) != 0xF)
            ++result;

        comps /= 16;
    }

    return result;
}

int GetListOfComponentsNotUsedByRules(unsigned int compRules, int numExtras, int* outList)
{
    int iCompsList[]{ 0, 1, 2, 3, 4, 5 };
    tVehicleCompsUnion comps;
    comps.m_nComps = compRules;

    if (comps.nExtraARule && IsValidCompRule(comps.nExtraARule)) {
        if (comps.nExtraARule == eComponentsRules::FULL_RANDOM)
            return 0;

        if (comps.nExtraA_comp1 != 0xF)
            iCompsList[comps.nExtraA_comp1] = 0xF;

        if (comps.nExtraA_comp2 != 0xF)
            iCompsList[comps.nExtraA_comp2] = 0xF;

        if (comps.nExtraA_comp3 != 0xF)
            iCompsList[comps.nExtraA_comp3] = 0xF;
    }

    if (comps.nExtraBRule && IsValidCompRule(comps.nExtraBRule)) {
        if (comps.nExtraBRule == eComponentsRules::FULL_RANDOM)
            return 0;

        if (comps.nExtraB_comp1 != 0xF)
            iCompsList[comps.nExtraA_comp1] = 0xF;

        if (comps.nExtraB_comp2 != 0xF)
            iCompsList[comps.nExtraA_comp2] = 0xF;

        if (comps.nExtraB_comp3 != 0xF)
            iCompsList[comps.nExtraA_comp3] = 0xF;
    }

    auto iNumComps = 0;
    for (int i = 0; i < numExtras; ++i) {
        if (iCompsList[i] == 0xF)
            continue;

        outList[iNumComps] = i;
        ++iNumComps;
    }

    return iNumComps;
}
