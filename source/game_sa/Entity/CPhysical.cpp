/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "StdInc.h"

float& CPhysical::DAMPING_LIMIT_IN_FRAME = *(float*)0x8CD7A0;
float& CPhysical::DAMPING_LIMIT_OF_SPRING_FORCE = *(float*)0x8CD7A4;
float& CPhysical::PHYSICAL_SHIFT_SPEED_DAMP = *(float*)0x8CD788;
float& CPhysical::SOFTCOL_SPEED_MULT = *(float*)0x8CD794;
float& CPhysical::SOFTCOL_SPEED_MULT2 = *(float*)0x8CD798;
float& CPhysical::SOFTCOL_DEPTH_MIN = *(float*)0x8CD78C;
float& CPhysical::SOFTCOL_DEPTH_MULT = *(float*)0x8CD790;
float& CPhysical::SOFTCOL_CARLINE_SPEED_MULT = *(float*)0x8CD79C;
float& CPhysical::TEST_ADD_AMBIENT_LIGHT_FRAC = *(float*)0x8CD7B8;
float& CPhysical::HIGHSPEED_ELASTICITY_MULT_COPCAR = *(float*)0x8CD784;
CVector& CPhysical::fxDirection = *(CVector*)0xB73720;

CPhysical::CPhysical() : CEntity(plugin::dummy)
{
    m_pCollisionList = nullptr;
    CPlaceable::AllocateStaticMatrix();
    m_matrix->SetUnity();
    m_vecMoveSpeed.Set(0.0F, 0.0F, 0.0F);
    m_vecTurnSpeed.Set(0.0F, 0.0F, 0.0F);
    m_vecFrictionMoveSpeed.Set(0.0F, 0.0F, 0.0F);
    m_vecFrictionTurnSpeed.Set(0.0F, 0.0F, 0.0F);
    m_vecForce.Set(0.0F, 0.0F, 0.0F);
    m_vecTorque.Set(0.0F, 0.0F, 0.0F);

    m_fMass = 1.0;
    m_fTurnMass = 1.0;
    m_fVelocityFrequency = 1.0;
    m_fAirResistance = 0.1;
    m_pMovingList = nullptr;
    m_bFakePhysics = 0;
    m_nNumEntitiesCollided = 0;
    memset(m_apCollidedEntities, 0, sizeof(m_apCollidedEntities));
    m_nPieceType = 0;
    m_fDamageIntensity = 0.0;
    m_pDamageEntity = nullptr;
    m_vecLastCollisionImpactVelocity.Set(0.0F, 0.0F, 0.0F);
    m_vecLastCollisionPosn.Set(0.0F, 0.0F, 0.0F);
    m_vecCentreOfMass.Set(0.0F, 0.0F, 0.0F);
    m_fMovingSpeed = 0.0;
    m_pAttachedTo = nullptr;
    m_pEntityIgnoredCollision = 0;
    m_qAttachedEntityRotation.real = 0.0;
    m_qAttachedEntityRotation.imag.Set(0.0F, 0.0F, 0.0F);
    m_fDynamicLighting = 0.0;
    m_fContactSurfaceBrightness = 1.0;
    m_pShadowData = nullptr;
    field_38 = 100.0;
    m_bUsesCollision = true;
    m_nPhysicalFlags = ePhysicalFlags::PHYSICAL_APPLY_GRAVITY;
}

void CPhysical::InjectHooks()
{
    HookInstall(0x542560, &CPhysical::RemoveAndAdd);
    HookInstall(0x542A50, &CPhysical::ApplyTurnForce);
    HookInstall(0x542B50, &CPhysical::ApplyForce);
    HookInstall(0x542CE0, &CPhysical::GetSpeed);
    //HookInstall(0x542DD0, &CPhysical::ApplyMoveSpeed); // Go to the function definition and see why this is commented
    HookInstall(0x542E20, &CPhysical::ApplyTurnSpeed);
    HookInstall(0x5429F0, (void(CPhysical::*)(CVector force)) & CPhysical::ApplyMoveForce);
    HookInstall(0x5428C0, &CPhysical::SetDamagedPieceRecord);
    HookInstall(0x542860, &CPhysical::RemoveFromMovingList);
    HookInstall(0x542800, &CPhysical::AddToMovingList);
    HookInstall(0x544A30, &CPhysical::Add_Reversed);
    HookInstall(0x5424C0, &CPhysical::Remove_Reversed);
    HookInstall(0x5449B0, &CPhysical::GetBoundRect_Reversed);
    HookInstall(0x5485E0, &CPhysical::ProcessControl_Reversed);
    HookInstall(0x54DFB0, &CPhysical::ProcessCollision_Reversed);
    HookInstall(0x54DB10, &CPhysical::ProcessShift_Reversed);
    HookInstall(0x54DEC0, &CPhysical::TestCollision_Reversed);
    HookInstall(0x546D00, &CPhysical::ProcessEntityCollision_Reversed);
    HookInstall(0x542FE0, &CPhysical::ApplyGravity);
    HookInstall(0x5430A0, &CPhysical::ApplyFrictionMoveForce);
    HookInstall(0x543220, &CPhysical::ApplyFrictionForce);
    HookInstall(0x5433B0, &CPhysical::SkipPhysics);
    HookInstall(0x543490, &CPhysical::AddCollisionRecord);
    HookInstall(0x543540, &CPhysical::GetHasCollidedWith);
    HookInstall(0x543580, &CPhysical::GetHasCollidedWithAnyObject);
    HookInstall(0x5435C0, (bool(CPhysical::*)(CEntity*, CColPoint*, float*)) & CPhysical::ApplyCollision);
    HookInstall(0x543890, (bool(CPhysical::*)(CEntity*, CColPoint*, float*)) & CPhysical::ApplySoftCollision);
    HookInstall(0x543C90, &CPhysical::ApplySpringCollision);
    HookInstall(0x543D60, &CPhysical::ApplySpringCollisionAlt);
    HookInstall(0x543E90, &CPhysical::ApplySpringDampening);
    HookInstall(0x544280, &CPhysical::RemoveRefsToEntity);
    HookInstall(0x5442F0, &CPhysical::DettachEntityFromEntity);
    HookInstall(0x5446A0, &CPhysical::DettachAutoAttachedEntity);
    HookInstall(0x5447B0, &CPhysical::GetLightingFromCol);
    HookInstall(0x544850, &CPhysical::GetLightingTotal);
    HookInstall(0x5448B0, &CPhysical::CanPhysicalBeDamaged);
    HookInstall(0x544C40, &CPhysical::ApplyAirResistance);
    HookInstall(0x544D50, &CPhysical::ApplyCollisionAlt);
    HookInstall(0x5454C0, (bool(CPhysical::*)(float, CColPoint*)) & CPhysical::ApplyFriction);
    HookInstall(0x545980, (bool(CPhysical::*)(CPhysical*, float, CColPoint*)) &CPhysical::ApplyFriction);
    HookInstall(0x546670, &CPhysical::ProcessShiftSectorList);
    HookInstall(0x547B80, &CPhysical::ApplySpeed);
    HookInstall(0x548320, &CPhysical::UnsetIsInSafePosition);
    HookInstall(0x5483D0, (void(CPhysical::*)()) & CPhysical::ApplyFriction);
    HookInstall(0x548680, (bool(CPhysical::*)(CEntity*, CColPoint*, float*, float*)) & CPhysical::ApplyCollision);
    HookInstall(0x54A2C0, (bool(CPhysical::*)(CPhysical*, CColPoint*, float*, float*))&CPhysical::ApplySoftCollision);
    HookInstall(0x54BA60, &CPhysical::ProcessCollisionSectorList);
    HookInstall(0x54CFF0, &CPhysical::ProcessCollisionSectorList_SimpleCar);
    HookInstall(0x54D570, (void(CPhysical::*)(CPhysical*, CVector, CVector))&CPhysical::AttachEntityToEntity);
    HookInstall(0x54D690, (void(CPhysical::*)(CPhysical*, CVector*, CQuaternion*))&CPhysical::AttachEntityToEntity);
    HookInstall(0x54D920, &CPhysical::CheckCollision);
    HookInstall(0x54DAB0, &CPhysical::CheckCollision_SimpleCar);
    HookInstall(0x546DB0, &CPhysical::PlacePhysicalRelativeToOtherPhysical);
    HookInstall(0x546FF0, &CPhysical::PositionAttachedEntity);
}

void CPhysical::Add()
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x544A30, CPhysical*>(this);
#else
    CPhysical::Add_Reversed();
#endif
}

void CPhysical::Add_Reversed()
{
    if (m_bIsBIGBuilding)
    {
        CEntity::Add();
    }
    else
    {
        CRect boundRect;
        GetBoundRect(&boundRect);
        std::int32_t startSectorX = CWorld::GetSectorX(boundRect.left);
        std::int32_t startSectorY = CWorld::GetSectorY(boundRect.top);
        std::int32_t endSectorX = CWorld::GetSectorX(boundRect.right);
        std::int32_t endSectorY = CWorld::GetSectorY(boundRect.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                CPtrListDoubleLink* pDoubleLinkList = nullptr;
                CRepeatSector* pRepeatSector = GetRepeatSector(sectorX, sectorY);
                switch (m_nType)
                {
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

                auto pNewEntityInfoNode = CPools::ms_pEntryInfoNodePool->New();
                if (pNewEntityInfoNode)
                {
                    auto pNewDoubleLink = CPools::ms_pPtrNodeDoubleLinkPool->New();;
                    if (pNewDoubleLink)
                        pNewDoubleLink->pItem = this;
           
                    pNewDoubleLink->pPrev = nullptr;
                    pNewDoubleLink->pNext = (CPtrNodeDoubleLink*)pDoubleLinkList->pNode;
                    if (pDoubleLinkList->GetNode())
                        pDoubleLinkList->GetNode()->pPrev = pNewDoubleLink;
                    pDoubleLinkList->pNode = (CPtrNode * )pNewDoubleLink;
                    pNewEntityInfoNode->m_pDoubleLink = pNewDoubleLink;
                    pNewEntityInfoNode->m_pRepeatSector = pRepeatSector;
                    pNewEntityInfoNode->m_pDoubleLinkList = pDoubleLinkList;
                }
                pNewEntityInfoNode->m_pPrevious = nullptr;
                pNewEntityInfoNode->m_pNext = m_pCollisionList;
                auto pEntityCollisionList = m_pCollisionList;
                if (pEntityCollisionList)
                    pEntityCollisionList->m_pPrevious = pNewEntityInfoNode;

                m_pCollisionList = pNewEntityInfoNode;
            }
        }
    }
}

void CPhysical::Remove()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CEntity*))(0x5424C0))(this);
#else
    CPhysical::Remove_Reversed();
#endif
}

void CPhysical::Remove_Reversed()
{
    if (m_bIsBIGBuilding)
    {
        CEntity::Remove();
    }
    else
    {
        CEntryInfoNode* pEntryInfoNode = m_pCollisionList;
        while (pEntryInfoNode)
        {
            CPtrNodeDoubleLink* pDoubleLink = pEntryInfoNode->m_pDoubleLink;
            if (pEntryInfoNode->m_pDoubleLinkList->pNode == (CPtrNode*)pDoubleLink)
                pEntryInfoNode->m_pDoubleLinkList->pNode = (CPtrNode*)pDoubleLink->pNext;
            CPtrNodeDoubleLink* pDoubleLinkPrevious = pDoubleLink->pPrev;
            if (pDoubleLinkPrevious)
                pDoubleLinkPrevious->pNext = pDoubleLink->pNext;
            CPtrNodeDoubleLink* pDoubleLinkNext = pDoubleLink->pNext;
            if (pDoubleLinkNext)
                pDoubleLinkNext->pPrev = pDoubleLink->pPrev;

            CPools::ms_pPtrNodeDoubleLinkPool->Delete(pDoubleLink);

            CEntryInfoNode* pEntryInfoNodeNext = pEntryInfoNode->m_pNext;
            if (m_pCollisionList == pEntryInfoNode)
                m_pCollisionList = pEntryInfoNodeNext;
            CEntryInfoNode* pEntryInfoNodePrevious = pEntryInfoNode->m_pPrevious;
            if (pEntryInfoNodePrevious)
                pEntryInfoNodePrevious->m_pNext = pEntryInfoNodeNext;
            if (pEntryInfoNodeNext)
                pEntryInfoNodeNext->m_pPrevious = pEntryInfoNode->m_pPrevious;

            CEntryInfoNode* pNextEntryInfoNode = pEntryInfoNode->m_pNext;
            CPools::ms_pEntryInfoNodePool->Delete(pEntryInfoNode);
            pEntryInfoNode = pNextEntryInfoNode;
        }
    }
}

CRect* CPhysical::GetBoundRect(CRect* pRect)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((CRect*(__thiscall*)(CEntity*, CRect*))(0x5449B0))(this, pRect);
#else
    return CPhysical::GetBoundRect_Reversed(pRect);
#endif
}

CRect* CPhysical::GetBoundRect_Reversed(CRect* pRect)
{
    CVector boundCentre;
    CEntity::GetBoundCentre(&boundCentre);
    float fRadius = CModelInfo::ms_modelInfoPtrs[m_nModelIndex]->m_pColModel->m_boundSphere.m_fRadius;
    *pRect = CRect(boundCentre.x - fRadius, boundCentre.y - fRadius, boundCentre.x + fRadius, boundCentre.y + fRadius);
    return pRect;
}

void CPhysical::ProcessControl()
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x5485E0, CPhysical*>(this);
#else
    CPhysical::ProcessControl_Reversed();
#endif
}

void CPhysical::ProcessControl_Reversed()
{
    if (m_nType != ENTITY_TYPE_PED)
        physicalFlags.bSubmergedInWater = false;

    m_bHasHitWall = false;
    m_bWasPostponed = false;
    m_bIsInSafePosition = false;
    m_bHasContacted = false;

    if (m_nStatus != STATUS_SIMPLE)
    {
        physicalFlags.b31 = false;
        physicalFlags.bOnSolidSurface = false;
        m_nNumEntitiesCollided = 0;
        m_nPieceType = 0;
        m_fDamageIntensity = 0.0f;
        if (m_pDamageEntity)
            m_pDamageEntity->CleanUpOldReference(&m_pDamageEntity);
        m_pDamageEntity = nullptr;
        ApplyFriction();
        if (!m_pAttachedTo || physicalFlags.bInfiniteMass)
        {
            ApplyGravity();
            ApplyAirResistance();
        }
    }
}

void CPhysical::ProcessCollision()
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x54DFB0, CPhysical*>(this);
#else
    ProcessCollision_Reversed();
#endif
}

void CPhysical::ProcessCollision_Reversed()
{
    m_fMovingSpeed = 0.0f;
    physicalFlags.bProcessingShift = false;
    physicalFlags.b13 = false;
    if (m_bUsesCollision && !physicalFlags.bDisableSimpleCollision)  {
        if (m_nStatus == STATUS_SIMPLE) {
            if (CheckCollision_SimpleCar() && m_nStatus == STATUS_SIMPLE) {
                m_nStatus = STATUS_PHYSICS;
                if (m_nType == ENTITY_TYPE_VEHICLE)
                    CCarCtrl::SwitchVehicleToRealPhysics(static_cast<CVehicle*>(this));
            }
            m_bIsStuck = false;
            m_bIsInSafePosition = true;
            RemoveAndAdd();
            return;
        }
        CVehicle* pVehicle = static_cast<CVehicle*>(this);
        CAutomobile* pAutomobile = static_cast<CAutomobile*>(this);
        CBike* pBike = static_cast<CBike*>(this);
        if (m_nStatus == STATUS_TRAILER) {
            CColPoint* pWheelsColPoints = nullptr;
            float* pfWheelsSuspensionCompression = nullptr;
            CVector* pWheelsCollisionPositions = nullptr;
            if (pVehicle->m_nVehicleSubClass) {
                pBike->m_apWheelCollisionEntity[0] = nullptr;
                pBike->m_apWheelCollisionEntity[1] = nullptr;
                pBike->m_apWheelCollisionEntity[2] = nullptr;
                pBike->m_apWheelCollisionEntity[3] = nullptr;
                pWheelsColPoints = pBike->m_anWheelColPoint;
                pfWheelsSuspensionCompression = pBike->m_fWheelsSuspensionCompression;
                pWheelsCollisionPositions = pBike->m_avTouchPointsLocalSpace;
            }
            else {
                pAutomobile->m_pWheelCollisionEntity[0] = nullptr;
                pAutomobile->m_pWheelCollisionEntity[1] = nullptr;
                pAutomobile->m_pWheelCollisionEntity[2] = nullptr;
                pAutomobile->m_pWheelCollisionEntity[3] = nullptr;
                pWheelsColPoints = pAutomobile->m_wheelColPoint;
                pfWheelsSuspensionCompression = pAutomobile->m_fWheelsSuspensionCompression;
                pWheelsCollisionPositions = pAutomobile->m_vWheelCollisionPos;
            }
            CCollisionData* pColData = GetColModel()->m_pColData;
            int collisionIndex = 0;
            while (true) {
                int colLinesCount = pColData->m_nNumLines;
                if (pColData->bUsesDisks)
                    colLinesCount = 0;
                if (collisionIndex >= colLinesCount) {
                    m_bIsStuck = false;
                    m_bIsInSafePosition = true;
                    m_vecMoveSpeed.z = 0.0f;
                    m_vecTurnSpeed.y = 0.0f;
                    m_vecTurnSpeed.x = 0.0f;
                    m_matrix->ForceUpVector(0.0f, 0.0f, 1.0f);
                    GetPosition().z = CCarCtrl::FindGhostRoadHeight(pVehicle) + pVehicle->GetHeightAboveRoad();
                    ApplySpeed();
                    m_matrix->Reorthogonalise();
                    RemoveAndAdd();
                    pVehicle->vehicleFlags.bVehicleColProcessed = true;
                    return;
                }

                CColPoint* pWheelColPoint = &pWheelsColPoints[collisionIndex];
                CColLine* pColLine = &pColData->m_pLines[collisionIndex];
                CVector vecColLinePosStart = *m_matrix * pColLine->m_vecStart;
                CVector vecColLinePosEnd = *m_matrix * pColLine->m_vecEnd;
                pWheelColPoint->m_vecNormal = CVector(0.0f, 0.0f, 1.0f);
                pWheelColPoint->m_nSurfaceTypeA = SURFACE_WHEELBASE;
                pWheelColPoint->m_nSurfaceTypeB = SURFACE_TARMAC;
                pWheelColPoint->m_fDepth = 0.0f;
                float fGhostRoadHeight = CCarCtrl::FindGhostRoadHeight(pVehicle);
                if (fGhostRoadHeight <= vecColLinePosStart.z) {
                    if (fGhostRoadHeight > vecColLinePosEnd.z) {
                        float fWheelSuspensionCompression = (vecColLinePosStart.z - fGhostRoadHeight) / (vecColLinePosStart.z - vecColLinePosEnd.z);
                        pfWheelsSuspensionCompression[collisionIndex] = fWheelSuspensionCompression;
                        pWheelColPoint->m_vecPoint = (vecColLinePosEnd - vecColLinePosStart) * fWheelSuspensionCompression + vecColLinePosStart;
                    }
                    else {
                        pfWheelsSuspensionCompression[collisionIndex] = 1.0f;
                        pWheelColPoint->m_vecPoint = vecColLinePosEnd;
                    }
                }
                else {
                    pfWheelsSuspensionCompression[collisionIndex] = 0.0f;
                    pWheelColPoint->m_vecPoint = vecColLinePosStart;
                }
                pWheelsCollisionPositions[collisionIndex] = pWheelColPoint->m_vecPoint - GetPosition();
                collisionIndex++;
            }
        }
        CVector vecOldMoveSpeed = m_vecMoveSpeed;
        float fOldTimeStep = CTimer::ms_fTimeStep;
        float fOldElasticity = m_fElasticity;
        CMatrix oldEntityMatrix(*m_matrix);
        bool bProcessCollisionBeforeSettingTimeStep = false;
        bool bUnknown = false;
        unsigned char collisionSteps = SpecialEntityCalcCollisionSteps(&bProcessCollisionBeforeSettingTimeStep, &bUnknown);
        float fStep = fOldTimeStep / collisionSteps;
        if (bProcessCollisionBeforeSettingTimeStep)
        {
            ApplySpeed();
            m_matrix->Reorthogonalise();
            physicalFlags.bProcessingShift = false;
            physicalFlags.b13 = false;
            physicalFlags.b17 = true;
            bool bOldUsesCollision = m_bUsesCollision;
            m_bUsesCollision = false;
            if (!CheckCollision())
            {
                physicalFlags.b17 = false;
                m_bUsesCollision = bOldUsesCollision;
                if (m_nType == ENTITY_TYPE_VEHICLE)
                    pVehicle->vehicleFlags.bVehicleColProcessed = true;
                m_bIsStuck = false;
                m_bIsInSafePosition = true;
                physicalFlags.bProcessCollisionEvenIfStationary = false;
                physicalFlags.b13 = false;
                m_fElasticity = fOldElasticity;
                m_fMovingSpeed = DistanceBetweenPoints(oldEntityMatrix.GetPosition(), GetPosition());
                RemoveAndAdd();
                return;
            }
            m_bUsesCollision = bOldUsesCollision;
            physicalFlags.b17 = false;
            *static_cast<CMatrix*>(m_matrix) = oldEntityMatrix;
            m_vecMoveSpeed = vecOldMoveSpeed;
            if (m_nType == ENTITY_TYPE_VEHICLE && pVehicle->vehicleFlags.bIsLawEnforcer)
                m_fElasticity *= HIGHSPEED_ELASTICITY_MULT_COPCAR;
        }

        CPed* pPed = static_cast<CPed*>(this);
        if (collisionSteps > 1u)
        {
            for (unsigned char stepIndex = 1; stepIndex < collisionSteps; stepIndex++)
            {
                CTimer::UpdateTimeStep(stepIndex * fStep);
                ApplySpeed();
                bool bCheckCollision = CheckCollision();
                if (m_nType == ENTITY_TYPE_PED && m_vecMoveSpeed.z == 0.0f && !pPed->bWasStanding && pPed->bIsStanding)
                    oldEntityMatrix.GetPosition().z = GetPosition().z;
                *static_cast<CMatrix*>(m_matrix) = oldEntityMatrix;
                CTimer::UpdateTimeStep(fOldTimeStep);
                if (bCheckCollision) {
                    m_fElasticity = fOldElasticity;
                    return;
                }
                if (m_nType == ENTITY_TYPE_VEHICLE) {
                    if (pVehicle->m_nVehicleClass) {
                        if (pVehicle->m_nVehicleClass == VEHICLE_BIKE) {
                            pBike->m_fWheelsSuspensionCompression[0] = 1.0f;
                            pBike->m_fWheelsSuspensionCompression[1] = 1.0f;
                            pBike->m_fWheelsSuspensionCompression[2] = 1.0f;
                            pBike->m_fWheelsSuspensionCompression[3] = 1.0f;
                        }
                        else if (pVehicle->m_nVehicleClass == VEHICLE_TRAILER) {
                            pAutomobile->m_fWheelsSuspensionCompression[0] = 1.0f;
                            pAutomobile->m_fWheelsSuspensionCompression[1] = 1.0f;
                            pAutomobile->m_fWheelsSuspensionCompression[2] = 1.0f;
                        }
                    }
                    else {
                        pAutomobile->m_fWheelsSuspensionCompression[0] = 1.0f;
                        pAutomobile->m_fWheelsSuspensionCompression[1] = 1.0f;
                        pAutomobile->m_fWheelsSuspensionCompression[2] = 1.0f;
                        pAutomobile->m_fWheelsSuspensionCompression[3] = 1.0f;
                    }
                }
            }
        }
        ApplySpeed();
        m_matrix->Reorthogonalise();
        physicalFlags.bProcessingShift = false;
        physicalFlags.b13 = false;
        if (m_vecMoveSpeed.x != 0.0f
            || m_vecMoveSpeed.y != 0.0f
            || m_vecMoveSpeed.z != 0.0f
            || m_vecTurnSpeed.x != 0.0f
            || m_vecTurnSpeed.y != 0.0f
            || m_vecTurnSpeed.z != 0.0f
            || physicalFlags.bProcessCollisionEvenIfStationary
            || m_nStatus == STATUS_PLAYER
            || m_nType == ENTITY_TYPE_VEHICLE && pVehicle->vehicleFlags.bIsCarParkVehicle
            || m_nType == ENTITY_TYPE_PED && (pPed->IsPlayer() || pPed->bTestForBlockedPositions|| !pPed->bIsStanding)) 
        {
            if (m_nType == ENTITY_TYPE_VEHICLE)
                pVehicle->vehicleFlags.bVehicleColProcessed = true;
            if (CheckCollision()) {
                *static_cast<CMatrix*>(m_matrix) = oldEntityMatrix;
                m_fElasticity = fOldElasticity;
                return;
            }
        }
        else if (m_nType == ENTITY_TYPE_PED) {
            pPed->bIsStanding = true;
        }
        m_bIsStuck = false;
        m_bIsInSafePosition = true;
        physicalFlags.bProcessCollisionEvenIfStationary = false;
        physicalFlags.b13 = false;
        m_fElasticity = fOldElasticity;
        m_fMovingSpeed = DistanceBetweenPoints(oldEntityMatrix.GetPosition(), GetPosition());
        RemoveAndAdd();
        return;
    }
    m_bIsStuck = false;
    m_bIsInSafePosition = true;
    RemoveAndAdd();
}

void CPhysical::ProcessShift()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x54DB10)(this);
#else
    CPhysical::ProcessShift_Reversed();
#endif
}

bool CPhysical::TestCollision(bool bApplySpeed) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CEntity*, bool))(0x54DEC0))(this, bApplySpeed);
#else
   return CPhysical::TestCollision_Reversed(bApplySpeed);
#endif
}

bool CPhysical::TestCollision_Reversed(bool bApplySpeed) {
    CMatrix entityMatrix(*m_matrix);
    physicalFlags.b17 = true;
    physicalFlags.b13 = true;
    bool bOldUsescollision = m_bUsesCollision;
    m_bUsesCollision = false;
    bool bTestForBlockedPositions = false;
    CPed* pPed = static_cast<CPed*>(this);
    if (m_nType == ENTITY_TYPE_PED && pPed->bTestForBlockedPositions) {
        bTestForBlockedPositions = true;
        pPed->bTestForBlockedPositions = false;
    }
    if (bApplySpeed)
        ApplySpeed();
    bool bCheckCollision = CheckCollision();
    m_bUsesCollision = bOldUsescollision;
    physicalFlags.b17 = false;
    physicalFlags.b13 = false;
    *(CMatrix*)m_matrix = entityMatrix;
    if (bTestForBlockedPositions)
        pPed->bTestForBlockedPositions = true;
    return bCheckCollision;
}

int CPhysical::ProcessEntityCollision(CPhysical* entity, CColPoint* colpoint) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((int (__thiscall*)(CPhysical*, CPhysical*, CColPoint*))0x546D00)(this, entity, colpoint);
#else
    return CPhysical::ProcessEntityCollision_Reversed(entity, colpoint);
#endif
}

int CPhysical::ProcessEntityCollision_Reversed(CPhysical* entity, CColPoint* colpoint) {
    if (!entity->m_matrix) {
        entity->AllocateMatrix();
        entity->m_placement.UpdateMatrix(entity->m_matrix);
    }
    CColModel* pColModel = CModelInfo::ms_modelInfoPtrs[m_nModelIndex]->m_pColModel;
    int totalColPointsToProcess = CCollision::ProcessColModels(*m_matrix, *pColModel, *entity->m_matrix, *entity->GetColModel(), colpoint, nullptr, nullptr, false);
    if (totalColPointsToProcess > 0) {
        AddCollisionRecord(entity);
        if (entity->m_nType != ENTITY_TYPE_BUILDING)
            entity->AddCollisionRecord(this);
        if (entity->m_nType == ENTITY_TYPE_BUILDING || entity->m_bIsStatic || entity->m_bIsStaticWaitingForCollision)
            m_bHasHitWall = true;
    }
    return totalColPointsToProcess;
}

void CPhysical::ProcessShift_Reversed()
{
    CRect boundingBox;
    GetBoundRect(&boundingBox);
    m_fMovingSpeed = 0.0f;

    bool bPhysicalFlagsSet = m_nPhysicalFlags & (PHYSICAL_DISABLE_MOVE_FORCE | PHYSICAL_INFINITE_MASS | PHYSICAL_DISABLE_Z);
    if (m_nStatus == STATUS_SIMPLE || bPhysicalFlagsSet)
    {
        if (bPhysicalFlagsSet)
        {
            m_vecTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
        }
        m_bIsStuck = false;
        m_bIsInSafePosition = true;
        RemoveAndAdd();
    }
    else
    {
        if (m_bHasHitWall)
        {
            CPed* pPed = static_cast<CPed*>(this);
            bool bSomeSpecificFlagsSet = false;
            if (m_nType == ENTITY_TYPE_PED && pPed->m_standingOnEntity)
            {
                if (!pPed->m_standingOnEntity->m_bIsStatic || pPed->m_standingOnEntity->m_bHasContacted)
                {
                    bSomeSpecificFlagsSet = true;
                }
            }
            if ((m_nType == ENTITY_TYPE_PED && bSomeSpecificFlagsSet) || CWorld::bSecondShift)
            {
                float fMoveSpeedShift = pow(CPhysical::PHYSICAL_SHIFT_SPEED_DAMP, CTimer::ms_fTimeStep);
                m_vecMoveSpeed *= fMoveSpeedShift;
                float fTurnSpeedShift = pow(CPhysical::PHYSICAL_SHIFT_SPEED_DAMP, CTimer::ms_fTimeStep);
                m_vecTurnSpeed *= fTurnSpeedShift;
            }
        }

        CMatrix oldEntityMatrix(*m_matrix);
        ApplySpeed();
        m_matrix->Reorthogonalise();
        SetNextScanCode();

        bool bShifted = false;
        if (m_nType == ENTITY_TYPE_VEHICLE)
        {
            physicalFlags.bProcessingShift = true;
        }

        std::int32_t startSectorX = CWorld::GetSectorX(boundingBox.left);
        std::int32_t startSectorY = CWorld::GetSectorY(boundingBox.top);
        std::int32_t endSectorX = CWorld::GetSectorX(boundingBox.right);
        std::int32_t endSectorY = CWorld::GetSectorY(boundingBox.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                if (ProcessShiftSectorList(sectorX, sectorY))
                    bShifted = true;
            }
        }
        physicalFlags.bProcessingShift = false;

        if (bShifted || m_nType == ENTITY_TYPE_VEHICLE) {
            CWorld::IncrementCurrentScanCode();
            bool bShifted2 = false;
            std::int32_t startSectorX = CWorld::GetSectorX(boundingBox.left);
            std::int32_t startSectorY = CWorld::GetSectorY(boundingBox.top);
            std::int32_t endSectorX = CWorld::GetSectorX(boundingBox.right);
            std::int32_t endSectorY = CWorld::GetSectorY(boundingBox.bottom);
            for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
                for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                    if (ProcessCollisionSectorList(sectorX, sectorY)) {
                        if (!CWorld::bSecondShift) {
                            *(CMatrix*)m_matrix = oldEntityMatrix;
                            return;
                        }
                        bShifted2 = true;
                    }
                }
            } 
            if (bShifted2)
            {
                *(CMatrix*)m_matrix = oldEntityMatrix;
                return;
            }
        }
        m_bIsStuck = false;
        m_bIsInSafePosition = true;
        m_fMovingSpeed = (GetPosition() - oldEntityMatrix.GetPosition()).Magnitude();
        RemoveAndAdd();
    }
}

// Converted from thiscall void CPhysical::RemoveAndAdd(void) 0x542560
void CPhysical::RemoveAndAdd()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x542560)(this);
#else
    if (m_bIsBIGBuilding)
    {
        CEntity::Remove();
        CEntity::Add();
    }
    else
    {
        CEntryInfoNode* pEntryInfoNode = m_pCollisionList;
        CRect boundRect;
        GetBoundRect(&boundRect);
        std::int32_t startSectorX = CWorld::GetSectorX(boundRect.left);
        std::int32_t startSectorY = CWorld::GetSectorY(boundRect.top);
        std::int32_t endSectorX = CWorld::GetSectorX(boundRect.right);
        std::int32_t endSectorY = CWorld::GetSectorY(boundRect.bottom);
        for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
            for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
                CPtrListDoubleLink* pDoubleLinkList = nullptr;
                CRepeatSector* pRepeatSector = GetRepeatSector(sectorX, sectorY);
                switch (m_nType)
                {
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

                if (pEntryInfoNode)
                {
                    CPtrNodeDoubleLink* pDoubleLink = pEntryInfoNode->m_pDoubleLink;
                    if (pEntryInfoNode->m_pDoubleLinkList->pNode == (CPtrNode*)pDoubleLink)
                        pEntryInfoNode->m_pDoubleLinkList->pNode = (CPtrNode*)pDoubleLink->pNext;
                    CPtrNodeDoubleLink* pDoubleLinkPrevious = pDoubleLink->pPrev;
                    if (pDoubleLinkPrevious)
                        pDoubleLinkPrevious->pNext = pDoubleLink->pNext;
                    CPtrNodeDoubleLink* pDoubleLinkNext = pDoubleLink->pNext;
                    if (pDoubleLinkNext)
                        pDoubleLinkNext->pPrev = pDoubleLink->pPrev;
                    pDoubleLink->pPrev = nullptr;
                    pDoubleLink->pNext = (CPtrNodeDoubleLink*)pDoubleLinkList->pNode;
                    if (pDoubleLink->pNext)
                        pDoubleLink->pNext->pPrev = pDoubleLink;
                    pDoubleLinkList->pNode = (CPtrNode*)pDoubleLink;
                    pEntryInfoNode->m_pRepeatSector = pRepeatSector;
                    pEntryInfoNode->m_pDoubleLinkList = pDoubleLinkList;
                    pEntryInfoNode = pEntryInfoNode->m_pNext;
                }
                else
                {
                    auto pNewDoubleLink = CPools::ms_pPtrNodeDoubleLinkPool->New();
                    if (pNewDoubleLink)
                    {
                        pNewDoubleLink->pItem = this;
                    }

                    pNewDoubleLink->pPrev = nullptr;
                    pNewDoubleLink->pNext = (CPtrNodeDoubleLink*)pDoubleLinkList->pNode;
                    if (pNewDoubleLink->pNext)
                        pNewDoubleLink->pNext->pPrev = pNewDoubleLink;
                    pDoubleLinkList->pNode = (CPtrNode*)pNewDoubleLink;
                    auto pNewEntryInfoNode = CPools::ms_pEntryInfoNodePool->New();
                    if (pNewEntryInfoNode)
                    {
                        pNewEntryInfoNode->m_pRepeatSector = pRepeatSector;
                        pNewEntryInfoNode->m_pDoubleLinkList = pDoubleLinkList;
                        pNewEntryInfoNode->m_pDoubleLink = pNewDoubleLink;
                    }
                    pNewEntryInfoNode->m_pPrevious = nullptr;
                    pNewEntryInfoNode->m_pNext = m_pCollisionList;
                    CEntryInfoNode* pCurrentEntryInfoNode = m_pCollisionList;
                    if (pCurrentEntryInfoNode)
                        pCurrentEntryInfoNode->m_pPrevious = pNewEntryInfoNode;
                    m_pCollisionList = pNewEntryInfoNode;
                }
            }
        }

        while (pEntryInfoNode)
        {
            CPtrNodeDoubleLink* pDoubleLink = pEntryInfoNode->m_pDoubleLink;
            if (pEntryInfoNode->m_pDoubleLinkList->pNode == (CPtrNode*)pDoubleLink)
                pEntryInfoNode->m_pDoubleLinkList->pNode = (CPtrNode*)pDoubleLink->pNext;
            CPtrNodeDoubleLink* pDoubleLinkPrevious = pDoubleLink->pPrev;
            if (pDoubleLinkPrevious)
                pDoubleLinkPrevious->pNext = pDoubleLink->pNext;
            CPtrNodeDoubleLink* pDoubleLinkNext = pDoubleLink->pNext;
            if (pDoubleLinkNext)
                pDoubleLinkNext->pPrev = pDoubleLink->pPrev;

            CPools::ms_pPtrNodeDoubleLinkPool->Delete(pDoubleLink);

            if (m_pCollisionList == pEntryInfoNode)
                m_pCollisionList = pEntryInfoNode->m_pNext;
            if (pEntryInfoNode->m_pPrevious)
                pEntryInfoNode->m_pPrevious->m_pNext = pEntryInfoNode->m_pNext;
            if (pEntryInfoNode->m_pNext)
                pEntryInfoNode->m_pNext->m_pPrevious = pEntryInfoNode->m_pPrevious;

            CEntryInfoNode* pNextEntryInfoNode = pEntryInfoNode->m_pNext;
            CPools::ms_pEntryInfoNodePool->Delete(pEntryInfoNode);
            pEntryInfoNode = pNextEntryInfoNode;
        }
    }
#endif
}

// Converted from thiscall void CPhysical::AddToMovingList(void) 0x542800
void CPhysical::AddToMovingList()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x542800)(this);
#else
    if (!m_pMovingList && !m_bIsStaticWaitingForCollision)
    {
        auto pNewDoubleLink = CPools::ms_pPtrNodeDoubleLinkPool->New();
        if (pNewDoubleLink)
            pNewDoubleLink->pItem = this;

        auto pNodeDoubleLink = (CPtrNodeDoubleLink*)CWorld::ms_listMovingEntityPtrs.pNode;
        m_pMovingList = pNewDoubleLink;
        pNewDoubleLink->pPrev = nullptr;
        pNewDoubleLink->pNext = pNodeDoubleLink;
        if (pNodeDoubleLink)
        {
            pNodeDoubleLink->pPrev = pNewDoubleLink;
        }
        CWorld::ms_listMovingEntityPtrs.pNode = (CPtrNode*)pNewDoubleLink;
    }
#endif
}

// Converted from thiscall void CPhysical::RemoveFromMovingList(void) 0x542860
void CPhysical::RemoveFromMovingList()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x542860)(this);
#else
   auto pMovingList = m_pMovingList;
    if (pMovingList)
    {
        if (CWorld::ms_listMovingEntityPtrs.pNode == (CPtrNode*)pMovingList)
            CWorld::ms_listMovingEntityPtrs.pNode = (CPtrNode*)pMovingList->pNext;
        CPtrNodeDoubleLink* pPrevious = pMovingList->pPrev;
        if (pPrevious)
            pPrevious->pNext = pMovingList->pNext;
        CPtrNodeDoubleLink* pNext = pMovingList->pNext;
        if (pNext)
            pNext->pPrev = pMovingList->pPrev;
        CPools::ms_pPtrNodeDoubleLinkPool->Delete(m_pMovingList);
        m_pMovingList = nullptr;
    }
#endif
}

// Converted from thiscall void CPhysical::SetDamagedPieceRecord(float damageIntensity,CEntity *damagingEntity,CColPoint &colPoint,float distanceMult) 0x5428C0
void CPhysical::SetDamagedPieceRecord(float fDamageIntensity, CEntity* entity, CColPoint* colPoint, float fDistanceMult)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, float, CEntity*, CColPoint*, float))0x5428C0)(this,fDamageIntensity, entity, colPoint, fDistanceMult);
#else
    CObject* pObject = static_cast<CObject*>(this);
    if (fDamageIntensity > m_fDamageIntensity)
    {
        m_fDamageIntensity = fDamageIntensity;
        m_nPieceType = colPoint->m_nPieceTypeA;
        if (m_pDamageEntity)
            m_pDamageEntity->CleanUpOldReference(&m_pDamageEntity);
        m_pDamageEntity = entity;
        entity->RegisterReference(&m_pDamageEntity);
        m_vecLastCollisionPosn = colPoint->m_vecPoint;
        m_vecLastCollisionImpactVelocity = fDistanceMult * colPoint->m_vecNormal;
        if (m_nType != ENTITY_TYPE_OBJECT || colPoint->m_nSurfaceTypeB != SURFACE_CAR_MOVINGCOMPONENT)
        {
            if (m_nType == ENTITY_TYPE_OBJECT && colPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
            {
            
                pObject->objectFlags.b20 = 1;
            }
        }
        else
        {
            pObject->objectFlags.b20 = 1;
        }
    }

    if (physicalFlags.bDisableZ)
    {
        if (entity->m_nModelIndex == MI_POOL_CUE_BALL && m_nType == ENTITY_TYPE_OBJECT)
        {
            pObject->m_nLastWeaponDamage = pObject->m_nLastWeaponDamage != -1 ? WEAPON_RUNOVERBYCAR : WEAPON_DROWNING;
        }
    }
#endif
}

void CPhysical::ApplyMoveForce(float x, float y, float z)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x4ABBA0, CPhysical*, float, float, float>(this, x, y, z);
#else
    return ApplyMoveForce(CVector(x, y ,z));
#endif
}

void CPhysical::ApplyMoveForce(CVector force)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CVector))0x5429F0)(this, force);
#else
    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce) {
        if (physicalFlags.bDisableZ)
            force.z = 0.0f;
        m_vecMoveSpeed += force / m_fMass;
    }
#endif
}

void CPhysical::ApplyTurnForce(CVector force, CVector point)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CVector, CVector))0x542A50)(this, force, point);
#else
    if (!physicalFlags.bDisableTurnForce)
    {
        CVector vecCentreOfMassMultiplied;
        if (!physicalFlags.bInfiniteMass)
            Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

        if (physicalFlags.bDisableMoveForce) {
            point.z = 0.0f;
            force.z = 0.0f;
        }
        CVector vecDifference = point - vecCentreOfMassMultiplied;
        m_vecTurnSpeed += CrossProduct(vecDifference, force) / m_fTurnMass;
    }
#endif
}

void CPhysical::ApplyForce(CVector vecForce, CVector point, bool bUpdateTurnSpeed)
{

#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CVector, CVector, bool))0x542B50)(this, vecForce, point, bUpdateTurnSpeed);
#else
    CVector vecMoveSpeedForce = vecForce;
    if (physicalFlags.bDisableZ)
        vecMoveSpeedForce.z = 0.0f;
    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce) 
        m_vecMoveSpeed += vecMoveSpeedForce / m_fMass;

    if (!physicalFlags.bDisableTurnForce && bUpdateTurnSpeed) {
        CVector vecCentreOfMassMultiplied;
        float fTurnMass = m_fTurnMass;
        if (physicalFlags.bInfiniteMass)
            fTurnMass += m_vecCentreOfMass.z * m_fMass * m_vecCentreOfMass.z * 0.5f;
        else
            Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

        if (physicalFlags.bDisableMoveForce) {
            point.z = 0.0f;
            vecForce.z = 0.0f;
        }

        CVector distance = point - vecCentreOfMassMultiplied;
        m_vecTurnSpeed += CrossProduct(distance, vecForce) / fTurnMass;
    }
#endif
}

CVector* CPhysical::GetSpeed(CVector* outSpeed, CVector point)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((CVector * (__thiscall*)(CPhysical*, CVector*, CVector))0x542CE0)(this, outSpeed, point);
#else
    CVector vecCentreOfMassMultiplied;
    if (!physicalFlags.bInfiniteMass)
        Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

    CVector vecDifference = point - vecCentreOfMassMultiplied;
    CVector vecTurnSpeed = m_vecTurnSpeed + m_vecFrictionTurnSpeed;
    CrossProduct(outSpeed, &vecTurnSpeed, &vecDifference);
    *outSpeed += m_vecMoveSpeed + m_vecFrictionMoveSpeed;
    return outSpeed;
#endif
}

/*
    The code for this function is fine, but it will crash if we hook it. This function should be
    only hooked after reversing all references to this function: 
    CPhysical::ApplySpeed
    CWorld::Process
    CAutoMobile::ProcessControlCollisionCheck
    CBike::ProcessControlCollisionCheck
    CTrain::ProcessControl (Done)
*/
void CPhysical::ApplyMoveSpeed()
{

//#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x542DD0)(this);
#if 0
    if (physicalFlags.bDontApplySpeed || physicalFlags.bDisableMoveForce)
        m_vecMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
  
    else
        GetPosition() += CTimer::ms_fTimeStep * m_vecMoveSpeed;
#endif
}

void CPhysical::ApplyTurnSpeed()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x542E20)(this);
#else
    if (physicalFlags.bDontApplySpeed) {
        m_vecTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
    }
    else
    {
        CVector vecTurnSpeedTimeStep = CTimer::ms_fTimeStep * m_vecTurnSpeed;
        CVector vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetRight());
        GetRight() += vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetForward());
        GetForward() += vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetUp());
        GetUp() += vecCrossProduct;
        if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce) {
            CVector vecNegativeCentreOfMass = m_vecCentreOfMass * -1.0f;
            CVector vecCentreOfMassMultiplied;
            Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &vecNegativeCentreOfMass);
            GetPosition() += CrossProduct(vecTurnSpeedTimeStep, vecCentreOfMassMultiplied);;
        }
    }
#endif
}

void CPhysical::ApplyGravity()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x542FE0)(this);
#else
    if (physicalFlags.bApplyGravity && !physicalFlags.bDisableMoveForce) {
        if (physicalFlags.bInfiniteMass) {
            float fMassTimeStep = CTimer::ms_fTimeStep * m_fMass;
            CVector point;
            Multiply3x3(&point, m_matrix, &m_vecCentreOfMass);
            CVector force (0.0f, 0.0f, fMassTimeStep * -0.008f);
            ApplyForce(force, point, true);
        }
        else if (m_bUsesCollision) {
            m_vecMoveSpeed.z -= CTimer::ms_fTimeStep * 0.008f;
        }
    }
#endif
}

// Converted from thiscall void CPhysical::ApplyFrictionMoveForce(CVector moveForce) 0x5430A0
void CPhysical::ApplyFrictionMoveForce(CVector moveForce)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CVector))0x5430A0)(this, moveForce);
#else
    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce)
    {
        if (physicalFlags.bDisableZ)
        {
            moveForce.z = 0.0f;
        }
        m_vecFrictionMoveSpeed += moveForce / m_fMass;
    }
#endif
}

// Unused
void CPhysical::ApplyFrictionTurnForce(CVector posn, CVector velocity)
{
    ((void(__thiscall*)(CPhysical*, CVector, CVector))0x543100)(this, posn, velocity);
}

void CPhysical::ApplyFrictionForce(CVector vecMoveForce, CVector point)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CVector, CVector))0x543220)(this, vecMoveForce, point);
#else
    CVector vecTheMoveForce = vecMoveForce;

    if (physicalFlags.bDisableZ)
    {
        vecTheMoveForce.z = 0.0f;
    }

    if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce)
    {
        m_vecFrictionMoveSpeed += vecTheMoveForce / m_fMass;
    }

    CVector vecCentreOfMassMultiplied;
    if (!physicalFlags.bDisableTurnForce)
    {
        float fTurnMass = m_fTurnMass;
        if (physicalFlags.bInfiniteMass)
        {
            fTurnMass += m_vecCentreOfMass.z * m_fMass * m_vecCentreOfMass.z * 0.5f;
        }
        else
        {
            Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
        }

        if (physicalFlags.bDisableMoveForce)
        {
            point.z = 0.0f;
            vecMoveForce.z = 0.0f;
        }

        CVector vecDifference = point - vecCentreOfMassMultiplied;
        CVector vecMoveForceCrossProduct;
        vecMoveForceCrossProduct.Cross(vecDifference, vecMoveForce);

        m_vecFrictionTurnSpeed += vecMoveForceCrossProduct / fTurnMass;
    }
#endif
}

void CPhysical::SkipPhysics()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x5433B0)(this);
#else
    if (m_nType != ENTITY_TYPE_PED && m_nType != ENTITY_TYPE_VEHICLE)
        physicalFlags.bSubmergedInWater = false;

    m_bHasHitWall = false;
    m_bWasPostponed = false;
    m_bIsInSafePosition = false;
    m_bHasContacted = false;

    if (m_nStatus != STATUS_SIMPLE)
    {
        physicalFlags.bOnSolidSurface = false;
        m_nNumEntitiesCollided = 0;
        m_nPieceType = 0;
        m_fDamageIntensity = 0.0f;
        if (m_pDamageEntity)
            m_pDamageEntity->CleanUpOldReference(&m_pDamageEntity);
        m_pDamageEntity = nullptr;
        m_vecFrictionTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
        m_vecFrictionMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
    }
#endif
}

void CPhysical::AddCollisionRecord(CEntity* collidedEntity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CEntity*))0x543490)(this, collidedEntity);
#else
    physicalFlags.bOnSolidSurface = true;
    m_nLastCollisionTime = CTimer::m_snTimeInMilliseconds;
    if (m_nType == ENTITY_TYPE_VEHICLE)
    {
        CVehicle* pVehicle = static_cast<CVehicle*>(this);
        if (collidedEntity->m_nType == ENTITY_TYPE_VEHICLE)
        {
            CVehicle* pCollidedVehicle = static_cast<CVehicle*>(collidedEntity);
            if (pVehicle->m_nAlarmState == -1)
                pVehicle->m_nAlarmState = 15000;
            if (pCollidedVehicle->m_nAlarmState == -1)
                pCollidedVehicle->m_nAlarmState = 15000;
        }
    }

    if (physicalFlags.bCanBeCollidedWith)
    {
        for (unsigned int i = 0; i < m_nNumEntitiesCollided; i++)
        {
            if (m_apCollidedEntities[i] == collidedEntity)
                return;
        }

        if (m_nNumEntitiesCollided < 6) {
            m_apCollidedEntities[m_nNumEntitiesCollided] = collidedEntity;
            m_nNumEntitiesCollided++;
        }
    }
#endif
}

bool CPhysical::GetHasCollidedWith(CEntity* entity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CEntity*))0x543540)(this, entity);
#else
    if (!physicalFlags.bCanBeCollidedWith || m_nNumEntitiesCollided <= 0)
        return false;

    for (unsigned int i = 0; i < m_nNumEntitiesCollided; i++)
    {
        if (m_apCollidedEntities[i] == entity)
            return true;
    }
    return false;
#endif
}

bool CPhysical::GetHasCollidedWithAnyObject()
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*))0x543580)(this);
#else
    if (!physicalFlags.bCanBeCollidedWith || m_nNumEntitiesCollided <= 0)
        return false;

    for (unsigned int i = 0; i < m_nNumEntitiesCollided; i++)
    {
        CEntity* pCollidedEntity = m_apCollidedEntities[i];
        if (pCollidedEntity && pCollidedEntity->m_nType == ENTITY_TYPE_OBJECT)
            return true;
    }
    return false;
#endif
}

bool CPhysical::ApplyCollision(CEntity* pEntity, CColPoint* pColPoint, float* pDamageIntensity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CEntity*, CColPoint*, float*))0x5435C0)(this, pEntity, pColPoint, pDamageIntensity);
#else
    if (physicalFlags.bDisableTurnForce)
    {
        float fSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &pColPoint->m_vecNormal);
        if (fSpeedDotProduct < 0.0f)
        {
            *pDamageIntensity = -(fSpeedDotProduct * m_fMass);
            CVector vecMoveSpeed = *pDamageIntensity * pColPoint->m_vecNormal;
            ApplyMoveForce(vecMoveSpeed.x, vecMoveSpeed.y, vecMoveSpeed.z);

            float fCollisionImpact1 = *pDamageIntensity / m_fMass;
            AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            return true;
        }
    }
    else
    {
        CVector vecDistanceToPoint = pColPoint->m_vecPoint - GetPosition();
        CVector vecSpeed;
        GetSpeed(&vecSpeed, vecDistanceToPoint);

        CVector vecMoveDirection = pColPoint->m_vecNormal;
        float fSpeedDotProduct = DotProduct(&vecMoveDirection, &vecSpeed);
        if (fSpeedDotProduct < 0.0f)
        {
            CVector vecCentreOfMassMultiplied;
            Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
            CVector vecDifference = vecDistanceToPoint - vecCentreOfMassMultiplied;
            CVector vecSpeedCrossProduct;
            vecSpeedCrossProduct.Cross(vecDifference, vecMoveDirection);
            float fSquaredMagnitude = vecMoveDirection.SquaredMagnitude();
            float fCollisionMass = 1.0f / (fSquaredMagnitude / m_fTurnMass + 1.0f / m_fMass);

            *pDamageIntensity = -((m_fElasticity + 1.0f) * fCollisionMass * fSpeedDotProduct);

            CVector vecMoveSpeed = vecMoveDirection * *pDamageIntensity;
            if (m_nType == ENTITY_TYPE_VEHICLE && vecMoveDirection.z < 0.7f)
            {
                vecMoveSpeed.z *= 0.3f;
            }

            if (!physicalFlags.bDisableCollisionForce)
            {
                bool bUpdateTurnSpeed = m_nType != ENTITY_TYPE_VEHICLE || !CWorld::bNoMoreCollisionTorque;
                ApplyForce(vecMoveSpeed, vecDistanceToPoint, bUpdateTurnSpeed);
            }

            float fCollisionImpact1 = *pDamageIntensity / fCollisionMass;
            AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            return true;
        }
    }
    return false;
#endif
}

bool CPhysical::ApplySoftCollision(CEntity* pEntity, CColPoint* pColPoint, float* pDamageIntensity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CEntity*, CColPoint*, float*))0x543890)(this, pEntity, pColPoint, pDamageIntensity);
#else
    if (physicalFlags.bDisableTurnForce)
    {
        ApplyCollision(pEntity, pColPoint, pDamageIntensity);
    }

    CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
    CVector vecSpeed;
    GetSpeed(&vecSpeed, vecDistanceToPointFromThis);

    CVector vecMoveDirection = pColPoint->m_vecNormal;

    float fSoftColSpeedMult = CPhysical::SOFTCOL_SPEED_MULT;

    CVehicle* pThisVehicle = static_cast<CVehicle*>(this);
    if (m_nType == ENTITY_TYPE_VEHICLE && pThisVehicle->m_nVehicleSubClass == VEHICLE_MTRUCK)
    {
        float fForwardsDotProduct = DotProduct(&vecMoveDirection, &GetUp());
        if (fForwardsDotProduct < -0.9f)
        {
            return false;
        }

        if (fForwardsDotProduct < 0.0f)
        {
            vecMoveDirection -= fForwardsDotProduct * GetUp();
            vecMoveDirection.Normalise();
        }
        else if (fForwardsDotProduct > 0.5f)
        {
            fSoftColSpeedMult = CPhysical::SOFTCOL_SPEED_MULT2;
        }
    }

    float fSpeedDotProduct = DotProduct(&vecSpeed, &vecMoveDirection);
    CVector vecCentreOfMassMultiplied;
    Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

    if (physicalFlags.bInfiniteMass)
    {
        vecCentreOfMassMultiplied = CVector(0.0f, 0.0f, 0.0f);
    }

    CVector vecDifference = vecDistanceToPointFromThis - vecCentreOfMassMultiplied;
    CVector vecSpeedCrossProduct;
    vecSpeedCrossProduct.Cross(vecDifference, vecMoveDirection);
    float fSquaredMagnitude = vecMoveDirection.SquaredMagnitude();
    float fCollisionMass = 1.0f / (fSquaredMagnitude / m_fTurnMass + 1.0f / m_fMass);

    if (m_nType != ENTITY_TYPE_VEHICLE || pThisVehicle->m_nVehicleSubClass
        || pColPoint->m_nPieceTypeA < 13u || pColPoint->m_nPieceTypeA > 16u)
    {
        float fDepth = CPhysical::SOFTCOL_DEPTH_MIN;
        if (CPhysical::SOFTCOL_DEPTH_MIN >= pColPoint->m_fDepth)
        {
            fDepth = pColPoint->m_fDepth;
        }

        *pDamageIntensity = fDepth * CTimer::ms_fTimeStep * CPhysical::SOFTCOL_DEPTH_MULT * fCollisionMass * 0.008f;
        if (fSpeedDotProduct >= 0.0f)
        {
            *pDamageIntensity = 0.0f;
            return false;
        }
        *pDamageIntensity = *pDamageIntensity - fCollisionMass * fSpeedDotProduct * fSoftColSpeedMult;
    }
    else
    {
        *pDamageIntensity = pColPoint->m_fDepth * CTimer::ms_fTimeStep * CPhysical::SOFTCOL_DEPTH_MULT * fCollisionMass * 0.008f;
        if (fSpeedDotProduct < 0.0f)
        {
            *pDamageIntensity = *pDamageIntensity - CPhysical::SOFTCOL_CARLINE_SPEED_MULT * fCollisionMass * fSpeedDotProduct;
        }

        float fRightDotProduct = DotProduct(&vecMoveDirection, &GetRight());
        vecMoveDirection -= 0.9f * fRightDotProduct * GetRight();
    }

    if (*pDamageIntensity == 0.0f)
    {
        return false;
    }

    ApplyForce(vecMoveDirection * *pDamageIntensity, vecDistanceToPointFromThis, true);
    if (*pDamageIntensity < 0.0f)
    {
        *pDamageIntensity *= -1.0f;
    }
    return true;
#endif
}

bool CPhysical::ApplySpringCollision(float fSuspensionForceLevel, CVector* direction, CVector* collisionPoint, float fSpringLength, float fSuspensionBias, float* fSpringForceDampingLimit) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, float, CVector*, CVector*, float, float, float*))0x543C90)(this, fSuspensionForceLevel, direction, collisionPoint, fSpringLength, fSuspensionBias, fSpringForceDampingLimit);
#else
    float fSpringStress = 1.0f - fSpringLength;
    if (fSpringStress <= 0.0f)
        return true;
    float fTimeStep = CTimer::ms_fTimeStep;
    if (CTimer::ms_fTimeStep >= 3.0f)
        fTimeStep = 3.0f;
    *fSpringForceDampingLimit = fSpringStress * m_fMass * fSuspensionForceLevel * 0.016f * fTimeStep * fSuspensionBias;
    ApplyForce((-1.0f * *fSpringForceDampingLimit) * *direction, *collisionPoint, true);
    return true;
#endif
}

bool CPhysical::ApplySpringCollisionAlt(float fSuspensionForceLevel, CVector* direction, CVector* collisionPoint, float fSpringLength, float fSuspensionBias, CVector* collisionPointDirection, float* fSpringForceDampingLimit) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, float, CVector*, CVector*, float, float, CVector*, float*))0x543D60)(this, fSuspensionForceLevel, direction, collisionPoint, fSpringLength, fSuspensionBias, collisionPointDirection, fSpringForceDampingLimit);
#else
    float fSpringStress = 1.0f - fSpringLength;
    if (fSpringStress <= 0.0f)
        return true;
    if (DotProduct(direction, collisionPointDirection) > 0.0f)
        *collisionPointDirection *= -1.0f;
    float fTimeStep = CTimer::ms_fTimeStep;
    if (CTimer::ms_fTimeStep >= 3.0f)
        fTimeStep = 3.0f;
    *fSpringForceDampingLimit = fSpringStress * (fTimeStep * m_fMass) * fSuspensionForceLevel * fSuspensionBias * 0.016f;
    if (physicalFlags.bMakeMassTwiceAsBig)
        *fSpringForceDampingLimit = *fSpringForceDampingLimit * 0.75f;
    ApplyForce(*fSpringForceDampingLimit * *collisionPointDirection, *collisionPoint, true);
    return true;
#endif
}

bool CPhysical::ApplySpringDampening(float fDampingForce, float fSpringForceDampingLimit, CVector* direction, CVector* collisionPoint, CVector* collisionPos) {
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, float, float, CVector*, CVector*, CVector*))0x543E90)(this, fDampingForce, fSpringForceDampingLimit, direction, collisionPoint, collisionPos);
#else
    float fCollisionPosDotProduct = DotProduct(collisionPos, direction);
    CVector vecCollisionPointSpeed;
    GetSpeed(&vecCollisionPointSpeed, *collisionPoint);
    float fCollisionPointSpeedDotProduct = DotProduct(&vecCollisionPointSpeed, direction);
    float fTimeStep = CTimer::ms_fTimeStep;
    if (CTimer::ms_fTimeStep >= 3.0f)
        fTimeStep = 3.0f;
    float fDampingForceTimeStep = fTimeStep * fDampingForce;
    if (physicalFlags.bMakeMassTwiceAsBig)
        fDampingForceTimeStep = fDampingForceTimeStep + fDampingForceTimeStep;
    if (fDampingForceTimeStep <= DAMPING_LIMIT_IN_FRAME) {
        float fNegativeDampingLimitInFrame = -DAMPING_LIMIT_IN_FRAME;
        if (fDampingForceTimeStep < fNegativeDampingLimitInFrame)
            fDampingForceTimeStep = fNegativeDampingLimitInFrame;
    }
    else {
        fDampingForceTimeStep = DAMPING_LIMIT_IN_FRAME;
    }

    float fDampingSpeed = -(fDampingForceTimeStep * fCollisionPosDotProduct);
    if (fDampingSpeed > 0.0 && fDampingSpeed + fCollisionPointSpeedDotProduct > 0.0f) {
        if (fCollisionPointSpeedDotProduct >= 0.0f)
            fDampingSpeed = 0.0f;
        else
            fDampingSpeed = -fCollisionPointSpeedDotProduct;
    }
    else if (fDampingSpeed < 0.0f && fDampingSpeed + fCollisionPointSpeedDotProduct < 0.0f) {
        if (fCollisionPointSpeedDotProduct <= 0.0f)
            fDampingSpeed = 0.0f;
        else
            fDampingSpeed = -fCollisionPointSpeedDotProduct;
    }

    CVector vecCentreOfMassMultiplied;
    Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
    CVector vecDifference = *collisionPoint - vecCentreOfMassMultiplied;
    CVector vecCrossProduct;
    CrossProduct(&vecCrossProduct, &vecDifference, direction);
    float fSpringForceDamping = 1.0f / (vecCrossProduct.SquaredMagnitude() / m_fTurnMass + 1.0f / m_fMass) * fDampingSpeed;
    fSpringForceDampingLimit = fabs(fSpringForceDampingLimit) * DAMPING_LIMIT_OF_SPRING_FORCE;
    if (fSpringForceDamping > fSpringForceDampingLimit)
        fSpringForceDamping = fSpringForceDampingLimit;
    ApplyForce(fSpringForceDamping * *direction, *collisionPoint, true);
    return true;
#endif
}

// Unused
bool CPhysical::ApplySpringDampeningOld(float arg0, float arg1, CVector& arg2, CVector& arg3, CVector& arg4)
{
    return ((bool(__thiscall*)(CPhysical*, float, float, CVector&, CVector&, CVector&))0x544100)(this, arg0, arg1, arg2, arg3, arg4);
}

void CPhysical::RemoveRefsToEntity(CEntity* entity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CEntity*))0x544280)(this, entity);
#else
    unsigned char collidedEntityIndex = m_nNumEntitiesCollided;
    while (collidedEntityIndex < m_nNumEntitiesCollided); {
        if (m_apCollidedEntities[collidedEntityIndex] == entity) {
            for (unsigned short i = collidedEntityIndex; i < m_nNumEntitiesCollided - 1; ++i) {
                m_apCollidedEntities[i] = m_apCollidedEntities[i + 1];
            }
            m_nNumEntitiesCollided--;
        }
        else {
            collidedEntityIndex++;
        }
    }
#endif
}

void CPhysical::DettachEntityFromEntity(float x, float y, float z, bool bApplyTurnForce)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, float, float, float, bool))0x5442F0)(this, x, y, z, bApplyTurnForce);
#else
    CMatrix vecDetachOffsetMatrix;
    if (m_nType == ENTITY_TYPE_VEHICLE && m_pAttachedTo && m_pAttachedTo->m_nType == ENTITY_TYPE_VEHICLE) {
        CColModel* pColModel = CEntity::GetColModel();
        CCollisionData* pColData = pColModel->m_pColData;
        if (pColData) {
            unsigned char nNumLines = pColData->m_nNumLines;
            pColData->m_nNumLines = 0;
            CColModel* pAttachedToColModel = m_pAttachedTo->GetColModel();
            CMatrix* pAttachedToMatrix = m_pAttachedTo->GetMatrix();
            if (CCollision::ProcessColModels(*m_matrix, *pColModel, *pAttachedToMatrix, *pAttachedToColModel, CWorld::m_aTempColPts, nullptr, nullptr, false))
                m_pEntityIgnoredCollision = m_pAttachedTo;
            else if (m_pEntityIgnoredCollision == m_pAttachedTo)
                m_pEntityIgnoredCollision = 0;
            pColData->m_nNumLines = nNumLines;
        }
        else if (m_pEntityIgnoredCollision == m_pAttachedTo) {
            m_pEntityIgnoredCollision = 0;
        }
    }
    else {
        m_pEntityIgnoredCollision = m_pAttachedTo;
    }

    vecDetachOffsetMatrix.ResetOrientation();
    vecDetachOffsetMatrix.RotateZ(y);
    vecDetachOffsetMatrix.RotateX(x);
    if (!m_pAttachedTo->m_matrix) {
        m_pAttachedTo->AllocateMatrix();
        m_pAttachedTo->m_placement.UpdateMatrix(m_pAttachedTo->m_matrix);
    }

    vecDetachOffsetMatrix *= *m_pAttachedTo->m_matrix;
    CVector vecForce = vecDetachOffsetMatrix.GetForward() * z;
    CWorld::Remove(this);
    SetIsStatic(false);
    physicalFlags.bAttachedToEntity = false;
    CWorld::Add(this);
    if (physicalFlags.bDisableCollisionForce) {
        CObject* pThisObject = static_cast<CObject*>(this);
        CObjectInfo* pThisObjectInfo = pThisObject->m_pObjectInfo;
        if (m_nType != ENTITY_TYPE_OBJECT || pThisObjectInfo->m_fMass >= 99998.0f)
        {
            physicalFlags.bCollidable = true;
            m_vecTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
            m_vecMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
            bApplyTurnForce = false;
        }
        else
        {
            physicalFlags.bDisableCollisionForce = false;
            m_fMass = pThisObjectInfo->m_fMass;
            m_fTurnMass = pThisObjectInfo->m_fTurnMass;
        }
    }

    if (!physicalFlags.bDisableCollisionForce) {
        if (m_pAttachedTo->m_nType > ENTITY_TYPE_BUILDING && m_pAttachedTo->m_nType < ENTITY_TYPE_DUMMY) {
            m_vecMoveSpeed = m_pAttachedTo->m_vecMoveSpeed;
            m_vecMoveSpeed += vecForce;
        }
    }

    if (bApplyTurnForce)
        ApplyTurnForce(vecForce, vecForce * 0.5f);

    m_pAttachedTo = nullptr;
    m_qAttachedEntityRotation.real = 0.0;
    m_qAttachedEntityRotation.imag = CVector(0.0f, 0.0f, 0.0f);
    m_vecAttachOffset = CVector(0.0f, 0.0f, 0.0f);
#endif
}

void CPhysical::DettachAutoAttachedEntity()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x5446A0)(this);
#else
    SetIsStatic(false);
    physicalFlags.bAttachedToEntity = false;
    m_bFakePhysics = false;
    if (!physicalFlags.bDisableCollisionForce) {
        if (m_pAttachedTo->m_nType == ENTITY_TYPE_VEHICLE) {
            m_vecMoveSpeed = m_pAttachedTo->m_vecMoveSpeed;
            m_vecTurnSpeed = m_pAttachedTo->m_vecTurnSpeed;
        }
    }
    else
    {
        physicalFlags.bCollidable = true;
        m_vecTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
        m_vecMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
    }
    m_vecAttachOffset = CVector(0.0f, 0.0f, 0.0f);
    m_pEntityIgnoredCollision = nullptr;
    m_pAttachedTo = nullptr;
    m_qAttachedEntityRotation.real = 0.0f;
    m_qAttachedEntityRotation.imag = CVector(0.0f, 0.0f, 0.0f);
    if (m_nType == ENTITY_TYPE_OBJECT) {
        CObject* pObject = static_cast<CObject*>(this);
        m_fElasticity = pObject->m_pObjectInfo->m_fElasticity;
    }
#endif
}

float CPhysical::GetLightingFromCol(bool bInteriorLighting)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((float(__thiscall*)(CPhysical*, bool))0x5447B0)(this, bInteriorLighting);
#else
    float fAmbientRedBlue = CTimeCycle::GetAmbientRed_BeforeBrightness() + CTimeCycle::GetAmbientBlue_BeforeBrightness();
    float fLighting = (CTimeCycle::GetAmbientGreen_BeforeBrightness() + fAmbientRedBlue) * 0.33333f + m_fContactSurfaceBrightness;
    if (!bInteriorLighting) {
        fLighting *= ((CTimeCycle::m_fCurrentRGB1Blue + CTimeCycle::m_fCurrentRGB1Green + CTimeCycle::m_fCurrentRGB1Red)
            * (1.0f / 765.0f) * TEST_ADD_AMBIENT_LIGHT_FRAC + 1.0f - TEST_ADD_AMBIENT_LIGHT_FRAC)
        + (CTimeCycle::m_fCurrentRGB2Blue + CTimeCycle::m_fCurrentRGB2Green + CTimeCycle::m_fCurrentRGB2Red)
        * (1.0f / 765.0f)  * TEST_ADD_AMBIENT_LIGHT_FRAC;
    }
    return fLighting;
#endif
}

float CPhysical::GetLightingTotal()
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((float(__thiscall*)(CPhysical*))0x544850)(this);
#else
    bool bInteriorLighting = false;
    if (m_nAreaCode)
        bInteriorLighting = true;

    if (m_nType  == ENTITY_TYPE_PED) {
        CPed* pPed = static_cast<CPed*>(this);
        if (pPed->m_pPlayerData && (CGame::currArea || pPed->m_pPlayerData->m_bForceInteriorLighting))
            bInteriorLighting = true;
    }
    return GetLightingFromCol(bInteriorLighting) + m_fDynamicLighting;
#endif
}

bool CPhysical::CanPhysicalBeDamaged(eWeaponType weapon, bool* bDamagedDueToFireOrExplosionOrBullet)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, eWeaponType, bool*))0x5448B0)(this, weapon, bDamagedDueToFireOrExplosionOrBullet);
#else
    if (bDamagedDueToFireOrExplosionOrBullet)
        *bDamagedDueToFireOrExplosionOrBullet = 0;
    switch (weapon)
    {
    case WEAPON_UNARMED:
    case WEAPON_BRASSKNUCKLE:
    case WEAPON_GOLFCLUB:
    case WEAPON_NIGHTSTICK:
    case WEAPON_KNIFE:
    case WEAPON_BASEBALLBAT:
    case WEAPON_SHOVEL:
    case WEAPON_POOL_CUE:
    case WEAPON_KATANA:
    case WEAPON_CHAINSAW:
    case WEAPON_DILDO1:
    case WEAPON_DILDO2:
    case WEAPON_VIBE1:
    case WEAPON_VIBE2:
    case WEAPON_FLOWERS:
    case WEAPON_CANE:
        if (!physicalFlags.bMeeleProof)
            return true;
        return false;
    case WEAPON_GRENADE:
    case WEAPON_ROCKET:
    case WEAPON_ROCKET_HS:
    case WEAPON_FREEFALL_BOMB:
    case WEAPON_RLAUNCHER:
    case WEAPON_RLAUNCHER_HS:
    case WEAPON_REMOTE_SATCHEL_CHARGE:
    case WEAPON_DETONATOR:
    case WEAPON_ARMOUR | WEAPON_NIGHTSTICK:
        if (!physicalFlags.bExplosionProof)
            break;
        return false;
    case WEAPON_TEARGAS:
    case WEAPON_SPRAYCAN:
    case WEAPON_EXTINGUISHER:
    case WEAPON_CAMERA:
    case WEAPON_NIGHTVISION:
    case WEAPON_INFRARED:
    case WEAPON_PARACHUTE:
    case WEAPON_LAST_WEAPON:
    case WEAPON_ARMOUR:
    case WEAPON_ARMOUR | WEAPON_BASEBALLBAT:
        return true;
    case WEAPON_MOLOTOV:
        if (!physicalFlags.bFireProof)
            break;
        return false;
    case WEAPON_PISTOL:
    case WEAPON_PISTOL_SILENCED:
    case WEAPON_DESERT_EAGLE:
    case WEAPON_SHOTGUN:
    case WEAPON_SAWNOFF_SHOTGUN:
    case WEAPON_SPAS12_SHOTGUN:
    case WEAPON_MICRO_UZI:
    case WEAPON_MP5:
    case WEAPON_AK47:
    case WEAPON_M4:
    case WEAPON_TEC9:
    case WEAPON_COUNTRYRIFLE:
    case WEAPON_SNIPERRIFLE:
    case WEAPON_MINIGUN:
    case WEAPON_ARMOUR | WEAPON_KNIFE:
        if (physicalFlags.bBulletProof)
            return false;
        break;
    case WEAPON_FLAMETHROWER:
        if (!physicalFlags.bFireProof)
            return true;
        return false;
    case WEAPON_ARMOUR | WEAPON_BRASSKNUCKLE:
    case WEAPON_ARMOUR | WEAPON_GOLFCLUB:
    case WEAPON_ARMOUR | WEAPON_SHOVEL:
        if (physicalFlags.bCollisionProof)
            return false;
        return true;
    }

    if (!bDamagedDueToFireOrExplosionOrBullet)
        return true;
    *bDamagedDueToFireOrExplosionOrBullet = true;
    return true;
#endif
}

void CPhysical::ApplyAirResistance()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x544C40)(this);
#else
    if (m_fAirResistance <= 0.1f || m_nType  == ENTITY_TYPE_VEHICLE)
    {
        float fSpeedMagnitude = m_vecMoveSpeed.Magnitude() * m_fAirResistance;
        if (CCullZones::DoExtraAirResistanceForPlayer())
        {
            if (m_nType == ENTITY_TYPE_VEHICLE)
            {
                CVehicle* pVehicle = static_cast<CVehicle*>(this);
                if (!pVehicle->m_nVehicleSubClass || pVehicle->m_nVehicleSubClass == VEHICLE_BIKE)
                    fSpeedMagnitude = CVehicle::m_fAirResistanceMult * fSpeedMagnitude;
            }
        }

        m_vecMoveSpeed *= pow(1.0f - fSpeedMagnitude, CTimer::ms_fTimeStep);
        m_vecTurnSpeed *= 0.99f;
    }
    else
    {
        float fAirResistanceTimeStep = pow(m_fAirResistance, CTimer::ms_fTimeStep);
        m_vecMoveSpeed *= fAirResistanceTimeStep;
        m_vecTurnSpeed *= fAirResistanceTimeStep;
    }
#endif
}

bool CPhysical::ApplyCollisionAlt(CPhysical* pEntity, CColPoint* pColPoint, float* pDamageIntensity, CVector* pVecMoveSpeed, CVector* pVecTurnSpeed)
{

#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CPhysical*, CColPoint*, float*, CVector*, CVector*))0x544D50)(this, pEntity, pColPoint, pDamageIntensity, pVecMoveSpeed, pVecTurnSpeed);
#else
    if (m_pAttachedTo)
    {
        if (m_pAttachedTo->m_nType > ENTITY_TYPE_BUILDING && m_pAttachedTo->m_nType < ENTITY_TYPE_DUMMY
            && m_pAttachedTo->m_nType != ENTITY_TYPE_PED)
        {
            float fDamageIntensity = 0.0f;
            m_pAttachedTo->ApplySoftCollision(pEntity, pColPoint, &fDamageIntensity);
        }
    }

    if (physicalFlags.bDisableTurnForce)
    {
        float fSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &pColPoint->m_vecNormal);
        if (fSpeedDotProduct < 0.0f)
        {
            *pDamageIntensity = -(fSpeedDotProduct * m_fMass);
            ApplyMoveForce(pColPoint->m_vecNormal * *pDamageIntensity);

            float fCollisionImpact1 = *pDamageIntensity / m_fMass;
            AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            return true;
        }
        return false;
    }

    CVehicle* pVehicle = static_cast<CVehicle*>(this);
    CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
    CVector vecSpeed;
    GetSpeed(&vecSpeed, vecDistanceToPointFromThis);

    if (physicalFlags.bAddMovingCollisionSpeed && m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
    {
        CVector outSpeed;
        pVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPointFromThis);
        vecSpeed += outSpeed;
    }

    CVector vecMoveDirection = pColPoint->m_vecNormal;
    float fSpeedDotProduct = DotProduct(&vecMoveDirection, &vecSpeed);
    if (fSpeedDotProduct >= 0.0f)
    {
        return false;
    }

    CVector vecCentreOfMassMultiplied;
    Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
    if (physicalFlags.bInfiniteMass)
    {
        vecCentreOfMassMultiplied = CVector(0.0f, 0.0f, 0.0f);
    }

    CVector vecDifference = vecDistanceToPointFromThis - vecCentreOfMassMultiplied;
    CVector vecCrossProduct;
    vecCrossProduct.Cross(vecDifference, vecMoveDirection);
    float fSquaredMagnitude = vecCrossProduct.SquaredMagnitude();
    float fCollisionMass = 1.0f / (fSquaredMagnitude / m_fTurnMass + 1.0f / m_fMass);

    unsigned short entityAltCol = ALT_ENITY_COL_DEFAULT;
    float fMoveSpeedLimit = CTimer::ms_fTimeStep * 0.008f;
    if (m_nType == ENTITY_TYPE_OBJECT)
    {
        entityAltCol = ALT_ENITY_COL_OBJECT;
        fMoveSpeedLimit *= 1.3f;
    }
    else
    {
        if (m_nType == ENTITY_TYPE_VEHICLE)
        {
            if (!physicalFlags.bSubmergedInWater)
            {
                float fMoveSpeedLimitMultiplier = 0.0f;
                unsigned int vehicleClass = pVehicle->m_nVehicleClass;
                if (vehicleClass != VEHICLE_BIKE || (m_nStatus != STATUS_ABANDONED) && m_nStatus != STATUS_WRECKED)
                {
                    if (vehicleClass == VEHICLE_BOAT)
                    {
                        fMoveSpeedLimitMultiplier = 1.5f;
                        entityAltCol = ALT_ENITY_COL_BOAT;
                    }
                    else
                    {
                        if (GetUp().z < -0.3f)
                        {
                            fMoveSpeedLimitMultiplier = 1.4f;
                            entityAltCol = ALT_ENITY_COL_VEHICLE;
                        }
                    }
                }
                else
                {
                    fMoveSpeedLimitMultiplier = 1.7f;
                    entityAltCol = ALT_ENITY_COL_BIKE_WRECKED;
                }
                fMoveSpeedLimit *= fMoveSpeedLimitMultiplier;
            }
        }
    }

    float fCollisionImpact2 = 1.0f;
    bool bUseElasticity = false;
    if (entityAltCol == ALT_ENITY_COL_OBJECT)
    {
        if (!m_bHasContacted
            && fabs(m_vecMoveSpeed.x) < fMoveSpeedLimit
            && fabs(m_vecMoveSpeed.y) < fMoveSpeedLimit
            && fMoveSpeedLimit + fMoveSpeedLimit > fabs(m_vecMoveSpeed.z))
        {
            fCollisionImpact2 = 0.0f;
            *pDamageIntensity = -0.98f * fCollisionMass * fSpeedDotProduct;
        }
        bUseElasticity = true;
    }
    else
    {
        if (entityAltCol != ALT_ENITY_COL_BIKE_WRECKED)
        {
            if (entityAltCol == ALT_ENITY_COL_VEHICLE)
            {
                if (fabs(m_vecMoveSpeed.x) < fMoveSpeedLimit
                    && fabs(m_vecMoveSpeed.y) < fMoveSpeedLimit
                    && fMoveSpeedLimit + fMoveSpeedLimit > fabs(m_vecMoveSpeed.z))
                {
                    pDamageIntensity = pDamageIntensity;
                    fCollisionImpact2 = 0.0f;
                    *pDamageIntensity = -0.95f * fCollisionMass * fSpeedDotProduct;
                }
                else
                {
                    bUseElasticity = true;
                }
            }
            else if (entityAltCol == ALT_ENITY_COL_BOAT
                && fabs(m_vecMoveSpeed.x) < fMoveSpeedLimit
                && fabs(m_vecMoveSpeed.y) < fMoveSpeedLimit
                && fMoveSpeedLimit + fMoveSpeedLimit > fabs(m_vecMoveSpeed.z))
            {
                fCollisionImpact2 = 0.0f;
                *pDamageIntensity = -0.95f * fCollisionMass * fSpeedDotProduct;
            }
            else
            {
                bUseElasticity = true;
            }
        }
        else
        {
            if (fabs(m_vecMoveSpeed.x) >= fMoveSpeedLimit
                || fabs(m_vecMoveSpeed.y) >= fMoveSpeedLimit
                || fMoveSpeedLimit + fMoveSpeedLimit <= fabs(m_vecMoveSpeed.z))
            {
                bUseElasticity = true;
            }
            else
            {
                fCollisionImpact2 = 0.0f;
                *pDamageIntensity = -0.95f * fCollisionMass * fSpeedDotProduct;
            }
        }
    }

    if (bUseElasticity)
    {
        float fElasticity = m_fElasticity + m_fElasticity;
        if (m_nType != ENTITY_TYPE_VEHICLE || pVehicle->m_nVehicleClass != VEHICLE_BOAT
            || pColPoint->m_nSurfaceTypeB != SURFACE_WOOD_SOLID && vecMoveDirection.z >= 0.5f)
        {
            fElasticity = m_fElasticity;
        }

        *pDamageIntensity = -((fElasticity + 1.0f) * fCollisionMass * fSpeedDotProduct);
    }

    CVector vecMoveSpeed = vecMoveDirection * *pDamageIntensity;

    if (physicalFlags.bDisableZ || physicalFlags.bInfiniteMass || physicalFlags.bDisableMoveForce)
    {
        ApplyForce(vecMoveSpeed, vecDistanceToPointFromThis, true);
    }
    else
    {
        CVector vecSpeed = vecMoveSpeed / m_fMass;
        if (m_nType == ENTITY_TYPE_VEHICLE)
        {
            if (!m_bHasHitWall || m_vecMoveSpeed.SquaredMagnitude() <= 0.1f
                && (pEntity->m_nType == ENTITY_TYPE_BUILDING || pEntity->physicalFlags.bDisableCollisionForce))
            {
                *pVecMoveSpeed += vecSpeed * 1.2f;
            }
            else
            {
                *pVecMoveSpeed += vecSpeed;
            }

            vecMoveSpeed *= 0.8f;
        }
        else
        {
            *pVecMoveSpeed += vecSpeed;
        }

        Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
        float fTurnMass = m_fTurnMass;
        CVector vecDifference = vecDistanceToPointFromThis - vecCentreOfMassMultiplied;
        CVector vecCrossProduct;
        vecCrossProduct.Cross(vecDifference, vecMoveSpeed);
        *pVecTurnSpeed += vecCrossProduct / fTurnMass;
    }

    float fCollisionImpact1 = *pDamageIntensity / fCollisionMass;
    AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
        &pColPoint->m_vecNormal, fCollisionImpact1, fCollisionImpact2, false, false);
    return true;
#endif
}

bool CPhysical::ApplyFriction(float fFriction, CColPoint* pColPoint)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, float, CColPoint*))0x5454C0)(this, fFriction, pColPoint);
#else
    if (physicalFlags.bDisableCollisionForce)
    {
        return false;
    }

    if (physicalFlags.bDisableTurnForce)
    {
        float fMoveSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &pColPoint->m_vecNormal);
        CVector vecSpeedDifference = m_vecMoveSpeed - (fMoveSpeedDotProduct * pColPoint->m_vecNormal);
        float fMoveSpeedMagnitude = vecSpeedDifference.Magnitude();
        if (fMoveSpeedMagnitude > 0.0f)
        {
            CVector vecMoveDirection = vecSpeedDifference / fMoveSpeedMagnitude;

            float fSpeed = -fMoveSpeedMagnitude;
            float fForce = -(CTimer::ms_fTimeStep / m_fMass * fFriction);
            if (fSpeed < fForce)
            {
                fSpeed = fForce;
            }

            m_vecFrictionMoveSpeed.x += vecMoveDirection.x * fSpeed;
            m_vecFrictionMoveSpeed.y += vecMoveDirection.y * fSpeed;
            return true;
        }
        return false;
    }

    CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
    CVector vecSpeed;
    GetSpeed(&vecSpeed, vecDistanceToPointFromThis);

    float fMoveSpeedDotProduct = DotProduct(&vecSpeed, &pColPoint->m_vecNormal);
    CVector vecSpeedDifference = vecSpeed - (fMoveSpeedDotProduct * pColPoint->m_vecNormal);

    float fMoveSpeedMagnitude = vecSpeedDifference.Magnitude();
    if (fMoveSpeedMagnitude <= 0.0f)
    {
        return false;
    }

    CVector vecMoveDirection = vecSpeedDifference / fMoveSpeedMagnitude;

    CVector vecCentreOfMassMultiplied;
    Multiply3x3(&vecCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

    CVector vecDifference = vecDistanceToPointFromThis - vecCentreOfMassMultiplied;
    CVector vecSpeedCrossProduct;
    vecSpeedCrossProduct.Cross(vecDifference, vecMoveDirection);
    float squaredMagnitude = vecSpeedCrossProduct.SquaredMagnitude();
    float fCollisionMass = -(1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass) * fMoveSpeedMagnitude);
    float fNegativeFriction = -fFriction;
    if (fCollisionMass < fNegativeFriction)
    {
        fCollisionMass = fNegativeFriction;
    }

    ApplyFrictionForce(vecMoveDirection * fCollisionMass, vecDistanceToPointFromThis);

    CVehicle* pVehicle = static_cast<CVehicle*>(this);
    if (fMoveSpeedMagnitude > 0.1f
        && g_surfaceInfos->GetFrictionEffect(pColPoint->m_nSurfaceTypeB)
        && (g_surfaceInfos->GetFrictionEffect(pColPoint->m_nSurfaceTypeA) == FRICTION_EFFECT_SPARKS || m_nType == ENTITY_TYPE_VEHICLE)
        && (m_nType != ENTITY_TYPE_VEHICLE || pVehicle->m_nVehicleSubClass != VEHICLE_BMX || !pVehicle->m_pDriver
            || fabs(DotProduct(&pColPoint->m_vecNormal, &GetRight())) >= 0.86669999f))
    {

        CVector across = vecMoveDirection * (fMoveSpeedMagnitude * 0.25f);
        CVector direction = vecMoveDirection + (pColPoint->m_vecNormal * 0.1f);
        CVector vecSpeedCrossProduct;
        vecSpeedCrossProduct.Cross(pColPoint->m_vecNormal, m_vecMoveSpeed);
        vecSpeedCrossProduct.Normalise();

        for (int i = 0; i < 8; i++)
        {
            float fRandom = CGeneral::GetRandomNumberInRange(-0.2f, 0.2f);
            CVector origin = pColPoint->m_vecPoint + (vecSpeedCrossProduct * fRandom);
            float force = fMoveSpeedMagnitude * 12.5f;
            g_fx.AddSparks(origin, direction, force, 1, across, SPARK_PARTICLE_SPARK2, 0.1f, 1.0f);
        }
    }
    return true;
#endif
}

bool CPhysical::ApplyFriction(CPhysical* pEntity, float fFriction, CColPoint* pColPoint)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CPhysical*, float, CColPoint*))0x545980)(this, pEntity, fFriction, pColPoint);
#else
    if (physicalFlags.bDisableTurnForce && pEntity->physicalFlags.bDisableTurnForce)
    {
        float fThisSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &pColPoint->m_vecNormal);
        float fEntitySpeedDotProduct = DotProduct(&pEntity->m_vecMoveSpeed, &pColPoint->m_vecNormal);

        CVector vecThisSpeedDifference = m_vecMoveSpeed - (fThisSpeedDotProduct * pColPoint->m_vecNormal);
        CVector vecEntitySpeedDifference = pEntity->m_vecMoveSpeed - (fEntitySpeedDotProduct * pColPoint->m_vecNormal);

        float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
        float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

        float fEntityMass = pEntity->m_fMass;
        float fThisMass = m_fMass;

        CVector vecMoveDirection = vecThisSpeedDifference / fThisSpeedMagnitude;
        float fSpeed = (fEntityMass * fEntitySpeedMagnitude + fThisMass * fThisSpeedMagnitude) / (fEntityMass + fThisMass);
        if (fThisSpeedMagnitude > fSpeed) {
            float fThisSpeed = fThisMass * (fSpeed - fThisSpeedMagnitude);
            float fEntitySpeed = fEntityMass * (fSpeed - fEntitySpeedMagnitude);
            float fFrictionTimeStep = -(CTimer::ms_fTimeStep * fFriction);
            if (fThisSpeed < fFrictionTimeStep)
            {
                fThisSpeed = fFrictionTimeStep;
            }

            // BUG: Both if conditions are the same.
            if (fThisSpeed < fFrictionTimeStep)
            {
                fThisSpeed = fFrictionTimeStep;
            }

            ApplyFrictionMoveForce(vecMoveDirection * fThisSpeed);
            pEntity->ApplyFrictionMoveForce(vecMoveDirection * fEntitySpeed);
            return true;
        }
        return false;
    }

    if (physicalFlags.bDisableTurnForce)
    {
        if (pEntity->m_nType == ENTITY_TYPE_VEHICLE)
        {
            return false;
        }

        CVector vecDistanceToPoint = pColPoint->m_vecPoint - pEntity->GetPosition();
        CVector vecEntitySpeed;
        pEntity->GetSpeed(&vecEntitySpeed, vecDistanceToPoint);

        float fThisSpeedDotProduct = DotProduct(&m_vecMoveSpeed, &pColPoint->m_vecNormal);
        float fEntitySpeedDotProduct = DotProduct(&vecEntitySpeed, &pColPoint->m_vecNormal);

        CVector vecThisSpeedDifference = m_vecMoveSpeed - (fThisSpeedDotProduct * pColPoint->m_vecNormal);
        CVector vecEntitySpeedDifference = vecEntitySpeed - (fEntitySpeedDotProduct * pColPoint->m_vecNormal);

        float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
        float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

        CVector vecMoveDirection = vecThisSpeedDifference * (1.0f / fThisSpeedMagnitude);
        CVector vecEntityCentreOfMassMultiplied;
        Multiply3x3(&vecEntityCentreOfMassMultiplied, pEntity->m_matrix, &pEntity->m_vecCentreOfMass);

        CVector vecEntityDifference = vecDistanceToPoint - vecEntityCentreOfMassMultiplied;
        CVector vecEntitySpeedCrossProduct;
        vecEntitySpeedCrossProduct.Cross(vecEntityDifference, vecMoveDirection);
        float squaredMagnitude = vecEntitySpeedCrossProduct.SquaredMagnitude();
        float fEntityCollisionMass = 1.0f / ((squaredMagnitude) / pEntity->m_fTurnMass + 1.0f / pEntity->m_fMass);
        float fThisMass = m_fMass;
        float fSpeed = (fEntitySpeedMagnitude * fEntityCollisionMass + fThisMass * fThisSpeedMagnitude) / (fEntityCollisionMass + fThisMass);
        if (fThisSpeedMagnitude > fSpeed) {
            float fThisSpeed = fThisMass * (fSpeed - fThisSpeedMagnitude);
            float fEntitySpeed = fEntityCollisionMass * (fSpeed - fEntitySpeedMagnitude);
            float fFrictionTimeStep = CTimer::ms_fTimeStep * fFriction;
            float fFrictionTimeStepNegative = -fFrictionTimeStep;
            if (fThisSpeed < fFrictionTimeStepNegative)
            {
                fThisSpeed = fFrictionTimeStepNegative;
            }

            if (fEntitySpeed > fFrictionTimeStep)
            {
                fEntitySpeed = fFrictionTimeStep;
            }

            ApplyFrictionMoveForce(vecMoveDirection * fThisSpeed);
            if (!pEntity->physicalFlags.bDisableCollisionForce)
            {
                pEntity->ApplyFrictionForce(vecMoveDirection * fEntitySpeed, vecDistanceToPoint);
                return true;
            }
            return true;
        }
        return false;
    }

    if (!pEntity->physicalFlags.bDisableTurnForce)
    {
        CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
        CVector vecThisSpeed;
        GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

        CVector vecDistanceToPoint = pColPoint->m_vecPoint - pEntity->GetPosition();
        CVector vecEntitySpeed;
        pEntity->GetSpeed(&vecEntitySpeed, vecDistanceToPoint);

        float fThisSpeedDotProduct = DotProduct(&vecThisSpeed, &pColPoint->m_vecNormal);
        float fEntitySpeedDotProduct = DotProduct(&vecEntitySpeed, &pColPoint->m_vecNormal);
        if (0.2f * 0.707f > fabs(fThisSpeedDotProduct))
        {
            fFriction = 0.05f * fFriction;
        }

        CVector vecThisSpeedDifference = vecThisSpeed - (fThisSpeedDotProduct * pColPoint->m_vecNormal);
        CVector vecEntitySpeedDifference = vecEntitySpeed - (fEntitySpeedDotProduct * pColPoint->m_vecNormal);

        float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
        float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

        CVector vecMoveDirection = vecThisSpeedDifference * (1.0f / fThisSpeedMagnitude);

        CVector vecThisCentreOfMassMultiplied;
        Multiply3x3(&vecThisCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

        CVector vecThisDifference = vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied;
        CVector vecThisSpeedCrossProduct;
        vecThisSpeedCrossProduct.Cross(vecThisDifference, vecMoveDirection);
        float squaredMagnitude = vecThisSpeedCrossProduct.SquaredMagnitude();
        float fThisCollisionMass = 1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass);

        CVector vecEntityCentreOfMassMultiplied;
        Multiply3x3(&vecEntityCentreOfMassMultiplied, pEntity->m_matrix, &pEntity->m_vecCentreOfMass);

        CVector vecEntityDifference = vecDistanceToPoint - vecEntityCentreOfMassMultiplied;
        CVector vecEntitySpeedCrossProduct;
        vecEntitySpeedCrossProduct.Cross(vecEntityDifference, vecMoveDirection);
        squaredMagnitude = vecEntitySpeedCrossProduct.SquaredMagnitude();
        float fEntityCollisionMass = 1.0f / (squaredMagnitude / pEntity->m_fTurnMass + 1.0f / pEntity->m_fMass);
        float fSpeed = (fEntitySpeedMagnitude * fEntityCollisionMass + fThisCollisionMass * fThisSpeedMagnitude) / (fEntityCollisionMass + fThisCollisionMass);
        if (fThisSpeedMagnitude > fSpeed) {
            float fThisSpeed = fThisCollisionMass * (fSpeed - fThisSpeedMagnitude);
            float fEntitySpeed = fEntityCollisionMass * (fSpeed - fEntitySpeedMagnitude);
            float fNegativeFriction = -fFriction;
            if (fThisSpeed < fNegativeFriction)
            {
                fThisSpeed = fNegativeFriction;
            }

            if (fEntitySpeed > fFriction)
            {
                fEntitySpeed = fFriction;
            }

            if (!physicalFlags.bDisableCollisionForce)
            {
                ApplyFrictionForce(vecMoveDirection * fThisSpeed, vecDistanceToPointFromThis);
            }

            if (!pEntity->physicalFlags.bDisableCollisionForce)
            {
                pEntity->ApplyFrictionForce(vecMoveDirection * fEntitySpeed, vecDistanceToPoint);
            }
            return true;
        }
        return false;
    }

    if (m_nType == ENTITY_TYPE_VEHICLE)
    {
        return false;
    }

    CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
    CVector vecThisSpeed;
    GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

    float fThisSpeedDotProduct = DotProduct(&vecThisSpeed, &pColPoint->m_vecNormal);
    float fEntitySpeedDotProduct = DotProduct(&pEntity->m_vecMoveSpeed, &pColPoint->m_vecNormal);

    CVector vecThisSpeedDifference = vecThisSpeed - (fThisSpeedDotProduct * pColPoint->m_vecNormal);
    CVector vecEntitySpeedDifference = pEntity->m_vecMoveSpeed - (fEntitySpeedDotProduct * pColPoint->m_vecNormal);

    float fThisSpeedMagnitude = vecThisSpeedDifference.Magnitude();
    float fEntitySpeedMagnitude = vecEntitySpeedDifference.Magnitude();

    CVector vecMoveDirection = vecThisSpeedDifference * (1.0f / fThisSpeedMagnitude);

    CVector vecThisCentreOfMassMultiplied;
    Multiply3x3(&vecThisCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);

    CVector vecThisDifference = vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied;
    CVector vecThisSpeedCrossProduct;
    vecThisSpeedCrossProduct.Cross(vecThisDifference, vecMoveDirection);
    float squaredMagnitude = vecThisSpeedCrossProduct.SquaredMagnitude();
    float fEntityMass = pEntity->m_fMass;
    float fThisCollisionMass = 1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass);
    float fSpeed = (fEntityMass * fEntitySpeedMagnitude + fThisCollisionMass * fThisSpeedMagnitude) / (fEntityMass + fThisCollisionMass);
    if (fThisSpeedMagnitude > fSpeed) {
        float fThisSpeed = (fSpeed - fThisSpeedMagnitude) * fThisCollisionMass;
        float fEntitySpeed = (fSpeed - fEntitySpeedMagnitude) * fEntityMass;
        float fFrictionTimeStep = CTimer::ms_fTimeStep * fFriction;
        float fNegativeFrictionTimeStep = -fFrictionTimeStep;
        if (fThisSpeed < fNegativeFrictionTimeStep)
        {
            fThisSpeed = fNegativeFrictionTimeStep;
        }

        if (fEntitySpeed > fFrictionTimeStep)
        {
            fEntitySpeed = fFrictionTimeStep;
        }

        if (!physicalFlags.bDisableCollisionForce)
        {
            ApplyFrictionForce(vecMoveDirection * fThisSpeed, vecDistanceToPointFromThis);
        }

        pEntity->ApplyFrictionMoveForce(vecMoveDirection * fEntitySpeed);
        return true;
    }
    return false;
#endif
}

bool CPhysical::ProcessShiftSectorList(int sectorX, int sectorY)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, int, int))0x546670)(this, sectorX, sectorY);
#else
    CBaseModelInfo* pModelInfo = CModelInfo::ms_modelInfoPtrs[m_nModelIndex];
    float fBoundingSphereRadius = pModelInfo->m_pColModel->m_boundSphere.m_fRadius;
    float fMaxColPointDepth = 0.0f;
    CVector vecShift;
    CColPoint colPoints[32];
    CVector vecBoundCentre;

    GetBoundCentre(&vecBoundCentre);

    CSector* pSector = GetSector(sectorX, sectorY);
    CRepeatSector* pRepeatSector = GetRepeatSector(sectorX, sectorY);

    int totalAcceptableColPoints = 0;
    int scanListIndex = 4;
    do
    {
        CPtrListDoubleLink* pDoubleLinkList = nullptr;
        switch (--scanListIndex)
        {
        case 0:
            pDoubleLinkList = &pSector->m_buildings;
            break;
        case 1:
            pDoubleLinkList = &pRepeatSector->m_lists[0];
            break;
        case 2:
            pDoubleLinkList = &pRepeatSector->m_lists[1];
            break;
        case 3:
            pDoubleLinkList = &pRepeatSector->m_lists[2];
            break;
        }
        CPtrNodeDoubleLink* pNode = pDoubleLinkList->GetNode();
        if (pDoubleLinkList->GetNode())
        {
            do
            {
                CPhysical* pEntity = reinterpret_cast<CPhysical*>(pNode->pItem);
                CPed* pPedEntity = static_cast<CPed*>(pEntity);
                CVehicle* pVehicleEntity = static_cast<CVehicle*>(pEntity);
                CObject* pObjectEntity = static_cast<CObject*>(pEntity);

                pNode = pNode->pNext;

                bool bProcessEntityCollision = true;
                if (pEntity->m_nType != ENTITY_TYPE_BUILDING
                    && (pEntity->m_nType != ENTITY_TYPE_OBJECT || !pEntity->physicalFlags.bDisableCollisionForce))
                {
                    if (m_nType != ENTITY_TYPE_PED || pEntity->m_nType != ENTITY_TYPE_OBJECT
                        || (!pEntity->m_bIsStatic && !pEntity->m_bIsStaticWaitingForCollision)
                        || pObjectEntity->objectFlags.bIsExploded)
                    {
                        bProcessEntityCollision = false;
                    }
                }
                if (pEntity != this
                    && pEntity->m_nScanCode != CWorld::ms_nCurrentScanCode
                    && pEntity->m_bUsesCollision && (!m_bHasHitWall || bProcessEntityCollision))
                {
                    if (pEntity->GetIsTouching(&vecBoundCentre, fBoundingSphereRadius))
                    {
                        bool bCollisionDisabled = false;
                        bool bCollidedEntityCollisionIgnored = false;
                        bool bCollidedEntityUnableToMove = false;
                        bool bThisOrCollidedEntityStuck = false;
                        if (pEntity->m_nType == ENTITY_TYPE_BUILDING)
                        {
                            if (physicalFlags.bDisableCollisionForce
                                && (m_nType != ENTITY_TYPE_VEHICLE || pVehicleEntity->m_nVehicleSubClass == VEHICLE_TRAIN))
                            {
                                bCollisionDisabled = true;
                            }
                            else
                            {
                                if (m_pAttachedTo)
                                {
                                    unsigned char attachedEntityType = m_pAttachedTo->m_nType;
                                    if (attachedEntityType > ENTITY_TYPE_BUILDING && attachedEntityType < ENTITY_TYPE_DUMMY
                                        && m_pAttachedTo->physicalFlags.bDisableCollisionForce)
                                    {
                                        bCollisionDisabled = true;
                                    }
                                }
                                else if (m_pEntityIgnoredCollision == pEntity)
                                {
                                    bCollisionDisabled = true;
                                }

                                else if (!physicalFlags.bDisableZ || physicalFlags.bApplyGravity)
                                {
                                    if (physicalFlags.bDontCollideWithFlyers)
                                    {
                                        if (m_nStatus)
                                        {
                                            if (m_nStatus != STATUS_HELI && pEntity->DoesNotCollideWithFlyers())
                                            {
                                                bCollisionDisabled = true;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    bCollisionDisabled = true;
                                }
                            }
                        }
                        else
                        {
                            SpecialEntityPreCollisionStuff(pEntity, true, &bCollisionDisabled, &bCollidedEntityCollisionIgnored, 
                                &bCollidedEntityUnableToMove, &bThisOrCollidedEntityStuck);
                        }

                        if (m_nType == ENTITY_TYPE_PED)
                        {
                            physicalFlags.b13 = true;
                        }

                        if (!bCollidedEntityCollisionIgnored && !bCollisionDisabled)
                        {

                            pEntity->m_nScanCode = CWorld::ms_nCurrentScanCode;
                            int totalColPointsToProcess = ProcessEntityCollision(pEntity, colPoints);
                            if (totalColPointsToProcess > 0)
                            {
                                for (int colpointIndex = 0; colpointIndex < totalColPointsToProcess; colpointIndex++)
                                {
                                    CColPoint* pColPoint = &colPoints[colpointIndex];
                                    if (pColPoint->m_fDepth > 0.0f)
                                    {
                                        unsigned char pieceTypeB = pColPoint->m_nPieceTypeB;
                                        if (pieceTypeB < 13 || pieceTypeB > 16)
                                        {
                                            totalAcceptableColPoints++;
                                            if (m_nType == ENTITY_TYPE_VEHICLE && pEntity->m_nType == ENTITY_TYPE_PED
                                                && pColPoint->m_vecNormal.z < 0.0f)
                                            {
                                                vecShift.x += pColPoint->m_vecNormal.x;
                                                vecShift.y += pColPoint->m_vecNormal.y;
                                                vecShift.z += pColPoint->m_vecNormal.z * 0.0f;
                                                fMaxColPointDepth = std::max(fMaxColPointDepth, pColPoint->m_fDepth);
                                            }
                                            else
                                            {
                                                if (m_nType != ENTITY_TYPE_PED || pEntity->m_nType != ENTITY_TYPE_OBJECT
                                                    || !pEntity->physicalFlags.bDisableMoveForce
                                                    || fabs(pColPoint->m_vecNormal.z) <= 0.1f)
                                                {
                                                    vecShift += pColPoint->m_vecNormal;
                                                    fMaxColPointDepth = std::max(fMaxColPointDepth, pColPoint->m_fDepth);
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            } while (pNode);
        }
    } while (scanListIndex);

    if (totalAcceptableColPoints == 0)
    {
        return false;
    }

    float shiftMagnitude = vecShift.Magnitude();
    if (shiftMagnitude > 1.0f)
    {   // normalize the shift boi
        float shiftMultiplier = 1.0f / shiftMagnitude;
        vecShift *= shiftMultiplier;
    }

    CVector& vecEntityPosition = GetPosition();
    if (vecShift.z >= -0.5f)
    {
        if (m_nType != ENTITY_TYPE_PED)
        {
            vecEntityPosition += vecShift * fMaxColPointDepth * 1.5f;
        }
        else
        {
            float fDepthMultiplied = 1.5f * fMaxColPointDepth;
            if (fDepthMultiplied >= 0.0049999999)
            {
                if (fDepthMultiplied > 0.30000001)
                {
                    vecEntityPosition += vecShift * fMaxColPointDepth * 0.3f;
                }
            }
            else
            {
                vecEntityPosition += vecShift * fMaxColPointDepth * 0.0049999999f;
            }

            vecEntityPosition += vecShift * fDepthMultiplied;
        }
    }
    else
    {
        vecEntityPosition += vecShift * fMaxColPointDepth * 0.75f;
    }

    if (m_nType != ENTITY_TYPE_VEHICLE || 1.5f <= 0.0f)
    {
        return true;
    }

    if (vecShift.z < 0.0)
    {
        vecShift.z = 0.0;
    }

    m_vecMoveSpeed += vecShift * 0.0080000004f * CTimer::ms_fTimeStep;
    return true;
#endif
}

// Used in driving school mission
void CPhysical::PlacePhysicalRelativeToOtherPhysical(CPhysical* relativeToPhysical, CPhysical* physicalToPlace, CVector offset)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__cdecl*)(CPhysical*, CPhysical*, CVector))0x546DB0)(relativeToPhysical, physicalToPlace, offset);
#else
    CVector vecRelativePosition = *relativeToPhysical->m_matrix * offset;
    vecRelativePosition += (CTimer::ms_fTimeStep * 0.9f) * relativeToPhysical->m_vecMoveSpeed;
    CWorld::Remove(physicalToPlace);
    *(CMatrix*)physicalToPlace->m_matrix = *relativeToPhysical->m_matrix;
    physicalToPlace->GetPosition() = vecRelativePosition;
    physicalToPlace->m_vecMoveSpeed = relativeToPhysical->m_vecMoveSpeed;
    physicalToPlace->UpdateRW();
    physicalToPlace->UpdateRwFrame();
    CWorld::Add(physicalToPlace);
#endif
}

// Unused
float CPhysical::ApplyScriptCollision(CVector arg0, float arg1, float arg2, CVector* arg3)
{
    return ((float(__thiscall*)(CPhysical*, CVector, float, float, CVector*))0x546ED0)(this, arg0, arg1, arg2, arg3);
}

void CPhysical::PositionAttachedEntity()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x546FF0)(this);
#else
    if (m_pAttachedTo) {
        CVehicle* pAttachedToVehicle = static_cast<CVehicle*>(m_pAttachedTo);
        CAutomobile* pAttachedToAutmobile = static_cast<CAutomobile*>(m_pAttachedTo);
        CBike* pAttachedToBike = static_cast<CBike*>(m_pAttachedTo);

        CMatrix attachedEntityMatrix;
        CMatrix attachedEntityRotationMatrix;
        if (!m_pAttachedTo->m_matrix) {
            m_pAttachedTo->AllocateMatrix();
            m_pAttachedTo->m_placement.UpdateMatrix(m_pAttachedTo->m_matrix);
        }
        CMatrix attachedToEntityMatrix (*m_pAttachedTo->m_matrix);
        if (m_pAttachedTo->m_nType != ENTITY_TYPE_VEHICLE || pAttachedToVehicle->m_nVehicleClass != VEHICLE_BIKE) {
            if (m_nType == ENTITY_TYPE_OBJECT && m_pAttachedTo->m_nModelIndex == MODEL_FORKLIFT) {
                RwFrame* carPart = pAttachedToAutmobile->m_aCarNodes[CAR_MISC_A];
                if (carPart)
                    attachedToEntityMatrix.UpdateMatrix(RwFrameGetLTM(carPart));
            }
        }
        else {
            pAttachedToBike->CalculateLeanMatrix();
            attachedToEntityMatrix = pAttachedToBike->m_mLeanMatrix;
        }

        if (m_qAttachedEntityRotation.imag.x != 0.0f || m_qAttachedEntityRotation.imag.y != 0.0f || m_qAttachedEntityRotation.imag.z != 0.0f) {
            RwMatrix rotationMatrix;
            RtQuatConvertToMatrix((RtQuat*)&m_qAttachedEntityRotation, &rotationMatrix);
            attachedEntityMatrix.UpdateMatrix(&rotationMatrix);
            attachedEntityMatrix = attachedToEntityMatrix * attachedEntityMatrix;
            CVector vecTranslation = attachedToEntityMatrix * m_vecAttachOffset;
            attachedEntityMatrix.SetTranslateOnly(vecTranslation);
        }
        else {
            if (physicalFlags.bInfiniteMass) {
                attachedEntityMatrix = *m_matrix;
            }
            else {
                attachedEntityRotationMatrix.ResetOrientation();
                attachedEntityRotationMatrix.Rotate(m_vecAttachedEntityRotation);
                attachedEntityMatrix = attachedToEntityMatrix;
                attachedEntityMatrix *= attachedEntityRotationMatrix;
            }
            CVector position;
            Multiply3x3(&position, &attachedToEntityMatrix, &m_vecAttachOffset);
            attachedEntityMatrix.GetPosition() += position;
        }
        SetMatrix(attachedEntityMatrix);

        if (m_nType == ENTITY_TYPE_OBJECT) {
            if (m_bIsStatic || m_bIsStaticWaitingForCollision)
                SetIsStatic(false);
            physicalFlags.bAttachedToEntity = true;
            m_bFakePhysics = false;
        }

        bool bUpdateSpeed = false;
        if (m_pAttachedTo->m_nType != ENTITY_TYPE_VEHICLE || m_nType != ENTITY_TYPE_OBJECT && m_nType != ENTITY_TYPE_VEHICLE) {
            if (m_nModelIndex == MODEL_SATCHEL && m_pAttachedTo->m_nType == ENTITY_TYPE_OBJECT && !m_pAttachedTo->m_bUsesCollision) {
                DettachAutoAttachedEntity();
                m_bUsesCollision = true;
                return;
            }
            bUpdateSpeed = true;
        }

        bool bDettachEntity = false;
        if (!bUpdateSpeed)
        {
            if (m_pAttachedTo->m_nModelIndex == MODEL_DUMPER) {
                short wMiscComponentAngle = pAttachedToAutmobile->m_wMiscComponentAngle;
                if (wMiscComponentAngle && wMiscComponentAngle != pAttachedToAutmobile->m_wVoodooSuspension) {
                    bDettachEntity = true;
                }
                else if (m_fDamageIntensity > 0.0f) {
                    if (m_pDamageEntity) {
                        if (m_pDamageEntity->m_nType  == ENTITY_TYPE_BUILDING)
                            bDettachEntity = true;
                    }
                }
            }
            else {
                if (m_pAttachedTo->m_nModelIndex != MODEL_FORKLIFT) {
                    if (m_nType == ENTITY_TYPE_VEHICLE) {
                        CMatrix* pAttachedToEntityMatrix = m_pAttachedTo->GetMatrix();
                        if (fabs(pAttachedToEntityMatrix->GetRight().z) > 0.707f || fabs(pAttachedToEntityMatrix->GetForward().z) > 0.707f) {
                            DettachEntityFromEntity(0.0f, 0.0f, 1.0f, false);
                            return;
                        }
                    }
                    bUpdateSpeed = true;
                }
                else if (!pAttachedToAutmobile->m_wMiscComponentAngle && pAttachedToAutmobile->m_wVoodooSuspension
                    || m_fDamageIntensity > 0.1f * m_fMass && m_pDamageEntity && m_pDamageEntity->m_nType  == ENTITY_TYPE_BUILDING) {
                    bDettachEntity = true;
                }
            }
            if (!bUpdateSpeed && !bDettachEntity) {
                float fDamagedIntensity = m_pAttachedTo->m_fDamageIntensity;
                CMatrix* pMatrix = m_pAttachedTo->m_matrix;
                if ((fDamagedIntensity <= 0.0f || m_pAttachedTo->GetUp().z >= 0.1f) && (fDamagedIntensity <= 1.0f || m_pAttachedTo->GetForward().z <= 0.87f)) {
                    float fDotProduct = DotProduct(&m_pAttachedTo->m_vecLastCollisionImpactVelocity, &pMatrix->GetUp());
                    if ((fDamagedIntensity <= 500.0f || fDotProduct >= 0.7f || 0.3f * 0.3f <= m_pAttachedTo->m_vecMoveSpeed.SquaredMagnitude())) {
                        bUpdateSpeed = true;
                    }
                }
            }
        }

        if (!bUpdateSpeed) {
            DettachAutoAttachedEntity();
            if (!physicalFlags.bDisableCollisionForce) {
                float randomNumber = CGeneral::GetRandomNumberInRange(-1.0f, 1.0f);
                CMatrix* pAttachedToEntityMatrix = m_pAttachedTo->GetMatrix();
                CVector randomRight = pAttachedToEntityMatrix->GetRight() * randomNumber;
                CVector randomUp = pAttachedToEntityMatrix->GetForward() * randomNumber;
                CVector force = (randomRight + randomUp) * (m_fMass * 0.02f);
                ApplyMoveForce(force);
                if (pAttachedToAutmobile->m_wMiscComponentAngle > pAttachedToAutmobile->m_wVoodooSuspension)
                    ApplyMoveForce(m_pAttachedTo->GetMatrix()->GetUp() * m_fMass * 0.02f);
            }
            return;
        }

        if (physicalFlags.bInfiniteMass) {
            CVector vecMoveSpeed = GetPosition() - m_vecAttachedEntityRotation;
            float fSquaredMagnitude = vecMoveSpeed.SquaredMagnitude();
            m_vecAttachedEntityRotation = GetPosition();
            if (fSquaredMagnitude > 4.0f)
                vecMoveSpeed *= 2.0f / sqrt(fSquaredMagnitude);

            float fTimeStep = 1.0f;
            if (CTimer::ms_fTimeStep >= 1.0f)
                fTimeStep = CTimer::ms_fTimeStep;

            vecMoveSpeed *= 1.0f / fTimeStep;
            CVector vecMoveSpeedDifference = vecMoveSpeed - m_vecMoveSpeed;
            m_vecMoveSpeed = vecMoveSpeed;
            CVector vecForce = vecMoveSpeedDifference * m_fMass * -1.0f;
            CVector vecCenterOfMassMultiplied;
            Multiply3x3(&vecCenterOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
            ApplyForce(vecForce, vecCenterOfMassMultiplied, true);
            if (m_pAttachedTo->m_nType == ENTITY_TYPE_VEHICLE || m_pAttachedTo->m_nType == ENTITY_TYPE_OBJECT) {
                if (m_pAttachedTo->m_bUsesCollision && !m_pAttachedTo->physicalFlags.bDisableCollisionForce) {
                    CVector vecDistance = (vecCenterOfMassMultiplied + GetPosition()) - m_pAttachedTo->GetPosition();
                    m_pAttachedTo->ApplyForce(vecForce * -1.0f, vecDistance, true);
                }
                float fRotationInRadians = m_pAttachedTo->GetHeading() - GetHeading();
                if (fRotationInRadians <= PI) {
                    if (fRotationInRadians < -PI)
                        fRotationInRadians += PI * 2;
                }
                else {
                    fRotationInRadians -= PI * 2;
                }
                if (fRotationInRadians <= 0.5f) {
                    if (fRotationInRadians < -0.5f)
                        fRotationInRadians = -0.5f;
                }
                else {
                    fRotationInRadians = 0.5f;
                }
                m_vecTurnSpeed.z += fRotationInRadians * 0.00001f * m_fMass;
            }
        }
        else {
            if (m_pAttachedTo->m_nType == ENTITY_TYPE_VEHICLE || m_pAttachedTo->m_nType == ENTITY_TYPE_OBJECT) {
                CVector outSpeed;
                m_pAttachedTo->GetSpeed(&outSpeed, GetPosition() - m_pAttachedTo->GetPosition());
                m_vecMoveSpeed = outSpeed;
                m_vecTurnSpeed = m_pAttachedTo->m_vecTurnSpeed;
            }
        }
    }
#endif
}

void CPhysical::ApplySpeed()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x547B80)(this);
#else
    CObject* pObject = static_cast<CObject*>(this);
    float fOldTimeStep = CTimer::ms_fTimeStep;
    if (physicalFlags.bDisableZ) {
        if (physicalFlags.bApplyGravity) {
            if (fOldTimeStep * m_vecMoveSpeed.z + GetPosition().z < CWorld::SnookerTableMin.z) {
                GetPosition().z = CWorld::SnookerTableMin.z;
                m_vecMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
                m_vecTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
            }
            ApplyMoveSpeed();
            ApplyTurnSpeed();
            CTimer::UpdateTimeStep(fOldTimeStep);
            return;
        }

        float fTimeStepX = 1000.0f;
        float fTimeStepY = 1000.0f;
        float fNewPositionX = fOldTimeStep * m_vecMoveSpeed.x + GetPosition().x;
        if (fNewPositionX <= CWorld::SnookerTableMax.x || m_vecMoveSpeed.x <= 0.0f)
        {
            if (fNewPositionX >= CWorld::SnookerTableMin.x || m_vecMoveSpeed.x >= 0.0f) {
                // nothing
            }
            else{
             fTimeStepX = (CWorld::SnookerTableMin.x - GetPosition().x) / m_vecMoveSpeed.x;
            }
        }
        else {
            fTimeStepX = (CWorld::SnookerTableMax.x - GetPosition().x) / m_vecMoveSpeed.x;
        }
        
        float fNewPositionY = CTimer::ms_fTimeStep * m_vecMoveSpeed.y + GetPosition().y;
        if (fNewPositionY <= CWorld::SnookerTableMax.y || m_vecMoveSpeed.y <= 0.0f) {
            if (fNewPositionY >= CWorld::SnookerTableMin.y || m_vecMoveSpeed.y >= 0.0f) {
                // nothing
            }
            else {
                fTimeStepY = (CWorld::SnookerTableMin.y - GetPosition().y) / m_vecMoveSpeed.y;
            }
        }
        else {
            fTimeStepY = (CWorld::SnookerTableMax.y - GetPosition().y) / m_vecMoveSpeed.y;
        }

        bool bTableWidthIsLessThanHeight = true;
        if (CWorld::SnookerTableMax.x - CWorld::SnookerTableMin.x < CWorld::SnookerTableMax.y - CWorld::SnookerTableMin.y)
            bTableWidthIsLessThanHeight = false;
        bool bApplyFriction = false;
        float fNormalX = 0.0f;
        float fNormalY = 0.0f;
        float fAbsoluteMoveSpeed = 0.0f;
        float fNewTimeStep = 0.0f;
        if (fTimeStepX < fTimeStepY && fTimeStepX < 1000.0f) {
            fNormalX = -1.0;
            fAbsoluteMoveSpeed = fabs(m_vecMoveSpeed.x);
            if (m_vecMoveSpeed.x <= 0.0)
                fNormalX = 1.0;
            CTimer::UpdateTimeStep(fTimeStepX);
            ApplyMoveSpeed();
            ApplyTurnSpeed();
            float fTableY = (CWorld::SnookerTableMin.y + CWorld::SnookerTableMax.y) * 0.5f;
            if (CWorld::SnookerTableMax.y - 0.06f >= GetPosition().y
                && CWorld::SnookerTableMin.y + 0.06f <= GetPosition().y
                && (bTableWidthIsLessThanHeight || fTableY - 0.06f >= GetPosition().y || GetPosition().y >= fTableY + 0.06f)) {
                m_vecMoveSpeed.x = m_vecMoveSpeed.x * -1.0f;
                fNewTimeStep = fOldTimeStep - fTimeStepX;
            }
            else
            {
                physicalFlags.bApplyGravity = true;
                float fTimeStepMoveSpeedX = fOldTimeStep * m_vecMoveSpeed.x;
                if (fTimeStepMoveSpeedX <= 0.03f) {
                    if (fTimeStepMoveSpeedX < -0.03f)
                        m_vecMoveSpeed.x = -(0.03f / fOldTimeStep);
                    fNewTimeStep = fOldTimeStep - fTimeStepX;
                }
                else {
                    m_vecMoveSpeed.x = 0.03f / fOldTimeStep;
                    fNewTimeStep = fOldTimeStep - fTimeStepX;
                }
            }
            bApplyFriction = true;
        }
        if (!bApplyFriction && fTimeStepY < 1000.0f) {
            fNormalY = -1.0f;
            fAbsoluteMoveSpeed = fabs(m_vecMoveSpeed.y);
            if (m_vecMoveSpeed.y <= 0.0f)
                fNormalY = 1.0f;
            CTimer::UpdateTimeStep(fTimeStepY);
            ApplyMoveSpeed();
            ApplyTurnSpeed();
            float fTableX = (CWorld::SnookerTableMin.x + CWorld::SnookerTableMax.x) * 0.5f;
            if (CWorld::SnookerTableMax.x - 0.06f < GetPosition().x
                || CWorld::SnookerTableMin.x + 0.06f > GetPosition().x
                || bTableWidthIsLessThanHeight &&  fTableX - 0.06f < GetPosition().x && GetPosition().x < fTableX + 0.06f) {
                physicalFlags.bApplyGravity = true;
                float fTimeStepMoveSpeedY = fOldTimeStep * m_vecMoveSpeed.y;
                if (fTimeStepMoveSpeedY <= 0.03f) {
                    if (fTimeStepMoveSpeedY < -0.03f) {
                        m_vecMoveSpeed.y = -(0.03f / fOldTimeStep);
                    }
                }
                else
                {
                    m_vecMoveSpeed.y = 0.03f / fOldTimeStep;
                }
            }
            else {
                m_vecMoveSpeed.y = m_vecMoveSpeed.y * -1.0f;
            }
            fNewTimeStep = fOldTimeStep - fTimeStepY;
            bApplyFriction = true;
        }

        if (bApplyFriction) {
            CTimer::UpdateTimeStep(fNewTimeStep);
            if (fAbsoluteMoveSpeed > 0.0f) {
                float fRadius = CModelInfo::ms_modelInfoPtrs[m_nModelIndex]->m_pColModel->m_boundSphere.m_fRadius;
                CVector thePosition = CVector(fNormalX * fRadius, fNormalY * fRadius, 0.0f);
                CColPoint colPoint;
                colPoint.m_vecPoint = GetPosition() - thePosition;
                colPoint.m_vecNormal = CVector(fNormalX, fNormalY, 0.0f);
                ApplyFriction(10.0f * fAbsoluteMoveSpeed, &colPoint);
                if (m_nType == ENTITY_TYPE_OBJECT)
                {
                    AudioEngine.ReportMissionAudioEvent(AE_CAS4_FE, pObject);
                    pObject->m_nLastWeaponDamage = 4 * (pObject->m_nLastWeaponDamage == -1) + 50;
                }
            }
        }
        ApplyMoveSpeed();
        ApplyTurnSpeed();
        CTimer::UpdateTimeStep(fOldTimeStep);
        return;
    }

    if (!physicalFlags.bDisableMoveForce || m_nType != ENTITY_TYPE_OBJECT || pObject->m_fDoorStartAngle <= -1000.0f) {
        ApplyMoveSpeed();
        ApplyTurnSpeed();
        CTimer::UpdateTimeStep(fOldTimeStep);
        return;
    }

    float fDoorStartAngle = pObject->m_fDoorStartAngle;
    float fHeading = GetHeading();
    if (fDoorStartAngle + PI >= fHeading) {
        if (fDoorStartAngle - PI > fHeading)
            fHeading += PI * 2;
    }
    else {
        fHeading -= PI * 2;
    }

    float fNewTimeStep = -1000.0f;
    float fTheDoorAngle  = DegreesToRadians(108.0f) + fDoorStartAngle;
    float fHeadingTimeStep = CTimer::ms_fTimeStep * m_vecTurnSpeed.z + fHeading;
    if (m_vecTurnSpeed.z <= 0.0f || fHeadingTimeStep <= DegreesToRadians(108.0f) + fDoorStartAngle) {
        if (m_vecTurnSpeed.z < 0.0f) {
            float fTheDoorAngle = fDoorStartAngle - DegreesToRadians(108.0f);
            if (fHeadingTimeStep < fTheDoorAngle)
                fNewTimeStep = (fTheDoorAngle - fHeading) / m_vecTurnSpeed.z;
        }
    }
    else
    {
        fNewTimeStep = (fTheDoorAngle - fHeading) / m_vecTurnSpeed.z;
    }
    if (-CTimer::ms_fTimeStep <= fNewTimeStep) {
        CTimer::UpdateTimeStep(fNewTimeStep);
        ApplyTurnSpeed();
        m_vecTurnSpeed.z = -0.2f * m_vecTurnSpeed.z;
        CTimer::UpdateTimeStep(fOldTimeStep - fNewTimeStep);
        physicalFlags.b31 = true;
    }
    ApplyMoveSpeed();
    ApplyTurnSpeed();
    CTimer::UpdateTimeStep(fOldTimeStep);
    if (pObject->objectFlags.bIsDoorMoving) {
        float fNewHeading = GetHeading();
        if (fNewHeading + PI >= fDoorStartAngle) {
            if (fNewHeading - PI > fDoorStartAngle)
                fNewHeading -= PI * 2;
        }
        else {
            fNewHeading += PI * 2;
        }

        fHeading -= fDoorStartAngle;
        fNewHeading -= fDoorStartAngle;
        if (fabs(fHeading) < 0.001f)
            fHeading = 0.0f;
        if (fabs(fNewHeading) < 0.001f)
            fNewHeading = 0.0f;
        if (fHeading * fNewHeading < 0.0f)
            m_vecTurnSpeed.z = 0.0f;
    }
#endif
}


void CPhysical::UnsetIsInSafePosition()
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*))0x548320)(this);
#else
    m_vecMoveSpeed *= -1.0f;
    m_vecTurnSpeed *= -1.0f;
    ApplySpeed();
    m_vecMoveSpeed *= -1.0f;
    m_vecTurnSpeed *= -1.0f;
    m_bIsInSafePosition = false;
#endif
}

void CPhysical::ApplyFriction()
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((void(__thiscall*)(CPhysical*))0x5483D0)(this);
#else
    if (physicalFlags.bDisableZ)
    {
        const CVector& vecPosition = GetPosition();
        float fSphereRadius = CModelInfo::ms_modelInfoPtrs[m_nModelIndex]->m_pColModel->m_boundSphere.m_fRadius;
        CColPoint colPoint;
        colPoint.m_vecPoint.x = vecPosition.x - (0.0f * fSphereRadius);
        colPoint.m_vecPoint.y = vecPosition.y - (0.0f * fSphereRadius);
        colPoint.m_vecPoint.z = vecPosition.z - fSphereRadius;
        colPoint.m_vecNormal = CVector (0.0f, 0.0f, 1.0f);

        ApplyFriction(CTimer::ms_fTimeStep * 0.001f, &colPoint);
        m_vecTurnSpeed.z = pow(0.98f, CTimer::ms_fTimeStep) * m_vecTurnSpeed.z;
    }

    m_vecMoveSpeed += m_vecFrictionMoveSpeed;
    m_vecTurnSpeed += m_vecFrictionTurnSpeed;
    m_vecFrictionMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
    m_vecFrictionTurnSpeed = CVector(0.0f, 0.0f, 0.0f);

    CVehicle* pVehicle = static_cast<CVehicle*>(this);
    if (m_nType == ENTITY_TYPE_VEHICLE && pVehicle->m_nVehicleClass == VEHICLE_BIKE
        && !physicalFlags.b32 && m_nStatus == STATUS_ABANDONED 
        && fabs(GetUp().z) < 0.707f
        && 0.05f * 0.05f > m_vecMoveSpeed.SquaredMagnitude() && 0.01f * 0.01f > m_vecTurnSpeed.SquaredMagnitude())
    {                                    
        m_vecMoveSpeed *= pow(0.5f, CTimer::ms_fTimeStep);
    }
#endif
}

// 0x548680
bool CPhysical::ApplyCollision(CEntity* pTheEntity, CColPoint* pColPoint, float* pThisDamageIntensity, float* pEntityDamageIntensity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CEntity*, CColPoint*, float*, float*))0x548680)(this, pTheEntity, pColPoint, pThisDamageIntensity, pEntityDamageIntensity);
#else
    CPhysical* pEntity = static_cast<CPhysical*>(pTheEntity);
    auto pEntityObject = static_cast<CObject*>(pEntity);
    auto pEntityPed = static_cast<CPed*>(pEntity);
    auto pEntityVehicle = static_cast<CVehicle*>(pEntity);

    auto pThisObject = static_cast<CObject*>(this);
    auto pThisPed = static_cast<CPed*>(this);
    auto pThisVehicle = static_cast<CVehicle*>(this);

    bool bThisPedIsStandingOnEntity = false;
    bool bEntityPedIsStandingOnThis = false;
    bool bEntityCollisionForceDisabled = false;

    float fThisMassFactor = 0.0;
    float fEntityMassFactor = 1.0f;

    if (!pEntity->physicalFlags.bDisableTurnForce || physicalFlags.bDisableMoveForce)
    {
        fThisMassFactor = 2.0f;
        if (!physicalFlags.bMakeMassTwiceAsBig)
        {
            fThisMassFactor = 1.0f;
        }
    }
    else
    {
        fThisMassFactor = 10.0f;
        if (pEntity->m_nType == ENTITY_TYPE_PED && pEntityPed->m_standingOnEntity == this)
        {
            bEntityPedIsStandingOnThis = true;
        }
    }

    if (physicalFlags.bDisableTurnForce)
    {
        if (m_nType == ENTITY_TYPE_PED && pThisPed->IsPlayer()
            && pEntity->m_nType == ENTITY_TYPE_VEHICLE
            && (pEntity->m_nStatus == STATUS_ABANDONED || pEntity->m_nStatus == STATUS_WRECKED || m_bIsStuck))
        {
            float fTheEntityMass = pEntity->m_fMass - 2000.0f;
            if (fTheEntityMass < 0.0f)
            {
                fTheEntityMass = 0.0f;
            }
            fEntityMassFactor = 1.0f / (fTheEntityMass * 0.00019999999f + 1.0f);
        }
        else if (!pEntity->physicalFlags.bDisableMoveForce)
        {
            fEntityMassFactor = 10.0f;
        }
        if (m_nType == ENTITY_TYPE_PED && pThisPed->m_standingOnEntity == pEntity)
        {
            bThisPedIsStandingOnEntity = true;
            fEntityMassFactor = 10.0f;
        }
    }
    else if (m_nType == ENTITY_TYPE_VEHICLE && pThisVehicle->m_pTrailer)
    {
        fEntityMassFactor = (pThisVehicle->m_pTrailer->m_fMass + m_fMass) / m_fMass;
    }
    else
    {
        fEntityMassFactor = 2.0f;
        if (!pEntity->physicalFlags.bMakeMassTwiceAsBig)
        {
            fEntityMassFactor = 1.0f;
        }
    }

    if (pEntity->physicalFlags.bDisableCollisionForce && !pEntity->physicalFlags.bCollidable
        || pEntity->m_pAttachedTo && !pEntity->physicalFlags.bInfiniteMass)
    {
        bEntityCollisionForceDisabled = true;
        bThisPedIsStandingOnEntity = false;
    }

    CVector vecThisCentreOfMassMultiplied;
    CVector vecEntityCentreOfMassMultiplied;

    Multiply3x3(&vecThisCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
    Multiply3x3(&vecEntityCentreOfMassMultiplied, pEntity->m_matrix, &pEntity->m_vecCentreOfMass);

    if (physicalFlags.bInfiniteMass)
    {
        vecThisCentreOfMassMultiplied = CVector(0.0f, 0.0f, 0.0f);
    }

    if (pEntity->physicalFlags.bInfiniteMass)
    {
        vecEntityCentreOfMassMultiplied = CVector(0.0f, 0.0f, 0.0f);
    }

    if (m_nType == ENTITY_TYPE_VEHICLE && pEntity->m_nType == ENTITY_TYPE_OBJECT
        && pEntityObject->objectFlags.bIsLampPost
        || pEntity->m_nType == ENTITY_TYPE_VEHICLE && m_nType == ENTITY_TYPE_OBJECT &&
        pThisObject->objectFlags.bIsLampPost)
    {
        pColPoint->m_vecNormal.z = 0.0f;
    }

    if ((pEntity->m_bIsStatic || pEntity->m_bIsStaticWaitingForCollision) && !bEntityCollisionForceDisabled)
    {
        if (physicalFlags.bDisableTurnForce)
        {
            float fThisSpeedDotProduct = m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                + m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                + pColPoint->m_vecNormal.x * m_vecMoveSpeed.x;
            if (fThisSpeedDotProduct < 0.0f)
            {
                if (pEntity->m_nType != ENTITY_TYPE_OBJECT)
                {
                    if (pEntity->physicalFlags.bDisableCollisionForce)
                    {
                        return ApplyCollision(pEntity, pColPoint, pThisDamageIntensity);
                    }

                    pEntity->SetIsStatic(false);
                }
                else
                {
                    *pThisDamageIntensity = -(fThisSpeedDotProduct * m_fMass);
                    *pEntityDamageIntensity = *pThisDamageIntensity;

                    if (pEntity->physicalFlags.bDisableCollisionForce)
                    {
                        return ApplyCollision(pEntity, pColPoint, pThisDamageIntensity);
                    }

                    CObjectInfo* pEntityObjectInfo = pEntityObject->m_pObjectInfo;
                    if (pEntityObjectInfo->m_fUprootLimit >= 9999.0f || *pThisDamageIntensity <= pEntityObjectInfo->m_fUprootLimit)
                    {
                        return ApplyCollision(pEntity, pColPoint, pThisDamageIntensity);
                    }

                    if (IsGlassModel(pEntity))
                    {
                        CGlass::WindowRespondsToCollision(pEntity, *pThisDamageIntensity, m_vecMoveSpeed, pColPoint->m_vecPoint, false);
                    }
                    else
                    {
                        if (!pEntity->physicalFlags.bDisableCollisionForce)
                        {
                            pEntity->SetIsStatic(false);
                            CWorld::Players[CWorld::PlayerInFocus].m_nHavocCaused += 2;
                            CStats::IncrementStat(STAT_COST_OF_PROPERTY_DAMAGED, static_cast<float>(rand() % 30 + 30));
                        }
                    }
                }
            }
        }
        else
        {
            CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
            CVector vecThisSpeed;
            GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

            if (physicalFlags.bAddMovingCollisionSpeed && m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
            {
                CVector outSpeed;
                pThisVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPointFromThis);
                vecThisSpeed += outSpeed;
            }

            float fThisSpeedDotProduct = vecThisSpeed.y * pColPoint->m_vecNormal.y
                + vecThisSpeed.z * pColPoint->m_vecNormal.z
                + vecThisSpeed.x * pColPoint->m_vecNormal.x;
            if (fThisSpeedDotProduct < 0.0f)
            {
                if (pEntity->m_nType != ENTITY_TYPE_OBJECT)
                {
                    if (pEntity->physicalFlags.bDisableCollisionForce)
                    {
                        return ApplyCollision(pEntity, pColPoint, pThisDamageIntensity);
                    }

                    pEntity->SetIsStatic(false);
                    CWorld::Players[CWorld::PlayerInFocus].m_nHavocCaused += 2;
                    CStats::IncrementStat(STAT_COST_OF_PROPERTY_DAMAGED, static_cast<float>(rand() % 30 + 30));
                }
                else
                {
                    CVector vecThisDifference = (vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied);
                    CVector vecThisCrossProduct;
                    vecThisCrossProduct.Cross(vecThisDifference, pColPoint->m_vecNormal);
                    float squaredMagnitude = vecThisCrossProduct.SquaredMagnitude();
                    float fThisCollisionMass = 1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass);
                    if (!m_bHasHitWall)
                    {
                        *pThisDamageIntensity = -((m_fElasticity + 1.0f) * fThisCollisionMass * fThisSpeedDotProduct);
                    }
                    else
                    {
                        *pThisDamageIntensity = fThisCollisionMass * fThisSpeedDotProduct * -1.0f;
                    }

                    *pEntityDamageIntensity = *pThisDamageIntensity;

                    CObjectInfo* pEntityObjectInfo = pEntityObject->m_pObjectInfo;

                    float fObjectDamageMultiplier = 1.0f;
                    if (m_nType == ENTITY_TYPE_VEHICLE && pThisVehicle->m_nVehicleSubClass == VEHICLE_BIKE)
                    {
                        fObjectDamageMultiplier = 3.0f;
                    }

                    if (pEntityObject->m_nColDamageEffect)
                    {
                        float fObjectDamage = fObjectDamageMultiplier * *pThisDamageIntensity;
                        if (fObjectDamage > 20.0f)
                        {
                            pEntityObject->ObjectDamage(fObjectDamage, &pColPoint->m_vecPoint, &pColPoint->m_vecNormal, this, WEAPON_UNIDENTIFIED);
                            if (!pEntity->m_bUsesCollision)
                            {
                                if (!physicalFlags.bDisableCollisionForce)
                                {
                                    float fColDamageMultiplier = pEntityObjectInfo->m_fColDamageMultiplier;
                                    float fCollisionDamage = fColDamageMultiplier + fColDamageMultiplier;
                                    CVector vecMoveForce = (pColPoint->m_vecNormal * *pThisDamageIntensity) / fCollisionDamage;
                                    ApplyForce(vecMoveForce, vecDistanceToPointFromThis, true);
                                }

                                float fDamageIntensityMultiplier = pEntityObjectInfo->m_fColDamageMultiplier / fThisCollisionMass;
                                float fCollisionImpact1 = fDamageIntensityMultiplier * *pThisDamageIntensity + fDamageIntensityMultiplier * *pThisDamageIntensity;

                                AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                                    &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                                return false;
                            }
                        }
                    }
                    if (pEntity->physicalFlags.bDisableCollisionForce || pEntityObjectInfo->m_fUprootLimit >= 9999.0f
                        || *pThisDamageIntensity <= pEntityObjectInfo->m_fUprootLimit && (!m_bIsStuck || !m_bHasHitWall))
                    {
                        if (IsGlassModel(pEntity))
                        {
                            CGlass::WindowRespondsToSoftCollision(pEntityObject, *pThisDamageIntensity);
                        }
                        return ApplyCollision(pEntity, pColPoint, pThisDamageIntensity);
                    }

                    if (IsGlassModel(pEntity))
                    {
                        CGlass::WindowRespondsToCollision(pEntity, *pThisDamageIntensity, m_vecMoveSpeed, pColPoint->m_vecPoint, false);
                    }
                    else
                    {
                        pEntity->SetIsStatic(false);
                    }

                    int entityModelIndex = pEntity->m_nModelIndex;
                    if (entityModelIndex != MI_FIRE_HYDRANT || pEntityObject->objectFlags.bIsExploded)
                    {
                        if (entityModelIndex != MI_PARKINGMETER && entityModelIndex != MI_PARKINGMETER2 || pEntityObject->objectFlags.bIsExploded)
                        {
                            if (pEntity->m_nType != ENTITY_TYPE_OBJECT || pEntityObjectInfo->m_bCausesExplosion)
                            {
                                // nothing
                            }
                            else
                            {
                                pEntityObject->objectFlags.bIsExploded = true;
                            }
                        }
                        else
                        {
                            CPickups::CreateSomeMoney(pEntity->GetPosition(), rand() % 100);
                            pEntityObject->objectFlags.bIsExploded = true;
                        }
                    }
                    else
                    {
                        g_fx.TriggerWaterHydrant(pEntity->GetPosition());
                        pEntityObject->objectFlags.bIsExploded = true;
                    }

                    if (!physicalFlags.bDisableCollisionForce && pEntityObjectInfo->m_fUprootLimit > 200.0f)
                    {
                        CVector vecMoveForce = (pColPoint->m_vecNormal * 0.2f) * *pThisDamageIntensity;
                        ApplyForce(vecMoveForce, vecDistanceToPointFromThis, true);
                    }
                }
            }
        }

        if (pEntity->m_bIsStatic || pEntity->m_bIsStaticWaitingForCollision)
        {
            return false;
        }
        if (!pEntity->physicalFlags.bDisableCollisionForce)
        {
            pEntity->AddToMovingList();
        }
    }

    if (physicalFlags.bDisableTurnForce)
    {
        if (pEntity->physicalFlags.bDisableTurnForce)
        {
            bool bApplyEntityCollisionForce = true;
            float fThisMass = m_fMass;
            float fEntityMass = pEntity->m_fMass;
            float fThisSpeedDotProduct = m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                + m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                + m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;
            float fEntitySpeedDotProduct = pEntity->m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                + pEntity->m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                + pEntity->m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;
            float fMoveSpeed = 0.0f;
            if (physicalFlags.bDisableCollisionForce || physicalFlags.bDontApplySpeed)
            {
                fMoveSpeed = fThisSpeedDotProduct;
            }
            else
            {
                if (pEntity->physicalFlags.bDisableCollisionForce || pEntity->physicalFlags.bDontApplySpeed)
                {
                    fMoveSpeed = fEntitySpeedDotProduct;
                    bApplyEntityCollisionForce = false;
                }
                else
                {
                    if (!pThisPed->bPedThirdFlags32) // Hmm?? Is this a bug?
                    {
                        if (fEntitySpeedDotProduct >= 0.0f)
                        {
                            fMoveSpeed = 0.0f;
                        }
                        else
                        {
                            fMoveSpeed = fEntitySpeedDotProduct;
                        }

                        bApplyEntityCollisionForce = false;
                    }
                    else
                    {
                        fMoveSpeed = (fThisMass * fThisSpeedDotProduct * 4.0f + fEntityMass * fEntitySpeedDotProduct) / (fThisMass * 4.0f + fEntityMass);
                    }
                }
            }

            float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
            if (fThisMoveSpeedDifference >= 0.0f)
            {
                return false;
            }

            float fThisMoveSpeedElasticity = 0.0f;
            float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
            if (m_bHasHitWall)
            {
                fThisMoveSpeedElasticity = fMoveSpeed;
            }
            else
            {
                fThisMoveSpeedElasticity = fMoveSpeed - fTheElasticity * fThisMoveSpeedDifference;
            }

            *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisMass;

            CVector vecThisMoveForce = pColPoint->m_vecNormal * *pThisDamageIntensity;
            if (!physicalFlags.bDisableCollisionForce && !physicalFlags.bDontApplySpeed)
            {
                ApplyMoveForce(vecThisMoveForce);

                float fCollisionImpact1 = *pThisDamageIntensity / fThisMass;
                AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                    &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            }

            if (bApplyEntityCollisionForce)
            {
                float fEntityMoveSpeedElasticity = 0.0f;
                if (pEntity->m_bHasHitWall)
                {
                    fEntityMoveSpeedElasticity = fMoveSpeed;
                }
                else
                {
                    fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
                }

                *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityMass);


                CVector vecEntityMoveForce = pColPoint->m_vecNormal * *pEntityDamageIntensity * -1.0f;
                if (!pEntity->physicalFlags.bDisableCollisionForce && !pEntity->physicalFlags.bDontApplySpeed)
                {
                    if (pEntity->m_bIsInSafePosition)
                    {
                        pEntity->UnsetIsInSafePosition();
                    }

                    pEntity->ApplyMoveForce(vecEntityMoveForce);

                    float fCollisionImpact1 = *pEntityDamageIntensity / fEntityMass;
                    AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                }
            }
            return true;
        }
    }

    if (physicalFlags.bDisableTurnForce)
    {
        CVector vecDistanceToPoint = pColPoint->m_vecPoint - pEntity->GetPosition();
        CVector vecEntitySpeed;
        pEntity->GetSpeed(&vecEntitySpeed, vecDistanceToPoint);

        if (!pEntity->physicalFlags.bAddMovingCollisionSpeed || pEntity->m_nType != ENTITY_TYPE_VEHICLE || pColPoint->m_nSurfaceTypeB != SURFACE_CAR_MOVINGCOMPONENT)
        {
            // nothing
        }
        else
        {
            CVector outSpeed;
            pEntityVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPoint);
            vecEntitySpeed += outSpeed;
        }

        float fThisSpeedDotProduct = m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
            + m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
            + m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;
        float fEntitySpeedDotProduct = vecEntitySpeed.z * pColPoint->m_vecNormal.z
            + vecEntitySpeed.y * pColPoint->m_vecNormal.y
            + vecEntitySpeed.x * pColPoint->m_vecNormal.x;

        float fThisMass = fThisMassFactor * m_fMass;

        CVector vecEntityDifference = (vecDistanceToPoint - vecEntityCentreOfMassMultiplied);
        CVector vecEntityCrossProduct;
        vecEntityCrossProduct.Cross(vecEntityDifference, pColPoint->m_vecNormal);
        float squaredMagnitude = vecEntityCrossProduct.SquaredMagnitude();
        float fEntityCollisionMass = 0.0f;
        if (pEntity->physicalFlags.bDisableMoveForce)
        {
            fEntityCollisionMass = squaredMagnitude / (fEntityMassFactor * pEntity->m_fTurnMass);
        }
        else
        {
            fEntityCollisionMass = squaredMagnitude / (fEntityMassFactor * pEntity->m_fTurnMass) + 1.0f / (fEntityMassFactor * pEntity->m_fMass);
        }

        fEntityCollisionMass = 1.0f / fEntityCollisionMass;

        float fMoveSpeed = 0.0f;
        if (bEntityCollisionForceDisabled)
        {
            fMoveSpeed = fEntitySpeedDotProduct;
        }
        else
        {
            fMoveSpeed = (fEntityCollisionMass * fEntitySpeedDotProduct + fThisMass * fThisSpeedDotProduct) / (fEntityCollisionMass + fThisMass);
        }

        float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
        if (fThisMoveSpeedDifference < 0.0f)
        {
            float fThisMoveSpeedElasticity = 0.0f;
            float fEntityMoveSpeedElasticity = 0.0f;

            float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
            if (m_bHasHitWall)
            {
                fThisMoveSpeedElasticity = fMoveSpeed;
            }
            else
            {
                fThisMoveSpeedElasticity = fMoveSpeed - fTheElasticity * fThisMoveSpeedDifference;
            }

            if (pEntity->m_bHasHitWall)
            {
                fEntityMoveSpeedElasticity = fMoveSpeed;
            }
            else
            {
                fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
            }

            *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisMass;
            *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityCollisionMass);

            CVector vecThisMoveForce = (*pThisDamageIntensity / fThisMassFactor) * pColPoint->m_vecNormal;
            CVector vecEntityMoveForce = (pColPoint->m_vecNormal * (*pEntityDamageIntensity / fEntityMassFactor) * -1.0f);

            if (!physicalFlags.bDisableCollisionForce)
            {
                if (vecThisMoveForce.z < 0.0f)
                {
                    vecThisMoveForce.z = 0.0f;
                }
                if (bThisPedIsStandingOnEntity)
                {
                    vecThisMoveForce.x = vecThisMoveForce.x + vecThisMoveForce.x;
                    vecThisMoveForce.y = vecThisMoveForce.y + vecThisMoveForce.y;
                }

                ApplyMoveForce(vecThisMoveForce);
            }
            if (!pEntity->physicalFlags.bDisableCollisionForce && !bThisPedIsStandingOnEntity)
            {
                if (pEntity->m_bIsInSafePosition)
                {
                    pEntity->UnsetIsInSafePosition();
                }
                pEntity->ApplyForce(vecEntityMoveForce, vecDistanceToPoint, true);
            }

            float fCollisionImpact1 = *pThisDamageIntensity / fThisMass;
            AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);

            fCollisionImpact1 = *pEntityDamageIntensity / fEntityCollisionMass;
            AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
                &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            return true;
        }
        return false;
    }

    if (pEntity->physicalFlags.bDisableTurnForce)
    {
        CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
        CVector vecThisSpeed;
        GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

        if (!physicalFlags.bAddMovingCollisionSpeed && m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
        {
            CVector outSpeed;
            pThisVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPointFromThis);
            vecThisSpeed += outSpeed;
        }

        float fThisSpeedDotProduct = vecThisSpeed.z * pColPoint->m_vecNormal.z
            + vecThisSpeed.y * pColPoint->m_vecNormal.y
            + vecThisSpeed.x * pColPoint->m_vecNormal.x;
        float fEntitySpeedDotProduct = pEntity->m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
            + pEntity->m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
            + pColPoint->m_vecNormal.x * pEntity->m_vecMoveSpeed.x;

        CVector vecThisDifference = (vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied);
        CVector vecThisCrossProduct;
        vecThisCrossProduct.Cross(vecThisDifference, pColPoint->m_vecNormal);
        float squaredMagnitude = vecThisCrossProduct.SquaredMagnitude();
        float fThisCollisionMass = 0.0f;
        if (physicalFlags.bDisableMoveForce)
        {
            fThisCollisionMass = squaredMagnitude / (fThisMassFactor * m_fTurnMass);
        }
        else
        {
            fThisCollisionMass = squaredMagnitude / (fThisMassFactor * m_fTurnMass) + 1.0f / (fThisMassFactor * m_fMass);
        }

        fThisCollisionMass = 1.0f / fThisCollisionMass;

        float fEntityMass = fEntityMassFactor * pEntity->m_fMass;
        float fMoveSpeed = (fEntityMass * fEntitySpeedDotProduct + fThisCollisionMass * fThisSpeedDotProduct) / (fEntityMass + fThisCollisionMass);
        float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
        if (fThisMoveSpeedDifference >= 0.0f)
        {
            return false;
        }

        float fThisMoveSpeedElasticity = 0.0f;
        float fEntityMoveSpeedElasticity = 0.0f;

        float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
        if (m_bHasHitWall)
        {
            fThisMoveSpeedElasticity = fMoveSpeed;
        }
        else
        {
            fThisMoveSpeedElasticity = fMoveSpeed - fTheElasticity * fThisMoveSpeedDifference;
        }

        if (pEntity->m_bHasHitWall)
        {
            fEntityMoveSpeedElasticity = fMoveSpeed;
        }
        else
        {
            fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
        }

        *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisCollisionMass;
        *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityMass);

        CVector vecThisMoveForce = pColPoint->m_vecNormal * (*pThisDamageIntensity / fThisMassFactor);
        CVector vecEntityMoveForce = pColPoint->m_vecNormal * (*pEntityDamageIntensity / fEntityMassFactor) * -1.0f;

        if (!physicalFlags.bDisableCollisionForce && !bEntityPedIsStandingOnThis)
        {
            if (vecThisMoveForce.z < 0.0f)
            {
                vecThisMoveForce.z = 0.0f;
            }
            ApplyForce(vecThisMoveForce, vecDistanceToPointFromThis, true);
        }

        if (!pEntity->physicalFlags.bDisableCollisionForce)
        {
            if (vecEntityMoveForce.z < 0.0f)
            {
                vecEntityMoveForce.z = 0.0f;
                if (fabs(fThisSpeedDotProduct) < 0.01f)
                {
                    vecEntityMoveForce.z = 0.0f;
                    vecEntityMoveForce.x = vecEntityMoveForce.x * 0.5f;
                    vecEntityMoveForce.y = vecEntityMoveForce.y * 0.5f;
                }
            }
            if (bEntityPedIsStandingOnThis)
            {
                vecEntityMoveForce.x = vecEntityMoveForce.x + vecEntityMoveForce.x;
                vecEntityMoveForce.y = vecEntityMoveForce.y + vecEntityMoveForce.y;
            }
            if (pEntity->m_bIsInSafePosition)
            {
                pEntity->UnsetIsInSafePosition();
            }

            pEntity->ApplyMoveForce(vecEntityMoveForce);
        }

        float fCollisionImpact1 = *pThisDamageIntensity / fThisCollisionMass;
        AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
            &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);

        fCollisionImpact1 = *pEntityDamageIntensity / fEntityMass;
        AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
            &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
    }
    else
    {
        CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
        CVector vecThisSpeed;
        GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

        if (physicalFlags.bAddMovingCollisionSpeed && m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
        {
            CVector outSpeed;
            pThisVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPointFromThis);
            vecThisSpeed += outSpeed;
        }

        CVector vecDistanceToPoint = pColPoint->m_vecPoint - pEntity->GetPosition();
        CVector vecEntitySpeed;
        pEntity->GetSpeed(&vecEntitySpeed, vecDistanceToPoint);

        if (pEntity->physicalFlags.bAddMovingCollisionSpeed && pEntity->m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeB == SURFACE_CAR_MOVINGCOMPONENT)
        {
            CVector outSpeed;
            pEntityVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPoint);
            vecEntitySpeed += outSpeed;
        }

        float fThisSpeedDotProduct = vecThisSpeed.z * pColPoint->m_vecNormal.z
            + vecThisSpeed.y * pColPoint->m_vecNormal.y
            + vecThisSpeed.x * pColPoint->m_vecNormal.x;
        float fEntitySpeedDotProduct = vecEntitySpeed.z * pColPoint->m_vecNormal.z
            + vecEntitySpeed.y * pColPoint->m_vecNormal.y
            + vecEntitySpeed.x * pColPoint->m_vecNormal.x;

        CVector vecThisDifference = (vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied);
        CVector vecThisCrossProduct;
        vecThisCrossProduct.Cross(vecThisDifference, pColPoint->m_vecNormal);
        float squaredMagnitude = vecThisCrossProduct.SquaredMagnitude();

        float fThisCollisionMass = 0.0f;
        if (physicalFlags.bDisableMoveForce)
        {
            fThisCollisionMass = squaredMagnitude / (fThisMassFactor * m_fTurnMass);
        }
        else
        {
            fThisCollisionMass = squaredMagnitude / (fThisMassFactor * m_fTurnMass) + 1.0f / (fThisMassFactor * m_fMass);
        }

        fThisCollisionMass = 1.0f / fThisCollisionMass;

        CVector vecEntityDifference = (vecDistanceToPoint - vecEntityCentreOfMassMultiplied);
        CVector vecEntityCrossProduct;
        vecEntityCrossProduct.Cross(vecEntityDifference, pColPoint->m_vecNormal);
        squaredMagnitude = vecEntityCrossProduct.SquaredMagnitude();

        float fEntityCollisionMass = 0.0f;
        if (pEntity->physicalFlags.bDisableMoveForce)
        {
            fEntityCollisionMass = squaredMagnitude / (fEntityMassFactor * pEntity->m_fTurnMass);
        }
        else
        {
            fEntityCollisionMass = squaredMagnitude / (fEntityMassFactor * pEntity->m_fTurnMass) + 1.0f / (fEntityMassFactor * pEntity->m_fMass);
        }

        fEntityCollisionMass = 1.0f / fEntityCollisionMass;

        float fMoveSpeed = (fEntityCollisionMass * fEntitySpeedDotProduct + fThisCollisionMass * fThisSpeedDotProduct) / (fEntityCollisionMass + fThisCollisionMass);
        float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
        if (fThisMoveSpeedDifference >= 0.0f)
        {
            return false;
        }

        float fThisMoveSpeedElasticity = 0.0f;
        float fEntityMoveSpeedElasticity = 0.0f;
        float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
        if (m_bHasHitWall)
        {
            fThisMoveSpeedElasticity = fMoveSpeed;
        }
        else
        {
            fThisMoveSpeedElasticity = fMoveSpeed - fTheElasticity * fThisMoveSpeedDifference;
        }

        if (pEntity->m_bHasHitWall)
        {
            fEntityMoveSpeedElasticity = fMoveSpeed;
        }
        else
        {
            fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
        }

        *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisCollisionMass;
        *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityCollisionMass);

        CVector vecThisMoveForce = pColPoint->m_vecNormal * (*pThisDamageIntensity / fThisMassFactor);
        CVector vecEntityMoveForce = pColPoint->m_vecNormal * (*pEntityDamageIntensity / fEntityMassFactor) * -1.0f;

        if (m_nType == ENTITY_TYPE_VEHICLE && !m_bHasHitWall && !physicalFlags.bDisableCollisionForce)
        {
            if (pColPoint->m_vecNormal.z < 0.69999999f)
            {
                vecThisMoveForce.z *= 0.30000001f;
            }

            if (!m_nStatus)
            {
                vecDistanceToPointFromThis *= 0.80000001f;
            }

            if (CWorld::bNoMoreCollisionTorque)
            {
                CVector vecFrictionForce = vecThisMoveForce * -0.30000001f;
                ApplyFrictionForce(vecFrictionForce, vecDistanceToPointFromThis);
            }
        }

        if (pEntity->m_nType == ENTITY_TYPE_VEHICLE && !pEntity->m_bHasHitWall && !pEntity->physicalFlags.bDisableCollisionForce)
        {
            if ((pColPoint->m_vecNormal.z * -1.0f) < 0.69999999f)
            {
                vecEntityMoveForce.z *= 0.30000001f;
            }

            if (!pEntity->m_nStatus)
            {
                vecDistanceToPoint *= 0.80000001f;
            }

            if (CWorld::bNoMoreCollisionTorque)
            {
                CVector vecFrictionForce = vecEntityMoveForce * -0.30000001f;
                pEntity->ApplyFrictionForce(vecFrictionForce, vecDistanceToPoint);
            }
        }

        if (CCheat::m_aCheatsActive[CHEAT_CARS_FLOAT_AWAY_WHEN_HIT])
        {
            if (FindPlayerVehicle(-1, false) == pThisVehicle
                && pEntity->m_nType == ENTITY_TYPE_VEHICLE && pEntityVehicle->m_nCreatedBy != MISSION_VEHICLE)
            {
                pEntity->physicalFlags.bApplyGravity = false;
            }
            if (FindPlayerVehicle(-1, 0) == pEntityVehicle
                && m_nType == ENTITY_TYPE_VEHICLE && pThisVehicle->m_nCreatedBy != MISSION_VEHICLE)
            {
                physicalFlags.bApplyGravity = false;
            }
        }

        if (!physicalFlags.bDisableCollisionForce)
        {
            ApplyForce(vecThisMoveForce, vecDistanceToPointFromThis, true);
        }

        if (!pEntity->physicalFlags.bDisableCollisionForce)
        {
            if (pEntity->m_bIsInSafePosition)
            {
                pEntity->UnsetIsInSafePosition();
            }
            pEntity->ApplyForce(vecEntityMoveForce, vecDistanceToPoint, true);
        }

        float fCollisionImpact1 = *pThisDamageIntensity / fThisCollisionMass;
        AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
            &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);

        fCollisionImpact1 = *pEntityDamageIntensity / fEntityCollisionMass;
        AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
            &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
    }
    return true;
#endif
}

bool CPhysical::ApplySoftCollision(CPhysical* pEntity, CColPoint* pColPoint, float* pThisDamageIntensity, float* pEntityDamageIntensity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CPhysical*, CColPoint*, float*, float*))0x54A2C0)(this, pEntity, pColPoint, pThisDamageIntensity, pEntityDamageIntensity);
#else
    auto pEntityObject = static_cast<CObject*>(pEntity);
    auto pEntityPed = static_cast<CPed*>(pEntity);
    auto pEntityVehicle = static_cast<CVehicle*>(pEntity);

    auto pThisObject = static_cast<CObject*>(this);
    auto pThisPed = static_cast<CPed*>(this);
    auto pThisVehicle = static_cast<CVehicle*>(this);

    bool bEntityPedIsStandingOnThis = false;
    bool bThisPedIsStandingOnEntity = false;
    bool bEntityCollisionForceDisabled = false;

    float fThisMassFactor = 0.0;
    float fEntityMassFactor = 0.0f;
    if (!pEntity->physicalFlags.bDisableTurnForce || physicalFlags.bDisableMoveForce || physicalFlags.bInfiniteMass)
    {
        fThisMassFactor = 2.0f;
        if (!physicalFlags.bMakeMassTwiceAsBig)
        {
            fThisMassFactor = 1.0f;
        }
    }
    else
    {
        fThisMassFactor = 10.0f;
        if (pEntityPed->m_standingOnEntity == this) // BUG: Game should be checking if entity is ped or not.
        {
            bEntityPedIsStandingOnThis = true;
        }
    }
    if (!physicalFlags.bDisableTurnForce || pEntity->physicalFlags.bDisableMoveForce || pEntity->physicalFlags.bInfiniteMass)
    {
        fEntityMassFactor = 2.0f;
        if (!pEntity->physicalFlags.bMakeMassTwiceAsBig)
        {
            fEntityMassFactor = 1.0f;
        }
    }
    else
    {
        if (m_nType == ENTITY_TYPE_PED && pThisPed->IsPlayer()
            && pEntity->m_nType == ENTITY_TYPE_VEHICLE
            && (pEntity->m_nStatus == STATUS_ABANDONED || pEntity->m_nStatus == STATUS_WRECKED || m_bIsStuck))
        {
            float fTheEntityMass = pEntity->m_fMass - 2000.0f;
            if (fTheEntityMass < 0.0f)
            {
                fTheEntityMass = 0.0f;
            }
            fEntityMassFactor = 1.0f / (fTheEntityMass * 0.00019999999f + 1.0f);
        }
        else
        {
            fEntityMassFactor = 10.0f;
        }
        if (m_nType == ENTITY_TYPE_PED && pThisPed->m_standingOnEntity == pEntity)
        {
            bThisPedIsStandingOnEntity = true;
        }
    }

    if (pEntity->physicalFlags.bDisableCollisionForce && !pEntity->physicalFlags.bCollidable)
    {
        bEntityCollisionForceDisabled = true;
        bThisPedIsStandingOnEntity = false;
    }

    CVector vecThisCentreOfMassMultiplied;
    CVector vecEntityCentreOfMassMultiplied;

    Multiply3x3(&vecThisCentreOfMassMultiplied, m_matrix, &m_vecCentreOfMass);
    Multiply3x3(&vecEntityCentreOfMassMultiplied, pEntity->m_matrix, &pEntity->m_vecCentreOfMass);

    if (physicalFlags.bInfiniteMass)
    {
        vecThisCentreOfMassMultiplied = CVector(0.0f, 0.0f, 0.0f);
    }
    if (pEntity->physicalFlags.bInfiniteMass)
    {
        vecEntityCentreOfMassMultiplied = CVector(0.0f, 0.0f, 0.0f);
    }

    bool bApplyCollisionWithElasticity = false;
    if (!pEntity->m_bIsStatic && !pEntity->m_bIsStaticWaitingForCollision || bEntityCollisionForceDisabled)
    {
        bApplyCollisionWithElasticity = true;
    }
    else
    {
        if (physicalFlags.bDisableTurnForce)
        {
            return ApplySoftCollision(pEntity, pColPoint, pThisDamageIntensity);
        }
    }

    CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
    CVector vecThisSpeed;
    GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

    float fThisSpeedDotProduct = vecThisSpeed.y * pColPoint->m_vecNormal.y +
        vecThisSpeed.z * pColPoint->m_vecNormal.z +
        vecThisSpeed.x * pColPoint->m_vecNormal.x;
    if (!bApplyCollisionWithElasticity && fThisSpeedDotProduct < 0.0f)
    {
        if (pEntity->m_nType != ENTITY_TYPE_OBJECT)
        {
            if (!pEntity->physicalFlags.bDisableCollisionForce)
            {
                pEntity->SetIsStatic(false);
                CWorld::Players[CWorld::PlayerInFocus].m_nHavocCaused += 2;
                CStats::IncrementStat(STAT_COST_OF_PROPERTY_DAMAGED, static_cast<float>(rand() % 30 + 30));
            }
        }
        else
        {
            CVector vecThisDifference = (vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied);
            CVector vecThisCrossProduct;
            vecThisCrossProduct.Cross(vecThisDifference, pColPoint->m_vecNormal);
            float squaredMagnitude = vecThisCrossProduct.SquaredMagnitude();

            float fThisCollisionMass = 1.0f / (squaredMagnitude / m_fTurnMass + 1.0f / m_fMass);
            if (!m_bHasHitWall)
            {
                *pThisDamageIntensity = -((m_fElasticity + 1.0f) * fThisCollisionMass * fThisSpeedDotProduct);
            }
            else
            {
                *pThisDamageIntensity = fThisCollisionMass * fThisSpeedDotProduct * -1.0f;
            }

            *pEntityDamageIntensity = *pThisDamageIntensity;

            if (pEntityObject->m_nColDamageEffect != COL_DAMAGE_EFFECT_NONE && *pThisDamageIntensity > 20.0f)
            {
                pEntityObject->ObjectDamage(*pThisDamageIntensity, &pColPoint->m_vecPoint, &pColPoint->m_vecNormal, this, WEAPON_UNIDENTIFIED);
                if (!pEntity->m_bUsesCollision)
                {
                    if (!physicalFlags.bDisableCollisionForce)
                    {
                        CVector vecMoveSpeed = (pColPoint->m_vecNormal * 0.2f) * *pThisDamageIntensity;
                        ApplyForce(vecMoveSpeed, vecDistanceToPointFromThis, true);
                    }

                    float fCollisionImpact1 = *pThisDamageIntensity / fThisCollisionMass * 0.2f;
                    AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                    return false;
                }
            }

            float fObjectUprootLimit = pEntityObject->m_pObjectInfo->m_fUprootLimit;
            if ((*pThisDamageIntensity > fObjectUprootLimit || m_bIsStuck) && !pEntity->physicalFlags.bDisableCollisionForce)
            {
                if (IsGlassModel(pEntity))
                {
                    CGlass::WindowRespondsToCollision(pEntity, *pThisDamageIntensity, m_vecMoveSpeed, pColPoint->m_vecPoint, false);
                }
                else if (!pEntity->physicalFlags.bDisableCollisionForce)
                {
                    pEntity->SetIsStatic(false);
                }

                int entityModelIndex = pEntity->m_nModelIndex;
                if (entityModelIndex != MODEL_FIRE_HYDRANT || pEntityObject->objectFlags.bIsExploded)
                {
                    if (entityModelIndex != MI_PARKINGMETER && entityModelIndex != MI_PARKINGMETER2 || pEntityObject->objectFlags.bIsExploded)
                    {
                        CBaseModelInfo* pBaseModelInfo = CModelInfo::ms_modelInfoPtrs[entityModelIndex];
                        if (pEntity->m_nType != ENTITY_TYPE_OBJECT || pBaseModelInfo->AsAtomicModelInfoPtr())
                        {
                            // nothing
                        }
                        else
                        {
                            pEntityObject->objectFlags.bIsExploded = true;
                        }
                    }
                    else
                    {
                        CPickups::CreateSomeMoney(pEntity->GetPosition(), rand() % 100);
                        pEntityObject->objectFlags.bIsExploded = true;
                    }
                }
                else
                {
                    g_fx.TriggerWaterHydrant(pEntity->GetPosition());
                    pEntityObject->objectFlags.bIsExploded = true;
                }

                if (pEntity->m_bIsStatic || pEntity->m_bIsStaticWaitingForCollision)
                {
                    return false;
                }

                if (!pEntity->physicalFlags.bDisableCollisionForce)
                {
                    pEntity->AddToMovingList();
                }

                bApplyCollisionWithElasticity = true;
            }
            else
            {
                if (IsGlassModel(pEntity))
                {
                    CGlass::WindowRespondsToSoftCollision(pEntityObject, *pThisDamageIntensity);
                }

                CVector vecThisMoveForce = pColPoint->m_vecNormal * *pThisDamageIntensity;
                if (m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_vecNormal.z < 0.69999999f)
                {
                    vecThisMoveForce.z *= 0.30000001f;
                }

                if (!physicalFlags.bDisableCollisionForce)
                {
                    bool bUpdateTorque = m_nType != ENTITY_TYPE_VEHICLE || !CWorld::bNoMoreCollisionTorque;
                    ApplyForce(vecThisMoveForce, vecDistanceToPointFromThis, bUpdateTorque);
                }

                float fCollisionImpact1 = *pThisDamageIntensity / fThisCollisionMass;
                AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                    &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            }
        }
    }

    if (bApplyCollisionWithElasticity)
    {
        if (physicalFlags.bDisableTurnForce && pEntity->physicalFlags.bDisableTurnForce)
        {
            float fThisSpeedDotProduct = m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                + m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                + m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;
            float fEntitySpeedDotProduct = pEntity->m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                + pEntity->m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                + pEntity->m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;

            float fMoveSpeed = fEntitySpeedDotProduct;
            float fThisMass = m_fMass;
            float fEntityMass = pEntity->m_fMass;
            bool bPedFollowPath = false;
            if (pThisPed->m_nPedState == PEDSTATE_FOLLOW_PATH)
            {
                bPedFollowPath = true;
                fMoveSpeed = (fEntityMass * fEntitySpeedDotProduct + fThisMass * fThisSpeedDotProduct + fThisMass * fThisSpeedDotProduct)
                    / (fThisMass + fThisMass + fEntityMass);
            }
            else if (fEntitySpeedDotProduct >= 0.0f)
            {
                fMoveSpeed = 0.0f;
            }

            float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
            if (fThisMoveSpeedDifference >= 0.0f)
            {
                return false;
            }

            float fThisMoveSpeedElasticity = 0.0f;
            float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
            if (m_bHasHitWall)
            {
                fThisMoveSpeedElasticity = fMoveSpeed;
            }
            else
            {
                fThisMoveSpeedElasticity = fMoveSpeed - fThisMoveSpeedDifference * fTheElasticity;
            }

            *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisMass;
            CVector vecThisMoveForce = pColPoint->m_vecNormal * *pThisDamageIntensity;

            if (!physicalFlags.bDisableCollisionForce)
            {
                ApplyMoveForce(vecThisMoveForce);
            }

            float fCollisionImpact1 = *pThisDamageIntensity / fThisMass;
            AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
            if (bPedFollowPath)
            {
                float fEntityMoveSpeedDifference = fEntitySpeedDotProduct - fMoveSpeed;
                if (fEntityMoveSpeedDifference < 0.0f)
                {
                    float fEntityMoveSpeedElasticity = 0.0f;
                    if (pEntity->m_bHasHitWall)
                    {
                        fEntityMoveSpeedElasticity = fMoveSpeed;
                    }
                    else
                    {
                        fEntityMoveSpeedElasticity = fMoveSpeed - fEntityMoveSpeedDifference * fTheElasticity;
                    }

                    *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityMass);
                    CVector vecEntityMoveForce = pColPoint->m_vecNormal * *pEntityDamageIntensity * -1.0f;
                    if (!pEntity->physicalFlags.bDisableCollisionForce)
                    {
                        pEntity->ApplyMoveForce(vecEntityMoveForce);
                    }

                    float fCollisionImpact1 = *pEntityDamageIntensity / fEntityMass;
                    AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                }
            }
            return true;
        }

        if (physicalFlags.bDisableTurnForce)
        {
            CVector vecDistanceToPoint = pColPoint->m_vecPoint - pEntity->GetPosition();
            CVector vecEntitySpeed;
            pEntity->GetSpeed(&vecEntitySpeed, vecDistanceToPoint);

            if (!pEntity->physicalFlags.bAddMovingCollisionSpeed || pEntity->m_nType != ENTITY_TYPE_VEHICLE
                || pColPoint->m_nSurfaceTypeB != SURFACE_CAR_MOVINGCOMPONENT)
            {
                // nothing
            }
            else
            {
                CVector outSpeed;
                pEntityVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPoint);
                vecEntitySpeed += outSpeed;
            }

            float fThisMass = fThisMassFactor * m_fMass;

            float fThisSpeedDotProduct = m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                + m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                + m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;
            float fEntitySpeedDotProduct = vecEntitySpeed.z * pColPoint->m_vecNormal.z
                + vecEntitySpeed.y * pColPoint->m_vecNormal.y
                + vecEntitySpeed.x * pColPoint->m_vecNormal.x;

            CVector vecEntityDifference = (vecDistanceToPoint - vecEntityCentreOfMassMultiplied);
            CVector vecEntityCrossProduct;
            vecEntityCrossProduct.Cross(vecEntityDifference, pColPoint->m_vecNormal);
            float squaredMagnitude = vecEntityCrossProduct.SquaredMagnitude();

            float fMoveSpeed = 0.0f;
            float fEntityCollisionMass = 1.0f / (squaredMagnitude / (fEntityMassFactor * pEntity->m_fTurnMass) + 1.0f / (fEntityMassFactor * pEntity->m_fMass));
            if (bEntityCollisionForceDisabled)
            {
                fMoveSpeed = fEntitySpeedDotProduct;
            }
            else
            {
                fMoveSpeed = (fEntityCollisionMass * fEntitySpeedDotProduct + fThisMass * fThisSpeedDotProduct) / (fEntityCollisionMass + fThisMass);
            }

            float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
            if (fThisMoveSpeedDifference < 0.0)
            {
                float fThisMoveSpeedElasticity = 0.0f;
                float fEntityMoveSpeedElasticity = 0.0f;
                float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
                if (m_bHasHitWall)
                {
                    fThisMoveSpeedElasticity = fMoveSpeed;
                }
                else
                {
                    fThisMoveSpeedElasticity = fMoveSpeed - fThisMoveSpeedDifference * fTheElasticity;
                }

                if (pEntity->m_bHasHitWall)
                {
                    fEntityMoveSpeedElasticity = fMoveSpeed;
                }
                else
                {
                    fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
                }

                *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisMass;
                *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityCollisionMass);

                CVector vecThisMoveForce = pColPoint->m_vecNormal * (*pThisDamageIntensity / fThisMassFactor);
                CVector vecEntityMoveForce = (pColPoint->m_vecNormal * (*pEntityDamageIntensity / fEntityMassFactor) * -1.0f);

                if (!physicalFlags.bDisableCollisionForce)
                {
                    if (vecThisMoveForce.z < 0.0)
                    {
                        vecThisMoveForce.z = 0.0;
                    }

                    if (bThisPedIsStandingOnEntity)
                    {
                        vecThisMoveForce.x = vecThisMoveForce.x + vecThisMoveForce.x;
                        vecThisMoveForce.y = vecThisMoveForce.y + vecThisMoveForce.y;
                    }

                    ApplyMoveForce(vecThisMoveForce);
                }

                if (pEntity->physicalFlags.bDisableCollisionForce || bThisPedIsStandingOnEntity)
                {
                    // nothing
                }
                else
                {
                    pEntity->ApplyForce(vecEntityMoveForce, vecDistanceToPoint, true);
                }
                float fCollisionImpact1 = *pThisDamageIntensity / fThisMass;
                AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                    &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);

                fCollisionImpact1 = *pEntityDamageIntensity / fEntityCollisionMass;
                AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
                    &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                return true;
            }
        }
        else
        {
            if (pEntity->physicalFlags.bDisableTurnForce)
            {
                CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
                CVector vecThisSpeed;
                GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

                if (physicalFlags.bAddMovingCollisionSpeed && m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
                {
                    CVector outSpeed;
                    pThisVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPointFromThis);
                    vecThisSpeed += outSpeed;
                }

                float fThisSpeedDotProduct = vecThisSpeed.z * pColPoint->m_vecNormal.z
                    + vecThisSpeed.y * pColPoint->m_vecNormal.y
                    + vecThisSpeed.x * pColPoint->m_vecNormal.x;
                float fEntitySpeedDotProduct = pEntity->m_vecMoveSpeed.z * pColPoint->m_vecNormal.z
                    + pEntity->m_vecMoveSpeed.y * pColPoint->m_vecNormal.y
                    + pEntity->m_vecMoveSpeed.x * pColPoint->m_vecNormal.x;

                CVector vecThisDifference = (vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied);
                CVector vecThisCrossProduct;
                vecThisCrossProduct.Cross(vecThisDifference, pColPoint->m_vecNormal);
                float squaredMagnitude = vecThisCrossProduct.SquaredMagnitude();
                float fThisCollisionMass = 1.0f / (squaredMagnitude / (fThisMassFactor * m_fTurnMass) + 1.0f / (fThisMassFactor * m_fMass));

                float fEntityMass = fEntityMassFactor * pEntity->m_fMass;
                float fMoveSpeed = (fEntityMass * fEntitySpeedDotProduct + fThisCollisionMass * fThisSpeedDotProduct) / (fEntityMass + fThisCollisionMass);
                float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
                if (fThisMoveSpeedDifference < 0.0)
                {
                    float fThisMoveSpeedElasticity = 0.0f;
                    float fEntityMoveSpeedElasticity = 0.0f;

                    float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
                    if (m_bHasHitWall)
                    {
                        fThisMoveSpeedElasticity = fMoveSpeed;
                    }
                    else
                    {
                        fThisMoveSpeedElasticity = fMoveSpeed - fThisMoveSpeedDifference * fTheElasticity;
                    }
                    if (pEntity->m_bHasHitWall)
                    {
                        fEntityMoveSpeedElasticity = fMoveSpeed;
                    }
                    else
                    {
                        fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
                    }

                    *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisCollisionMass;
                    *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityMass);

                    CVector vecThisMoveForce = pColPoint->m_vecNormal * (*pThisDamageIntensity / fThisMassFactor);
                    CVector vecEntityMoveForce = pColPoint->m_vecNormal * (*pEntityDamageIntensity / fEntityMassFactor) * -1.0f;
                    if (!physicalFlags.bDisableCollisionForce && !bEntityPedIsStandingOnThis)
                    {
                        if (vecThisMoveForce.z < 0.0f)
                        {
                            vecThisMoveForce.z = 0.0f;
                        }
                        ApplyForce(vecThisMoveForce, vecDistanceToPointFromThis, true);
                    }

                    if (!pEntity->physicalFlags.bDisableCollisionForce)
                    {
                        if (vecEntityMoveForce.z < 0.0f)
                        {
                            vecEntityMoveForce.z = 0.0;
                            if (fabs(fThisSpeedDotProduct) < 0.01f)
                            {
                                vecEntityMoveForce.z = 0.0f;
                                vecEntityMoveForce.x = vecEntityMoveForce.x * 0.5f;
                                vecEntityMoveForce.y = vecEntityMoveForce.y * 0.5f;
                            }
                        }
                        if (bEntityPedIsStandingOnThis)
                        {
                            vecEntityMoveForce.x = vecEntityMoveForce.x + vecEntityMoveForce.x;
                            vecEntityMoveForce.y = vecEntityMoveForce.y + vecEntityMoveForce.y;
                        }

                        pEntity->ApplyMoveForce(vecEntityMoveForce);
                    }

                    float fCollisionImpact1 = *pThisDamageIntensity / fThisCollisionMass;
                    AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);

                    fCollisionImpact1 = *pEntityDamageIntensity / fEntityMass;
                    AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                    return true;
                }
            }
            else
            {
                CVector vecDistanceToPointFromThis = pColPoint->m_vecPoint - GetPosition();
                CVector vecThisSpeed;
                GetSpeed(&vecThisSpeed, vecDistanceToPointFromThis);

                if (physicalFlags.bAddMovingCollisionSpeed && m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeA == SURFACE_CAR_MOVINGCOMPONENT)
                {
                    CVector outSpeed;
                    pThisVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPointFromThis);
                    vecThisSpeed += outSpeed;
                }

                CVector vecDistanceToPoint = pColPoint->m_vecPoint - pEntity->GetPosition();
                CVector vecEntitySpeed;
                pEntity->GetSpeed(&vecEntitySpeed, vecDistanceToPoint);

                if (pEntity->physicalFlags.bAddMovingCollisionSpeed && pEntity->m_nType == ENTITY_TYPE_VEHICLE && pColPoint->m_nSurfaceTypeB == SURFACE_CAR_MOVINGCOMPONENT)
                {
                    CVector outSpeed;
                    pEntityVehicle->AddMovingCollisionSpeed(&outSpeed, vecDistanceToPoint);
                    vecEntitySpeed += outSpeed;
                }

                float fThisSpeedDotProduct = vecThisSpeed.z * pColPoint->m_vecNormal.z
                    + vecThisSpeed.y * pColPoint->m_vecNormal.y
                    + vecThisSpeed.x * pColPoint->m_vecNormal.x;
                float fEntitySpeedDotProduct = vecEntitySpeed.z * pColPoint->m_vecNormal.z
                    + vecEntitySpeed.y * pColPoint->m_vecNormal.y
                    + vecEntitySpeed.x * pColPoint->m_vecNormal.x;


                CVector vecThisDifference = (vecDistanceToPointFromThis - vecThisCentreOfMassMultiplied);
                CVector vecThisCrossProduct;
                vecThisCrossProduct.Cross(vecThisDifference, pColPoint->m_vecNormal);
                float squaredMagnitude = vecThisCrossProduct.SquaredMagnitude();
                float fThisCollisionMass = 1.0f / (squaredMagnitude / (fThisMassFactor * m_fTurnMass) + 1.0f / (fThisMassFactor * m_fMass));

                CVector vecEntityDifference = (vecDistanceToPoint - vecEntityCentreOfMassMultiplied);
                CVector vecEntityCrossProduct;
                vecEntityCrossProduct.Cross(vecEntityDifference, pColPoint->m_vecNormal);
                squaredMagnitude = vecEntityCrossProduct.SquaredMagnitude();
                float fEntityCollisionMass = 1.0f / (squaredMagnitude / (fEntityMassFactor * pEntity->m_fTurnMass) + 1.0f / (fEntityMassFactor * pEntity->m_fMass));

                float fMoveSpeed = (fEntityCollisionMass * fEntitySpeedDotProduct + fThisCollisionMass * fThisSpeedDotProduct) / (fEntityCollisionMass + fThisCollisionMass);
                float fThisMoveSpeedDifference = fThisSpeedDotProduct - fMoveSpeed;
                if (fThisMoveSpeedDifference < 0.0f)
                {
                    float fThisMoveSpeedElasticity = 0.0f;
                    float fEntityMoveSpeedElasticity = 0.0f;

                    float fTheElasticity = (pEntity->m_fElasticity + m_fElasticity) * 0.5f;
                    if (m_bHasHitWall)
                    {
                        fThisMoveSpeedElasticity = fMoveSpeed;
                    }
                    else
                    {
                        fThisMoveSpeedElasticity = fMoveSpeed - fThisMoveSpeedDifference * fTheElasticity;
                    }
                    if (pEntity->m_bHasHitWall)
                    {
                        fEntityMoveSpeedElasticity = fMoveSpeed;
                    }
                    else
                    {
                        fEntityMoveSpeedElasticity = fMoveSpeed - (fEntitySpeedDotProduct - fMoveSpeed) * fTheElasticity;
                    }

                    *pThisDamageIntensity = (fThisMoveSpeedElasticity - fThisSpeedDotProduct) * fThisCollisionMass;
                    *pEntityDamageIntensity = -((fEntityMoveSpeedElasticity - fEntitySpeedDotProduct) * fEntityCollisionMass);

                    CVector vecThisMoveForce = pColPoint->m_vecNormal * (*pThisDamageIntensity / fThisMassFactor);
                    CVector vecEntityMoveForce = pColPoint->m_vecNormal * (*pEntityDamageIntensity / fEntityMassFactor) * -1.0f;

                    if (m_nType == ENTITY_TYPE_VEHICLE && !m_bHasHitWall)
                    {
                        if (pColPoint->m_vecNormal.z < 0.69999999f)
                        {
                            vecThisMoveForce.z *= 0.30000001f;
                        }

                        if (!m_nStatus)
                        {
                            vecDistanceToPointFromThis *= 0.80000001f;
                        }

                        if (CWorld::bNoMoreCollisionTorque)
                        {
                            CVector vecFrictionForce = vecThisMoveForce * -0.30000001f;
                            ApplyFrictionForce(vecFrictionForce, vecDistanceToPointFromThis);
                        }
                    }

                    if (pEntity->m_nType == ENTITY_TYPE_VEHICLE && !pEntity->m_bHasHitWall)
                    {
                        if ((pColPoint->m_vecNormal.z * -1.0f) < 0.69999999f)
                        {
                            vecEntityMoveForce.z *= 0.30000001f;
                        }

                        if (!pEntity->m_nStatus)
                        {
                            vecDistanceToPoint *= 0.80000001f;

                        }
                        if (CWorld::bNoMoreCollisionTorque)
                        {
                            CVector vecFrictionForce = vecEntityMoveForce * -0.30000001f;
                            pEntity->ApplyFrictionForce(vecFrictionForce, vecDistanceToPoint);
                        }
                    }
                    if (!physicalFlags.bDisableCollisionForce)
                    {
                        ApplyForce(vecThisMoveForce, vecDistanceToPointFromThis, true);
                    }

                    if (pEntity->physicalFlags.bDisableCollisionForce)
                    {
                        // nothing
                    }
                    else
                    {
                        if (pEntity->m_bIsInSafePosition)
                        {
                            pEntity->m_vecMoveSpeed *= -1.0f;
                            pEntity->m_vecTurnSpeed *= -1.0f;
                            pEntity->ApplySpeed();

                            pEntity->m_vecMoveSpeed *= -1.0f;
                            pEntity->m_vecTurnSpeed *= -1.0f;
                            pEntity->m_bIsInSafePosition = false;
                        }

                        pEntity->ApplyForce(vecEntityMoveForce, vecDistanceToPoint, true);
                    }

                    float fCollisionImpact1 = *pThisDamageIntensity / fThisCollisionMass;
                    AudioEngine.ReportCollision(this, pEntity, pColPoint->m_nSurfaceTypeA, pColPoint->m_nSurfaceTypeB, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);

                    fCollisionImpact1 = *pEntityDamageIntensity / fEntityCollisionMass;
                    AudioEngine.ReportCollision(pEntity, this, pColPoint->m_nSurfaceTypeB, pColPoint->m_nSurfaceTypeA, pColPoint,
                        &pColPoint->m_vecNormal, fCollisionImpact1, 1.0f, false, false);
                    return true;
                }
            }
        }
        return false;
    }
    return true;
#endif
}

bool CPhysical::ProcessCollisionSectorList(int sectorX, int sectorY)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, int, int))0x54BA60)(this, sectorX, sectorY);
#else
    static CColPoint colPoints[32];

    bool bResult = false;

    bool bCollisionDisabled = false;
    bool bCollidedEntityCollisionIgnored = false;
    bool bCollidedEntityUnableToMove = false;
    bool bThisOrCollidedEntityStuck = false;

    float fThisDamageIntensity = -1.0;
    float fEntityDamageIntensity = -1.0;
    float fThisMaxDamageIntensity = 0.0;
    float fEntityMaxDamageIntensity = 0.0;

    CBaseModelInfo* pModelInfo = CModelInfo::ms_modelInfoPtrs[m_nModelIndex];
    float fBoundingSphereRadius = pModelInfo->m_pColModel->m_boundSphere.m_fRadius;

    CVector vecBoundCentre;
    GetBoundCentre(&vecBoundCentre);

    CSector* pSector = GetSector(sectorX, sectorY);
    CRepeatSector* pRepeatSector = GetRepeatSector(sectorX, sectorY);

    int scanListIndex = 4;
    do
    {
        CPtrListDoubleLink* pDoubleLinkList = nullptr;
        --scanListIndex;
        switch (scanListIndex)
        {
        case 0:
            pDoubleLinkList = &pRepeatSector->m_lists[1];
            break;
        case 1:
            pDoubleLinkList = &pRepeatSector->m_lists[2];
            break;
        case 2:
            pDoubleLinkList = &pRepeatSector->m_lists[0];
            break;
        case 3:
            pDoubleLinkList = &pSector->m_buildings;
            break;
        }
        CPtrNodeDoubleLink* pNode = pDoubleLinkList->GetNode();
        if (pNode)
        {
            CEntity* pEntity = nullptr;

            CPhysical* pPhysicalEntity = nullptr;
            CObject* pEntityObject = nullptr;
            CPed* pEntityPed = nullptr;
            CVehicle* pEntityVehicle = nullptr;;

            CObject* pThisObject = nullptr;
            CPed* pThisPed = nullptr;
            CVehicle* pThisVehicle = nullptr;

            while (pNode)
            {
                pEntity = (CEntity*)pNode->pItem;
                pNode = pNode->pNext;

                pPhysicalEntity = static_cast<CPhysical*>(pEntity);
                pEntityObject = static_cast<CObject*>(pEntity);
                pEntityPed = static_cast<CPed*>(pEntity);
                pEntityVehicle = static_cast<CVehicle*>(pEntity);

                pThisObject = static_cast<CObject*>(this);
                pThisPed = static_cast<CPed*>(this);
                pThisVehicle = static_cast<CVehicle*>(this);

                if (!pEntity->m_bUsesCollision || pPhysicalEntity == this || pEntity->m_nScanCode == CWorld::ms_nCurrentScanCode)
                {
                    continue;
                }

                if (!pEntity->GetIsTouching(&vecBoundCentre, fBoundingSphereRadius))
                {
                    if (m_pEntityIgnoredCollision == pEntity && m_pAttachedTo != pEntity)
                    {
                        m_pEntityIgnoredCollision = 0;
                    }

                    if (pEntity->m_nType > ENTITY_TYPE_BUILDING && pEntity->m_nType < ENTITY_TYPE_DUMMY
                        && pPhysicalEntity->m_pEntityIgnoredCollision == this && pPhysicalEntity->m_pAttachedTo != this)
                    {
                        pPhysicalEntity->m_pEntityIgnoredCollision = 0;
                    }
                    continue;
                }


                bCollisionDisabled = false;
                bCollidedEntityCollisionIgnored = false;
                bCollidedEntityUnableToMove = false;
                bThisOrCollidedEntityStuck = false;

                physicalFlags.b13 = false;

                if (pEntity->m_nType == ENTITY_TYPE_BUILDING)
                {
                    bCollidedEntityCollisionIgnored = false;
                    if (physicalFlags.bInfiniteMass && m_bIsStuck)
                    {
                        bThisOrCollidedEntityStuck = true;
                    }

                    if (physicalFlags.bDisableCollisionForce
                        && (m_nType != ENTITY_TYPE_VEHICLE || pThisVehicle->m_nVehicleSubClass == VEHICLE_TRAIN))
                    {
                        bCollisionDisabled = true;
                    }
                    else
                    {
                        if (m_pAttachedTo
                            && m_pAttachedTo->m_nType > ENTITY_TYPE_BUILDING && m_pAttachedTo->m_nType < ENTITY_TYPE_DUMMY
                            && m_pAttachedTo->physicalFlags.bDisableCollisionForce)
                        {
                            bCollisionDisabled = true;
                        }
                        else if (m_pEntityIgnoredCollision == pEntity)
                        {
                            bCollisionDisabled = true;
                        }
                        else if (!physicalFlags.bDisableZ || physicalFlags.bApplyGravity)
                        {
                            if (physicalFlags.bDontCollideWithFlyers)
                            {
                                if (m_nStatus)
                                {
                                    if (m_nStatus != STATUS_HELI && pEntity->DoesNotCollideWithFlyers())
                                    {
                                        bCollisionDisabled = true;
                                    }
                                }
                            }
                        }
                        else
                        {
                            bCollisionDisabled = true;
                        }
                    }
                }
                else
                {
                    SpecialEntityPreCollisionStuff(pEntity, false, &bCollisionDisabled, &bCollidedEntityCollisionIgnored,
                        &bCollidedEntityUnableToMove, &bThisOrCollidedEntityStuck);
                }

                if (!m_bUsesCollision || bCollidedEntityCollisionIgnored || bCollisionDisabled)
                {
                    pEntity->m_nScanCode = CWorld::ms_nCurrentScanCode;
                    if (!bCollisionDisabled) // if collision is enabled then
                    {
                        int totalColPointsToProcess = ProcessEntityCollision(pPhysicalEntity, &colPoints[0]);
                        if (physicalFlags.b17 && !bCollidedEntityCollisionIgnored && totalColPointsToProcess > 0)
                        {
                            return true;
                        }
                        if (!totalColPointsToProcess && m_pEntityIgnoredCollision == pEntity && this == FindPlayerPed(-1))
                        {
                            m_pEntityIgnoredCollision = 0;
                        }
                    }
                    continue;
                }

                if (pEntity->m_nType == ENTITY_TYPE_BUILDING || pPhysicalEntity->physicalFlags.bCollidable || bCollidedEntityUnableToMove)
                {
                    pEntity->m_nScanCode = CWorld::ms_nCurrentScanCode;

                    int totalAcceptableColPoints = 0;
                    float fThisMaxDamageIntensity = 0.0;
                    CVector vecMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
                    CVector vecTurnSpeed = CVector(0.0f, 0.0f, 0.0f);

                    int totalColPointsToProcess = ProcessEntityCollision(pPhysicalEntity, &colPoints[0]);
                    if (totalColPointsToProcess > 0)
                    {
                        if (m_bHasContacted)
                        {
                            if (totalColPointsToProcess > 0)
                            {
                                for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                                {
                                    CColPoint* pColPoint = &colPoints[colPointIndex];
                                    if (bThisOrCollidedEntityStuck
                                        || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16))
                                    {
                                        ApplySoftCollision(pEntity, pColPoint, &fThisDamageIntensity);
                                    }
                                    else if (ApplyCollisionAlt(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &vecMoveSpeed, &vecTurnSpeed))
                                    {
                                        ++totalAcceptableColPoints;
                                        if (fThisDamageIntensity > fThisMaxDamageIntensity)
                                        {
                                            fThisMaxDamageIntensity = fThisDamageIntensity;
                                        }

                                        if (m_nType == ENTITY_TYPE_VEHICLE)
                                        {
                                            if (pThisVehicle->m_nVehicleClass != VEHICLE_BOAT || pColPoint->m_nSurfaceTypeB != SURFACE_WOOD_SOLID)
                                            {
                                                SetDamagedPieceRecord(fThisDamageIntensity, pPhysicalEntity, pColPoint, 1.0f);
                                            }
                                        }
                                        else
                                        {
                                            SetDamagedPieceRecord(fThisDamageIntensity, pPhysicalEntity, pColPoint, 1.0f);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                            {
                                CColPoint* pColPoint = &colPoints[colPointIndex];
                                if (bThisOrCollidedEntityStuck
                                    || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16)
                                    )
                                {
                                    if (ApplySoftCollision(pEntity, pColPoint, &fThisDamageIntensity)
                                        && (pColPoint->m_nSurfaceTypeA != SURFACE_WHEELBASE || pColPoint->m_nSurfaceTypeB != SURFACE_WHEELBASE))
                                    {
                                        float fSurfaceFriction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                                        if (ApplyFriction(fSurfaceFriction, pColPoint))
                                        {
                                            m_bHasContacted = true;
                                        }
                                        continue;
                                    }
                                }
                                else if (ApplyCollisionAlt(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &vecMoveSpeed, &vecTurnSpeed))
                                {
                                    ++totalAcceptableColPoints;
                                    if (fThisDamageIntensity > fThisMaxDamageIntensity)
                                    {
                                        fThisMaxDamageIntensity = fThisDamageIntensity;
                                    }

                                    float fSurfaceFirction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                                    float fFriction = fSurfaceFirction / totalColPointsToProcess;
                                    if (m_nType != ENTITY_TYPE_VEHICLE)
                                    {
                                        fFriction *= 150.0f * fThisDamageIntensity;
                                        SetDamagedPieceRecord(fThisDamageIntensity, pEntity, pColPoint, 1.0f);
                                        if (ApplyFriction(fFriction, pColPoint))
                                        {
                                            m_bHasContacted = true;
                                        }
                                        continue;
                                    }
                                    if (pThisVehicle->m_nVehicleClass != VEHICLE_BOAT || pColPoint->m_nSurfaceTypeB != SURFACE_WOOD_SOLID)
                                    {
                                        SetDamagedPieceRecord(fThisDamageIntensity, pEntity, pColPoint, 1.0f);
                                    }
                                    else
                                    {
                                        fFriction = 0.0f;
                                    }

                                    if (m_nModelIndex == MODEL_RCBANDIT)
                                    {
                                        fFriction *= 0.2f;
                                    }
                                    else
                                    {
                                        if (pThisVehicle->m_nVehicleClass == VEHICLE_BOAT)
                                        {
                                            if (pColPoint->m_vecNormal.z > 0.6f)
                                            {
                                                if (g_surfaceInfos->GetAdhesionGroup(pColPoint->m_nSurfaceTypeB) == ADHESION_GROUP_LOOSE
                                                    || g_surfaceInfos->GetAdhesionGroup(pColPoint->m_nSurfaceTypeB) == ADHESION_GROUP_SAND)
                                                {
                                                    fFriction *= 3.0f;
                                                }
                                            }
                                            else
                                            {
                                                fFriction = 0.0f;
                                            }
                                        }

                                        if (pThisVehicle->m_nVehicleSubClass != VEHICLE_TRAIN)
                                        {
                                            if (m_nStatus == STATUS_WRECKED)
                                            {
                                                fFriction *= 3.0f;
                                            }
                                            else
                                            {
                                                if (GetUp().z > 0.3f && m_vecMoveSpeed.SquaredMagnitude() < 0.02f && m_vecTurnSpeed.SquaredMagnitude() < 0.01f)
                                                {
                                                    fFriction = 0.0f;
                                                }
                                                else
                                                {
                                                    if (m_nStatus != STATUS_ABANDONED
                                                        && DotProduct(&pColPoint->m_vecNormal, &GetUp()) >= 0.707f)
                                                    {
                                                    }
                                                    else
                                                    {
                                                        fFriction = 150.0f / m_fMass * fFriction * fThisDamageIntensity;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (m_nType == ENTITY_TYPE_VEHICLE && pThisVehicle->m_nVehicleSubClass == VEHICLE_TRAIN)
                                    {
                                        fFriction = fFriction + fFriction;
                                    }
                                    if (ApplyFriction(fFriction, pColPoint))
                                    {
                                        m_bHasContacted = true;
                                    }
                                }
                            }
                        }
                    }

                    if (totalAcceptableColPoints)
                    {
                        float fSpeedFactor = 1.0f / totalAcceptableColPoints;
                        m_vecMoveSpeed += vecMoveSpeed * fSpeedFactor;
                        m_vecTurnSpeed += vecTurnSpeed * fSpeedFactor;
                        if (!CWorld::bNoMoreCollisionTorque)
                        {
                            if (!m_nStatus && m_nType == ENTITY_TYPE_VEHICLE)
                            {
                                float fThisMoveSpeedX = m_vecMoveSpeed.x;
                                if (m_vecMoveSpeed.x < 0.0f)
                                {
                                    fThisMoveSpeedX = -fThisMoveSpeedX;
                                }

                                if (fThisMoveSpeedX > 0.2f)
                                {
                                    float fThisMoveSpeedY = m_vecMoveSpeed.y;
                                    if (m_vecMoveSpeed.y < 0.0f)
                                    {
                                        fThisMoveSpeedY = -fThisMoveSpeedY;
                                    }
                                    if (fThisMoveSpeedY > 0.2f)
                                    {
                                        if (!physicalFlags.bSubmergedInWater)
                                        {
                                            m_vecFrictionMoveSpeed.x -= vecMoveSpeed.x * 0.3f / (float)totalColPointsToProcess;
                                            m_vecFrictionMoveSpeed.y -= vecMoveSpeed.y * 0.3f / (float)totalColPointsToProcess;
                                            m_vecFrictionTurnSpeed += (vecTurnSpeed * -0.3f) / (float)totalColPointsToProcess;
                                        }
                                    }
                                }
                            }
                        }

                        if (pEntity->m_nType == ENTITY_TYPE_OBJECT && pEntityObject->m_nColDamageEffect && fThisMaxDamageIntensity > 20.0f)
                        {
                            pEntityObject->ObjectDamage(fThisMaxDamageIntensity, &colPoints[0].m_vecPoint, &CPhysical::fxDirection, this, WEAPON_UNIDENTIFIED);
                        }
                        if (!CWorld::bSecondShift)
                        {
                            return true;
                        }

                        bResult = true;
                    }
                }
                else
                {
                    pEntity->m_nScanCode = CWorld::ms_nCurrentScanCode;

                    int totalAcceptableColPoints = 0;
                    int totalColPointsToProcess = ProcessEntityCollision(pPhysicalEntity, &colPoints[0]);
                    if (totalColPointsToProcess > 0)
                    {
                        fThisMaxDamageIntensity = 0.0;
                        fEntityMaxDamageIntensity = 0.0;
                        if (m_bHasContacted && pEntity->m_bHasContacted)
                        {
                            if (totalColPointsToProcess > 0)
                            {
                                for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                                {
                                    CColPoint* pColPoint = &colPoints[colPointIndex];
                                    if (bThisOrCollidedEntityStuck
                                        || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16)
                                        || (pColPoint->m_nPieceTypeB >= 13 && pColPoint->m_nPieceTypeB <= 16))
                                    {
                                        ++totalAcceptableColPoints;
                                        ApplySoftCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity);
                                    }
                                    else
                                    {
                                        if (ApplyCollision(pEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                                        {
                                            if (fThisDamageIntensity > fThisMaxDamageIntensity)
                                            {
                                                fThisMaxDamageIntensity = fThisDamageIntensity;
                                            }
                                            if (fEntityDamageIntensity > fEntityMaxDamageIntensity)
                                            {
                                                fEntityMaxDamageIntensity = fEntityDamageIntensity;
                                            }

                                            SetDamagedPieceRecord(fThisDamageIntensity, pPhysicalEntity, pColPoint, 1.0f);
                                            pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);
                                        }
                                    }
                                }
                            }
                        }
                        else if (m_bHasContacted)
                        {
                            m_bHasContacted = false;
                            CVector vecThisFrictionMoveSpeed = m_vecFrictionMoveSpeed;
                            CVector vecThisFrictionTurnSpeed = m_vecFrictionTurnSpeed;
                            m_vecFrictionMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
                            m_vecFrictionTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
                            if (totalColPointsToProcess > 0)
                            {
                                for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                                {
                                    CColPoint* pColPoint = &colPoints[colPointIndex];
                                    if (bThisOrCollidedEntityStuck
                                        || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16)
                                        || (pColPoint->m_nPieceTypeB >= 13 && pColPoint->m_nPieceTypeB <= 16)
                                        )
                                    {
                                        ++totalAcceptableColPoints;
                                        ApplySoftCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity);
                                        if (pColPoint->m_nPieceTypeB >= 13 && pColPoint->m_nPieceTypeB <= 16)
                                        {
                                            pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);
                                        }
                                    }
                                    else if (ApplyCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                                    {
                                        if (fThisDamageIntensity > fThisMaxDamageIntensity)
                                        {
                                            fThisMaxDamageIntensity = fThisDamageIntensity;
                                        }
                                        if (fEntityDamageIntensity > fEntityMaxDamageIntensity)
                                        {
                                            fEntityMaxDamageIntensity = fEntityDamageIntensity;
                                        }

                                        SetDamagedPieceRecord(fThisDamageIntensity, pPhysicalEntity, pColPoint, 1.0f);
                                        pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);

                                        float fSurfaceFriction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                                        float fFriction = fSurfaceFriction / totalColPointsToProcess;
                                        if (m_nType == ENTITY_TYPE_VEHICLE && pEntity->m_nType == ENTITY_TYPE_VEHICLE
                                            && (m_vecMoveSpeed.SquaredMagnitude() > 0.02f || m_vecTurnSpeed.SquaredMagnitude() > 0.01f))
                                        {
                                            fFriction *= 1.0f * fThisDamageIntensity;
                                        }

                                        if (pEntity->m_bIsStatic || pEntity->m_bIsStaticWaitingForCollision)
                                        {
                                            if (ApplyFriction(fFriction, pColPoint))
                                            {
                                                m_bHasContacted = true;
                                            }
                                        }
                                        else if (ApplyFriction(pPhysicalEntity, fFriction, pColPoint))
                                        {
                                            m_bHasContacted = true;
                                            pEntity->m_bHasContacted = true;
                                        }
                                    }
                                }
                            }

                            if (!m_bHasContacted)
                            {
                                m_bHasContacted = true;
                                m_vecFrictionMoveSpeed = vecThisFrictionMoveSpeed;
                                m_vecFrictionTurnSpeed = vecThisFrictionTurnSpeed;
                            }
                        }
                        else
                        {
                            if (pEntity->m_bHasContacted)
                            {
                                pEntity->m_bHasContacted = false;
                                CVector vecEntityMoveSpeed = pPhysicalEntity->m_vecFrictionMoveSpeed;
                                CVector vecEntityFrictionTurnSpeed = pPhysicalEntity->m_vecFrictionTurnSpeed;
                                pPhysicalEntity->m_vecFrictionMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
                                pPhysicalEntity->m_vecFrictionTurnSpeed = CVector(0.0f, 0.0f, 0.0f);

                                if (totalColPointsToProcess > 0)
                                {
                                    for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                                    {
                                        CColPoint* pColPoint = &colPoints[colPointIndex];
                                        if (bThisOrCollidedEntityStuck
                                            || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16)
                                            || (pColPoint->m_nPieceTypeB >= 13 && pColPoint->m_nPieceTypeB <= 16)
                                            )
                                        {
                                            ++totalAcceptableColPoints;
                                            ApplySoftCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity);
                                            if (pColPoint->m_nPieceTypeB >= 13 && pColPoint->m_nPieceTypeB <= 16)
                                            {
                                                pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);
                                            }
                                        }
                                        else if (ApplyCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                                        {
                                            if (fThisDamageIntensity > fThisMaxDamageIntensity)
                                            {
                                                fThisMaxDamageIntensity = fThisDamageIntensity;
                                            }

                                            if (fEntityDamageIntensity > fEntityMaxDamageIntensity)
                                            {
                                                fEntityMaxDamageIntensity = fEntityDamageIntensity;
                                            }

                                            SetDamagedPieceRecord(fThisDamageIntensity, pPhysicalEntity, pColPoint, 1.0f);
                                            pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);

                                            float fSurfaceFirction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);

                                            float fFriction = fSurfaceFirction / totalColPointsToProcess;
                                            if (m_nType == ENTITY_TYPE_VEHICLE && pEntity->m_nType == ENTITY_TYPE_VEHICLE
                                                && (m_vecMoveSpeed.SquaredMagnitude() > 0.02f || m_vecTurnSpeed.SquaredMagnitude() > 0.01f))
                                            {
                                                fFriction *= 1.0f * fThisDamageIntensity;
                                            }

                                            if (pEntity->m_bIsStatic || pEntity->m_bIsStaticWaitingForCollision)
                                            {
                                                if (ApplyFriction(fFriction, pColPoint))
                                                {
                                                    m_bHasContacted = true;
                                                }
                                            }
                                            else if (ApplyFriction(pPhysicalEntity, fFriction, pColPoint))
                                            {
                                                m_bHasContacted = true;
                                                pEntity->m_bHasContacted = true;
                                            }
                                        }
                                    }
                                }

                                if (!pEntity->m_bHasContacted)
                                {
                                    pEntity->m_bHasContacted = true;
                                    pPhysicalEntity->m_vecFrictionMoveSpeed = vecEntityMoveSpeed;
                                    pPhysicalEntity->m_vecFrictionTurnSpeed = vecEntityFrictionTurnSpeed;
                                }
                            }
                            else if (totalColPointsToProcess > 0)
                            {
                                for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                                {
                                    CColPoint* pColPoint = &colPoints[colPointIndex];
                                    if (bThisOrCollidedEntityStuck
                                        || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16)
                                        || (pColPoint->m_nPieceTypeA >= 13 && pColPoint->m_nPieceTypeA <= 16) // BUG: I think it should be m_nPieceTypeB
                                        )
                                    {
                                        ++totalAcceptableColPoints;
                                        ApplySoftCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity);
                                        if (pColPoint->m_nPieceTypeB >= 13 && pColPoint->m_nPieceTypeB <= 16)
                                        {
                                            pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);
                                        }
                                    }
                                    else if (ApplyCollision(pPhysicalEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                                    {
                                        if (fThisDamageIntensity > fThisMaxDamageIntensity)
                                        {
                                            fThisMaxDamageIntensity = fThisDamageIntensity;
                                        }

                                        if (fEntityDamageIntensity > fEntityMaxDamageIntensity)
                                        {
                                            fEntityMaxDamageIntensity = fEntityDamageIntensity;
                                        }

                                        SetDamagedPieceRecord(fThisDamageIntensity, pPhysicalEntity, pColPoint, 1.0f);
                                        pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0f);

                                        float fSurfaceFirction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                                        float fFriction = fSurfaceFirction / totalColPointsToProcess;
                                        if (m_nType == ENTITY_TYPE_VEHICLE && pEntity->m_nType == ENTITY_TYPE_VEHICLE
                                            && (m_vecMoveSpeed.SquaredMagnitude() > 0.02f || m_vecTurnSpeed.SquaredMagnitude() > 0.01f))
                                        {
                                            fFriction *= 1.0f * fThisDamageIntensity;
                                        }

                                        if (pEntity->m_bIsStatic || pEntity->m_bIsStaticWaitingForCollision)
                                        {
                                            if (ApplyFriction(fFriction, pColPoint))
                                            {
                                                m_bHasContacted = true;
                                            }
                                        }
                                        else if (ApplyFriction(pPhysicalEntity, fFriction, pColPoint))
                                        {
                                            m_bHasContacted = true;
                                            pEntity->m_bHasContacted = true;
                                        }
                                    }
                                }
                            }
                        }

                        if (pEntity->m_nType == ENTITY_TYPE_PED && m_nType == ENTITY_TYPE_VEHICLE)
                        {
                            float fThisMoveSpeedDot = m_vecMoveSpeed.SquaredMagnitude();
                            if (!pEntityPed->IsPlayer() || pEntity->m_bIsStuck && m_vecMoveSpeed.SquaredMagnitude() > 0.0025f)
                            {
                                pEntityPed->KillPedWithCar(static_cast<CVehicle*>(this), fEntityMaxDamageIntensity, false);
                            }
                        }
                        else if (m_nType == ENTITY_TYPE_PED && pEntity->m_nType == ENTITY_TYPE_VEHICLE
                            && pEntityVehicle->m_nVehicleSubClass == VEHICLE_TRAIN
                            && (DotProduct(&pEntityVehicle->m_vecMoveSpeed, &m_vecLastCollisionImpactVelocity) > 0.2f
                                || pThisPed->bFallenDown && pEntityVehicle->m_vecMoveSpeed.SquaredMagnitude() > 0.0005f))
                        {
                            float fDamageIntensity = fThisMaxDamageIntensity + fThisMaxDamageIntensity;
                            pThisPed->KillPedWithCar(pEntityVehicle, fDamageIntensity, false);
                        }
                        else if (pEntity->m_nType == ENTITY_TYPE_OBJECT && m_nType == ENTITY_TYPE_VEHICLE
                            && pEntity->m_bUsesCollision)
                        {
                            if (pEntityObject->m_nColDamageEffect && fEntityMaxDamageIntensity > 20.0)
                            {
                                pEntityObject->ObjectDamage(fEntityMaxDamageIntensity, &colPoints[0].m_vecPoint, &CPhysical::fxDirection, this, WEAPON_RUNOVERBYCAR);
                            }
                            else
                            {
                                if (pEntityObject->m_nColDamageEffect >= COL_DAMAGE_EFFECT_SMASH_COMPLETELY)
                                {
                                    CBaseModelInfo* pEntityModelInfo = CModelInfo::ms_modelInfoPtrs[pEntity->m_nModelIndex];
                                    CColModel* pColModel = pEntityModelInfo->m_pColModel;
                                    CVector vecResult = pColModel->m_boundBox.m_vecMax - pColModel->m_boundBox.m_vecMin;
                                    vecResult = (*pEntity->m_matrix) * vecResult;

                                    bool bObjectDamage = false;
                                    if (GetPosition().z > vecResult.z)
                                    {
                                        bObjectDamage = true;
                                    }
                                    else
                                    {
                                        CMatrix invertedMatrix;
                                        if (Invert(m_matrix, &invertedMatrix))
                                        {
                                            vecResult = invertedMatrix * vecResult;
                                            if (vecResult.z < 0.0f)
                                            {
                                                bObjectDamage = true;
                                            }
                                        }
                                    }
                                    if (bObjectDamage)
                                    {
                                        pEntityObject->ObjectDamage(50.0f, &colPoints[0].m_vecPoint, &CPhysical::fxDirection, this, WEAPON_RUNOVERBYCAR);
                                    }
                                }
                            }
                        }
                        else if (m_nType == ENTITY_TYPE_OBJECT && pEntity->m_nType == ENTITY_TYPE_VEHICLE && m_bUsesCollision)
                        {
                            if (pThisObject->m_nColDamageEffect && fEntityMaxDamageIntensity > 20.0)
                            {
                                pThisObject->ObjectDamage(fEntityMaxDamageIntensity, &colPoints[0].m_vecPoint, &CPhysical::fxDirection, pEntity, WEAPON_RUNOVERBYCAR);
                            }
                            else
                            {
                                // BUG: pEntity is a vehicle here, but we are treating it as an object?
                                if (pEntityObject->m_nColDamageEffect >= COL_DAMAGE_EFFECT_SMASH_COMPLETELY)
                                {
                                    
                                    CColModel* pColModel = pModelInfo->m_pColModel;
                                    CVector vecResult = pColModel->m_boundBox.m_vecMax - pColModel->m_boundBox.m_vecMin;
                                    vecResult = (*m_matrix) * vecResult;

                                    bool bObjectDamage = false;
                                    if (vecResult.z < pEntity->GetPosition().z)
                                    {
                                        bObjectDamage = true;
                                    }
                                    else
                                    {
                                        CMatrix invertedMatrix;
                                        if (Invert(pEntity->m_matrix, &invertedMatrix))
                                        {
                                            vecResult = invertedMatrix * vecResult;
                                            if (vecResult.z < 0.0f)
                                            {
                                                bObjectDamage = true;
                                            }
                                        }
                                    }

                                    if (bObjectDamage)
                                    {
                                        pThisObject->ObjectDamage(50.0f, &colPoints[0].m_vecPoint, &CPhysical::fxDirection, pEntity, WEAPON_RUNOVERBYCAR);
                                    }
                                }
                            }
                        }

                        if (pEntity->m_nStatus == STATUS_SIMPLE)
                        {
                            pEntity->m_nStatus = STATUS_PHYSICS;
                            if (pEntity->m_nType == ENTITY_TYPE_VEHICLE)
                            {
                                CCarCtrl::SwitchVehicleToRealPhysics(pEntityVehicle);
                            }
                        }
                        if (CWorld::bSecondShift)
                        {
                            bResult = true;
                        }
                        else if (totalColPointsToProcess > totalAcceptableColPoints)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    } while (scanListIndex);
    return bResult;
#endif
}

bool CPhysical::ProcessCollisionSectorList_SimpleCar(CRepeatSector* pRepeatSector)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*, CRepeatSector*))0x54CFF0)(this, pRepeatSector);
#else
    CColPoint colPoints[32];
    float fThisDamageIntensity = -1.0;
    float fEntityDamageIntensity = -1.0;

    if (!m_bUsesCollision)
    {
        return false;
    }
  
    CVector vecBoundingCentre;
    GetBoundCentre(&vecBoundingCentre);

    float fBoundingRadius = CModelInfo::ms_modelInfoPtrs[m_nModelIndex]->m_pColModel->m_boundSphere.m_fRadius;
    CPtrListDoubleLink* pDoubleLinkList = nullptr;
  
    int scanListIndex = 2;
    while (1)
    {
        switch (--scanListIndex)
        {
        case 0:
            pDoubleLinkList = &pRepeatSector->m_lists[0];
            break;
        case 1:
            pDoubleLinkList = &pRepeatSector->m_lists[2];
            break;
        }

        if (pDoubleLinkList->GetNode())
        {
            break;
        }

        if (!scanListIndex)
        {
            return false;
        } 
    }
 
    CEntity* pEntity = nullptr;
    CPhysical* pPhysicalEntity = nullptr;

    int totalColPointsToProcess = 0;

    CPtrNodeDoubleLink* pNode = pDoubleLinkList->GetNode();
    while (pNode)
    {
        pEntity = reinterpret_cast<CEntity*>(pNode->pItem);
        pPhysicalEntity = static_cast<CPhysical*>(pEntity);
        pNode = pNode->pNext;

        CObject* pObjectEntity = static_cast<CObject*>(pEntity);
        CPed* pPedEntity = static_cast<CPed*>(pEntity);
        bool bLampPostForwardZLess = false; // Rename this later (probably never going to happen)
        if (pEntity->m_nType == ENTITY_TYPE_OBJECT)
        {
            if (pObjectEntity->objectFlags.bIsLampPost)
            {
                if (!pEntity->m_matrix)
                {
                    pEntity->AllocateMatrix();
                    pEntity->m_placement.UpdateMatrix(pEntity->m_matrix);
                }
                if (pEntity->GetUp().z < 0.66000003)
                {
                    bLampPostForwardZLess = true;
                }
            }
        }

        if (pEntity != this
            && !bLampPostForwardZLess
            && pEntity->m_nScanCode != CWorld::ms_nCurrentScanCode)
        {
            if (pEntity->m_bUsesCollision)
            {
                if (pEntity->GetIsTouching(&vecBoundingCentre, fBoundingRadius))
                {
                    pEntity->m_nScanCode = CWorld::ms_nCurrentScanCode;
                    totalColPointsToProcess = ProcessEntityCollision(pPhysicalEntity, &colPoints[0]);
                    if (totalColPointsToProcess > 0)
                    {
                        break;
                    }
                }
            }
        }
    }

    if (!pNode)
    {
        if (!scanListIndex)
        {
            return false;
        }
    }

    if (m_bHasContacted && pEntity->m_bHasContacted)
    {
        if (totalColPointsToProcess > 0)
        {
            for( int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
            {
                CColPoint* pColPoint = &colPoints[colPointIndex];
                if (ApplyCollision(pEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                {
                    SetDamagedPieceRecord(fThisDamageIntensity, pEntity, pColPoint, 1.0);
                    pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0);
                }
            }
        }
    }
    else if (m_bHasContacted)
    {
        CVector vecOldFrictionMoveSpeed = m_vecFrictionMoveSpeed;
        CVector vecOldFrictionTurnSpeed = m_vecFrictionTurnSpeed;
        m_vecFrictionTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
        m_vecFrictionMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
        m_bHasContacted = false;

        if (totalColPointsToProcess > 0)
        {
            for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
            {
                CColPoint* pColPoint = &colPoints[colPointIndex];
                if (ApplyCollision(pEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                {
                    SetDamagedPieceRecord(fThisDamageIntensity, pEntity, pColPoint, 1.0);
                    pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0);
                    float fSurfaceFriction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                    float fFriction = fSurfaceFriction / totalColPointsToProcess;
                    if (ApplyFriction(pPhysicalEntity, fFriction, pColPoint))
                    {
                        m_bHasContacted = true;
                        pEntity->m_bHasContacted = true;
                    }
                }
            } 
        }

        if (!m_bHasContacted)
        {
            m_vecFrictionMoveSpeed = vecOldFrictionMoveSpeed;
            m_vecFrictionTurnSpeed = vecOldFrictionTurnSpeed;
            m_bHasContacted = true;
        }
    }
    else
    {
        if (pEntity->m_bHasContacted)
        {
            CVector vecOldFrictionMoveSpeed = pPhysicalEntity->m_vecFrictionMoveSpeed;
            CVector vecOldFrictionTurnSpeed = pPhysicalEntity->m_vecFrictionTurnSpeed;
            pPhysicalEntity->m_vecFrictionTurnSpeed = CVector(0.0f, 0.0f, 0.0f);
            pPhysicalEntity->m_vecFrictionMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
            pEntity->m_bHasContacted = false;

            if (totalColPointsToProcess > 0)
            {
                for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
                {
                    CColPoint* pColPoint = &colPoints[colPointIndex];
                    if (ApplyCollision(pEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                    {
                        SetDamagedPieceRecord(fThisDamageIntensity, pEntity, pColPoint, 1.0);
                        pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0);
                        float fSurfaceFriction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                        float fFriction = fSurfaceFriction / totalColPointsToProcess;
                        if (ApplyFriction(pPhysicalEntity, fFriction, pColPoint))
                        {
                            m_bHasContacted = true;
                            pEntity->m_bHasContacted = true;
                        }
                    }
                }
            }
            if (!pEntity->m_bHasContacted)
            {
                pPhysicalEntity->m_vecFrictionMoveSpeed = vecOldFrictionMoveSpeed;
                pPhysicalEntity->m_vecFrictionTurnSpeed = vecOldFrictionTurnSpeed;
                pEntity->m_bHasContacted = true;
            }
        }
        else if (totalColPointsToProcess > 0)
        {
            for (int colPointIndex = 0; colPointIndex < totalColPointsToProcess; colPointIndex++)
            {
                CColPoint* pColPoint = &colPoints[colPointIndex];
                if (ApplyCollision(pEntity, pColPoint, &fThisDamageIntensity, &fEntityDamageIntensity))
                {
                    SetDamagedPieceRecord(fThisDamageIntensity, pEntity, pColPoint, 1.0);
                    pPhysicalEntity->SetDamagedPieceRecord(fEntityDamageIntensity, this, pColPoint, -1.0);
                    float fSurfaceFriction = g_surfaceInfos->GetAdhesiveLimit(pColPoint);
                    float fFriction = fSurfaceFriction / totalColPointsToProcess;
                    if (ApplyFriction(pPhysicalEntity, fFriction, pColPoint))
                    {
                        m_bHasContacted = true;
                        pEntity->m_bHasContacted = true;
                    }
                }
            }
        }
    }
  
    if (pEntity->m_nStatus == STATUS_SIMPLE)
    {
        pEntity->m_nStatus = STATUS_PHYSICS;
        if (pEntity->m_nType == ENTITY_TYPE_VEHICLE)
        {
            CCarCtrl::SwitchVehicleToRealPhysics(static_cast<CVehicle*>(pEntity));
        }
    }
    return true; 
#endif
}


void CPhysical::AttachEntityToEntity(CPhysical* pEntityAttachTo, CVector vecAttachOffset, CVector vecAttachRotation)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CPhysical*, CVector, CVector))0x54D570)(this, pEntityAttachTo, vecAttachOffset, vecAttachRotation);
#else
    if (!pEntityAttachTo)
        return;

    CEntity* pOldEntityAttachedTo = m_pAttachedTo;
    m_pAttachedTo = pEntityAttachTo;
    m_pAttachedTo->RegisterReference(reinterpret_cast<CEntity**>(&m_pAttachedTo));
    m_vecAttachOffset = vecAttachOffset;
    if (physicalFlags.bInfiniteMass) 
        m_vecAttachedEntityRotation = GetPosition();
    else 
        m_vecAttachedEntityRotation = vecAttachRotation;
    m_qAttachedEntityRotation.real = 0.0f;
    m_qAttachedEntityRotation.imag = CVector(0.0f, 0.0f, 0.0f);
    m_pEntityIgnoredCollision = pOldEntityAttachedTo;
    if (physicalFlags.bDisableCollisionForce) {
        physicalFlags.bCollidable = false;
        PositionAttachedEntity();
    }
    else {
        if (m_pAttachedTo->m_nType > ENTITY_TYPE_BUILDING && m_pAttachedTo->m_nType < ENTITY_TYPE_DUMMY
            && m_pAttachedTo->physicalFlags.bDisableCollisionForce 
            && m_nType == ENTITY_TYPE_OBJECT && !physicalFlags.bInfiniteMass) 
        {
            physicalFlags.bDisableCollisionForce = true;
            m_fMass = 99999.9f;
            m_fTurnMass = 99999.9f;
        }
        PositionAttachedEntity();
    }
#endif
}

void CPhysical::AttachEntityToEntity(CPhysical* pEntityAttachTo, CVector* vecAttachOffset, CQuaternion* attachRotation)
{
#ifdef USE_DEFAULT_FUNCTIONS
    ((void(__thiscall*)(CPhysical*, CPhysical*, CVector*, CQuaternion*))0x54D690)(this, pEntityAttachTo, vecAttachOffset, attachRotation);
#else
    if (!pEntityAttachTo)
        return;

    CPhysical* pOldEntityAttachedTo = m_pAttachedTo;
    m_pAttachedTo = pEntityAttachTo;
    m_pAttachedTo->RegisterReference(reinterpret_cast<CEntity**>(&m_pAttachedTo));
    if (!m_pAttachedTo->m_matrix) {
        m_pAttachedTo->AllocateMatrix();
        m_pAttachedTo->m_placement.UpdateMatrix(m_pAttachedTo->m_matrix);
    }

    CMatrix entityAttachedtoMatrix(*m_pAttachedTo->m_matrix);
    CAutomobile* pAttachedToAutoMobile = static_cast<CAutomobile*>(m_pAttachedTo);
    if (m_nType == ENTITY_TYPE_OBJECT && m_pAttachedTo->m_nModelIndex == MODEL_FORKLIFT) {
        RwFrame* pCarMiscAFrame = pAttachedToAutoMobile->m_aCarNodes[CAR_MISC_A];
        if (pCarMiscAFrame) 
            entityAttachedtoMatrix.UpdateMatrix(RwFrameGetLTM(pCarMiscAFrame));
    }

    if (vecAttachOffset) {
        m_vecAttachOffset = *vecAttachOffset;
    }
    else {
        CVector distance = GetPosition() - entityAttachedtoMatrix.GetPosition();
        m_vecAttachOffset.x = DotProduct(&entityAttachedtoMatrix.GetRight(), &distance);
        m_vecAttachOffset.y = DotProduct(&entityAttachedtoMatrix.GetForward(), &distance);
        m_vecAttachOffset.z = DotProduct(&entityAttachedtoMatrix.GetUp(), &distance);
    }

    if (attachRotation) {
        m_qAttachedEntityRotation = *attachRotation;
    }
    else {
        CMatrix entityAttachedToMatrixInverted;
        Invert(&entityAttachedtoMatrix, &entityAttachedToMatrixInverted);
        entityAttachedToMatrixInverted *= *m_matrix;
        RwMatrixTag rwMatrix;
        entityAttachedToMatrixInverted.CopyToRwMatrix(&rwMatrix);
        RtQuatConvertFromMatrix((RtQuat*)&m_qAttachedEntityRotation, &rwMatrix);
    }

    if (physicalFlags.bDisableCollisionForce) {
        physicalFlags.bCollidable = false;
    }
    else {
        if (m_pAttachedTo->m_nType > ENTITY_TYPE_BUILDING && m_pAttachedTo->m_nType < ENTITY_TYPE_DUMMY
            && m_pAttachedTo->physicalFlags.bDisableCollisionForce
            && m_nType == ENTITY_TYPE_OBJECT && !physicalFlags.bInfiniteMass) 
        {
            physicalFlags.bDisableCollisionForce = true;
            m_fMass = 99999.9f;
            m_fTurnMass = 99999.9f;
        }
    }

    if (physicalFlags.bInfiniteMass)
        m_vecAttachedEntityRotation = GetPosition();

    m_pEntityIgnoredCollision = pOldEntityAttachedTo;
    PositionAttachedEntity();
#endif
}

bool CPhysical::CheckCollision()
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*))0x54D920)(this);
#else
    m_bCollisionProcessed = false;
    if (m_nType == ENTITY_TYPE_PED)
    {
        CPed* pPed = static_cast<CPed*>(this);
        if (!m_pAttachedTo && !physicalFlags.b17 && !physicalFlags.bProcessingShift && !physicalFlags.b13) {
            pPed->m_standingOnEntity = nullptr;
            if (pPed->bIsStanding) {
                pPed->bIsStanding = false;
                pPed->bWasStanding = true;
            }
        }

        if (pPed->IsPlayer()) {
            CTaskSimpleClimb* pTaskClimb = pPed->m_pIntelligence->GetTaskClimb();
            if (pTaskClimb) {
                char nHeightForPos = pTaskClimb->m_nHeightForPos;
                if (nHeightForPos == CLIMB_GRAB || nHeightForPos == CLIMB_PULLUP 
                    || nHeightForPos == CLIMB_STANDUP || nHeightForPos == CLIMB_VAULT) {
                    physicalFlags.b13 = true;
                }
            }
        }
    }

    if (CWorld::ms_nCurrentScanCode >= 65535u) {
        CWorld::ClearScanCodes();
        CWorld::ms_nCurrentScanCode = 1;
    }
    else {
        CWorld::ms_nCurrentScanCode++;
    }

    CRect boundRect;
    GetBoundRect(&boundRect);
    std::int32_t startSectorX = CWorld::GetSectorX(boundRect.left);
    std::int32_t startSectorY = CWorld::GetSectorY(boundRect.top);
    std::int32_t endSectorX = CWorld::GetSectorX(boundRect.right);
    std::int32_t endSectorY = CWorld::GetSectorY(boundRect.bottom);
    for (std::int32_t sectorY = startSectorY; sectorY <= endSectorY; ++sectorY) {
        for (std::int32_t sectorX = startSectorX; sectorX <= endSectorX; ++sectorX) {
            if (ProcessCollisionSectorList(sectorX, sectorY))
                return true;
        }
    }
    return false;  
#endif
}

bool CPhysical::CheckCollision_SimpleCar()
{
#ifdef USE_DEFAULT_FUNCTIONS
    return ((bool(__thiscall*)(CPhysical*))0x54DAB0)(this);
#else
    m_bCollisionProcessed = false;
    if (CWorld::ms_nCurrentScanCode >= 65535u) {
        CWorld::ClearScanCodes();
        CWorld::ms_nCurrentScanCode = 1;
    }
    else {
        ++CWorld::ms_nCurrentScanCode;
    }

    CEntryInfoNode* pEntryInfoNode = m_pCollisionList;
    if (!pEntryInfoNode)
        return false;

    while (!ProcessCollisionSectorList_SimpleCar(pEntryInfoNode->m_pRepeatSector))
    {
        pEntryInfoNode = pEntryInfoNode->m_pNext;
        if (!pEntryInfoNode)
            return false;
    }
    return true;
#endif
}
