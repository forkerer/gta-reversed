/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "StdInc.h"

unsigned short& CObject::nNoTempObjects = *(unsigned short*)(0xBB4A70);
float& CObject::fDistToNearestTree = *(float*)0x8D0A20;

void CObject::InjectHooks()
{
// VIRTUAL
    ReversibleHooks::Install("CObject", "SetIsStatic", 0x5A0760, &CObject::SetIsStatic_Reversed);
    ReversibleHooks::Install("CObject", "CreateRwObject", 0x59F110, &CObject::CreateRwObject_Reversed);
    ReversibleHooks::Install("CObject", "ProcessControl", 0x5A2130, &CObject::ProcessControl_Reversed);
    ReversibleHooks::Install("CObject", "Teleport", 0x5A17B0, &CObject::Teleport_Reversed);
    ReversibleHooks::Install("CObject", "SpecialEntityPreCollisionStuff", 0x59FEE0, &CObject::SpecialEntityPreCollisionStuff_Reversed);

// CLASS
    ReversibleHooks::Install("CObject", "Init", 0x59F840, &CObject::Init);
}

CObject::CObject() : CPhysical()
{
    m_pDummyObject = nullptr;
    CObject::Init();
    m_nObjectType = eObjectCreatedBy::OBJECT_UNKNOWN;
}

CObject::CObject(int modelId, bool bCreate) : CPhysical()
{
    m_pDummyObject = nullptr;
    if (bCreate)
        CEntity::SetModelIndex(modelId);
    else
        CEntity::SetModelIndexNoCreate(modelId);

    CObject::Init();
}

CObject::CObject(CDummyObject* pDummyObj) : CPhysical()
{
    CEntity::SetModelIndexNoCreate(pDummyObj->m_nModelIndex);
    if (pDummyObj->m_pRwObject)
        CEntity::AttachToRwObject(pDummyObj->m_pRwObject, true);
    else
        CPlaceable::SetMatrix(*pDummyObj->GetMatrix());

    pDummyObj->DetachFromRwObject();
    CObject::Init();

    m_nIplIndex = pDummyObj->m_nIplIndex;
    m_nAreaCode = pDummyObj->m_nAreaCode;
    m_bRenderDamaged = pDummyObj->m_bRenderDamaged;

    
    if (m_pRwObject)
    {
        auto* pAtomic = m_pRwAtomic;
        if (RwObjectGetType(m_pRwObject) != rpATOMIC)
            pAtomic = GetFirstAtomic(m_pRwClump);

        if (!CCustomBuildingRenderer::IsCBPCPipelineAttached(pAtomic))
            m_bLightObject = true;
    }
}

CObject::~CObject()
{
    if (objectFlags.b0x200000 || objectFlags.b0x100000)
    {
        const auto iIndex = CTheScripts::ScriptsForBrains.m_aScriptForBrains[m_wScriptTriggerIndex].m_nIMGindex + RESOURCE_ID_SCM;
        CStreaming::SetMissionDoesntRequireModel(iIndex);
        objectFlags.b0x100000 = false;
        objectFlags.b0x200000 = false;
        CTheScripts::RemoveFromWaitingForScriptBrainArray(this, m_wScriptTriggerIndex);
        m_wScriptTriggerIndex = -1;
    }

    if (objectFlags.bHasNoModel)
    {
        auto* const colModel = CModelInfo::GetModelInfo((m_nModelIndex))->GetColModel();
        CColStore::RemoveRef(colModel->m_boundSphere.m_nMaterial); //This seems weird, maybe BUG or Union field?
    }

    CRadar::ClearBlipForEntity(eBlipType::BLIP_OBJECT, CPools::ms_pObjectPool->GetRef(this));

    if (m_nRefModelIndex != -1)
        CModelInfo::GetModelInfo(m_nRefModelIndex)->RemoveRef();

    if (m_wRemapTxd != -1)
        CTxdStore::RemoveRef(m_wRemapTxd);

    if (IsTemporary() && CObject::nNoTempObjects)
        --CObject::nNoTempObjects;

    CObject::RemoveFromControlCodeList();
    if (m_nModelIndex == ModelIndices::MI_TRAINCROSSING1)
    {
        const auto& pDummyPos = m_pDummyObject->GetPosition();
        ThePaths.SetLinksBridgeLights(pDummyPos.x - 12.0F, pDummyPos.x + 12.0F, pDummyPos.y - 12.0F, pDummyPos.y + 12.0F, false);
    }

    if (m_pFire)
        m_pFire->Extinguish();
}

void* CObject::operator new(unsigned int size)
{
    return CPools::ms_pObjectPool->New();
}

void CObject::operator delete(void* pObj)
{
    CPools::ms_pObjectPool->Delete(static_cast<CObject*>(pObj));
}

void CObject::SetIsStatic(bool isStatic)
{
    return SetIsStatic_Reversed(isStatic);
}
void CObject::SetIsStatic_Reversed(bool isStatic)
{
    m_bIsStatic = isStatic;
    physicalFlags.b31 = false;
    if (!isStatic && (physicalFlags.bDisableMoveForce && m_fDoorStartAngle < -1000.0F))
        m_fDoorStartAngle = GetHeading();
}

void CObject::CreateRwObject()
{
    CObject::CreateRwObject_Reversed();
}
void CObject::CreateRwObject_Reversed()
{
    CEntity::CreateRwObject();
}

void CObject::ProcessControl()
{
    CObject::ProcessControl_Reversed();
}
void CObject::ProcessControl_Reversed()
{
    auto* pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
    auto bIsAnimated = false;
    if (pModelInfo->GetRwModelType() == rpCLUMP && pModelInfo->bHasAnimBlend && m_pRwObject)
        bIsAnimated = true;

    if (m_fDamageIntensity > 0.0F
        && objectFlags.bDamaged
        && !m_pAttachedTo
        && !IsCraneMovingPart()
        && !physicalFlags.bInfiniteMass
        && !physicalFlags.bDisableMoveForce
        && m_pDamageEntity)
    {
        if ((m_nModelIndex == eModelID::MODEL_DUMPER || m_nModelIndex == eModelID::MODEL_FORKLIFT) && !CRopes::IsCarriedByRope(this))
        {
            if (m_nColDamageEffect != eObjectColDamageEffect::COL_DAMAGE_EFFECT_NONE && m_fDamageIntensity > 5.0F)
                CObject::ObjectDamage(m_fDamageIntensity, &m_vecLastCollisionPosn, &m_vecLastCollisionImpactVelocity, m_pDamageEntity, eWeaponType::WEAPON_RAMMEDBYCAR);

            if (m_vecLastCollisionImpactVelocity.z > 0.3F)
            {
                m_bFakePhysics = 0;
                const auto vecColDir = GetPosition() - m_pDamageEntity->GetPosition();
                const auto vecEntSpeed = static_cast<CPhysical*>(m_pDamageEntity)->GetSpeed(vecColDir);
                auto vecSpeedDiff = vecEntSpeed - m_vecMoveSpeed;
                if (vecSpeedDiff.SquaredMagnitude() < 0.0001F && m_vecTurnSpeed.SquaredMagnitude() < 0.0001F)
                {
                    CPhysical::AttachEntityToEntity(static_cast<CPhysical*>(m_pDamageEntity), nullptr, nullptr);
                    m_fElasticity = 0.2F;
                }
            }
        }
    }

    objectFlags.bDamaged = false;
    if (!m_bIsStuck && !m_bCollisionProcessed && !m_bIsStaticWaitingForCollision)
    {
        if (!physicalFlags.bDisableZ && !physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce)
        {
            m_vecForce += m_vecMoveSpeed;
            m_vecForce /= 2.0F;
            m_vecTorque += m_vecTurnSpeed;
            m_vecTorque /= 2.0F;

            const auto fTimeStep = CTimer::ms_fTimeStep * 0.003F;
            if (pow(fTimeStep, 2.0F) <= m_vecForce.SquaredMagnitude()
                || pow(fTimeStep, 2.0F) <= m_vecTorque.SquaredMagnitude())
            {
                m_bFakePhysics = 0;
            }
            else
            {
                m_bFakePhysics++;
                if (m_bFakePhysics > 10 && !physicalFlags.bAttachedToEntity)
                {
                    m_bFakePhysics = 10;
                    if (!bIsAnimated)
                        SetIsStatic(true);

                    m_vecMoveSpeed.Set(0.0F, 0.0F, 0.0F);
                    m_vecTurnSpeed.Set(0.0F, 0.0F, 0.0F);
                    m_vecFrictionMoveSpeed.Set(0.0F, 0.0F, 0.0F);
                    m_vecFrictionTurnSpeed.Set(0.0F, 0.0F, 0.0F);
                    return;
                }
            }
        }
    }

    if (!m_bIsStatic && !m_bIsStaticWaitingForCollision)
        CPhysical::ProcessControl();

    if (bIsAnimated)
        SetIsStatic(false);

    CVector vecBuoyancyTurnPoint, vecBuoyancyForce;
    if (mod_Buoyancy.ProcessBuoyancy(this, m_fBuoyancyConstant, &vecBuoyancyTurnPoint, &vecBuoyancyForce))
    {
        physicalFlags.bTouchingWater = true;
        physicalFlags.bSubmergedInWater = true;
        SetIsStatic(false);

        CPhysical::ApplyMoveForce(vecBuoyancyForce);
        CPhysical::ApplyTurnForce(vecBuoyancyForce, vecBuoyancyTurnPoint);
        const auto fTimeStep = pow(0.97F, CTimer::ms_fTimeStep);
        m_vecMoveSpeed *= fTimeStep;
        m_vecTurnSpeed *= fTimeStep;
    }
    else if (m_nModelIndex != ModelIndices::MI_BUOY)
        physicalFlags.bTouchingWater = false; // Not clearing bSubmergedInWater, BUG?

    if (m_pObjectInfo->m_bCausesExplosion
        && !objectFlags.bIsExploded
        && m_bIsVisible
        && (rand() & 0x1F) == 10)
    {
        m_bUsesCollision = false;
        m_bIsVisible = false;
        physicalFlags.bExplosionProof = true;
        physicalFlags.bApplyGravity = false;
        m_vecMoveSpeed.Set(0.0F, 0.0F, 0.0F);
        DeleteRwObject();
    }

    if (m_nModelIndex == ModelIndices::MI_RCBOMB)
    {
        CPhysical::ApplyTurnForce(m_vecMoveSpeed * -0.05F, -GetForward());
        m_vecMoveSpeed *= pow(CTimer::ms_fTimeStep, 1.0F - m_vecMoveSpeed.SquaredMagnitude() * 0.2F);
    }

    if (m_bIsBIGBuilding)
        m_bIsInSafePosition = true;

    if (physicalFlags.bDisableMoveForce && m_fDoorStartAngle > -1000.0F)
    {
        auto fHeading = GetHeading();
        if (m_fDoorStartAngle + PI < fHeading)
            fHeading -= TWO_PI;
        else if (m_fDoorStartAngle - PI > fHeading)
            fHeading += TWO_PI;

        auto fDiff = m_fDoorStartAngle - fHeading;
        if (fabs(fDiff) > PI / 6.0F)
            objectFlags.bIsDoorOpen = true;

        static float fMaxDoorDiff = 0.3F;
        static float fDoorCutoffSpeed = 0.02F;
        static float fDoorSpeedMult = 0.002F;
        fDiff = clamp(fDiff, -fMaxDoorDiff, fMaxDoorDiff);
        if (fDiff > 0.0F && m_vecTurnSpeed.z < fDoorCutoffSpeed
            || fDiff < 0.0F && m_vecTurnSpeed.z > fDoorCutoffSpeed)
        {
            m_vecTurnSpeed.z += CTimer::ms_fTimeStep * fDoorSpeedMult * fDiff;
        }

        if (fDiff != 0.0F && objectFlags.bIsDoorMoving)
            AudioEngine.ReportDoorMovement(this);

        if (!m_bIsBIGBuilding
            && !m_bIsStatic
            && !m_bIsStaticWaitingForCollision
            && fabs(fDiff) < 0.01F
            && (objectFlags.bIsDoorMoving || fabs(m_vecTurnSpeed.z) < 0.01F))
        {
            SetIsStatic(true);
            m_vecMoveSpeed.Set(0.0F, 0.0F, 0.0F);
            m_vecTurnSpeed.Set(0.0F, 0.0F, 0.0F);
            m_vecFrictionMoveSpeed.Set(0.0F, 0.0F, 0.0F);
            m_vecFrictionTurnSpeed.Set(0.0F, 0.0F, 0.0F);

            if (objectFlags.bIsDoorMoving && objectFlags.bIsDoorOpen)
                CObject::LockDoor();
        }
    }
}

void CObject::Teleport(CVector destination, bool resetRotation)
{
    CObject::Teleport_Reversed(destination, resetRotation);
}
void CObject::Teleport_Reversed(CVector destination, bool resetRotation)
{
    CWorld::Remove(this);
    SetPosn(destination);
    CEntity::UpdateRW();
    CEntity::UpdateRwFrame();
    CWorld::Add(this);
}

void CObject::SpecialEntityPreCollisionStuff(CEntity* colEntity, bool bIgnoreStuckCheck, bool* bCollisionDisabled,
    bool* bCollidedEntityCollisionIgnored, bool* bCollidedEntityUnableToMove, bool* bThisOrCollidedEntityStuck)
{
    CObject::SpecialEntityPreCollisionStuff_Reversed(colEntity, bIgnoreStuckCheck, bCollisionDisabled,
        bCollidedEntityCollisionIgnored, bCollidedEntityUnableToMove, bThisOrCollidedEntityStuck);
}
void CObject::SpecialEntityPreCollisionStuff_Reversed(CEntity* colEntity, bool bIgnoreStuckCheck, bool* bCollisionDisabled,
    bool* bCollidedEntityCollisionIgnored, bool* bCollidedEntityUnableToMove, bool* bThisOrCollidedEntityStuck)
{
    auto* const pColPhysical = static_cast<CPhysical*>(colEntity);
    if(m_pEntityIgnoredCollision == colEntity || pColPhysical->m_pEntityIgnoredCollision == this)
    {
        *bCollidedEntityCollisionIgnored = true;
        return;
    }

    if (m_pAttachedTo == colEntity)
        *bCollisionDisabled = true;
    else if(pColPhysical->m_pAttachedTo == this || (m_pAttachedTo && m_pAttachedTo == pColPhysical->m_pAttachedTo))
        *bCollisionDisabled = true;
    else if(physicalFlags.bDisableZ && !physicalFlags.bApplyGravity && pColPhysical->physicalFlags.bDisableZ)
        *bCollisionDisabled = true;
    else
    {
        if (!physicalFlags.bDisableZ) {
            if (physicalFlags.bInfiniteMass || physicalFlags.bDisableMoveForce)
            {
                if (bIgnoreStuckCheck || m_bIsStuck)
                    *bCollisionDisabled = true;
                else if (!pColPhysical->m_bIsStuck) { /* Do nothing pretty much, and skip further calc */ }
                else if (!pColPhysical->m_bHasHitWall)
                    *bThisOrCollidedEntityStuck = true;
                else
                    *bCollidedEntityUnableToMove = true;
            }
            else if(objectFlags.bIsLampPost && (GetUp().z < 0.66F || m_bIsStuck))
            {
                if (colEntity->IsVehicle() || colEntity->IsPed()) {
                    *bCollidedEntityCollisionIgnored = true;
                    if (colEntity->IsVehicle())
                        return;

                    m_pEntityIgnoredCollision = colEntity;
                }
            }
            else if( colEntity->IsVehicle())
            {
                if (IsModelTempCollision())
                    *bCollisionDisabled = true;
                else if (m_nObjectType == eObjectCreatedBy::OBJECT_TEMPORARY || (objectFlags.bIsExploded || !CEntity::IsStatic()))
                {
                    if (colEntity->m_nModelIndex == eModelID::MODEL_DUMPER
                        || colEntity->m_nModelIndex == eModelID::MODEL_DOZER
                        || colEntity->m_nModelIndex == eModelID::MODEL_FORKLIFT)
                    {

                        if (m_bIsStuck || colEntity->m_bIsStuck)
                            *bThisOrCollidedEntityStuck = true;
                    }
                    else if (m_nColDamageEffect < eObjectColDamageEffect::COL_DAMAGE_EFFECT_SMASH_COMPLETELY)
                    {
                        auto tempMat = CMatrix();
                        auto* pColModel = CEntity::GetColModel();
                        auto vecSize = pColModel->GetBoundingBox().m_vecMax - pColModel->GetBoundingBox().m_vecMin;
                        auto vecTransformed = *m_matrix * vecSize;

                        auto& vecCollidedPos = colEntity->GetPosition();
                        if (vecTransformed.z < vecCollidedPos.z)
                        {
                            *bCollidedEntityCollisionIgnored = true;
                            m_pEntityIgnoredCollision = colEntity;
                        }
                        else
                        {
                            Invert(colEntity->GetMatrix(), &tempMat);
                            if ((tempMat * vecTransformed).z < 0.0F)
                            {
                                *bCollidedEntityCollisionIgnored = true;
                                m_pEntityIgnoredCollision = colEntity;
                            }
                        }
                    }
                }
            }
            else if(m_nModelIndex != eModelID::MODEL_GRENADE
                || !colEntity->IsPed()
                || GetPosition().z > colEntity->GetPosition().z)
            {
                if (colEntity->IsObject() && static_cast<CObject*>(colEntity)->m_pObjectInfo->m_fUprootLimit > 0.0F && !pColPhysical->m_pAttachedTo)
                {
                    if ((!pColPhysical->physicalFlags.bDisableCollisionForce || pColPhysical->physicalFlags.bCollidable)
                        && pColPhysical->m_fMass * 10.0F > m_fMass)
                    {
                        *bCollidedEntityUnableToMove = true;
                    }
                }
            }
        }
        else
        {
            if (bIgnoreStuckCheck)
                *bCollisionDisabled = true;
            else if (m_bIsStuck || colEntity->m_bIsStuck)
                *bThisOrCollidedEntityStuck = true;
        }
    }


    if (!*bCollidedEntityCollisionIgnored && (bIgnoreStuckCheck || m_bIsStuck))
        *bThisOrCollidedEntityStuck = true;

}

unsigned char CObject::SpecialEntityCalcCollisionSteps(bool* bProcessCollisionBeforeSettingTimeStep, bool* unk2)
{
    return CObject::SpecialEntityCalcCollisionSteps_Reversed(bProcessCollisionBeforeSettingTimeStep, unk2);
}
unsigned char CObject::SpecialEntityCalcCollisionSteps_Reversed(bool* bProcessCollisionBeforeSettingTimeStep, bool* unk2)
{
    return plugin::CallMethodAndReturn<unsigned char, 0x5A02E0, CObject*, bool*, bool*>(this, bProcessCollisionBeforeSettingTimeStep, unk2);
}

void CObject::PreRender()
{
    CObject::PreRender_Reversed();
}
void CObject::PreRender_Reversed()
{
    plugin::CallMethod<0x59FD50, CObject*>(this);
}

void CObject::Render()
{
    CObject::Render_Reversed();
}
void CObject::Render_Reversed()
{
    plugin::CallMethod<0x59F180, CObject*>(this);
}

bool CObject::SetupLighting()
{
    return CObject::SetupLighting_Reversed();
}
bool CObject::SetupLighting_Reversed()
{
    return plugin::CallMethodAndReturn<bool, 0x554FA0, CObject*>(this);
}

void CObject::RemoveLighting(bool bRemove)
{
    CObject::RemoveLighting_Reversed(bRemove);
}
void CObject::RemoveLighting_Reversed(bool bRemove)
{
    plugin::CallMethod<0x553E10, CObject*, bool>(this, bRemove);
}

// Converted from thiscall void CObject::ProcessGarageDoorBehaviour(void) 0x44A4D0
void CObject::ProcessGarageDoorBehaviour() {
    ((void(__thiscall*)(CObject*))0x44A4D0)(this);
}

// Converted from thiscall bool CObject::CanBeDeleted(void) 0x59F120
bool CObject::CanBeDeleted() {
    return ((bool(__thiscall*)(CObject*))0x59F120)(this);
}

// Converted from thiscall void CObject::SetRelatedDummy(CDummyObject *relatedDummy) 0x59F160
void CObject::SetRelatedDummy(CDummyObject* relatedDummy) {
    ((void(__thiscall*)(CObject*, CDummyObject*))0x59F160)(this, relatedDummy);
}



// Converted from cdecl void CObject::SetMatrixForTrainCrossing(CMatrix *matrix,float) 0x59F200
void CObject::SetMatrixForTrainCrossing(CMatrix* matrix, float arg1) {
    ((void(__cdecl*)(CMatrix*, float))0x59F200)(matrix, arg1);
}

// Converted from thiscall bool CObject::TryToExplode(void) 0x59F2D0
bool CObject::TryToExplode() {
    return ((bool(__thiscall*)(CObject*))0x59F2D0)(this);
}

// Converted from thiscall void CObject::SetObjectTargettable(uchar targetable) 0x59F300
void CObject::SetObjectTargettable(unsigned char targetable) {
    ((void(__thiscall*)(CObject*, unsigned char))0x59F300)(this, targetable);
}

// Converted from thiscall bool CObject::CanBeTargetted(void) 0x59F320
bool CObject::CanBeTargetted() {
    return ((bool(__thiscall*)(CObject*))0x59F320)(this);
}

// Converted from thiscall void CObject::RefModelInfo(int modelIndex) 0x59F330
void CObject::RefModelInfo(int modelIndex) {
    plugin::CallMethod<0x59F330, CObject*, int>(this, modelIndex);
}

// Converted from thiscall void CObject::SetRemapTexture(RwTexture *remapTexture, short txdIndex) 0x59F350
void CObject::SetRemapTexture(RwTexture* remapTexture, short txdIndex) {
    ((void(__thiscall*)(CObject*, RwTexture*, short))0x59F350)(this, remapTexture, txdIndex);
}

// Converted from thiscall float CObject::GetRopeHeight(void) 0x59F380
float CObject::GetRopeHeight() {
    return ((float(__thiscall*)(CObject*))0x59F380)(this);
}

// Converted from thiscall void CObject::SetRopeHeight(float height) 0x59F3A0
void CObject::SetRopeHeight(float height) {
    ((void(__thiscall*)(CObject*, float))0x59F3A0)(this, height);
}

// Converted from thiscall CEntity* CObject::GetObjectCarriedWithRope(void) 0x59F3C0
CEntity* CObject::GetObjectCarriedWithRope() {
    return ((CEntity * (__thiscall*)(CObject*))0x59F3C0)(this);
}

// Converted from thiscall void CObject::ReleaseObjectCarriedWithRope(void) 0x59F3E0
void CObject::ReleaseObjectCarriedWithRope() {
    ((void(__thiscall*)(CObject*))0x59F3E0)(this);
}

// Converted from thiscall void CObject::AddToControlCodeList(void) 0x59F400
void CObject::AddToControlCodeList() {
    ((void(__thiscall*)(CObject*))0x59F400)(this);
}

// Converted from thiscall void CObject::RemoveFromControlCodeList(void) 0x59F450
void CObject::RemoveFromControlCodeList() {
    ((void(__thiscall*)(CObject*))0x59F450)(this);
}

// Converted from thiscall void CObject::ResetDoorAngle(void) 0x59F4B0
void CObject::ResetDoorAngle() {
    ((void(__thiscall*)(CObject*))0x59F4B0)(this);
}

// Converted from thiscall void CObject::LockDoor(void) 0x59F5C0
void CObject::LockDoor() {
    ((void(__thiscall*)(CObject*))0x59F5C0)(this);
}

// Converted from thiscall void CObject::Init(void) 0x59F840
void CObject::Init() {
    m_nType = eEntityType::ENTITY_TYPE_OBJECT;
    m_pObjectInfo = &CObjectData::GetDefault();
    m_nColDamageEffect = eObjectColDamageEffect::COL_DAMAGE_EFFECT_NONE;
    m_nSpecialColResponseCase = eObjectSpecialColResponseCases::COL_SPECIAL_RESPONSE_NONE;
    m_nObjectType = eObjectCreatedBy::OBJECT_GAME;
    SetIsStatic(true);

    m_nObjectFlags &= 0x0FC040000;
    objectFlags.bCanBeAttachedToMagnet = true;

    if (m_nModelIndex == 0xFFFF)
    {
        objectFlags.bHasNoModel = false;
    }
    else
    {
        CObjectData::SetObjectData(m_nModelIndex, *this);
        auto *pModelInfo = CModelInfo::GetModelInfo(m_nModelIndex);
        if (pModelInfo->GetColModel()->m_boundSphere.m_bFlag0x01)
        {
            CColStore::AddRef(pModelInfo->GetColModel()->m_boundSphere.m_nMaterial);
            objectFlags.bHasNoModel = true;

            auto *pAtomicInfo = pModelInfo->AsAtomicModelInfoPtr();
            if (pAtomicInfo && pAtomicInfo->SwaysInWind() && !physicalFlags.bDisableCollisionForce)
            {
                auto& pBndBox = pModelInfo->GetColModel()->GetBoundingBox();
                m_vecCentreOfMass.z = pBndBox.m_vecMin.z + (pBndBox.m_vecMax.z - pBndBox.m_vecMin.z) * 0.2F;
            }
        }
    }

    if (physicalFlags.bDisableMoveForce)
    {
        auto* pColData = CEntity::GetColModel()->m_pColData;
        if (pColData)
            pColData->m_nNumSpheres = 0;
    }

    m_fHealth = 1000.0F;
    m_fDoorStartAngle = -1001.0F;
    m_dwRemovalTime = 0;
    m_nBonusValue = 0;
    m_wCostValue = 0;
    for (auto& col : m_nCarColor)
        col = 0;

    m_nRefModelIndex = -1;
    m_nGarageDoorGarageIndex = -1;
    m_nLastWeaponDamage = -1;
    m_pFire = nullptr;

    if (m_nModelIndex == ModelIndices::MI_BUOY)
        physicalFlags.bTouchingWater = true;

    if (m_nModelIndex != 0xFFFF && CModelInfo::GetModelInfo(m_nModelIndex)->GetModelType() == ModelInfoType::MODEL_INFO_WEAPON)
        m_bLightObject = true;

    if (m_nModelIndex == ModelIndices::MI_MLAMPPOST
        || m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_MIAMI
        || m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_VEGAS
        || m_nModelIndex == ModelIndices::MI_TRAFFICLIGHTS_TWOVERTICAL
        || m_nModelIndex == ModelIndices::MI_SINGLESTREETLIGHTS1
        || m_nModelIndex == ModelIndices::MI_SINGLESTREETLIGHTS2
        || m_nModelIndex == ModelIndices::MI_SINGLESTREETLIGHTS3
        || m_nModelIndex == ModelIndices::MI_DOUBLESTREETLIGHTS
        || m_nModelIndex != 0xFFFF && CModelInfo::GetModelInfo(m_nModelIndex)->AsAtomicModelInfoPtr() && CModelInfo::GetModelInfo(m_nModelIndex)->SwaysInWind())
    {
        objectFlags.bIsLampPost = true;
    }
    else
    {
        objectFlags.bIsLampPost = false;
    }

    objectFlags.bIsTargatable = false;
    physicalFlags.bAttachedToEntity = false;

    m_nAreaCode = eAreaCodes::AREA_CODE_13;
    m_wRemapTxd = -1;
    m_pRemapTexture = nullptr;
    m_pControlCodeList = nullptr;

    if (m_nModelIndex == ModelIndices::MI_SAMSITE
        || m_nModelIndex == ModelIndices::MI_SAMSITE2
        || m_nModelIndex == ModelIndices::MI_TRAINCROSSING
        || m_nModelIndex == ModelIndices::MI_TRAINCROSSING1
        || m_nModelIndex == ModelIndices::MI_MAGNOCRANE
        || m_nModelIndex == ModelIndices::MI_CRANETROLLEY
        || m_nModelIndex == ModelIndices::MI_QUARRYCRANE_ARM
        || CGarages::IsModelIndexADoor(static_cast<int16_t>(m_nModelIndex)))
    {
        CObject::AddToControlCodeList();
    }

    m_dwBurnTime = 0;
    m_fScale = 1.0F;

    m_nDayBrightness = 0x8;
    m_nNightBrightness = 0x4;
    m_wScriptTriggerIndex = -1;
}

// Converted from thiscall void CObject::DoBurnEffect(void) 0x59FB50
void CObject::DoBurnEffect() {
    ((void(__thiscall*)(CObject*))0x59FB50)(this);
}

// Converted from thiscall uchar CObject::GetLightingFromCollisionBelow(void) 0x59FD00
unsigned char CObject::GetLightingFromCollisionBelow() {
    return ((unsigned char(__thiscall*)(CObject*))0x59FD00)(this);
}

// Converted from thiscall void CObject::ProcessSamSiteBehaviour(void) 0x5A07D0
void CObject::ProcessSamSiteBehaviour() {
    ((void(__thiscall*)(CObject*))0x5A07D0)(this);
}

// Converted from thiscall void CObject::ProcessTrainCrossingBehaviour(void) 0x5A0B50
void CObject::ProcessTrainCrossingBehaviour() {
    ((void(__thiscall*)(CObject*))0x5A0B50)(this);
}

// Converted from thiscall void CObject::ObjectDamage(float damage,CVector *fxOrigin,CVector *fxDirection,CEntity *damager,eWeaponType weaponType) 0x5A0D90
void CObject::ObjectDamage(float damage, CVector* fxOrigin, CVector* fxDirection, CEntity* damager, eWeaponType weaponType) {
    ((void(__thiscall*)(CObject*, float, CVector*, CVector*, CEntity*, eWeaponType))0x5A0D90)(this, damage, fxOrigin, fxDirection, damager, weaponType);
}

// Converted from thiscall void CObject::Explode(void) 0x5A1340
void CObject::Explode() {
    ((void(__thiscall*)(CObject*))0x5A1340)(this);
}

// Converted from thiscall void CObject::ObjectFireDamage(float damage,CEntity *damager) 0x5A1580
void CObject::ObjectFireDamage(float damage, CEntity* damager) {
    ((void(__thiscall*)(CObject*, float, CEntity*))0x5A1580)(this, damage, damager);
}

// Converted from cdecl void CObject::TryToFreeUpTempObjects(int numObjects) 0x5A1840
void CObject::TryToFreeUpTempObjects(int numObjects) {
    ((void(__cdecl*)(int))0x5A1840)(numObjects);
}

// Converted from cdecl void CObject::DeleteAllTempObjects(void) 0x5A18B0
void CObject::DeleteAllTempObjects() {
    ((void(__cdecl*)())0x5A18B0)();
}

// Converted from cdecl void CObject::DeleteAllMissionObjects(void) 0x5A1910
void CObject::DeleteAllMissionObjects() {
    ((void(__cdecl*)())0x5A1910)();
}

// Converted from cdecl void CObject::DeleteAllTempObjectsInArea(CVector point,float radius) 0x5A1980
void CObject::DeleteAllTempObjectsInArea(CVector point, float radius) {
    ((void(__cdecl*)(CVector, float))0x5A1980)(point, radius);
}

// Converted from thiscall void CObject::GrabObjectToCarryWithRope(CPhysical *attachTo) 0x5A1AB0
void CObject::GrabObjectToCarryWithRope(CPhysical* attachTo) {
    ((void(__thiscall*)(CObject*, CPhysical*))0x5A1AB0)(this, attachTo);
}

// Converted from thiscall bool CObject::CanBeUsedToTakeCoverBehind(void) 0x5A1B60
bool CObject::CanBeUsedToTakeCoverBehind() {
    return ((bool(__thiscall*)(CObject*))0x5A1B60)(this);
}

CObject* CObject::Create(int modelIndex) {
    return ((CObject * (__cdecl*)(int))0x5A1F60)(modelIndex);
}

CObject* CObject::Create(CDummyObject* dummyObject) {
    return ((CObject * (__cdecl*)(CDummyObject*))0x5A2070)(dummyObject);
}

// Converted from thiscall void CObject::ProcessControlLogic(void) 0x5A29A0
void CObject::ProcessControlLogic() {
    ((void(__thiscall*)(CObject*))0x5A29A0)(this);
}

// Converted from cdecl bool IsObjectPointerValid_NotInWorld(CObject *object) 0x5A2B90
bool IsObjectPointerValid_NotInWorld(CObject* object) {
    return ((bool(__cdecl*)(CObject*))0x5A2B90)(object);
}

// Converted from cdecl bool IsObjectPointerValid(CObject *object) 0x5A2C20
bool IsObjectPointerValid(CObject* object) {
    return ((bool(__cdecl*)(CObject*))0x5A2C20)(object);
}
