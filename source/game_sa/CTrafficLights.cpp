#include "StdInc.h"

RwUInt8(&CTrafficLights::aTrafficLightColoursR)[3] = *(RwUInt8(*)[3])0x8A6214;
RwUInt8(&CTrafficLights::aTrafficLightColoursG)[3] = *(RwUInt8(*)[3])0x8A6218;
RwUInt8(&CTrafficLights::aTrafficLightColoursB)[3] = *(RwUInt8(*)[3])0xA9AD94;

void CTrafficLights::InjectHooks()
{
    ReversibleHooks::Install("CTrafficLights", "ShouldCarStopForLight", 0x49D610, &CTrafficLights::ShouldCarStopForLight);
    ReversibleHooks::Install("CTrafficLights", "ShouldCarStopForBridge", 0x49D420, &CTrafficLights::ShouldCarStopForBridge);
    ReversibleHooks::Install("CTrafficLights", "IsMITrafficLight", 0x49D5A0, &CTrafficLights::IsMITrafficLight);
    ReversibleHooks::Install("CTrafficLights", "FindTrafficLightType", 0x49D580, &CTrafficLights::FindTrafficLightType);
    ReversibleHooks::Install("CTrafficLights", "FindTrafficLightTypeFromOrientation", 0x49D4D0, &CTrafficLights::FindTrafficLightTypeFromOrientation);
    ReversibleHooks::Install("CTrafficLights", "LightForCars1", 0x49D2D0, &CTrafficLights::LightForCars1);
    ReversibleHooks::Install("CTrafficLights", "LightForCars2", 0x49D310, &CTrafficLights::LightForCars2);
    ReversibleHooks::Install("CTrafficLights", "LightForCars1_Visual", 0x49D350, &CTrafficLights::LightForCars1_Visual);
    ReversibleHooks::Install("CTrafficLights", "LightForCars2_Visual", 0x49D3A0, &CTrafficLights::LightForCars2_Visual);
}

bool CTrafficLights::ShouldCarStopForLight(CVehicle* pVehicle, bool bUnkn)
{
    auto iAppeareance = pVehicle->GetVehicleAppearance();
    if (iAppeareance == eVehicleApperance::VEHICLE_APPEARANCE_HELI || iAppeareance == eVehicleApperance::VEHICLE_APPEARANCE_PLANE)
        return false;

    auto& pCurrentNodeInfo = pVehicle->m_autoPilot.m_nCurrentPathNodeInfo;
    if (pCurrentNodeInfo.IsValid() && ThePaths.m_pPathNodes[pCurrentNodeInfo.m_wAreaId]) {
        const auto& pNaviNode = ThePaths.m_pNaviNodes[pCurrentNodeInfo.m_wAreaId][pCurrentNodeInfo.m_wCarPathLinkId];
        if (pNaviNode.m_nTrafficLightState) {
            if ((!pNaviNode.m_bTrafficLightDirection || pNaviNode.m_address == pVehicle->m_autoPilot.m_currentAddress)
              && (pNaviNode.m_bTrafficLightDirection || pNaviNode.m_address != pVehicle->m_autoPilot.m_currentAddress)) {

                if (!bUnkn
                    && (pNaviNode.m_nTrafficLightState != DIR_NORTH_SOUTH || !CTrafficLights::LightForCars1())
                    && (pNaviNode.m_nTrafficLightState != DIR_WEST_EAST   || !CTrafficLights::LightForCars2())) {

                    return false;
                }

                const auto& vehPos = pVehicle->GetPosition();
                auto fDot = (vehPos.y - static_cast<float>(pNaviNode.m_posn.y) / 8.0F) * (static_cast<float>(pNaviNode.m_nDirY) / 100.0F)
                          + (vehPos.x - static_cast<float>(pNaviNode.m_posn.x) / 8.0F) * (static_cast<float>(pNaviNode.m_nDirX) / 100.0F);

                if (pVehicle->m_autoPilot._smthCurr == -1) {
                    if (fDot > 0.0F && fDot < 12.0F)
                        return true;
                }
                else if (fDot < 0.0F && fDot > -12.0F)
                    return true;
            }
        }
    }

    auto& pNextNodeInfo = pVehicle->m_autoPilot.m_nNextPathNodeInfo;
    if (pNextNodeInfo.IsValid() && ThePaths.m_pPathNodes[pNextNodeInfo.m_wAreaId]) {
        const auto& pNaviNode = ThePaths.m_pNaviNodes[pNextNodeInfo.m_wAreaId][pNextNodeInfo.m_wCarPathLinkId];
        if (pNaviNode.m_nTrafficLightState) {
            if ((!pNaviNode.m_bTrafficLightDirection || pNaviNode.m_address == pVehicle->m_autoPilot.m_startingRouteNode)
              && (pNaviNode.m_bTrafficLightDirection || pNaviNode.m_address != pVehicle->m_autoPilot.m_startingRouteNode)) {

                if (!bUnkn
                    && (pNaviNode.m_nTrafficLightState != DIR_NORTH_SOUTH || !CTrafficLights::LightForCars1())
                    && (pNaviNode.m_nTrafficLightState != DIR_WEST_EAST   || !CTrafficLights::LightForCars2())) {

                    return false;
                }

                const auto& vehPos = pVehicle->GetPosition();
                auto fDot = (vehPos.y - static_cast<float>(pNaviNode.m_posn.y) / 8.0F) * (static_cast<float>(pNaviNode.m_nDirY) / 100.0F)
                          + (vehPos.x - static_cast<float>(pNaviNode.m_posn.x) / 8.0F) * (static_cast<float>(pNaviNode.m_nDirX) / 100.0F);

                if (pVehicle->m_autoPilot._smthNext == -1) {
                    if (fDot > 0.0F && fDot < 12.0F)
                        return true;
                }
                else if (fDot < 0.0F && fDot > -12.0F)
                    return true;
            }
        }
    }

    auto& pPrevNodeInfo = pVehicle->m_autoPilot.m_nPreviousPathNodeInfo;
    if (pPrevNodeInfo.IsValid() && ThePaths.m_pPathNodes[pPrevNodeInfo.m_wAreaId]) {
        const auto& pNaviNode = ThePaths.m_pNaviNodes[pPrevNodeInfo.m_wAreaId][pPrevNodeInfo.m_wCarPathLinkId];
        if (pVehicle->m_nStatus == eEntityStatus::STATUS_PHYSICS && pNaviNode.m_nTrafficLightState) {
            if ((!pNaviNode.m_bTrafficLightDirection || pNaviNode.m_address == pVehicle->m_autoPilot.m_endingRouteNode)
              && (pNaviNode.m_bTrafficLightDirection || pNaviNode.m_address != pVehicle->m_autoPilot.m_endingRouteNode)) {

                if (!bUnkn
                    && (pNaviNode.m_nTrafficLightState != DIR_NORTH_SOUTH || !CTrafficLights::LightForCars1())
                    && (pNaviNode.m_nTrafficLightState != DIR_WEST_EAST   || !CTrafficLights::LightForCars2())) {

                    return false;
                }

                const auto& vehPos = pVehicle->GetPosition();
                auto fDot = (vehPos.y - static_cast<float>(pNaviNode.m_posn.y) / 8.0F) * (static_cast<float>(pNaviNode.m_nDirY) / 100.0F)
                          + (vehPos.x - static_cast<float>(pNaviNode.m_posn.x) / 8.0F) * (static_cast<float>(pNaviNode.m_nDirX) / 100.0F);

                if (pVehicle->m_autoPilot._smthPrev == -1) {
                    if (fDot > 0.0F && fDot < 6.0F)
                        return true;
                }
                else if (fDot < 0.0F && fDot > -6.0F)
                    return true;
            }
        }
    }

    return false;
}

bool CTrafficLights::ShouldCarStopForBridge(CVehicle* pVehicle)
{
    auto iAppeareance = pVehicle->GetVehicleAppearance();
    if (iAppeareance == eVehicleApperance::VEHICLE_APPEARANCE_HELI || iAppeareance == eVehicleApperance::VEHICLE_APPEARANCE_PLANE)
        return false;

    auto& pNextNodeInfo = pVehicle->m_autoPilot.m_nNextPathNodeInfo;
    if (!pNextNodeInfo.IsValid() || !ThePaths.m_pPathNodes[pNextNodeInfo.m_wAreaId])
        return false;

    auto& pCurNodeInfo = pVehicle->m_autoPilot.m_nCurrentPathNodeInfo;
    if (!pCurNodeInfo.IsValid())
        return false;

    if (ThePaths.m_pPathNodes[pCurNodeInfo.m_wAreaId]
        && ThePaths.m_pNaviNodes[pNextNodeInfo.m_wAreaId][pNextNodeInfo.m_wCarPathLinkId].m_bTrainCrossing
        && ThePaths.m_pNaviNodes[pCurNodeInfo.m_wAreaId][pCurNodeInfo.m_wCarPathLinkId].m_bTrainCrossing)
        return true;

    return false;
}

int CTrafficLights::FindTrafficLightType(CEntity* pEntity)
{
    auto pHeading = pEntity->GetHeading();
    return CTrafficLights::FindTrafficLightTypeFromOrientation(pHeading);
}

void CTrafficLights::DisplayActualLight(CEntity* pEntity)
{
    plugin::Call<0x49DAB0, CEntity*>(pEntity);
}

bool CTrafficLights::IsMITrafficLight(int modelIndex)
{
    return modelIndex == ModelIndices::MI_TRAFFICLIGHTS
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_VERTICAL
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_MIAMI
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_VEGAS
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_TWOVERTICAL
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_3
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_4
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_5
        || modelIndex == ModelIndices::MI_TRAFFICLIGHTS_GAY;
}

unsigned char CTrafficLights::LightForCars1()
{
    if (CGameLogic::LaRiotsActiveHere() || CCheat::m_aCheatsActive[eCheats::CHEAT_BGREEN_LIGHTS_CHEAT])
        return eTrafficLightsState::LIGHT_GREEN;

    auto uiMaskedTime = (CTimer::m_snTimeInMilliseconds / 2) & 0x3FFF;
    if (uiMaskedTime < 5000)
        return eTrafficLightsState::LIGHT_GREEN;

    if (uiMaskedTime < 6000)
        return eTrafficLightsState::LIGHT_ORANGE;

    return eTrafficLightsState::LIGHT_RED;
}

unsigned char CTrafficLights::LightForCars2()
{
    if (CGameLogic::LaRiotsActiveHere() || CCheat::m_aCheatsActive[eCheats::CHEAT_BGREEN_LIGHTS_CHEAT])
        return eTrafficLightsState::LIGHT_GREEN;

    auto uiMaskedTime = (CTimer::m_snTimeInMilliseconds / 2) & 0x3FFF;
    if (uiMaskedTime < 6000)
        return eTrafficLightsState::LIGHT_RED;

    if (uiMaskedTime < 11000)
        return eTrafficLightsState::LIGHT_GREEN;

    if (uiMaskedTime < 12000)
        return eTrafficLightsState::LIGHT_ORANGE;

    return eTrafficLightsState::LIGHT_RED;
}

unsigned char CTrafficLights::LightForCars1_Visual()
{
    if (CGameLogic::LaRiotsActiveHere()) {
        if ((CTimer::m_snTimeInMilliseconds / 512) & 2)
            return eTrafficLightsState::LIGHT_OFF;
        else
            return eTrafficLightsState::LIGHT_ORANGE;
    }

    return CTrafficLights::LightForCars1();
}

unsigned char CTrafficLights::LightForCars2_Visual()
{
    if (CGameLogic::LaRiotsActiveHere()) {
        if ((CTimer::m_snTimeInMilliseconds / 512) & 2)
            return eTrafficLightsState::LIGHT_OFF;
        else
            return eTrafficLightsState::LIGHT_ORANGE;
    }

    return CTrafficLights::LightForCars2();
}

int CTrafficLights::FindTrafficLightTypeFromOrientation(float fOrientation)
{
    if ((fOrientation <= 60.0F || fOrientation >= 150.0F)
        && (fOrientation <= 240.0F || fOrientation >= 330.0F))
        return 2;

    return 1;
}
