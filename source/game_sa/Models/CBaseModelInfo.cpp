/*
Plugin-SDK (Grand Theft Auto San Andreas) source file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/
#include "StdInc.h"

void CBaseModelInfo::InjectHooks()
{
// Vtable
    ReversibleHooks::Install("CBaseModelInfo", "AsAtomicModelInfoPtr", 0x4C4A80, &CBaseModelInfo::AsAtomicModelInfoPtr_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "AsDamageAtomicModelInfoPtr", 0x4C4A90, &CBaseModelInfo::AsDamageAtomicModelInfoPtr_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "AsLodAtomicModelInfoPtr", 0x4C4AA0, &CBaseModelInfo::AsLodAtomicModelInfoPtr_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "GetTimeInfo", 0x4C4AB0, &CBaseModelInfo::GetTimeInfo_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "Init", 0x4C4B10, &CBaseModelInfo::Init_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "Shutdown", 0x4C4D50, &CBaseModelInfo::Shutdown_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "SetAnimFile", 0x4C4AC0, &CBaseModelInfo::SetAnimFile_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "ConvertAnimFileIndex", 0x4C4AD0, &CBaseModelInfo::ConvertAnimFileIndex_Reversed);
    ReversibleHooks::Install("CBaseModelInfo", "GetAnimFileIndex", 0x4C4AE0, &CBaseModelInfo::GetAnimFileIndex_Reversed);

// Helpers
    ReversibleHooks::Install("CBaseModelInfo", "IsBackfaceCulled", 0x5328F0, &CBaseModelInfo::IsBackfaceCulled);
    ReversibleHooks::Install("CBaseModelInfo", "HasBeenPreRendered", 0x5328B0, &CBaseModelInfo::HasBeenPreRendered);
    ReversibleHooks::Install("CBaseModelInfo", "HasComplexHierarchy", 0x4C4E00, &CBaseModelInfo::HasComplexHierarchy);
    ReversibleHooks::Install("CBaseModelInfo", "GetIsDrawLast", 0x5328C0, &CBaseModelInfo::GetIsDrawLast);
    ReversibleHooks::Install("CBaseModelInfo", "IsBreakableStatuePart", 0x59F090, &CBaseModelInfo::IsBreakableStatuePart);
    ReversibleHooks::Install("CBaseModelInfo", "IsLod", 0x4C4A00, &CBaseModelInfo::IsLod);
    ReversibleHooks::Install("CBaseModelInfo", "IsRoad", 0x4C4DF0, &CBaseModelInfo::IsRoad);
    ReversibleHooks::Install("CBaseModelInfo", "IsTagModel", 0x49CC20, &CBaseModelInfo::IsTagModel);
    ReversibleHooks::Install("CBaseModelInfo", "SwaysInWind", 0x4212C0, &CBaseModelInfo::SwaysInWind);
}

CAtomicModelInfo* CBaseModelInfo::AsAtomicModelInfoPtr()
{
    return CBaseModelInfo::AsAtomicModelInfoPtr_Reversed();
}
CAtomicModelInfo* CBaseModelInfo::AsAtomicModelInfoPtr_Reversed()
{
    return nullptr;
}

CDamagableModelInfo* CBaseModelInfo::AsDamageAtomicModelInfoPtr()
{
    return CBaseModelInfo::AsDamageAtomicModelInfoPtr_Reversed();
}
CDamagableModelInfo* CBaseModelInfo::AsDamageAtomicModelInfoPtr_Reversed()
{
    return nullptr;
}

CLodAtomicModelInfo *CBaseModelInfo::AsLodAtomicModelInfoPtr()
{
    return CBaseModelInfo::AsLodAtomicModelInfoPtr_Reversed();
}
CLodAtomicModelInfo* CBaseModelInfo::AsLodAtomicModelInfoPtr_Reversed()
{
    return nullptr;
}

tTimeInfo *CBaseModelInfo::GetTimeInfo()
{
    return CBaseModelInfo::GetTimeInfo_Reversed();
}
tTimeInfo* CBaseModelInfo::GetTimeInfo_Reversed()
{
    return nullptr;
}

void CBaseModelInfo::Init()
{
    CBaseModelInfo::Init_Reversed();
}
void CBaseModelInfo::Init_Reversed()
{
    m_nRefCount = 0;
    m_nTxdIndex = -1;
    m_pColModel = nullptr;
    m_n2dEffectIndex = -1;
    m_n2dfxCount = 0;
    m_nObjectInfoIndex = -1;
    m_fDrawDistance = 2000.0F;
    m_pRwObject = nullptr;

    m_nFlags = 0;
    bIsBackfaceCulled = true;
    bIsLod = true;
}


void CBaseModelInfo::Shutdown()
{
    CBaseModelInfo::Shutdown_Reversed();
}
void CBaseModelInfo::Shutdown_Reversed()
{
    DeleteRwObject();
    if (m_pColModel && bIsLod)
        delete m_pColModel;

    bIsLod = true;
    m_pColModel = nullptr;
    m_n2dEffectIndex = -1;
    m_n2dfxCount = 0;
    m_nObjectInfoIndex = -1;
    m_nTxdIndex = -1;
}

void CBaseModelInfo::SetAnimFile(char const* filename)
{
    CBaseModelInfo::SetAnimFile_Reversed(filename);
}
void CBaseModelInfo::SetAnimFile_Reversed(char const* filename)
{}

void CBaseModelInfo::ConvertAnimFileIndex()
{
    CBaseModelInfo::ConvertAnimFileIndex_Reversed();
}
void CBaseModelInfo::ConvertAnimFileIndex_Reversed()
{}

signed int CBaseModelInfo::GetAnimFileIndex()
{
    return CBaseModelInfo::GetAnimFileIndex_Reversed();
}
signed int CBaseModelInfo::GetAnimFileIndex_Reversed()
{
    return -1;
}

void CBaseModelInfo::SetTexDictionary(const char *txdName)
{
    ((void(__thiscall *)(CBaseModelInfo *, const char*))0x4C4B40)(this, txdName);
}

void CBaseModelInfo::ClearTexDictionary()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4B70)(this);
}

void CBaseModelInfo::AddTexDictionaryRef()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4B80)(this);
}

void CBaseModelInfo::RemoveTexDictionaryRef()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4B90)(this);
}

void CBaseModelInfo::AddRef()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4BA0)(this);
}

void CBaseModelInfo::RemoveRef()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4BB0)(this);
}

void CBaseModelInfo::SetColModel(CColModel *colModel, bool initPairedModel)
{
    ((void(__thiscall *)(CBaseModelInfo *, CColModel *, bool))0x4C4BC0)(this, colModel, initPairedModel);
}

void CBaseModelInfo::Init2dEffects()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4C20)(this);
}

void CBaseModelInfo::DeleteCollisionModel()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4C40)(this);
}

C2dEffect *CBaseModelInfo::Get2dEffect(int index)
{
    return ((C2dEffect *(__thiscall *)(CBaseModelInfo *, int))0x4C4C70)(this, index);
}

void CBaseModelInfo::Add2dEffect(C2dEffect *effect)
{
    ((void(__thiscall *)(CBaseModelInfo *, C2dEffect *))0x4C4D20)(this, effect);
}

CBaseModelInfo::CBaseModelInfo()
{
    ((void(__thiscall *)(CBaseModelInfo *))0x4C4A60)(this);
}

bool CBaseModelInfo::IsBackfaceCulled() {
    return bIsBackfaceCulled;
}

bool CBaseModelInfo::HasBeenPreRendered() {
    return bHasBeenPreRendered;
}

// Converted from thiscall bool CBaseModelInfo::HasComplexHierarchy(void) 0x4C4E00
bool CBaseModelInfo::HasComplexHierarchy() {
    return bHasComplexHierarchy;
}

// Converted from thiscall bool CBaseModelInfo::GetIsDrawLast(void) 0x5328C0
bool CBaseModelInfo::GetIsDrawLast() {
    return bDrawLast;
}

// Converted from thiscall bool CBaseModelInfo::IsBreakableStatuePart(void) 0x59F090
bool CBaseModelInfo::IsBreakableStatuePart() {
    return nSpecialType == 11;
}

// Converted from thiscall bool CBaseModelInfo::IsLod(void) 0x4C4A00
bool CBaseModelInfo::IsLod() {
    return bIsLod;
}

// Converted from thiscall bool CBaseModelInfo::IsRoad(void) 0x4C4DF0
bool CBaseModelInfo::IsRoad() {
    return bIsRoad;
}

// Converted from thiscall bool CBaseModelInfo::IsTagModel(void) 0x49CC20
bool CBaseModelInfo::IsTagModel() {
    return nSpecialType == 6;
}

// Converted from thiscall bool CBaseModelInfo::SwaysInWind(void) 0x4212C0	
bool CBaseModelInfo::SwaysInWind() {
    return IsSwayInWind1() || IsSwayInWind2();
}

// Converted from thiscall void CBaseModelInfo::SetHasBeenPreRendered(int bHasBeenPreRendered) 0x4C42F0
void CBaseModelInfo::SetHasBeenPreRendered(int bHasBeenPreRendered) {
    plugin::CallMethod<0x4C42F0, CBaseModelInfo *, int>(this, bHasBeenPreRendered);
}

// Converted from thiscall void CBaseModelInfo::SetIsLod(int bIsLod) 0x4C4A10
void CBaseModelInfo::SetIsLod(int bIsLod) {
    plugin::CallMethod<0x4C4A10, CBaseModelInfo *, int>(this, bIsLod);
}

// Converted from thiscall void CBaseModelInfo::SetOwnsColModel(int bOwns) 0x5328D0
void CBaseModelInfo::SetOwnsColModel(int bOwns) {
    plugin::CallMethod<0x5328D0, CBaseModelInfo *, int>(this, bOwns);
}

// Converted from thiscall void CBaseModelInfo::IncreaseAlpha(void) 0x532900
void CBaseModelInfo::IncreaseAlpha() {
    plugin::CallMethod<0x532900, CBaseModelInfo *>(this);
}
