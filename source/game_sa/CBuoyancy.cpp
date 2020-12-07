#include "StdInc.h"

cBuoyancy& mod_Buoyancy = *(cBuoyancy*)0xC1C890;
float& cBuoyancy::fPointVolMultiplier = *(float*)0x8D32C8;
CBuoyancyCalcStruct& cBuoyancy::calcStruct = *(CBuoyancyCalcStruct*)0xC1C858;
float(*cBuoyancy::afBoatVolumeDistributionSpeed)[3] = (float(*)[3])0x8D335C;
float(*cBuoyancy::afBoatVolumeDistributionDinghy)[3] = (float(*)[3])0x8D3338;
float(*cBuoyancy::afBoatVolumeDistributionSail)[3] = (float(*)[3])0x8D3314;
float(*cBuoyancy::afBoatVolumeDistribution)[3] = (float(*)[3])0x8D32CC;
float(*cBuoyancy::afBoatVolumeDistributionCat)[3] = (float(*)[3])0x8D3314; // Catamaran volume distribution, unused in game, as there is no matching vehicle

void cBuoyancy::InjectHooks()
{
    HookInstall(0x6C3EF0, &cBuoyancy::ProcessBuoyancy);
    HookInstall(0x6C3030, &cBuoyancy::ProcessBuoyancyBoat);
    HookInstall(0x6C2750, &cBuoyancy::CalcBuoyancyForce);
    HookInstall(0x6C2B90, &cBuoyancy::PreCalcSetup);
    HookInstall(0x6C34E0, &cBuoyancy::AddSplashParticles);
    HookInstall(0x6C3B00, &cBuoyancy::SimpleCalcBuoyancy);
    HookInstall(0x6C2970, &cBuoyancy::SimpleSumBuoyancyData);
    HookInstall(0x6C2810, &cBuoyancy::FindWaterLevel);
    HookInstall(0x6C28C0, &cBuoyancy::FindWaterLevelNorm);
}

bool cBuoyancy::ProcessBuoyancy(CPhysical* pEntity, float fBuoyancy, CVector* pVecTurnSpeed, CVector* pBuoyancy)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<bool, 0x6C3EF0, cBuoyancy*, CEntity*, float, CVector*, CVector*> 
        (this, pEntity, fBuoyancy, pVecTurnSpeed, pBuoyancy);
#else
    CVector& entityPosition = pEntity->GetPosition();
    if (CWaterLevel::GetWaterLevel(entityPosition.x, entityPosition.y, entityPosition.z,
        &m_fWaterLevel, pEntity->physicalFlags.bTouchingWater, nullptr))
    {
        m_EntityMatrix = *pEntity->m_matrix;
        PreCalcSetup(pEntity, fBuoyancy);
        if (pEntity->m_nType == ENTITY_TYPE_PED)
        {
            pEntity->GetColModel(); // for some reason, this is here?

            m_bInWater = 1;
            m_fEntityWaterImmersion = ((m_fWaterLevel - entityPosition.z) + 1.0F) / 1.9F;
            if (m_fEntityWaterImmersion > 1.0F)
            {
                m_fEntityWaterImmersion = 1.0F;
            }
            else {
                if (m_fEntityWaterImmersion < 0.0F) 
                {
                    m_fEntityWaterImmersion = 0.0F;
                    m_bInWater = 0;
                }
            }

            m_vecMoveForce = CVector(0.0F, 0.0F, 0.0F);
            if (m_fEntityWaterImmersion > 0.0F && m_fEntityWaterImmersion < 1.0F)
            {
                float fDistanceZ = m_fWaterLevel - entityPosition.z;
                CVector forward = pEntity->GetForwardVector();
                cBuoyancy::AddSplashParticles
                    (pEntity, CVector(0.0F, 0.0F, fDistanceZ), 
                        CVector(0.0F, 0.0F, fDistanceZ), 
                        CVector(-forward.x, -forward.y,-forward.z), true);
            }
        }
        else 
        {
            SimpleCalcBuoyancy(pEntity);
        }

        bool bCalcBuoyancyForce = CalcBuoyancyForce(pEntity, pVecTurnSpeed, pBuoyancy);
        if (m_bProcessingBoat || bCalcBuoyancyForce)
        {
            return true;
        }
    }
    return false;
#endif
}

bool cBuoyancy::ProcessBuoyancyBoat(CVehicle* pVehicle, float fBuoyancy, CVector* pVecTurnSpeed, CVector* pVecUnknown, bool bUnderwater)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<bool, 0x6C3030, cBuoyancy*, CVehicle*, float, CVector*, CVector*, bool>
        (this, pVehicle, fBuoyancy, pVecTurnSpeed, pVecUnknown, bUnderwater);
#else
    const CVector& entityPosition = pVehicle->GetPosition();
    if (!CWaterLevel::GetWaterLevel(entityPosition.x, entityPosition.y, entityPosition.z,
        &m_fWaterLevel, pVehicle->physicalFlags.bTouchingWater, nullptr)) {

        return false;
    }

    m_EntityMatrix = *pVehicle->m_matrix;
    PreCalcSetup(pVehicle, fBuoyancy);
    float fBoundingHeight = m_vecBoundingMax.z - m_vecBoundingMin.z;
    auto fBoatHeightRatio = 1.0F / (fBoundingHeight * 9.0F);

    for (int32_t iXMult = 0; iXMult < 3; ++iXMult) {
        for (int32_t iYMult = 0; iYMult < 3; ++iYMult) {
            auto fCurrentX = m_vecBoundingMin.x + m_vecCenterOffset.x * iXMult;
            auto fCurrentY = m_vecBoundingMin.y + m_vecCenterOffset.y * iYMult;
            CVector vecCurPoint(fCurrentX, fCurrentY, 0.0F);

            auto vecWaveNormal = CVector(0.0F, 0.0F, 1.0F);
            eBuoyancyPointState eState;
            FindWaterLevelNorm(m_vecInitialZPos, &vecCurPoint, &eState, &vecWaveNormal);

            auto fThird = 1.0F / 3.0F;
            vecWaveNormal.z += 2.0F;
            vecWaveNormal *= fThird;

            switch (pVehicle->m_nModelIndex) {
            case eModelID::MODEL_SQUALO:  //446
            case eModelID::MODEL_SPEEDER: //452
            case eModelID::MODEL_JETMAX:  //493
            case eModelID::MODEL_LAUNCH:  //595
                cBuoyancy::fPointVolMultiplier = cBuoyancy::afBoatVolumeDistributionSpeed[iYMult][iXMult];
                break;
            case eModelID::MODEL_COASTG:  //472
            case eModelID::MODEL_DINGHY:  //473
                cBuoyancy::fPointVolMultiplier = cBuoyancy::afBoatVolumeDistributionDinghy[iYMult][iXMult];
                break;
            case eModelID::MODEL_MARQUIS: //484
                cBuoyancy::fPointVolMultiplier = cBuoyancy::afBoatVolumeDistributionSail[iYMult][iXMult];
                break;
            default:
                cBuoyancy::fPointVolMultiplier = cBuoyancy::afBoatVolumeDistribution[iYMult][iXMult];
                break;
            }

            if (eState == eBuoyancyPointState::COMPLETELY_ABOVE_WATER)
                continue;

            auto fAddedDistToSurface = SimpleSumBuoyancyData(&vecCurPoint, eState);
            auto fCurBuoyancy = CTimer::ms_fTimeStep * m_fBuoyancy;
            auto fPointContribution = fCurBuoyancy * (fAddedDistToSurface * fBoatHeightRatio);

            CVector vecOffset(fCurrentX, fCurrentY, 0.0F);
            CVector vecTransformedPos;
            Multiply3x3(&vecTransformedPos, pVehicle->m_matrix, &vecOffset);

            CVector vecSpeedAtPoint;
            pVehicle->GetSpeed(&vecSpeedAtPoint, vecTransformedPos);
            auto pHandling = pVehicle->m_pHandlingData;
            auto fWavePower = 1.0F - DotProduct(vecSpeedAtPoint, vecWaveNormal) * pHandling->m_fSuspensionDampingLevel;
            fWavePower = std::max(fWavePower, 0.0F);
            fWavePower *= fPointContribution;
            pVecUnknown->z += fWavePower;

            if (!bUnderwater) {
                auto vecTurnForceAtPoint = vecWaveNormal * fWavePower;
                CVector vecAppliedForcePoint;
                Multiply3x3(&vecAppliedForcePoint, &m_EntityMatrix, &vecCurPoint);
                pVehicle->ApplyTurnForce(vecTurnForceAtPoint, vecAppliedForcePoint);
            }
        }
    }

    m_fEntityWaterImmersion *= fBoatHeightRatio;
    Multiply3x3(pVecTurnSpeed, &m_EntityMatrix, &m_vecMoveForce);

    if (m_bProcessingBoat)
        return true;

    return m_bInWater;
#endif
}

bool cBuoyancy::CalcBuoyancyForce(CPhysical* pEntity, CVector* pVecTurnSpeed, CVector* pBuoyancy)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<bool, 0x6C2750, cBuoyancy*, CPhysical* , CVector*, CVector*>(this, pEntity, pVecTurnSpeed, pBuoyancy);
#else
    if (!m_bInWater)
    {
        return false;
    }

    CVector vecOut;
    *pVecTurnSpeed = *Multiply3x3(&vecOut, &m_EntityMatrix, &m_vecMoveForce);
    const float fCurrentBuoyancy = m_fEntityWaterImmersion * m_fBuoyancy * CTimer::ms_fTimeStep;
    *pBuoyancy = CVector(0.0F, 0.0F, fCurrentBuoyancy);

    float fSubmerge = pEntity->m_fMass * pEntity->m_vecMoveSpeed.z;
    if (fSubmerge <= fCurrentBuoyancy * 4.0F)
    {
        return true;
    }

    float fBuoyancy = fCurrentBuoyancy - fSubmerge;
    if (fBuoyancy < 0.0F)
    {
        fBuoyancy = 0.0F;
    }
    pBuoyancy->z = fBuoyancy;
    return true;
#endif
}

void cBuoyancy::PreCalcSetup(CPhysical* pEntity, float fBuoyancy)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x6C2B90, cBuoyancy*, CPhysical*, float>(this, pEntity, fBuoyancy);
#else
    auto bIsVehicle = pEntity->m_nType == eEntityType::ENTITY_TYPE_VEHICLE;
    m_bProcessingBoat = bIsVehicle && ((CVehicle*)pEntity)->m_nVehicleClass == eVehicleClass::CLASS_BIG;
    auto pColModel = pEntity->GetColModel();
    m_vecBoundingMin = pColModel->m_boundBox.m_vecMin;
    m_vecBoundingMax = pColModel->m_boundBox.m_vecMax;

    if (!m_bProcessingBoat) {
        switch (pEntity->m_nModelIndex) {
        case eModelID::MODEL_LEVIATHN: //417
            m_vecBoundingMin.y *= 0.4F;
            m_vecBoundingMax.y *= 1.15F;
            break;
        case eModelID::MODEL_SKIMMER: //460
            m_vecBoundingMin.x *= 0.3F;
            m_vecBoundingMin.y = m_vecBoundingMax.y * -0.6F;
            m_vecBoundingMin.z -= 0.1F;
            m_vecBoundingMax.x *= 0.3F;
            m_vecBoundingMax.y *= 1.4F;
            break;
        default:
            if (bIsVehicle && ((CVehicle*)pEntity)->m_nVehicleSubClass == 3) {
                m_vecBoundingMin.y = -m_vecBoundingMax.y;
                m_vecBoundingMax.z = m_vecBoundingMin.z * -1.1F;
                m_vecBoundingMin.z *= 0.85F;
            }
            break;
        }
    }
    else {
        switch (pEntity->m_nModelIndex) {
        case eModelID::MODEL_SQUALO: //446
            m_vecBoundingMax.y *= 0.9F;
            m_vecBoundingMin.y *= 0.9F;
            break;
        case eModelID::MODEL_SPEEDER: //452
            m_vecBoundingMax.y *= 1.25F;
            m_vecBoundingMin.y *= 0.83F;
            break;
        case eModelID::MODEL_REEFER: //453
        case eModelID::MODEL_JETMAX: //498
            m_vecBoundingMin.y *= 0.9F;
            break;
        case eModelID::MODEL_TROPIC: //454
            m_vecBoundingMax.y *= 1.3F;
            m_vecBoundingMin.y *= 0.82F;
            m_vecBoundingMin.z -= 0.2F;
            break;
        case eModelID::MODEL_COASTG: //472
            m_vecBoundingMax.y *= 1.1F;
            m_vecBoundingMin.y *= 0.9F;
            m_vecBoundingMin.z -= 0.3F;
            break;
        case eModelID::MODEL_DINGHY: //473
            m_vecBoundingMax.y *= 1.3F;
            m_vecBoundingMin.y *= 0.9F;
            m_vecBoundingMin.z -= 0.2F;
            break;
        case eModelID::MODEL_MARQUIS: //484
            m_vecBoundingMax.y *= 1.1F;
            m_vecBoundingMin.y *= 0.9F;
            break;
        case eModelID::MODEL_LAUNCH: //595
            m_vecBoundingMax.y *= 1.25F;
            m_vecBoundingMin.y *= 0.8F;
            m_vecBoundingMin.z -= 0.1F;
            break;
        default:
            m_vecBoundingMax.y *= 1.05F;
            m_vecBoundingMin.y *= 0.9F;
            break;
        }
    }

    // Calculate center offset and "normalized" version of it
    // (it's not normalised vector, just the offset scaled by multiplicative inverse of it's longest component
    //  so the longest component is equal to 1.0F)
    m_vecCenterOffset = (m_vecBoundingMax - m_vecBoundingMin) * 0.5F;
    float fScale;
    if ((m_vecCenterOffset.z > m_vecCenterOffset.x) && (m_vecCenterOffset.z > m_vecCenterOffset.y))
        fScale = 1.0F / m_vecCenterOffset.z;
    else if ((m_vecCenterOffset.y > m_vecCenterOffset.x) && (m_vecCenterOffset.y > m_vecCenterOffset.z))
        fScale = 1.0F / m_vecCenterOffset.y;
    else
        fScale = 1.0F / m_vecCenterOffset.x;

    m_vecNormalizedCenterOffset = m_vecCenterOffset * fScale;
    m_fNumCheckedPoints = 1.0F;
    m_vecMoveForce.Set(0.0F, 0.0F, 0.0F);
    m_bInWater = false;
    m_fEntityWaterImmersion = 0.0F;
    m_vecPos = pEntity->GetPosition();
    m_fWaterLevel += m_fUnkn2;
    m_vecInitialZPos.Set(0.0F, 0.0F, m_vecPos.z);
    m_fBuoyancy = fBuoyancy;
#endif
}

void cBuoyancy::AddSplashParticles(CPhysical* pEntity, CVector vecFrom, CVector vecTo, CVector vecSplashDir, bool bReduceParticleSize)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x6C34E0, cBuoyancy*, CPhysical*, CVector, CVector, CVector, bool>(this, pEntity, vecFrom, vecTo, vecSplashDir, bReduceParticleSize);
#else
    auto fDistBetweenPoints = DistanceBetweenPoints(vecFrom, vecTo);
    auto vecUsedSpeed = pEntity->m_vecMoveSpeed;
    auto fMoveSpeed = vecUsedSpeed.Magnitude();
    if (fMoveSpeed > 0.1F) {
        auto fMult = 1.0F / fMoveSpeed;
        vecUsedSpeed *= (fMult * 0.1F); // Get the speed in [0 : 0.1] range
        fMoveSpeed = 0.1F;
    }

    if (fMoveSpeed <= 0.05F)
        return;

    FxPrtMult_c curParticle(1.0F, 1.0F, 1.0F, 0.1F, 0.5F, 1.0F, 0.2F);
    if (bReduceParticleSize) {
        curParticle.m_fSize *= 0.5F;
        curParticle.m_fLife *= 0.5F;
    }

    auto vecVelocityModifier = vecUsedSpeed * CVector(0.0F, 0.0F, -1.0F) * 120.0F;

    auto iNumParticles = std::max(1, static_cast<int>(fDistBetweenPoints + fDistBetweenPoints));
    for (int32_t iIter = 0; iIter < iNumParticles; ++iIter) {
        auto fCurrentProgress = static_cast<float>(iIter) / static_cast<float>(iNumParticles);
        auto vecCurPoint = Lerp(vecFrom, vecTo, fCurrentProgress);
        auto vecTransformedPoint = (*pEntity->m_matrix) * vecCurPoint;

        if (pEntity->m_nType != eEntityType::ENTITY_TYPE_PED) {
            auto& vecEntPos = pEntity->GetPosition();
            vecSplashDir = vecTransformedPoint - vecEntPos;
        }
        vecSplashDir.z = 0.0F;
        vecSplashDir.Normalise();

        auto vecVelocity = vecSplashDir * fMoveSpeed * 60.0F + vecVelocityModifier;
        float fRand = CGeneral::GetRandomNumberInRange(0.0F, 0.5F);
        vecTransformedPoint += (vecSplashDir * fRand);
        g_fx.m_pPrtWatersplash->AddParticle(&vecTransformedPoint, &vecVelocity, 0.0F, &curParticle, -1.0F, 1.2F, 0.6F, 0);
    }

    if (pEntity->m_nType == eEntityType::ENTITY_TYPE_PED) {
        auto pPed = reinterpret_cast<CPed*>(pEntity);
        auto pSwimTask = pPed->m_pIntelligence->GetTaskSwim();
        if (!pSwimTask) {
            auto& vecPedForward = pEntity->GetForwardVector();
            auto fPedAngle = CGeneral::GetAngleBetweenPoints(vecPedForward.x, vecPedForward.y, 0.0F, 0.0F);
            fPedAngle = CGeneral::LimitAngle(fPedAngle) + 180.0F;

            curParticle = FxPrtMult_c(1.0F, 1.0F, 1.0F, 0.2F, 0.4F, 0.0F, 0.5F);
            auto vecPedVelocity = CVector(0.0F, 0.0F, 0.0F);
            auto vecPedParticlePos = pEntity->GetPosition() + (vecPedForward * 0.4F);
        
            if (pPed->m_pPlayerData)
                vecPedParticlePos.z = pPed->m_pPlayerData->m_fWaterHeight;
            else
                vecPedParticlePos.z += 0.5F;

            g_fx.m_pPrtWake->AddParticle(&vecPedParticlePos, &vecPedVelocity, 0.0F, &curParticle, fPedAngle, 1.2F, 0.6F, 0);
            pPed->m_pedAudio.AddAudioEvent(eAudioEvents::AE_PED_SWIM_WAKE, 0.0F, 1.0F, 0, 0, 0, 0);
        }
    }

    AudioEngine.ReportWaterSplash(pEntity, -100.0F, 0);
#endif
}

void cBuoyancy::SimpleCalcBuoyancy(CPhysical* pEntity)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x6C3B00, cBuoyancy*, CPhysical*>(this, pEntity);
#else
    CVector vecAllPoints[3][3];

    for (int32_t iXMult = 0; iXMult < 3; ++iXMult) {
        for (int32_t iYMult = 0; iYMult < 3; ++iYMult) {
            auto& pCurVec = vecAllPoints[iXMult][iYMult];
            pCurVec.Set(0.0F, 0.0F, 0.0F);

            auto vecCurPoint = CVector();
            vecCurPoint.x = m_vecBoundingMin.x + (m_vecCenterOffset.x * iXMult);
            vecCurPoint.y = m_vecBoundingMin.y + (m_vecCenterOffset.y * iYMult);
            vecCurPoint.z = 0.0F;
            eBuoyancyPointState eState;
            FindWaterLevel(m_vecInitialZPos, &vecCurPoint, &eState);
            cBuoyancy::fPointVolMultiplier = 1.0F;

            if (eState == eBuoyancyPointState::COMPLETELY_ABOVE_WATER)
                pCurVec.z = m_vecBoundingMin.z;
            else {
                SimpleSumBuoyancyData(&vecCurPoint, eState);
                pCurVec = vecCurPoint;
            }
        }
    }

    auto fHeight = m_vecBoundingMax.z - m_vecBoundingMin.z;
    m_fEntityWaterImmersion = m_fEntityWaterImmersion / (9.0F * fHeight);
    auto fHalfHeightZ = m_vecBoundingMin.z + fHeight * 0.5F;

    for (int32_t i = 0; i < 2; ++i) {
        auto fMinYHeight = vecAllPoints[i][0].z;            // 000      000     ^y
        auto fMinYHeight2 = vecAllPoints[i + 1][0].z;       // 000  ->  000     │   x
        if (fMinYHeight > fHalfHeightZ                      // xx0      0xx     └───>
            && fMinYHeight < m_vecBoundingMax.z
            && fMinYHeight2 > fHalfHeightZ
            && fMinYHeight2 < m_vecBoundingMax.z) {

            CVector vecDir(0.0F, -1.0F, 0.0F);
            AddSplashParticles(pEntity, vecAllPoints[i][0], vecAllPoints[i + 1][0], vecDir, false);
        }

        auto fMaxYHeight = vecAllPoints[i][2].z;            // xx0      0xx     ^y
        auto fMaxYHeight2 = vecAllPoints[i + 1][2].z;       // 000  ->  000     │   x
        if (fMaxYHeight > fHalfHeightZ                      // 000      000     └───>
            && fMaxYHeight < m_vecBoundingMax.z
            && fMaxYHeight2 > fHalfHeightZ
            && fMaxYHeight2 < m_vecBoundingMax.z) {

            CVector vecDir(0.0F, 1.0F, 0.0F);
            AddSplashParticles(pEntity, vecAllPoints[i][2], vecAllPoints[i + 1][2], vecDir, false);
        }
    }

    for (int32_t i = 0; i < 2; ++i) {
        auto fMinXHeight = vecAllPoints[0][i].z;            // 000      x00     ^y
        auto fMinXHeight2 = vecAllPoints[0][i + 1].z;       // x00  ->  x00     │   x
        if (fMinXHeight > fHalfHeightZ                      // x00      000     └───>
            && fMinXHeight < m_vecBoundingMax.z
            && fMinXHeight2 > fHalfHeightZ
            && fMinXHeight2 < m_vecBoundingMax.z) {

            CVector vecDir(-1.0F, 0.0F, 0.0F);
            AddSplashParticles(pEntity, vecAllPoints[0][i], vecAllPoints[0][i + 1], vecDir, false);
        }

        auto fMaxXHeight = vecAllPoints[2][i].z;            // 000      00x     ^y
        auto fMaxXHeight2 = vecAllPoints[2][i + 1].z;       // 00x  ->  00x     │   x
        if (fMaxXHeight > fHalfHeightZ                      // 00x      000     └───>
            && fMaxXHeight < m_vecBoundingMax.z
            && fMaxXHeight2 > fHalfHeightZ
            && fMaxXHeight2 < m_vecBoundingMax.z) {

            CVector vecDir(1.0F, 0.0F, 0.0F);
            AddSplashParticles(pEntity, vecAllPoints[i][2], vecAllPoints[i + 1][2], vecDir, false);
        }
    }

#endif
}

double cBuoyancy::SimpleSumBuoyancyData(CVector* vecWaterOffset, eBuoyancyPointState ePointState)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<double, 0x6C2970, cBuoyancy*, CVector*, eBuoyancyPointState>(this, vecWaterOffset, ePointState);
#else
    if (!cBuoyancy::calcStruct.bBuoyancyDataSummed)
        cBuoyancy::calcStruct.bBuoyancyDataSummed = true;

    auto fPointDistanceToWaterSurface = fabs(vecWaterOffset->z - m_vecBoundingMin.z) - (1.0F - cBuoyancy::fPointVolMultiplier);
    cBuoyancy::calcStruct.fAddedDistToWaterSurface = fPointDistanceToWaterSurface;
    if (fPointDistanceToWaterSurface < 0.0F)
        return 0.0F;

    if (m_bProcessingBoat)
        cBuoyancy::calcStruct.fAddedDistToWaterSurface = pow(cBuoyancy::calcStruct.fAddedDistToWaterSurface * cBuoyancy::fPointVolMultiplier, 2);

    m_fEntityWaterImmersion += cBuoyancy::calcStruct.fAddedDistToWaterSurface;

    cBuoyancy::calcStruct.vecAverageMoveForce.x = vecWaterOffset->x * m_vecNormalizedCenterOffset.x;
    cBuoyancy::calcStruct.vecAverageMoveForce.y = vecWaterOffset->y * m_vecNormalizedCenterOffset.y;
    cBuoyancy::calcStruct.vecAverageMoveForce.z = (vecWaterOffset->z + m_vecBoundingMin.z) * m_vecNormalizedCenterOffset.z * 0.5F;

    if (m_bFlipUnknVector)
        cBuoyancy::calcStruct.vecAverageMoveForce *= -1.0F;

    // Rolling average calculation, averages the move force from all checked points
    cBuoyancy::calcStruct.fNewPointContribution = 1.0F / m_fNumCheckedPoints;
    cBuoyancy::calcStruct.fCurrentAverageContribution = 1.0F - cBuoyancy::calcStruct.fNewPointContribution;
    auto vecNewPointContribution = cBuoyancy::calcStruct.vecAverageMoveForce * cBuoyancy::calcStruct.fNewPointContribution * cBuoyancy::calcStruct.fAddedDistToWaterSurface;
    auto vecCurrentAverageContibution = m_vecMoveForce * cBuoyancy::calcStruct.fCurrentAverageContribution;
    m_vecMoveForce = vecNewPointContribution + vecCurrentAverageContibution;

    m_fNumCheckedPoints += 1.0F;
    m_bInWater = true;

    return cBuoyancy::calcStruct.fAddedDistToWaterSurface;
#endif
}

void cBuoyancy::FindWaterLevel(CVector const& vecInitialZPos, CVector* outVecOffset, eBuoyancyPointState* outInWaterState)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x6C2810, cBuoyancy*, CVector const&, CVector*, eBuoyancyPointState*>(this, vecInitialZPos, outVecOffset, outInWaterState);
#else
    CVector transformedPos;
    Multiply3x3(&transformedPos, &m_EntityMatrix, outVecOffset);
    auto vecWorldPos = transformedPos + m_vecPos;
    CWaterLevel::GetWaterLevel(vecWorldPos.x, vecWorldPos.y, m_vecPos.z, &outVecOffset->z, true, nullptr);
    outVecOffset->z -= (transformedPos.z + vecInitialZPos.z);

    if (outVecOffset->z > m_vecBoundingMax.z) {
        outVecOffset->z = m_vecBoundingMax.z;
        *outInWaterState = eBuoyancyPointState::COMPLETELY_UNDER_WATER;
    }
    else if (outVecOffset->z < m_vecBoundingMin.z) {
        outVecOffset->z = m_vecBoundingMin.z;
        *outInWaterState = eBuoyancyPointState::COMPLETELY_ABOVE_WATER;
    }
    else {
        *outInWaterState = eBuoyancyPointState::COLLIDING_WITH_WATER;
    }
#endif
}

void cBuoyancy::FindWaterLevelNorm(CVector const& vecInitialZPos, CVector* outVecOffset, eBuoyancyPointState* outInWaterState, CVector* outVecNormal)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x6C28C0, cBuoyancy*, CVector const&, CVector*, eBuoyancyPointState*, CVector*>(this, vecInitialZPos, outVecOffset, outInWaterState, outVecNormal);
#else
    CVector transformedPos;
    Multiply3x3(&transformedPos, &m_EntityMatrix, outVecOffset);
    auto vecWorldPos = transformedPos + m_vecPos;
    CWaterLevel::GetWaterLevel(vecWorldPos.x, vecWorldPos.y, m_vecPos.z, &outVecOffset->z, true, outVecNormal);
    outVecOffset->z -= (transformedPos.z + vecInitialZPos.z);

    if (outVecOffset->z > m_vecBoundingMax.z) {
        outVecOffset->z = m_vecBoundingMax.z;
        *outInWaterState = eBuoyancyPointState::COMPLETELY_UNDER_WATER;
    }
    else if (outVecOffset->z < m_vecBoundingMin.z) {
        outVecOffset->z = m_vecBoundingMin.z;
        *outInWaterState = eBuoyancyPointState::COMPLETELY_ABOVE_WATER;
    }
    else {
        *outInWaterState = eBuoyancyPointState::COLLIDING_WITH_WATER;
    }
#endif
}
