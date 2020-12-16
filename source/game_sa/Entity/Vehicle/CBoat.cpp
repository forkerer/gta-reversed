#include "StdInc.h"

CBoat** CBoat::apFrameWakeGeneratingBoats; // static CBoat *apFrameWakeGeneratingBoats[4]
float& CBoat::MAX_WAKE_LENGTH = *(float*)0x8D3938; // 50.0
float& CBoat::MIN_WAKE_INTERVAL = *(float*)0x8D393C; // 2.0
float& CBoat::WAKE_LIFETIME = *(float*)0x8D3940; // 150.0
float& CBoat::fShapeLength = *(float*)0x8D3944; // 0.4
float& CBoat::fShapeTime = *(float*)0x8D3948; // 0.05
float& CBoat::fRangeMult = *(float*)0x8D394C; // 0.6

void CBoat::InjectHooks()
{
    ReversibleHooks::Install("CBoat", "PruneWakeTrail", 0x6F0E20, &CBoat::PruneWakeTrail);
    ReversibleHooks::Install("CBoat", "ProcessControl", 0x6F1770, &CBoat::ProcessControl);
}

void CBoat::PruneWakeTrail()
{
    int16_t iInd;
    for (iInd = 0; iInd < 32; ++iInd) {
        auto fPointLifeTime = m_afWakePointLifeTime[iInd];
        if (fPointLifeTime <= 0.0F)
            break;

        if (fPointLifeTime <= CTimer::ms_fTimeStep) {
            m_afWakePointLifeTime[iInd] = 0.0F;
            break;
        }

        m_afWakePointLifeTime[iInd] -= CTimer::ms_fTimeStep;
    }

    if (iInd >= 32)
        return;

    m_nNumWaterTrailPoints = iInd;
}

void CBoat::ProcessControl() {
    m_vehicleAudio.Service();
    CBoat::PruneWakeTrail();
    CVehicle::ProcessDelayedExplosion();
    auto fMassCheck = (m_fMass * 0.008F * 100.0F) / 125.0F;
    if (physicalFlags.bDestroyed && fMassCheck < m_fBuoyancyConstant)
        m_fBuoyancyConstant -= ((m_fMass * 0.001F) * 0.008F);

    auto pWantedInfo = FindPlayerPed()->m_pPlayerData->m_pWanted;
    if (pWantedInfo->m_nWantedLevel > 0 && m_nModelIndex == eModelID::MODEL_PREDATOR) {
        auto pPlayerVeh = FindPlayerVehicle(-1, false);
        if (pPlayerVeh && pPlayerVeh->GetVehicleAppearance() == eVehicleApperance::VEHICLE_APPEARANCE_BOAT) {
            auto iCarMission = m_autoPilot.m_nCarMission;
            if (iCarMission == eCarMission::MISSION_ATTACKPLAYER
                || (iCarMission >= eCarMission::MISSION_RAMPLAYER_FARAWAY && iCarMission <= eCarMission::MISSION_BLOCKPLAYER_CLOSE)) {

                if (CTimer::m_snTimeInMilliseconds > m_nAttackPlayerTime)
                    m_nAttackPlayerTime = (rand() && 0xFFF) + CTimer::m_snTimeInMilliseconds + 4500;
            }
        }
    }

    CVehicle::UpdateClumpAlpha();
    CVehicle::ProcessCarAlarm();
    switch (m_nStatus) {
    case eEntityStatus::STATUS_PLAYER:
        m_nBoatFlags.bAnchored = false;
        m_fAnchoredAngle = -9999.99F;
        if (m_pDriver)
            CVehicle::ProcessControlInputs(m_pDriver->m_nPedType);

        if (m_nModelIndex == eModelID::MODEL_PREDATOR)
            CVehicle::DoFixedMachineGuns();

        CVehicle::DoDriveByShooting();
        break;
    case eEntityStatus::STATUS_SIMPLE:
        m_nBoatFlags.bAnchored = false;
        m_fAnchoredAngle = -9999.99F;
        CCarAI::UpdateCarAI(this);
        CPhysical::ProcessControl();
        physicalFlags.bSubmergedInWater = true;
        m_nBoatFlags.bOnWater = true;
        m_nBoatFlags.bMovingOnWater = true;
        return;
    case eEntityStatus::STATUS_PHYSICS:
        m_nBoatFlags.bAnchored = false;
        m_fAnchoredAngle = -9999.99F;
        CCarAI::UpdateCarAI(this);
        CCarCtrl::SteerAICarWithPhysics(this);
        break;
    case eEntityStatus::STATUS_ABANDONED:
    case eEntityStatus::STATUS_WRECKED:
        vehicleFlags.bIsHandbrakeOn = false; //?
        physicalFlags.bSubmergedInWater = true;
        m_nBoatFlags.bOnWater = true;
        m_nBoatFlags.bMovingOnWater = true;

        m_fSteerAngle = 0.0F;
        m_fBreakPedal = 0.5F;
        m_fGasPedal = 0.0F;

        auto fDist = (GetPosition() - FindPlayerCentreOfWorld_NoSniperShift(0)).Magnitude();
        if (fDist > 150.0F) {
            m_vecMoveSpeed.Set(0.0F, 0.0F, 0.0F);
            m_vecTurnSpeed.Set(0.0F, 0.0F, 0.0F);
            return;
        }
        break;
    }

    if (m_nStatus == eEntityStatus::STATUS_PLAYER || m_nStatus == eEntityStatus::STATUS_HELI || m_nStatus == eEntityStatus::STATUS_PHYSICS) {
        auto fSTDPropSpeed = 0.0F;
        auto fROCPropSpeed = CPlane::PLANE_ROC_PROP_SPEED;
        if (m_nModelIndex == eModelID::MODEL_SKIMMER)
            fSTDPropSpeed = CPlane::PLANE_STD_PROP_SPEED;
        else if (m_fGasPedal != 0.0F)
            fROCPropSpeed *= 5.0F;

        if (m_fGasPedal == 0.0F)
            m_fPropSpeed += (fSTDPropSpeed - m_fPropSpeed) * CTimer::ms_fTimeStep * fROCPropSpeed;
        else if (m_fGasPedal < 0.0F) {
            fSTDPropSpeed = (CPlane::PLANE_STD_PROP_SPEED - 0.05F) * m_fGasPedal + CPlane::PLANE_STD_PROP_SPEED;
            m_fPropSpeed += (fSTDPropSpeed - m_fPropSpeed) * CTimer::ms_fTimeStep * fROCPropSpeed;
        }
        else {
            fSTDPropSpeed = (CPlane::PLANE_MAX_PROP_SPEED - CPlane::PLANE_STD_PROP_SPEED) * m_fGasPedal + CPlane::PLANE_STD_PROP_SPEED;
            m_fPropSpeed += (fSTDPropSpeed - m_fPropSpeed) * CTimer::ms_fTimeStep * fROCPropSpeed;
        }
    }
    else if (m_fPropSpeed > 0.0F)
        m_fPropSpeed *= 0.95F;

    auto fDamagePower = m_fDamageIntensity * m_pHandlingData->m_fCollisionDamageMultiplier;
    if (fDamagePower > 25.0F && m_nStatus != eEntityStatus::STATUS_WRECKED && m_fHealth >= 250.0F) {
        auto fSavedHealth = m_fHealth;
        if (m_nStatus == eEntityStatus::STATUS_PLAYER && CStats::GetPercentageProgress() >= 100.0F)
            fDamagePower *= 0.5F;

        auto fGivenDamage = fDamagePower;
        if (this == reinterpret_cast<CBoat*>(FindPlayerVehicle(-1, 0)))
        {
            fGivenDamage -= 25.0F;
            if (vehicleFlags.bTakeLessDamage)
                fGivenDamage /= 6.0F;
            else
                fGivenDamage *= 0.5F;
        }
        else {
            if (fGivenDamage > 60.0F && m_pDriver)
                m_pDriver->Say(0x44U, 0, 1.0F, 0, 0, 0);

            fGivenDamage -= 25.0F;
            if (vehicleFlags.bTakeLessDamage)
                fGivenDamage /= 12.0F;
            else
                fGivenDamage *= 0.25F;
        }

        m_fHealth -= fGivenDamage;
        if (m_fHealth <= 0.0F && fSavedHealth > 0.0F) {
            m_fHealth = 1.0F;
            m_pWhoDestroyedMe = m_pDamageEntity;
        }
    }

    if (m_fHealth > 460.0F || m_nStatus == eEntityStatus::STATUS_WRECKED) {
        m_fBurningTimer = 0.0F;
        if (m_pFireParticle) {
            m_pFireParticle->Kill();
            m_pFireParticle = nullptr;
        }
    }
    else {
        auto vecDist = GetPosition() - TheCamera.GetPosition();
        if (fabs(vecDist.x) < 200.0F && fabs(vecDist.y) < 200.0F) {

            CVector vecFirePos(-1.5F, -0.5F, 1.2F);
            if (m_nModelIndex == eModelID::MODEL_SPEEDER)
                vecFirePos.Set(0.4F, -2.4F, 0.8F);
            else if (m_nModelIndex == eModelID::MODEL_REEFER)
                vecFirePos.Set(2.0F, -1.0F, 0.5F);

            if (m_fHealth < 250.0F) {
                if (!m_pFireParticle) {
                    auto pModellingMat = GetModellingMatrix();
                    if (pModellingMat) {
                        m_pFireParticle = g_fxMan.CreateFxSystem("fire_car", &vecFirePos, pModellingMat, 0);
                        if (m_pFireParticle) {
                            m_pFireParticle->Play();
                            CEventVehicleOnFire vehOnFireEvent(this);
                            GetEventGlobalGroup()->Add(&vehOnFireEvent, false);
                        }
                    }
                }

                m_fBurningTimer += (CTimer::ms_fTimeStep * 20.0F);
                if (m_fBurningTimer > 5000.0F)
                    BlowUpCar(m_pWhoDestroyedMe, false);
            }
        }
    }

    auto bPostCollision = m_fDamageIntensity > 0.0F && m_vecLastCollisionImpactVelocity.z > 0.1F;
    CPhysical::ProcessControl();
    CVehicle::ProcessBoatControl(m_pBoatHandling, &m_fWaterResistance, m_bHasHitWall, bPostCollision);

    if (m_nModelIndex == eModelID::MODEL_SKIMMER
        && (m_fPropSpeed > CPlane::PLANE_MIN_PROP_SPEED || m_vecMoveSpeed.SquaredMagnitude() > CPlane::PLANE_MIN_PROP_SPEED)) {
        CVehicle::FlyingControl(3, -9999.99F, -9999.99F, -9999.99F, -9999.99F);
    }
    else if (CCheat::m_aCheatsActive[eCheats::CHEAT_BOATS_FLY])
        CVehicle::FlyingControl(5, -9999.99F, -9999.99F, -9999.99F, -9999.99F);

    if (m_nBoatFlags.bAnchored) {
        m_vecMoveSpeed.x = 0.0F;
        m_vecMoveSpeed.y = 0.0F;

        auto bNoAnchorAngle = m_fAnchoredAngle == -9999.99F;
        if (bNoAnchorAngle)
            m_fAnchoredAngle = GetHeading();
        else {
            CVector vecPos = GetMatrix()->GetPosition();
            GetMatrix()->RotateZ(m_fAnchoredAngle - GetHeading());
            GetMatrix()->SetTranslateOnly(vecPos);
        }
    }
}
