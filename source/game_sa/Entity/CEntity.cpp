/*
Plugin-SDK (Grand Theft Auto San Andreas) source file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

void CEntity::InjectHooks()
{
//Virtual
    ReversibleHooks::Install("CEntity", "Add", 0x5347D0, &CEntity::Add_Reversed);
    ReversibleHooks::Install("CEntity", "Add_", 0x533020, &CEntity::Add__Reversed);
    ReversibleHooks::Install("CEntity", "Remove", 0x534AE0, &CEntity::Remove_Reversed);
    ReversibleHooks::Install("CEntity", "SetIsStatic", 0x403E20, &CEntity::SetIsStatic_Reversed);
    ReversibleHooks::Install("CEntity", "SetModelIndexNoCreate", 0x533700, &CEntity::SetModelIndexNoCreate_Reversed);
    ReversibleHooks::Install("CEntity", "CreateRwObject", 0x533D30, &CEntity::CreateRwObject_Reversed);
    ReversibleHooks::Install("CEntity", "DeleteRwObject", 0x534030, &CEntity::DeleteRwObject_Reversed);
    ReversibleHooks::Install("CEntity", "GetBoundRect", 0x534120, &CEntity::GetBoundRect_Reversed);
    ReversibleHooks::Install("CEntity", "ProcessControl", 0x403E40, &CEntity::ProcessControl_Reversed);
    ReversibleHooks::Install("CEntity", "ProcessCollision", 0x403E50, &CEntity::ProcessCollision_Reversed);
    ReversibleHooks::Install("CEntity", "ProcessShift", 0x403E60, &CEntity::ProcessShift_Reversed);
    ReversibleHooks::Install("CEntity", "TestCollision", 0x403E70, &CEntity::TestCollision_Reversed);
    ReversibleHooks::Install("CEntity", "Teleport", 0x403E80, &CEntity::Teleport_Reversed);
    ReversibleHooks::Install("CEntity", "SpecialEntityPreCollisionStuff", 0x403E90, &CEntity::SpecialEntityPreCollisionStuff_Reversed);
    ReversibleHooks::Install("CEntity", "SpecialEntityCalcCollisionSteps", 0x403EA0, &CEntity::SpecialEntityCalcCollisionSteps_Reversed);
    ReversibleHooks::Install("CEntity", "PreRender", 0x535FA0, &CEntity::PreRender_Reversed);
    ReversibleHooks::Install("CEntity", "Render", 0x534310, &CEntity::Render_Reversed);
    ReversibleHooks::Install("CEntity", "SetupLighting", 0x553DC0, &CEntity::SetupLighting_Reversed);
    ReversibleHooks::Install("CEntity", "RemoveLighting", 0x553370, &CEntity::RemoveLighting_Reversed);
    ReversibleHooks::Install("CEntity", "FlagToDestroyWhenNextProcessed", 0x533240, &CEntity::FlagToDestroyWhenNextProcessed_Reversed);

//Class
    ReversibleHooks::Install("CEntity", "UpdateRwFrame", 0x532B00, &CEntity::UpdateRwFrame);
    ReversibleHooks::Install("CEntity", "UpdateRpHAnim", 0x532B20, &CEntity::UpdateRpHAnim);
    ReversibleHooks::Install("CEntity", "HasPreRenderEffects", 0x532B70, &CEntity::HasPreRenderEffects);
    ReversibleHooks::Install("CEntity", "DoesNotCollideWithFlyers", 0x532D40, &CEntity::DoesNotCollideWithFlyers);
    ReversibleHooks::Install("CEntity", "ModifyMatrixForPoleInWind", 0x532DB0, &CEntity::ModifyMatrixForPoleInWind);
    ReversibleHooks::Install("CEntity", "LivesInThisNonOverlapSector", 0x533050, &CEntity::LivesInThisNonOverlapSector);
    ReversibleHooks::Install("CEntity", "SetupBigBuilding", 0x533150, &CEntity::SetupBigBuilding);
    ReversibleHooks::Install("CEntity", "ModifyMatrixForCrane", 0x533170, &CEntity::ModifyMatrixForCrane);
    ReversibleHooks::Install("CEntity", "PreRenderForGlassWindow", 0x533170, &CEntity::PreRenderForGlassWindow);
    ReversibleHooks::Install("CEntity", "SetRwObjectAlpha", 0x5332C0, &CEntity::SetRwObjectAlpha);
    ReversibleHooks::Install("CEntity", "FindTriggerPointCoors", 0x533380, &CEntity::FindTriggerPointCoors);
    ReversibleHooks::Install("CEntity", "GetRandom2dEffect", 0x533410, &CEntity::GetRandom2dEffect);
    ReversibleHooks::Install("CEntity", "TransformFromObjectSpace_ref", 0x5334F0, (CVector(CEntity::*)(CVector const&)) (&CEntity::TransformFromObjectSpace));
    ReversibleHooks::Install("CEntity", "TransformFromObjectSpace_ptr", 0x533560, (CVector*(CEntity::*)(CVector&, CVector const&)) (&CEntity::TransformFromObjectSpace));
    ReversibleHooks::Install("CEntity", "CreateEffects", 0x533790, &CEntity::CreateEffects);
    ReversibleHooks::Install("CEntity", "DestroyEffects", 0x533BF0, &CEntity::DestroyEffects);
    ReversibleHooks::Install("CEntity", "AttachToRwObject", 0x533ED0, &CEntity::AttachToRwObject);
    ReversibleHooks::Install("CEntity", "DetachFromRwObject", 0x533FB0, &CEntity::DetachFromRwObject);
    ReversibleHooks::Install("CEntity", "GetBoundCentre_ptr", 0x534250, (CVector*(CEntity::*)(CVector*)) (&CEntity::GetBoundCentre));
    ReversibleHooks::Install("CEntity", "GetBoundCentre_ref", 0x534290, (void(CEntity::*)(CVector&)) (&CEntity::GetBoundCentre));
    ReversibleHooks::Install("CEntity", "RenderEffects", 0x5342B0, &CEntity::RenderEffects);
    ReversibleHooks::Install("CEntity", "GetIsTouching_ent", 0x5343F0, (bool(CEntity::*)(CEntity*)) (&CEntity::GetIsTouching));
    ReversibleHooks::Install("CEntity", "GetIsTouching_vec", 0x5344B0, (bool(CEntity::*)(CVector const&, float)) (&CEntity::GetIsTouching));
    ReversibleHooks::Install("CEntity", "GetIsOnScreen", 0x534540, &CEntity::GetIsOnScreen);
    ReversibleHooks::Install("CEntity", "GetIsBoundingBoxOnScreen", 0x5345D0, &CEntity::GetIsBoundingBoxOnScreen);
    ReversibleHooks::Install("CEntity", "ModifyMatrixForTreeInWind", 0x534E90, &CEntity::ModifyMatrixForTreeInWind);
    ReversibleHooks::Install("CEntity", "GetColModel", 0x535300, &CEntity::GetColModel);

    ReversibleHooks::Install("CEntity", "GetModellingMatrix", 0x46A2D0, &CEntity::GetModellingMatrix);
    ReversibleHooks::Install("CEntity", "UpdateRW", 0x446F90, &CEntity::UpdateRW);
    ReversibleHooks::Install("CEntity", "SetAtomicAlphaCB", 0x533290, &CEntity::SetAtomicAlphaCB);
    ReversibleHooks::Install("CEntity", "SetMaterialAlphaCB", 0x533280, &CEntity::SetMaterialAlphaCB);

//Statics
    ReversibleHooks::Install("CEntity", "MaterialUpdateUVAnimCB", 0x532D70, &MaterialUpdateUVAnimCB);
    ReversibleHooks::Install("CEntity", "IsEntityPointerValid", 0x533310, &IsEntityPointerValid);
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
                auto& pLodListEntry = CWorld::GetLodPtrList(sectorX, sectorY);
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

                if (IsBuilding()) { //Buildings are treated as single link here, needs checking if the list is actually single or double
                    reinterpret_cast<CPtrListSingleLink*>(&pSector->m_buildings)->AddItem(this);
                    continue;
                }

                switch (m_nType)
                {
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
                auto& pLodListEntry = CWorld::GetLodPtrList(sectorX, sectorY);
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

                if (IsBuilding()) { //Buildings are treated as single link here
                    reinterpret_cast<CPtrListSingleLink*>(&pSector->m_buildings)->DeleteItem(this);
                    continue;
                }

                switch (m_nType)
                {
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
    return CEntity::SetIsStatic_Reversed(isStatic);
}
void CEntity::SetIsStatic_Reversed(bool isStatic)
{
    m_bIsStatic = isStatic;
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

    CEntity::UpdateRW();
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
            CWorld::ms_listMovingEntityPtrs.AddItem(this);
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
    CEntity::DeleteRwObject_Reversed();
}
void CEntity::DeleteRwObject_Reversed()
{
    if (!m_pRwObject)
        return;

    if (RwObjectGetType(m_pRwObject) == rpATOMIC) {
        auto pParent = static_cast<RwFrame*>(rwObjectGetParent(m_pRwAtomic));
        RpAtomicDestroy(m_pRwAtomic);
        RwFrameDestroy(pParent);
    }
    else if (RwObjectGetType(m_pRwObject) == rpCLUMP) {
        auto pFirstAtomic = GetFirstAtomic(m_pRwClump);
        if (pFirstAtomic && RpSkinGeometryGetSkin(RpAtomicGetGeometry(pFirstAtomic)))
            RpClumpForAllAtomics(m_pRwClump, AtomicRemoveAnimFromSkinCB, nullptr);

        RpClumpDestroy(m_pRwClump);
    }
    m_pRwObject = nullptr;
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    pModelInfo->RemoveRef();
    CStreaming::RemoveEntity(m_pStreamingLink);
    m_pStreamingLink = nullptr;

    if (IsBuilding())
        --gBuildings;

    if (pModelInfo->GetModelType() == MODEL_INFO_CLUMP
        && pModelInfo->bIsRoad
        && !IsObject()) {

        CWorld::ms_listMovingEntityPtrs.DeleteItem(this);
    }

    CEntity::DestroyEffects();
    CEntity::RemoveEscalatorsForEntity();
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
    CEntity::ProcessControl_Reversed();
}
void CEntity::ProcessControl_Reversed()
{
    return;
}

void CEntity::ProcessCollision()
{
    CEntity::ProcessCollision_Reversed();
}
void CEntity::ProcessCollision_Reversed()
{
    return;
}

void CEntity::ProcessShift()
{
    CEntity::ProcessShift_Reversed();
}
void CEntity::ProcessShift_Reversed()
{
    return;
}

bool CEntity::TestCollision(bool bApplySpeed)
{
    return CEntity::TestCollision_Reversed(bApplySpeed);
}
bool CEntity::TestCollision_Reversed(bool bApplySpeed)
{
    return false;
}

void CEntity::Teleport(CVector destination, bool resetRotation)
{
    CEntity::Teleport_Reversed(destination, resetRotation);
}
void CEntity::Teleport_Reversed(CVector destination, bool resetRotation)
{
    return;
}

void CEntity::SpecialEntityPreCollisionStuff(CEntity *colEntity, bool bIgnoreStuckCheck, bool* bCollisionDisabled, bool* bCollidedEntityCollisionIgnored, bool* bCollidedEntityUnableToMove, bool* bThisOrCollidedEntityStuck)
{
    CEntity::SpecialEntityPreCollisionStuff_Reversed(colEntity, bIgnoreStuckCheck, bCollisionDisabled, bCollidedEntityCollisionIgnored, bCollidedEntityUnableToMove, bThisOrCollidedEntityStuck);
}
void CEntity::SpecialEntityPreCollisionStuff_Reversed(CEntity* colEntity, bool bIgnoreStuckCheck, bool* bCollisionDisabled, bool* bCollidedEntityCollisionIgnored, bool* bCollidedEntityUnableToMove, bool* bThisOrCollidedEntityStuck)
{
    return;
}

unsigned char CEntity::SpecialEntityCalcCollisionSteps(bool * bProcessCollisionBeforeSettingTimeStep, bool* unk2)
{
    return CEntity::SpecialEntityCalcCollisionSteps_Reversed(bProcessCollisionBeforeSettingTimeStep, unk2);
}
unsigned char CEntity::SpecialEntityCalcCollisionSteps_Reversed(bool* bProcessCollisionBeforeSettingTimeStep, bool* unk2)
{
    return 1;
}

void CEntity::PreRender()
{
    CEntity::PreRender_Reversed();
}
void CEntity::PreRender_Reversed()
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    auto pAtomicInfo = pModelInfo->AsAtomicModelInfoPtr();

    if (pModelInfo->m_n2dfxCount)
        CEntity::ProcessLightsForEntity();

    if (!pModelInfo->bHasBeenPreRendered) {
        pModelInfo->bHasBeenPreRendered = true;

        if (pAtomicInfo && pAtomicInfo->m_pRwObject) {
            if (RpMatFXAtomicQueryEffects(pAtomicInfo->m_pRwAtomic) && RpAtomicGetGeometry(pAtomicInfo->m_pRwAtomic)) {
                RpGeometryForAllMaterials(RpAtomicGetGeometry(pAtomicInfo->m_pRwAtomic), MaterialUpdateUVAnimCB, nullptr);
            }
        }

        if (pModelInfo->m_nAlpha >= 239)
            pModelInfo->m_nAlpha = 255;
        else
            pModelInfo->m_nAlpha += 16;

        if (pAtomicInfo) {
            CCustomBuildingDNPipeline::PreRenderUpdate(pAtomicInfo->m_pRwAtomic, false);
        }
        else if (pModelInfo->GetModelType() == MODEL_INFO_CLUMP) {
            RpClumpForAllAtomics(pModelInfo->m_pRwClump, CCustomBuildingDNPipeline::PreRenderUpdateRpAtomicCB, false);
        }
    }

    if (!m_bHasPreRenderEffects)
        return;

    if (pAtomicInfo && pAtomicInfo->SwaysInWind()
        && (!IsObject() || !static_cast<CObject*>(this)->objectFlags.bIsExploded)) {

        auto vecCamPos = CVector2D(TheCamera.GetPosition());
        auto vecEntPos = CVector2D(GetPosition());
        auto fDist = DistanceBetweenPoints(vecCamPos, vecEntPos);
        CObject::fDistToNearestTree = std::min(CObject::fDistToNearestTree, fDist);
        CEntity::ModifyMatrixForTreeInWind();
    }

    if (IsBuilding()) {
        if (pAtomicInfo && pAtomicInfo->IsCrane())
            CEntity::ModifyMatrixForCrane();

        return;
    }

    if (!IsObject() && !IsDummy())
        return;

    if (IsObject() && !IsDummy()) {
        auto pObject = reinterpret_cast<CObject*>(this);
        if(m_nModelIndex == ModelIndices::MI_COLLECTABLE1) {
            CPickups::DoCollectableEffects(this);
            CEntity::UpdateRW();
            CEntity::UpdateRwFrame();
        }
        else if (m_nModelIndex == ModelIndices::MI_MONEY) {
            CPickups::DoMoneyEffects(this);
            CEntity::UpdateRW();
            CEntity::UpdateRwFrame();
        }
        else if (m_nModelIndex == ModelIndices::MI_CARMINE
            || m_nModelIndex == ModelIndices::MI_NAUTICALMINE
            || m_nModelIndex == ModelIndices::MI_BRIEFCASE) {

            if (pObject->objectFlags.bIsPickup) {
                CPickups::DoMineEffects(this);
                CEntity::UpdateRW();
                CEntity::UpdateRwFrame();
            }
        }
        else if (m_nModelIndex == eModelID::MODEL_MISSILE) {
            if (CReplay::Mode != 1) {
                auto vecPos = GetPosition();
                auto fRand = static_cast<float>(rand() & 0xF) / 16.0F;
                CShadows::StoreShadowToBeRendered(eShadowTextureType::SHADOWTEX_PED,
                                                  gpShadowExplosionTex,
                                                  &vecPos,
                                                  8.0F,
                                                  0.0F,
                                                  0.0F,
                                                  -8.0F,
                                                  255,
                                                  fRand * 200.0F,
                                                  fRand * 160.0F,
                                                  fRand * 120.0F,
                                                  20.0F,
                                                  false,
                                                  1.0F,
                                                  nullptr,
                                                  false);

                CPointLights::AddLight(ePointLightType::PLTYPE_POINTLIGHT,
                                       vecPos,
                                       CVector(0.0F, 0.0F, 0.0F),
                                       8.0F,
                                       fRand,
                                       fRand * 0.8F,
                                       fRand * 0.6F,
                                       RwFogType::rwFOGTYPENAFOGTYPE,
                                       true,
                                       nullptr);

                CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(this),
                                         nullptr,
                                         fRand * 255.0F,
                                         fRand * 220.0F,
                                          fRand * 190.0F,
                                         255,
                                         vecPos,
                                         fRand * 6.0F,
                                         300.0F,
                                         gpCoronaTexture[0],
                                         eCoronaFlareType::FLARETYPE_NONE,
                                         true,
                                         false,
                                         0,
                                         0.0F,
                                         false,
                                         1.5F,
                                         0,
                                         15.0F,
                                         false,
                                         false);
            }
        }
        else if (m_nModelIndex == ModelIndices::MI_FLARE) {
            auto vecPos = GetPosition();
            auto fRand = static_cast<float>(rand() & 0xF) / 16.0F;
            fRand = std::max(fRand, 0.5F);
            CShadows::StoreShadowToBeRendered(eShadowTextureType::SHADOWTEX_PED,
                                              gpShadowExplosionTex,
                                              &vecPos,
                                              8.0F,
                                              0.0F,
                                              0.0F,
                                              -8.0F,
                                              255,
                                              static_cast<unsigned char>(fRand * 200.0F),
                                              static_cast<unsigned char>(fRand * 200.0F),
                                              static_cast<unsigned char>(fRand * 200.0F),
                                              20.0F,
                                              false,
                                              1.0F,
                                              nullptr,
                                              false);

            CPointLights::AddLight(ePointLightType::PLTYPE_POINTLIGHT,
                                   vecPos,
                                   CVector(0.0F, 0.0F, 0.0F),
                                   32.0F,
                                   fRand,
                                   fRand,
                                   fRand,
                                   RwFogType::rwFOGTYPENAFOGTYPE,
                                   true,
                                   nullptr);

            CCoronas::RegisterCorona(reinterpret_cast<unsigned int>(this),
                                     nullptr,
                                     fRand * 255.0F,
                                     fRand * 255.0F,
                                     fRand * 255.0F,
                                     255,
                                     vecPos,
                                     fRand * 6.0F,
                                     300.0F,
                                     gpCoronaTexture[0],
                                     eCoronaFlareType::FLARETYPE_NONE,
                                     true,
                                     false,
                                     0,
                                     0.0F,
                                     false,
                                     1.5F,
                                     0,
                                     15.0F,
                                     false,
                                     false);
        }
        else if (IsGlassModel(this)) {
            CEntity::PreRenderForGlassWindow();
        }
        else if (pObject->objectFlags.bIsPickup) {
            CPickups::DoPickUpEffects(this);
            CEntity::UpdateRW();
            CEntity::UpdateRwFrame();
        }
        else if (m_nModelIndex == eModelID::MODEL_GRENADE) {
            auto const& vecPos = GetPosition();
            auto vecScaledCam = TheCamera.m_mCameraMatrix.GetRight() * 0.07F;
            auto vecStreakStart = vecPos - vecScaledCam;
            auto vecStreakEnd = vecPos + vecScaledCam;
            if (CVector2D(pObject->m_vecMoveSpeed).Magnitude() > 0.03F) {
                CMotionBlurStreaks::RegisterStreak(reinterpret_cast<unsigned int>(this),
                                                   100,
                                                   100,
                                                   100,
                                                   255,
                                                   vecStreakStart,
                                                   vecStreakEnd);
            }
        }
        else if (m_nModelIndex == eModelID::MODEL_MOLOTOV) {
            auto const& vecPos = GetPosition();
            auto vecScaledCam = TheCamera.m_mCameraMatrix.GetRight() * 0.07F;
            auto vecStreakStart = vecPos - vecScaledCam;
            auto vecStreakEnd = vecPos + vecScaledCam;
            if (CVector2D(pObject->m_vecMoveSpeed).Magnitude() > 0.03F) {
                float fWaterLevel;
                if (!CWaterLevel::GetWaterLevelNoWaves(vecPos.x, vecPos.y, vecPos.z, &fWaterLevel, nullptr, nullptr) || vecPos.z > fWaterLevel) {
                    CMotionBlurStreaks::RegisterStreak(reinterpret_cast<unsigned int>(this),
                                                   255,
                                                   160,
                                                   100,
                                                   255,
                                                   vecStreakStart,
                                                   vecStreakEnd);
                }
            }
        }
        else if (m_nModelIndex == ModelIndices::MI_BEACHBALL) {
            if (DistanceBetweenPoints(GetPosition(), TheCamera.GetPosition()) < 50.0F) {
                CShadows::StoreShadowToBeRendered(eShadowType::SHADOW_DEFAULT,
                                                  gpShadowPedTex,
                                                  &GetPosition(),
                                                  0.4F,
                                                  0.0F,
                                                  0.0F,
                                                  -0.4F,
                                                  CTimeCycle::m_CurrentColours.m_nShadowStrength,
                                                  CTimeCycle::m_CurrentColours.m_nShadowStrength,
                                                  CTimeCycle::m_CurrentColours.m_nShadowStrength,
                                                  CTimeCycle::m_CurrentColours.m_nShadowStrength,
                                                  20.0F,
                                                  false,
                                                  1.0F,
                                                  nullptr,
                                                  false);
            }
        }
        else if (m_nModelIndex == ModelIndices::MI_MAGNOCRANE_HOOK
            || m_nModelIndex == ModelIndices::MI_WRECKING_BALL
            || m_nModelIndex == ModelIndices::MI_CRANE_MAGNET
            || m_nModelIndex == ModelIndices::MI_MINI_MAGNET
            || m_nModelIndex == ModelIndices::MI_CRANE_HARNESS) {

            if (DistanceBetweenPoints(GetPosition(), TheCamera.GetPosition()) < 100.0F) {
                CShadows::StoreShadowToBeRendered(eShadowType::SHADOW_DEFAULT,
                                                  gpShadowPedTex,
                                                  &GetPosition(),
                                                  2.0F,
                                                  0.0F,
                                                  0.0F,
                                                  -2.0F,
                                                  128,
                                                  128,
                                                  128,
                                                  128,
                                                  50.0F,
                                                  false,
                                                  1.0F,
                                                  nullptr,
                                                  false);
            }
        }
        else if (m_nModelIndex == ModelIndices::MI_WINDSOCK) {
            CEntity::ModifyMatrixForPoleInWind();
        }
    }

    if (m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS) {
        CTrafficLights::DisplayActualLight(this);
        CShadows::StoreShadowForPole(this, 2.957F, 0.147F, 0.0F, 16.0F, 0.4F, 0);
        return;
    }
    if (m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_VERTICAL)
        CTrafficLights::DisplayActualLight(this);
    if (m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_MIAMI) {
        CTrafficLights::DisplayActualLight(this);
        CShadows::StoreShadowForPole(this, 4.81F, 0.0F, 0.0F, 16.0F, 0.4F, 0);
        return;
    }
    if (m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_TWOVERTICAL) {
        CTrafficLights::DisplayActualLight(this);
        CShadows::StoreShadowForPole(this, 7.503F, 0.0F, 0.0F, 16.0F, 0.4F, 0);
        return;
    }
    if (m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_3
        || m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_4
        || m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_5
        || m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_GAY) {

        CTrafficLights::DisplayActualLight(this);
    }
    else if (m_nModelIndex == ModelIndices::MI_SINGLESTREETLIGHTS1)
        CShadows::StoreShadowForPole(this, 7.744F, 0.0F, 0.0F, 16.0F, 0.4F, 0);
    else if (m_nModelIndex == ModelIndices::MI_SINGLESTREETLIGHTS2)
        CShadows::StoreShadowForPole(this, 0.043F, 0.0F, 0.0F, 16.0F, 0.4F, 0);
    else if (m_nModelIndex == ModelIndices::MI_SINGLESTREETLIGHTS3)
        CShadows::StoreShadowForPole(this, 1.143F, 0.145F, 0.0F, 16.0F, 0.4F, 0);
    else if (m_nModelIndex == ModelIndices::MI_DOUBLESTREETLIGHTS)
        CShadows::StoreShadowForPole(this, 0.0F, -0.048F, 0.0F, 16.0F, 0.4F, 0);
    else if (m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_VEGAS) {
        CTrafficLights::DisplayActualLight(this);
        CShadows::StoreShadowForPole(this, 7.5F, 0.2F, 0.0F, 16.0F, 0.4F, 0);
    }
}

void CEntity::Render()
{
    CEntity::Render_Reversed();
}
void CEntity::Render_Reversed()
{
    if (!m_pRwObject)
        return;

    if (RwObjectGetType(m_pRwObject) == rpATOMIC && CTagManager::IsTag(this)) {
        CTagManager::RenderTagForPC(m_pRwAtomic);
        return;
    }

    uint32_t savedAlphaRef;
    if (m_nModelIndex == ModelIndices::MI_JELLYFISH || m_nModelIndex == ModelIndices::MI_JELLYFISH01) {
        RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTIONREF, &savedAlphaRef);
        RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, 0u);
    }

    m_bImBeingRendered = true;

    if (RwObjectGetType(m_pRwObject) == rpATOMIC)
        RpAtomicRender(m_pRwAtomic);
    else
        RpClumpRender(m_pRwClump);

    CStreaming::RenderEntity(m_pStreamingLink);
    CEntity::RenderEffects();

    m_bImBeingRendered = false;

    if (m_nModelIndex == ModelIndices::MI_JELLYFISH || m_nModelIndex == ModelIndices::MI_JELLYFISH01) {
        RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)savedAlphaRef);
    }
}

bool CEntity::SetupLighting()
{
    return CEntity::SetupLighting_Reversed();
}
bool CEntity::SetupLighting_Reversed()
{
    if (!m_bLightObject)
        return false;

    ActivateDirectional();
    const auto& vecPos = GetPosition();
    auto fLight = CPointLights::GenerateLightsAffectingObject(&vecPos, nullptr, this) * 0.5;
    SetLightColoursForPedsCarsAndObjects(fLight);

    return true;
}

void CEntity::RemoveLighting(bool bRemove)
{
    CEntity::RemoveLighting_Reversed(bRemove);
}
void CEntity::RemoveLighting_Reversed(bool bRemove)
{
    if (!bRemove)
        return;

    SetAmbientColours();
    DeActivateDirectional();
    CPointLights::RemoveLightsAffectingObject();
}

void CEntity::FlagToDestroyWhenNextProcessed()
{
    CEntity::FlagToDestroyWhenNextProcessed_Reversed();
}
void CEntity::FlagToDestroyWhenNextProcessed_Reversed()
{
    return;
}

// Converted from thiscall void CEntity::UpdateRwFrame(void) 0x532B00
void CEntity::UpdateRwFrame()
{
    if (!m_pRwObject)
        return;

    RwFrameUpdateObjects((RwFrame*)rwObjectGetParent(m_pRwObject));
}

// Converted from thiscall void CEntity::UpdateRpHAnim(void) 0x532B20
void CEntity::UpdateRpHAnim()
{
    auto pFirstAtomic = GetFirstAtomic(m_pRwClump);
    if (!pFirstAtomic)
        return;

    if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(pFirstAtomic)) && !m_bDontUpdateHierarchy) {
        auto pAnimHierarchy = GetAnimHierarchyFromClump(m_pRwClump);
        RpHAnimHierarchyUpdateMatrices(pAnimHierarchy);
    }
}

// Converted from thiscall bool CEntity::HasPreRenderEffects(void) 0x532B70
bool CEntity::HasPreRenderEffects()
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (!pModelInfo->SwaysInWind()
        && !pModelInfo->IsCrane()
        && m_nModelIndex != ModelIndices::MI_COLLECTABLE1
        && m_nModelIndex != ModelIndices::MI_MONEY
        && m_nModelIndex != ModelIndices::MI_CARMINE
        && m_nModelIndex != ModelIndices::MI_NAUTICALMINE
        && m_nModelIndex != ModelIndices::MI_BRIEFCASE
        && m_nModelIndex != eModelID::MODEL_MISSILE
        && m_nModelIndex != eModelID::MODEL_GRENADE
        && m_nModelIndex != eModelID::MODEL_MOLOTOV
        && m_nModelIndex != ModelIndices::MI_BEACHBALL
        && m_nModelIndex != ModelIndices::MI_MAGNOCRANE_HOOK
        && m_nModelIndex != ModelIndices::MI_WRECKING_BALL
        && m_nModelIndex != ModelIndices::MI_CRANE_MAGNET
        && m_nModelIndex != ModelIndices::MI_MINI_MAGNET
        && m_nModelIndex != ModelIndices::MI_CRANE_HARNESS
        && m_nModelIndex != ModelIndices::MI_WINDSOCK
        && m_nModelIndex != ModelIndices::MI_FLARE
        && !IsGlassModel(this)
        && (!IsObject() || !reinterpret_cast<CObject*>(this)->objectFlags.bIsPickup)
        && !CTrafficLights::IsMITrafficLight(m_nModelIndex)
        && m_nModelIndex != ModelIndices::MI_SINGLESTREETLIGHTS1
        && m_nModelIndex != ModelIndices::MI_SINGLESTREETLIGHTS2
        && m_nModelIndex != ModelIndices::MI_SINGLESTREETLIGHTS3
        && m_nModelIndex != ModelIndices::MI_DOUBLESTREETLIGHTS) {

        if (!pModelInfo->m_n2dfxCount)
            return false;

        for (int32_t i = 0; i < pModelInfo->m_n2dfxCount; ++i) {
            if (pModelInfo->Get2dEffect(i)->m_nType == e2dEffectType::EFFECT_LIGHT)
                return true;
        }

        return false;
    }
    return true;
}

// Converted from thiscall bool CEntity::DoesNotCollideWithFlyers(void) 0x532D40
bool CEntity::DoesNotCollideWithFlyers()
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (pModelInfo->SwaysInWind())
        return true;

    return pModelInfo->bDontCollideWithFlyer;
}

// Converted from stdcall RpMaterial* MaterialUpdateUVAnimCB(RpMaterial *material,void *data) 0x532D70
RpMaterial* MaterialUpdateUVAnimCB(RpMaterial* material, void* data)
{
    if (!RpMaterialUVAnimExists(material))
        return material;

    auto fTimeStep = CTimer::ms_fTimeStep / 50.0F;
    RpMaterialUVAnimAddAnimTime(material, fTimeStep);
    RpMaterialUVAnimApplyUpdate(material);
    return material;
}

// Converted from thiscall void CEntity::ModifyMatrixForPoleInWind(void) 0x532DB0
void CEntity::ModifyMatrixForPoleInWind()
{
    auto vecWindDir = CVector(CWeather::WindDir.x + 0.01F, CWeather::WindDir.y + 0.01F, 0.1F);
    auto vecNormalisedDir = vecWindDir;
    vecNormalisedDir.Normalise();

    auto vecCross = CrossProduct(CVector(0.0F, 0.0F, 1.0F), vecNormalisedDir);
    vecCross.Normalise();
    auto vecCross2 = CrossProduct(vecNormalisedDir, vecCross);

    auto pMatrix = GetMatrix();
    pMatrix->GetRight() = vecCross;
    pMatrix->GetForward() = vecNormalisedDir;
    pMatrix->GetUp() = vecCross2;
    CEntity::UpdateRW();
    CEntity::UpdateRwFrame();
}

// Converted from thiscall bool CEntity::LivesInThisNonOverlapSector(int sectorX,int sectorY) 0x533050
bool CEntity::LivesInThisNonOverlapSector(int sectorX, int sectorY)
{
    auto rect = CRect();
    GetBoundRect(&rect);
    float xCenter, yCenter;
    rect.GetCenter(&xCenter, &yCenter);
    auto xEntSector = CWorld::GetSectorX(xCenter);
    auto yEntSector = CWorld::GetSectorY(yCenter);

    return sectorX == xEntSector && sectorY == yEntSector;

}

// Converted from thiscall void CEntity::SetupBigBuilding(void) 0x533150
void CEntity::SetupBigBuilding()
{
    m_bUsesCollision = false;
    m_bIsBIGBuilding = true;
    m_bStreamingDontDelete = true;

    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    pModelInfo->bDoWeOwnTheColModel = true;
}

// Converted from thiscall void CEntity::ModifyMatrixForCrane(void) 0x533170
void CEntity::ModifyMatrixForCrane()
{
    if (CTimer::GetIsPaused())
        return;

    if (!m_pRwObject)
        return;

    auto pRwMat = CEntity::GetModellingMatrix();
    if (!pRwMat)
        return;

    auto tempMat = CMatrix(pRwMat, 0);
    auto fRot = (CTimer::m_snTimeInMilliseconds & 0x3FF) * (PI / 512.26F);
    tempMat.SetRotateZOnly(fRot);
    tempMat.UpdateRW();
    CEntity::UpdateRwFrame();

}

// Converted from thiscall void CEntity::PreRenderForGlassWindow(void) 0x533240
void CEntity::PreRenderForGlassWindow()
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (pModelInfo->IsGlassType2())
        return;

    CGlass::AskForObjectToBeRenderedInGlass(this);
    m_bIsVisible = false;
}

// Converted from thiscall void CEntity::SetRwObjectAlpha(int alpha) 0x5332C0
void CEntity::SetRwObjectAlpha(int alpha)
{
    if (!m_pRwObject)
        return;

    if (RwObjectGetType(m_pRwObject) == rpATOMIC) {
        CEntity::SetAtomicAlphaCB(m_pRwAtomic, (void*)alpha);
    }
    else if (RwObjectGetType(m_pRwObject) == rpCLUMP) {
        RpClumpForAllAtomics(m_pRwClump, CEntity::SetAtomicAlphaCB, (void*)alpha);
    }
}

// Converted from stdcall bool IsEntityPointerValid(CEntity *entity) 0x533310
bool IsEntityPointerValid(CEntity* entity)
{
    if (!entity)
        return false;

    switch (entity->m_nType) {
    case eEntityType::ENTITY_TYPE_BUILDING:
        return IsBuildingPointerValid(reinterpret_cast<CBuilding*>(entity));
    case eEntityType::ENTITY_TYPE_VEHICLE:
        return IsVehiclePointerValid(reinterpret_cast<CVehicle*>(entity));
    case eEntityType::ENTITY_TYPE_PED:
        return IsPedPointerValid(reinterpret_cast<CPed*>(entity));
    case eEntityType::ENTITY_TYPE_OBJECT:
        return IsObjectPointerValid(reinterpret_cast<CObject*>(entity));
    case eEntityType::ENTITY_TYPE_DUMMY:
        return IsDummyPointerValid(reinterpret_cast<CDummy*>(entity));
    case eEntityType::ENTITY_TYPE_NOTINPOOLS:
        return true;
    }

    return false;
}

// Converted from thiscall CVector CEntity::FindTriggerPointCoors(int triggerIndex) 0x533380
CVector* CEntity::FindTriggerPointCoors(CVector* pOutVec, int triggerIndex)
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    for (int32_t iFxInd = 0; iFxInd < pModelInfo->m_n2dfxCount; ++iFxInd) {
        auto pEffect = pModelInfo->Get2dEffect(iFxInd);
        if (pEffect->m_nType == e2dEffectType::EFFECT_SLOTMACHINE_WHEEL && pEffect->iSlotMachineIndex == triggerIndex) {
            *pOutVec = *GetMatrix() * pEffect->m_vecPosn;
            return pOutVec;
        }
    }

    pOutVec->Set(0.0F, 0.0F, 0.0F);
    return pOutVec;
}

// Converted from thiscall C2dEffect* CEntity::GetRandom2dEffect(int effectType,uchar) 0x533410
C2dEffect* CEntity::GetRandom2dEffect(int effectType, unsigned char bCheckForEmptySlot)
{
    C2dEffect* apArr[32];
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    int iFoundCount = 0;
    for (int32_t iFxInd = 0; iFxInd < pModelInfo->m_n2dfxCount; ++iFxInd) {
        auto pEffect = pModelInfo->Get2dEffect(iFxInd);
        if (pEffect->m_nType != effectType)
            continue;

        if (bCheckForEmptySlot && !GetPedAttractorManager()->HasEmptySlot(pEffect, this))
            continue;

        if (iFoundCount < 32) {
            apArr[iFoundCount] = pEffect;
            ++iFoundCount;
        }
    }

    if (iFoundCount) {
        auto iRandInd = CGeneral::GetRandomNumberInRange(0, iFoundCount);
        return apArr[iRandInd];
    }

    return nullptr;
}

// Converted from thiscall CVector CEntity::TransformFromObjectSpace(CVector const &offset) 0x5334F0
CVector CEntity::TransformFromObjectSpace(CVector const& offset)
{
    auto result = CVector();
    if (m_matrix) {
        result = *m_matrix * offset;
        return result;
    }

    TransformPoint(result, m_placement, offset);
    return result;
}

// Converted from thiscall void CEntity::TransformFromObjectSpace(CVector &outPosn,CVector const &offset) 0x533560
CVector* CEntity::TransformFromObjectSpace(CVector& outPosn, CVector const& offset)
{
    auto result = CEntity::TransformFromObjectSpace(offset);
    outPosn = result;
    return &outPosn;
}

// Converted from thiscall void CEntity::CreateEffects(void) 0x533790
void CEntity::CreateEffects()
{
    m_bHasRoadsignText = false;
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (!pModelInfo->m_n2dfxCount)
        return;

    for (int32_t iFxInd = 0; iFxInd < pModelInfo->m_n2dfxCount; ++iFxInd) {
        auto pEffect = pModelInfo->Get2dEffect(iFxInd);
        if (pEffect->m_nType == e2dEffectType::EFFECT_LIGHT) {
            m_bHasPreRenderEffects = true;
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_PARTICLE) {
            auto pMatrix = CEntity::GetModellingMatrix();
            g_fx.CreateEntityFx(this, pEffect->particle.m_szName, &pEffect->m_vecPosn, pMatrix);
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_ATTRACTOR) {
            if (pEffect->pedAttractor.m_nAttractorType == ePedAttractorType::PED_ATTRACTOR_TRIGGER_SCRIPT)
                CTheScripts::ScriptsForBrains.RequestAttractorScriptBrainWithThisName(pEffect->pedAttractor.m_szScriptName);
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_ENEX) {
            auto vecExit = pEffect->m_vecPosn + pEffect->enEx.m_vecExitPosn;
            auto vecWorldEffect = CEntity::TransformFromObjectSpace(pEffect->m_vecPosn);
            auto vecWorldExit = CEntity::TransformFromObjectSpace(vecExit);

            if (pEffect->enEx.bTimedEffect) {
                auto ucDays = CClock::ms_nGameClockDays;
                if (pEffect->enEx.m_nTimeOn > pEffect->enEx.m_nTimeOff && CClock::ms_nGameClockHours < pEffect->enEx.m_nTimeOff)
                    ucDays--;

                srand(reinterpret_cast<unsigned int>(this) + ucDays);
            }

            auto fHeading = GetHeading();
            auto fExitRot = pEffect->enEx.m_fExitAngle + RadiansToDegrees(fHeading);
            auto fEnterRot = pEffect->enEx.m_fEnterAngle + RadiansToDegrees(fHeading);
            auto iEnExId = CEntryExitManager::AddOne(vecWorldEffect.x,
                                                     vecWorldEffect.y,
                                                     vecWorldEffect.z,
                                                     fEnterRot,
                                                     pEffect->enEx.m_vecRadius.x,
                                                     pEffect->enEx.m_vecRadius.y,
                                                     0,
                                                     vecWorldExit.x,
                                                     vecWorldExit.y,
                                                     vecWorldExit.z,
                                                     fExitRot,
                                                     pEffect->enEx.m_nInteriorId,
                                                     pEffect->enEx.m_nFlags1 + pEffect->enEx.m_nFlags2 << 8,
                                                     pEffect->enEx.m_nSkyColor,
                                                     pEffect->enEx.m_nTimeOn,
                                                     pEffect->enEx.m_nTimeOff,
                                                     0,
                                                     pEffect->enEx.m_szInteriorName);

            if (iEnExId != -1) {
                auto pAddedEffect = CEntryExitManager::mp_poolEntryExits->GetAt(iEnExId);
                if (pAddedEffect->m_pLink && !pAddedEffect->m_pLink->m_nFlags.bEnableAccess)
                    pAddedEffect->m_nFlags.bEnableAccess = false;
            }
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_ROADSIGN) {
            m_bHasRoadsignText = true;
            auto uiPalleteId = C2dEffect::Roadsign_GetPaletteIDFromFlags(pEffect->roadsign.m_nFlags);
            auto uiLettersPerLine = C2dEffect::Roadsign_GetNumLettersFromFlags(pEffect->roadsign.m_nFlags);
            auto uiNumLines = C2dEffect::Roadsign_GetNumLinesFromFlags(pEffect->roadsign.m_nFlags);

            auto pSignAtomic = CCustomRoadsignMgr::CreateRoadsignAtomic(pEffect->roadsign.m_vecSize.x,
                pEffect->roadsign.m_vecSize.y,
                uiNumLines,
                &pEffect->roadsign.m_pText[0],
                &pEffect->roadsign.m_pText[16],
                &pEffect->roadsign.m_pText[32],
                &pEffect->roadsign.m_pText[48],
                uiLettersPerLine,
                uiPalleteId);

            auto pFrame = RpAtomicGetFrame(pSignAtomic);
            RwFrameSetIdentity(pFrame);
            RwFrameRotate(pFrame, &CVector(0.0F, 0.0F, 1.0F), pEffect->roadsign.m_vecRotation.z, RwOpCombineType::rwCOMBINEREPLACE);
            RwFrameRotate(pFrame, &CVector(1.0F, 0.0F, 0.0F), pEffect->roadsign.m_vecRotation.x, RwOpCombineType::rwCOMBINEPOSTCONCAT);
            RwFrameRotate(pFrame, &CVector(0.0F, 1.0F, 0.0F), pEffect->roadsign.m_vecRotation.y, RwOpCombineType::rwCOMBINEPOSTCONCAT);
            RwFrameTranslate(pFrame, &pEffect->m_vecPosn, RwOpCombineType::rwCOMBINEPOSTCONCAT);
            RwFrameUpdateObjects(pFrame);
            pEffect->roadsign.m_pAtomic = pSignAtomic;
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_ESCALATOR) {
            auto vecStart = CEntity::TransformFromObjectSpace(pEffect->m_vecPosn);
            auto vecBottom = CEntity::TransformFromObjectSpace(pEffect->escalator.m_vecBottom);
            auto vecTop = CEntity::TransformFromObjectSpace(pEffect->escalator.m_vecTop);
            auto vecEnd = CEntity::TransformFromObjectSpace(pEffect->escalator.m_vecEnd);
            auto bMovingDown = pEffect->escalator.m_nDirection == 0;

            CEscalators::AddOne(vecStart, vecBottom, vecTop, vecEnd, bMovingDown, this);
        }
    }
}

// Converted from thiscall void CEntity::DestroyEffects(void) 0x533BF0
void CEntity::DestroyEffects()
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (!pModelInfo->m_n2dfxCount)
        return;

    for (int32_t iFxInd = 0; iFxInd < pModelInfo->m_n2dfxCount; ++iFxInd) {
        auto pEffect = pModelInfo->Get2dEffect(iFxInd);

        if (pEffect->m_nType == e2dEffectType::EFFECT_ATTRACTOR) {
            if (pEffect->pedAttractor.m_nAttractorType == ePedAttractorType::PED_ATTRACTOR_TRIGGER_SCRIPT)
                CTheScripts::ScriptsForBrains.MarkAttractorScriptBrainWithThisNameAsNoLongerNeeded(pEffect->pedAttractor.m_szScriptName);
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_PARTICLE) {
            g_fx.DestroyEntityFx(this);
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_ROADSIGN) {
            if (pEffect->roadsign.m_pAtomic) {
                auto pFrame = RpAtomicGetFrame(pEffect->roadsign.m_pAtomic);
                if (pFrame) {
                    RpAtomicSetFrame(pEffect->roadsign.m_pAtomic, nullptr);
                    RwFrameDestroy(pFrame);
                }
                RpAtomicDestroy(pEffect->roadsign.m_pAtomic);
                pEffect->roadsign.m_pAtomic = nullptr;
            }
        }
        else if (pEffect->m_nType == e2dEffectType::EFFECT_ENEX) {
            auto vecWorld = CEntity::TransformFromObjectSpace(pEffect->m_vecPosn);
            auto iNearestEnex = CEntryExitManager::FindNearestEntryExit(vecWorld, 1.5F, -1);
            if (iNearestEnex != -1) {
                auto enex = CEntryExitManager::mp_poolEntryExits->GetAt(iNearestEnex);
                if (enex->m_nFlags.bEnteredWithoutExit)
                    enex->m_nFlags.bDeleteEnex = true;
                else
                    CEntryExitManager::DeleteOne(iNearestEnex);
            }
        }
    }
}

// Converted from thiscall void CEntity::AttachToRwObject(RwObject *object, bool updateEntityMatrix) 0x533ED0
void CEntity::AttachToRwObject(RwObject* object, bool updateEntityMatrix)
{
    if (!m_bIsVisible)
        return;

    m_pRwObject = object;
    if (!m_pRwObject)
        return;

    if (updateEntityMatrix) {
        auto pMatrix = GetMatrix();
        auto pRwMatrix = CEntity::GetModellingMatrix();
        pMatrix->UpdateMatrix(pRwMatrix);
    }

    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (RwObjectGetType(m_pRwObject) == rpCLUMP && pModelInfo->bIsRoad) {
        if (IsObject())
        {
            reinterpret_cast<CObject*>(this)->AddToMovingList();
            SetIsStatic(false);
        }
        else {
            CWorld::ms_listMovingEntityPtrs.AddItem(this);
        }
    }
    

    pModelInfo->AddRef();
    m_pStreamingLink = CStreaming::AddEntity(this);
    CEntity::CreateEffects();
}

// Converted from thiscall void CEntity::DetachFromRwObject(void) 0x533FB0
void CEntity::DetachFromRwObject()
{
    if (!m_pRwObject)
        return;

    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    pModelInfo->RemoveRef();
    CStreaming::RemoveEntity(m_pStreamingLink);
    m_pStreamingLink = nullptr;

    if (pModelInfo->GetModelType() == ModelInfoType::MODEL_INFO_CLUMP
        && pModelInfo->bIsRoad
        && !IsObject()) {

        CWorld::ms_listMovingEntityPtrs.DeleteItem(this);
    }

    CEntity::DestroyEffects();
    m_pRwObject = nullptr;
}

// Converted from thiscall CVector CEntity::GetBoundCentre(void) 0x534250
CVector* CEntity::GetBoundCentre(CVector* pOutCentre)
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    const auto& pColCenter = pModelInfo->GetColModel()->GetBoundCenter();
    return CEntity::TransformFromObjectSpace(*pOutCentre, pColCenter);
}

// Converted from thiscall void CEntity::GetBoundCentre(CVector &outCentre) 0x534290
void CEntity::GetBoundCentre(CVector& outCentre)
{
    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    const auto& pColCenter = pModelInfo->GetColModel()->GetBoundCenter();
    CEntity::TransformFromObjectSpace(outCentre, pColCenter);
}

// Converted from thiscall void CEntity::RenderEffects(void) 0x5342B0
void CEntity::RenderEffects()
{
    if (!m_bHasRoadsignText)
        return;

    auto pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    if (!pModelInfo->m_n2dfxCount)
        return;

    for (int32_t iFxInd = 0; iFxInd < pModelInfo->m_n2dfxCount; ++iFxInd) {
        auto pEffect = pModelInfo->Get2dEffect(iFxInd);
        if (pEffect->m_nType != e2dEffectType::EFFECT_ROADSIGN)
            continue;

        CCustomRoadsignMgr::RenderRoadsignAtomic(pEffect->roadsign.m_pAtomic, TheCamera.GetPosition());
    }
}

// Converted from thiscall bool CEntity::GetIsTouching(CEntity *entity) 0x5343F0
bool CEntity::GetIsTouching(CEntity* entity)
{
    CVector thisVec;
    CEntity::GetBoundCentre(thisVec);

    CVector otherVec;
    entity->GetBoundCentre(otherVec);

    auto fThisRadius = CModelInfo::GetModelInfo(m_nModelIndex)->GetColModel()->GetBoundRadius();
    auto fOtherRadius = CModelInfo::GetModelInfo(entity->m_nModelIndex)->GetColModel()->GetBoundRadius();

    return (thisVec - otherVec).Magnitude() <= (fThisRadius + fOtherRadius);
}

// Converted from thiscall bool CEntity::GetIsTouching(CVector const &centre,float radius) 0x5344B0
bool CEntity::GetIsTouching(CVector const& centre, float radius)
{
    CVector thisVec;
    CEntity::GetBoundCentre(thisVec);
    auto fThisRadius = CModelInfo::GetModelInfo(m_nModelIndex)->GetColModel()->GetBoundRadius();

    return (thisVec - centre).Magnitude() <= (fThisRadius + radius);
}

// Converted from thiscall bool CEntity::GetIsOnScreen(void) 0x534540
bool CEntity::GetIsOnScreen()
{
    CVector thisVec;
    CEntity::GetBoundCentre(thisVec);
    auto fThisRadius = CModelInfo::GetModelInfo(m_nModelIndex)->GetColModel()->GetBoundRadius();

    if (TheCamera.IsSphereVisible(&thisVec, fThisRadius, reinterpret_cast<RwMatrixTag*>(&TheCamera.m_mMatInverse)))
        return true;

    if (TheCamera.m_bMirrorActive)
        return TheCamera.IsSphereVisible(&thisVec, fThisRadius, reinterpret_cast<RwMatrixTag*>(&TheCamera.m_mMatMirrorInverse));

    return false;
}

// Converted from thiscall bool CEntity::GetIsBoundingBoxOnScreen(void) 0x5345D0
bool CEntity::GetIsBoundingBoxOnScreen()
{
    auto pColModel = CModelInfo::GetModelInfo(m_nModelIndex)->GetColModel();
    CVector vecBnd[2]{ pColModel->m_boundBox.m_vecMin, pColModel->m_boundBox.m_vecMax };

    RwV3d vecNormals[2];
    if (m_matrix) {
        auto tempMat = CMatrix();
        Invert(m_matrix, &tempMat);
        TransformVectors(&vecNormals[0], 2, tempMat, &TheCamera.m_avecFrustumWorldNormals[0]);
    }
    else {
        auto tempTrans = CSimpleTransform();
        tempTrans.Invert(m_placement);
        TransformVectors(&vecNormals[0], 2, tempTrans, &TheCamera.m_avecFrustumWorldNormals[0]);
    }

    for (int32_t i = 0; i < 2; ++i) {
        CVector vecUsed;
        vecUsed.x = vecBnd[signbit(vecNormals[i].x)].x;
        vecUsed.y = vecBnd[signbit(vecNormals[i].y)].y;
        vecUsed.z = vecBnd[signbit(vecNormals[i].z)].z;

        auto vecWorld = CEntity::TransformFromObjectSpace(vecUsed);
        if (DotProduct(vecWorld, TheCamera.m_avecFrustumWorldNormals[i]) > TheCamera.m_fFrustumPlaneOffsets[i]
            && (!TheCamera.m_bMirrorActive
                || DotProduct(vecWorld, TheCamera.m_avecFrustumWorldNormals_Mirror[i]) > TheCamera.m_fFrustumPlaneOffsets_Mirror[i])) {

            ++numBBFailed;
            return false;
        }
    }
    return true;
}

// Converted from thiscall void CEntity::ModifyMatrixForTreeInWind(void) 0x534E90
void CEntity::ModifyMatrixForTreeInWind()
{
    if (CTimer::GetIsPaused())
        return;

    auto pRwMat = CEntity::GetModellingMatrix();
    if (!pRwMat)
        return;

    auto pAt = RwMatrixGetAt(pRwMat);

    float fWindOffset;
    if (CWeather::Wind >= 0.5F) {
        auto uiOffset1 = (((m_nRandomSeed + CTimer::m_snTimeInMilliseconds * 8) & 0xFFFF) / 4096) & 0xF;
        auto uiOffset2 = (uiOffset1 + 1) & 0xF;
        auto fContrib = static_cast<float>(((m_nRandomSeed + CTimer::m_snTimeInMilliseconds * 8) & 0xFFF)) / 4096.0F;

        fWindOffset = (1.0F - fContrib) * CWeather::saTreeWindOffsets[uiOffset1];
        fWindOffset += 1.0F + fContrib * CWeather::saTreeWindOffsets[uiOffset2];
        fWindOffset *= CWeather::Wind;
        fWindOffset *= 0.015F;

    }
    else {
        auto uiTimeOffset = (reinterpret_cast<unsigned int>(this) + CTimer::m_snTimeInMilliseconds) & 0xFFF;
        
        fWindOffset = sin(uiTimeOffset * 0.0015332032F) * 0.005F;
        if (CWeather::Wind >= 0.2F)
            fWindOffset *= 1.6F;
    }

    pAt->x = fWindOffset;
    if (CModelInfo::GetModelInfo(m_nModelIndex)->IsSwayInWind2())
        pAt->x += CWeather::Wind * 0.03F;

    pAt->y = pAt->x;    
    pAt->x *= CWeather::WindDir.x;
    pAt->y *= CWeather::WindDir.y;

    CWindModifiers::FindWindModifier(GetPosition(), &pAt->x, &pAt->y);
    CEntity::UpdateRwFrame();

}

// Converted from thiscall void CEntity::ModifyMatrixForBannerInWind(void) 0x535040
void CEntity::ModifyMatrixForBannerInWind()
{
    ((void(__thiscall *)(CEntity*))0x535040)(this);
}

RwMatrix* CEntity::GetModellingMatrix()
{
    if (!m_pRwObject)
        return nullptr;

    return RwFrameGetMatrix((RwFrame*)rwObjectGetParent(m_pRwObject));
}

// Converted from thiscall CColModel* CEntity::GetColModel(void) 0x535300
CColModel* CEntity::GetColModel()
{
    if (IsVehicle() && static_cast<CVehicle*>(this)->m_nSpecialColModel != 0xFF)
        return &CVehicle::m_aSpecialColModel[static_cast<CVehicle*>(this)->m_nSpecialColModel];
    else
        return CModelInfo::GetModelInfo(m_nModelIndex)->m_pColModel;
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
    if (!m_pRwObject)
        return;

    auto pRwMatrix = CEntity::GetModellingMatrix();
    if (m_matrix)
        m_matrix->UpdateRwMatrix(pRwMatrix);
    else
        m_placement.UpdateRwMatrix(pRwMatrix);
}

RpAtomic* CEntity::SetAtomicAlphaCB(RpAtomic* pAtomic, void* pData)
{
    auto pGeometry = RpAtomicGetGeometry(pAtomic);
    RpGeometrySetFlags(pGeometry, rpGEOMETRYMODULATEMATERIALCOLOR);
    RpGeometryForAllMaterials(pGeometry, CEntity::SetMaterialAlphaCB, pData);
    return pAtomic;
}

RpMaterial* CEntity::SetMaterialAlphaCB(RpMaterial* pMaterial, void* pData)
{
    pMaterial->color.alpha = reinterpret_cast<RwUInt8>(pData);
    return pMaterial;
}
