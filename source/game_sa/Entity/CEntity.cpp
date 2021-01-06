/*
Plugin-SDK (Grand Theft Auto San Andreas) source file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

void CEntity::InjectHooks()
{
    ReversibleHooks::Install("CEntity", "Add", 0x5347D0, &CEntity::Add_Reversed);
    ReversibleHooks::Install("CEntity", "Add_", 0x533020, &CEntity::Add__Reversed);
    ReversibleHooks::Install("CEntity", "Remove", 0x534AE0, &CEntity::Remove);
    ReversibleHooks::Install("CEntity", "GetBoundRect", 0x534120, &CEntity::GetBoundRect_Reversed);
    ReversibleHooks::Install("CEntity", "SetModelIndexNoCreate", 0x533700, &CEntity::SetModelIndexNoCreate_Reversed);
    ReversibleHooks::Install("CEntity", "CreateRwObject", 0x533D30, &CEntity::CreateRwObject_Reversed);
}

void CEntity::Add(CRect const& rect)
{
    CEntity::Add_Reversed(rect);
}
void CEntity::Add_Reversed(CRect const& rect)
{
    CRect usedRect = rect;
    if (usedRect.left < -3000.0F)
        usedRect.left = -3000.0F;

    if (usedRect.right >= 3000.0F)
        usedRect.right = 2999.0F;

    if (usedRect.top < -3000.0F)
        usedRect.top = -3000.0F;

    if (usedRect.bottom >= 3000.0F)
        usedRect.bottom = 2999.0F;

    if (m_bIsBIGBuilding) {
        std::int32_t startSectorX = CWorld::GetLodSectorX(usedRect.left);
        std::int32_t startSectorY = CWorld::GetLodSectorY(usedRect.top);
        std::int32_t endSectorX = CWorld::GetLodSectorX(usedRect.right);
        std::int32_t endSectorY = CWorld::GetLodSectorY(usedRect.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                auto pLodListEntry = CWorld::GetLodPtrList(sectorX, sectorY);
                pLodListEntry.AddItem(this);
            }
        }
    }
    else {
        std::int32_t startSectorX = CWorld::GetSectorX(usedRect.left);
        std::int32_t startSectorY = CWorld::GetSectorY(usedRect.top);
        std::int32_t endSectorX = CWorld::GetSectorX(usedRect.right);
        std::int32_t endSectorY = CWorld::GetSectorY(usedRect.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                CPtrListDoubleLink* pDoubleLinkList = nullptr;
                auto pRepeatSector = GetRepeatSector(sectorX, sectorY);
                auto pSector = GetSector(sectorX, sectorY);
                switch (m_nType)
                {
                case ENTITY_TYPE_BUILDING:
                    pDoubleLinkList = &pSector->m_buildings;
                    break;
                case ENTITY_TYPE_DUMMY:
                    pDoubleLinkList = &pSector->m_dummies;
                    break;
                case ENTITY_TYPE_VEHICLE:
                    pDoubleLinkList = &pRepeatSector->m_lists[REPEATSECTOR_VEHICLES];
                    break;
                case ENTITY_TYPE_PED:
                    pDoubleLinkList = &pRepeatSector->m_lists[REPEATSECTOR_PEDS];
                    break;
                case ENTITY_TYPE_OBJECT:
                    pDoubleLinkList = &pRepeatSector->m_lists[REPEATSECTOR_OBJECTS];
                    break;
                }

                pDoubleLinkList->AddItem(this);
            }
        }
    }
}

void CEntity::Add_()
{
    CEntity::Add__Reversed();
}
void CEntity::Add__Reversed()
{
    auto rect = CRect();
    GetBoundRect(&rect);
    Add(rect);
}

void CEntity::Remove()
{
    CEntity::Remove_Reversed();
}
void CEntity::Remove_Reversed()
{
    auto usedRect = CRect();
    GetBoundRect(&usedRect);

    if (usedRect.left < -3000.0F)
        usedRect.left = -3000.0F;

    if (usedRect.right >= 3000.0F)
        usedRect.right = 2999.0F;

    if (usedRect.top < -3000.0F)
        usedRect.top = -3000.0F;

    if (usedRect.bottom >= 3000.0F)
        usedRect.bottom = 2999.0F;

    if (m_bIsBIGBuilding) {
        std::int32_t startSectorX = CWorld::GetLodSectorX(usedRect.left);
        std::int32_t startSectorY = CWorld::GetLodSectorY(usedRect.top);
        std::int32_t endSectorX = CWorld::GetLodSectorX(usedRect.right);
        std::int32_t endSectorY = CWorld::GetLodSectorY(usedRect.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                auto pLodListEntry = CWorld::GetLodPtrList(sectorX, sectorY);
                pLodListEntry.DeleteItem(this);
            }
        }
    }
    else {
        std::int32_t startSectorX = CWorld::GetSectorX(usedRect.left);
        std::int32_t startSectorY = CWorld::GetSectorY(usedRect.top);
        std::int32_t endSectorX = CWorld::GetSectorX(usedRect.right);
        std::int32_t endSectorY = CWorld::GetSectorY(usedRect.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                CPtrListDoubleLink* pDoubleLinkList = nullptr;
                auto pRepeatSector = GetRepeatSector(sectorX, sectorY);
                auto pSector = GetSector(sectorX, sectorY);
                switch (m_nType)
                {
                case ENTITY_TYPE_BUILDING:
                    pDoubleLinkList = &pSector->m_buildings;
                    break;
                case ENTITY_TYPE_DUMMY:
                    pDoubleLinkList = &pSector->m_dummies;
                    break;
                case ENTITY_TYPE_VEHICLE:
                    pDoubleLinkList = &pRepeatSector->m_lists[REPEATSECTOR_VEHICLES];
                    break;
                case ENTITY_TYPE_PED:
                    pDoubleLinkList = &pRepeatSector->m_lists[REPEATSECTOR_PEDS];
                    break;
                case ENTITY_TYPE_OBJECT:
                    pDoubleLinkList = &pRepeatSector->m_lists[REPEATSECTOR_OBJECTS];
                    break;
                }

                pDoubleLinkList->DeleteItem(this);
            }
        }
    }
}

void CEntity::SetIsStatic(bool isStatic)
{
    ((void(__thiscall *)(CEntity *, bool))(*(void ***)this)[4])(this, isStatic);
}

void CEntity::SetModelIndex(unsigned int index)
{
    return CEntity::SetModelIndex_Reversed(index);
}

void CEntity::SetModelIndex_Reversed(unsigned int index)
{
    CEntity::SetModelIndexNoCreate(index);
    CEntity::CreateRwObject();
}

void CEntity::SetModelIndexNoCreate(unsigned int index)
{
    return CEntity::SetModelIndexNoCreate_Reversed(index);
}

void CEntity::SetModelIndexNoCreate_Reversed(unsigned int index)
{
    auto pModelInfo = CModelInfo::GetModelInfo(index);
    m_nModelIndex = index;
    m_bHasPreRenderEffects = CEntity::HasPreRenderEffects();

    if (pModelInfo->bDrawLast)
        m_bDrawLast = true;

    if (!pModelInfo->bIsBackfaceCulled)
        m_bBackfaceCulled = false;

    auto pAtomicInfo = pModelInfo->AsAtomicModelInfoPtr();
    if (pAtomicInfo && !pAtomicInfo->bTagDisabled && pAtomicInfo->IsTagModel())
        CTagManager::AddTag(this);
}

void CEntity::CreateRwObject()
{
    return CEntity::CreateRwObject_Reversed();
}

void CEntity::CreateRwObject_Reversed()
{
    if (!m_bIsVisible)
        return;

    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (m_bRenderDamaged) {
        CDamageAtomicModelInfo::ms_bCreateDamagedVersion = true;
        m_pRwObject = pModelInfo->CreateInstance_();
        CDamageAtomicModelInfo::ms_bCreateDamagedVersion = false;
    }
    else {
        m_pRwObject = pModelInfo->CreateInstance_();
    }

    if (!m_pRwObject)
        return;

    if (IsBuilding())
        ++gBuildings;

    auto pRwMat = RwFrameGetMatrix((RwFrame*)rwObjectGetParent(m_pRwObject));
    auto pMatrix = GetMatrix();
    if (pMatrix)
        pMatrix->UpdateRwMatrix(pRwMat);
    else
        m_placement.UpdateRwMatrix(pRwMat);

    if (RwObjectGetType(m_pRwObject) == rpATOMIC) {
        if (CTagManager::IsTag(this))
            CTagManager::ResetAlpha(this);

        CCustomBuildingDNPipeline::PreRenderUpdate(m_pRwAtomic, true);
    }
    else if (RwObjectGetType(m_pRwObject) == rpCLUMP && pModelInfo->bIsRoad) {
        if (IsObject()) {
            auto pObj = static_cast<CObject*>(this);
            if (!pObj->m_pMovingList)
                pObj->AddToMovingList();

            pObj->SetIsStatic(false);
        }
        else {
            auto pPtrList = reinterpret_cast<CPtrNodeDoubleLink*>(CPtrNodeDoubleLink::operator new(0xC));
            if (pPtrList)
                pPtrList->pItem = this;

            pPtrList->AddToList(&CWorld::ms_listMovingEntityPtrs); //pPtrList can be nullptr when calling this method if the operator new failed
        }

        if (m_pLod && m_pLod->m_pRwObject && RwObjectGetType(m_pLod->m_pRwObject) == rpCLUMP) {
            auto pLodAssoc = RpAnimBlendClumpGetFirstAssociation(m_pLod->m_pRwClump);
            if (pLodAssoc) {
                auto pAssoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump);
                if (pAssoc)
                    pAssoc->SetCurrentTime(pLodAssoc->m_fCurrentTime);
            }
        }
    }

    pModelInfo->AddRef();
    m_pStreamingLink = CStreaming::AddEntity(this);
    CEntity::CreateEffects();

    auto pUsedAtomic = m_pRwAtomic;
    if (RwObjectGetType(m_pRwObject) != rpATOMIC)
        pUsedAtomic = GetFirstAtomic(m_pRwClump);

    if (!CCustomBuildingRenderer::IsCBPCPipelineAttached(pUsedAtomic))
        m_bLightObject = true;
}


void CEntity::DeleteRwObject()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[8])(this);
}

CRect* CEntity::GetBoundRect(CRect* pRect)
{
    return CEntity::GetBoundRect_Reversed(pRect);
}

CRect* CEntity::GetBoundRect_Reversed(CRect* pRect)
{
    CColModel* colModel = CModelInfo::GetModelInfo(m_nModelIndex)->m_pColModel;
    CVector vecMin = colModel->m_boundBox.m_vecMin;
    CVector vecMax = colModel->m_boundBox.m_vecMax;
    CRect rect;
    CVector point;
    TransformFromObjectSpace(point, vecMin);
    rect.StretchToPoint(point.x, point.y);
    TransformFromObjectSpace(point, vecMax);
    rect.StretchToPoint(point.x, point.y);
    float maxX = vecMax.x;
    vecMax.x = vecMin.x;
    vecMin.x = maxX;
    TransformFromObjectSpace(point, vecMin);
    rect.StretchToPoint(point.x, point.y);
    TransformFromObjectSpace(point, vecMax);
    rect.StretchToPoint(point.x, point.y);
    *pRect = rect;
    return pRect;
}

void CEntity::ProcessControl()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[10])(this);
}

void CEntity::ProcessCollision()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[11])(this);
}

void CEntity::ProcessShift()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[12])(this);
}

bool CEntity::TestCollision(bool bApplySpeed)
{
    return ((bool(__thiscall *)(CEntity *, bool))(*(void ***)this)[13])(this, bApplySpeed);
}

void CEntity::Teleport(CVector destination, bool resetRotation)
{
    ((void(__thiscall *)(CEntity *, CVector, bool))(*(void ***)this)[14])(this, destination, resetRotation);
}

void CEntity::SpecialEntityPreCollisionStuff(CEntity *colEntity, bool bIgnoreStuckCheck, bool* bCollisionDisabled, bool* bCollidedEntityCollisionIgnored, bool* bCollidedEntityUnableToMove, bool* bThisOrCollidedEntityStuck)
{
    ((void(__thiscall *)(CEntity *, CEntity *, bool, bool *, bool *, bool *, bool *))(*(void ***)this)[15])(this, colEntity, bIgnoreStuckCheck, bCollisionDisabled, bCollidedEntityCollisionIgnored, bCollidedEntityUnableToMove, bThisOrCollidedEntityStuck);
}

unsigned char CEntity::SpecialEntityCalcCollisionSteps(bool * bProcessCollisionBeforeSettingTimeStep, bool* unk2)
{
    return ((unsigned char(__thiscall *)(CEntity *, bool*, bool*))(*(void ***)this)[16])(this, bProcessCollisionBeforeSettingTimeStep, unk2);
}

void CEntity::PreRender()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[17])(this);
}

void CEntity::Render()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[18])(this);
}

bool CEntity::SetupLighting()
{
    return ((bool(__thiscall *)(CEntity *))(*(void ***)this)[19])(this);
}

void CEntity::RemoveLighting(bool bRemove)
{
    ((void(__thiscall *)(CEntity *, bool))(*(void ***)this)[20])(this, bRemove);
}

void CEntity::FlagToDestroyWhenNextProcessed()
{
    ((void(__thiscall *)(CEntity *))(*(void ***)this)[21])(this);
}

// Converted from thiscall void CEntity::UpdateRwFrame(void) 0x532B00
void CEntity::UpdateRwFrame()
{
    ((void(__thiscall *)(CEntity*))0x532B00)(this);
}

// Converted from thiscall void CEntity::UpdateRpHAnim(void) 0x532B20
void CEntity::UpdateRpHAnim()
{
    ((void(__thiscall *)(CEntity*))0x532B20)(this);
}

// Converted from thiscall bool CEntity::HasPreRenderEffects(void) 0x532B70
bool CEntity::HasPreRenderEffects()
{
    return ((bool(__thiscall *)(CEntity*))0x532B70)(this);
}

// Converted from thiscall bool CEntity::DoesNotCollideWithFlyers(void) 0x532D40
bool CEntity::DoesNotCollideWithFlyers()
{
    return ((bool(__thiscall *)(CEntity*))0x532D40)(this);
}

// Converted from stdcall RpMaterial* MaterialUpdateUVAnimCB(RpMaterial *material,void *data) 0x532D70
RpMaterial* MaterialUpdateUVAnimCB(RpMaterial* material, void* data)
{
    return ((RpMaterial* (__cdecl *)(RpMaterial*, void*))0x532D70)(material, data);
}

// Converted from thiscall void CEntity::ModifyMatrixForPoleInWind(void) 0x532DB0
void CEntity::ModifyMatrixForPoleInWind()
{
    ((void(__thiscall *)(CEntity*))0x532DB0)(this);
}

// Converted from thiscall bool CEntity::LivesInThisNonOverlapSector(int sectorX,int sectorY) 0x533050
bool CEntity::LivesInThisNonOverlapSector(int sectorX, int sectorY)
{
    return ((bool(__thiscall *)(CEntity*, int, int))0x533050)(this, sectorX, sectorY);
}

// Converted from thiscall void CEntity::SetupBigBuilding(void) 0x533150
void CEntity::SetupBigBuilding()
{
    ((void(__thiscall *)(CEntity*))0x533150)(this);
}

// Converted from thiscall void CEntity::ModifyMatrixForCrane(void) 0x533170
void CEntity::ModifyMatrixForCrane()
{
    ((void(__thiscall *)(CEntity*))0x533170)(this);
}

// Converted from thiscall void CEntity::PreRenderForGlassWindow(void) 0x533240
void CEntity::PreRenderForGlassWindow()
{
    ((void(__thiscall *)(CEntity*))0x533240)(this);
}

// Converted from thiscall void CEntity::SetRwObjectAlpha(int alpha) 0x5332C0
void CEntity::SetRwObjectAlpha(int alpha)
{
    ((void(__thiscall *)(CEntity*, int))0x5332C0)(this, alpha);
}

// Converted from stdcall bool IsEntityPointerValid(CEntity *entity) 0x533310
bool IsEntityPointerValid(CEntity* entity)
{
    return ((bool(__cdecl *)(CEntity*))0x533310)(entity);
}

// Converted from thiscall CVector CEntity::FindTriggerPointCoors(int triggerIndex) 0x533380
CVector CEntity::FindTriggerPointCoors(int triggerIndex)
{
    CVector result;
    ((void(__thiscall *)(CEntity*, CVector*, int))0x533380)(this, &result, triggerIndex);
    return result;
}

// Converted from thiscall C2dEffect* CEntity::GetRandom2dEffect(int effectType,uchar) 0x533410
C2dEffect* CEntity::GetRandom2dEffect(int effectType, unsigned char arg1)
{
    return ((C2dEffect* (__thiscall *)(CEntity*, int, unsigned char))0x533410)(this, effectType, arg1);
}

// Converted from thiscall CVector CEntity::TransformFromObjectSpace(CVector const &offset) 0x5334F0
CVector CEntity::TransformFromObjectSpace(CVector const& offset)
{
    CVector result;
    ((void(__thiscall *)(CEntity*, CVector*, CVector const&))0x5334F0)(this, &result, offset);
    return result;
}

// Converted from thiscall void CEntity::TransformFromObjectSpace(CVector &outPosn,CVector const &offset) 0x533560
void CEntity::TransformFromObjectSpace(CVector& outPosn, CVector const& offset)
{
    ((void(__thiscall *)(CEntity*, CVector&, CVector const&))0x533560)(this, outPosn, offset);
}

// Converted from thiscall void CEntity::CreateEffects(void) 0x533790
void CEntity::CreateEffects()
{
    ((void(__thiscall *)(CEntity*))0x533790)(this);
}

// Converted from thiscall void CEntity::DestroyEffects(void) 0x533BF0
void CEntity::DestroyEffects()
{
    ((void(__thiscall *)(CEntity*))0x533BF0)(this);
}

// Converted from thiscall void CEntity::AttachToRwObject(RwObject *object, bool updateEntityMatrix) 0x533ED0
void CEntity::AttachToRwObject(RwObject* object, bool updateEntityMatrix)
{
    ((void(__thiscall *)(CEntity*, RwObject*, bool))0x533ED0)(this, object, updateEntityMatrix);
}

// Converted from thiscall void CEntity::DetachFromRwObject(void) 0x533FB0
void CEntity::DetachFromRwObject()
{
    ((void(__thiscall *)(CEntity*))0x533FB0)(this);
}

// Converted from thiscall CVector CEntity::GetBoundCentre(void) 0x534250
CVector* CEntity::GetBoundCentre(CVector* pOutCentre)
{
    return ((CVector * (__thiscall *)(CEntity*, CVector*))0x534250)(this, pOutCentre);
}

// Converted from thiscall void CEntity::GetBoundCentre(CVector &outCentre) 0x534290
void CEntity::GetBoundCentre(CVector& outCentre)
{
    ((void(__thiscall *)(CEntity*, CVector&))0x534290)(this, outCentre);
}

// Converted from thiscall void CEntity::RenderEffects(void) 0x5342B0
void CEntity::RenderEffects()
{
    ((void(__thiscall *)(CEntity*))0x5342B0)(this);
}

// Converted from thiscall bool CEntity::GetIsTouching(CEntity *entity) 0x5343F0
bool CEntity::GetIsTouching(CEntity* entity)
{
    return ((bool(__thiscall *)(CEntity*, CEntity*))0x5343F0)(this, entity);
}

// Converted from thiscall bool CEntity::GetIsTouching(CVector const &centre,float radius) 0x5344B0
bool CEntity::GetIsTouching(CVector* centre, float radius)
{
    return ((bool(__thiscall *)(CEntity*, CVector *, float))0x5344B0)(this, centre, radius);
}

// Converted from thiscall bool CEntity::GetIsOnScreen(void) 0x534540
bool CEntity::GetIsOnScreen()
{
    return ((bool(__thiscall *)(CEntity*))0x534540)(this);
}

// Converted from thiscall bool CEntity::GetIsBoundingBoxOnScreen(void) 0x5345D0
bool CEntity::GetIsBoundingBoxOnScreen()
{
    return ((bool(__thiscall *)(CEntity*))0x5345D0)(this);
}

// Converted from thiscall void CEntity::ModifyMatrixForTreeInWind(void) 0x534E90
void CEntity::ModifyMatrixForTreeInWind()
{
    ((void(__thiscall *)(CEntity*))0x534E90)(this);
}

// Converted from thiscall void CEntity::ModifyMatrixForBannerInWind(void) 0x535040
void CEntity::ModifyMatrixForBannerInWind()
{
    ((void(__thiscall *)(CEntity*))0x535040)(this);
}

RwMatrix* CEntity::GetModellingMatrix()
{
    return plugin::CallMethodAndReturn<RwMatrix*, 0x46A2D0, CEntity*>(this);
}

// Converted from thiscall CColModel* CEntity::GetColModel(void) 0x535300
CColModel* CEntity::GetColModel()
{
    return ((CColModel* (__thiscall *)(CEntity*))0x535300)(this);
}

// Converted from thiscall void CEntity::CalculateBBProjection(CVector *,CVector *,CVector *,CVector *) 0x535340
void CEntity::CalculateBBProjection(CVector* arg0, CVector* arg1, CVector* arg2, CVector* arg3)
{
    ((void(__thiscall *)(CEntity*, CVector*, CVector*, CVector*, CVector*))0x535340)(this, arg0, arg1, arg2, arg3);
}

// Converted from thiscall void CEntity::UpdateAnim(void) 0x535F00
void CEntity::UpdateAnim()
{
    ((void(__thiscall *)(CEntity*))0x535F00)(this);
}

// Converted from thiscall bool CEntity::IsVisible(void) 0x536BC0
bool CEntity::IsVisible()
{
    return ((bool(__thiscall *)(CEntity*))0x536BC0)(this);
}

// Converted from thiscall float CEntity::GetDistanceFromCentreOfMassToBaseOfModel(void) 0x536BE0
float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
    return ((float(__thiscall *)(CEntity*))0x536BE0)(this);
}

// Converted from thiscall void CEntity::CleanUpOldReference(CEntity** entity) 0x571A00
void CEntity::CleanUpOldReference(CEntity** entity)
{
    ((void(__thiscall *)(CEntity*, CEntity**))0x571A00)(this, entity);
}

// Converted from thiscall void CEntity::ResolveReferences(void) 0x571A40
void CEntity::ResolveReferences()
{
    ((void(__thiscall *)(CEntity*))0x571A40)(this);
}

// Converted from thiscall void CEntity::PruneReferences(void) 0x571A90
void CEntity::PruneReferences()
{
    ((void(__thiscall *)(CEntity*))0x571A90)(this);
}

// Converted from thiscall void CEntity::RegisterReference(CEntity** entity) 0x571B70
void CEntity::RegisterReference(CEntity** entity)
{
    ((void(__thiscall *)(CEntity*, CEntity**))0x571B70)(this, entity);
}

// Converted from thiscall void CEntity::ProcessLightsForEntity(void) 0x6FC7A0
void CEntity::ProcessLightsForEntity()
{
    ((void(__thiscall *)(CEntity*))0x6FC7A0)(this);
}

// Converted from thiscall void CEntity::RemoveEscalatorsForEntity(void) 0x717900
void CEntity::RemoveEscalatorsForEntity()
{
    ((void(__thiscall *)(CEntity*))0x717900)(this);
}

// Converted from thiscall bool CEntity::IsEntityOccluded(void) 0x71FAE0
bool CEntity::IsEntityOccluded()
{
    return ((bool(__thiscall *)(CEntity*))0x71FAE0)(this);
}

bool CEntity::IsCurrentAreaOrBarberShopInterior()
{
    return m_nAreaCode == CGame::currArea || m_nAreaCode == AREA_CODE_13;
}

void CEntity::UpdateRW() {
#ifdef USE_DEFAULT_FUNCTIONS 
    plugin::CallMethod<0x446F90, CEntity*>(this);
#else
    if (m_pRwObject) {
        RwFrame* frame = reinterpret_cast<RwFrame*>(rwObjectGetParent(m_pRwObject));
        RwMatrix* pRwMatrix = RwFrameGetMatrix(frame);
        if (m_matrix)
            m_matrix->UpdateRwMatrix(pRwMatrix);
        else
            m_placement.UpdateRwMatrix(pRwMatrix);
    }
#endif
}
