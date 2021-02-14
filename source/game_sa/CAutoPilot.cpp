#include "StdInc.h"

CAutoPilot::CAutoPilot() : m_nCurrentPathNodeInfo(), m_nNextPathNodeInfo(), m_nPreviousPathNodeInfo()
{
    m_currentAddress.m_wAreaId = -1;
    m_startingRouteNode.m_wAreaId = -1;
    field_8.m_wAreaId = -1;

    for (size_t i = 0; i < 8; ++i)
        m_aPathFindNodesInfo[i].m_wAreaId = -1;

    _smthNext = 1;
    _smthCurr = 1;

    m_nCarCtrlFlags &= 0x4;

    field_C = 0;
    m_nSpeedScaleFactor = 1000;
    m_nNextLane = 0;
    m_nCurrentLane = 0;
    m_nCarDrivingStyle = eCarDrivingStyle::DRIVINGSTYLE_STOP_FOR_CARS;
    m_nCarMission = eCarMission::MISSION_NONE;
    m_nTempAction = 0;
    m_nCruiseSpeed = 10;
    field_38 = 10.0;
    m_nPathFindNodesCount = 0;
    m_pTargetCar = 0;

    m_nTimeToStartMission = CTimer::m_snTimeInMilliseconds;
    m_nTimeSwitchedToRealPhysics = CTimer::m_snTimeInMilliseconds;
    _someStartTime = 0;

    m_nStraightLineDistance = 20;
    field_34 = 0;
    field_35 = 0;
    field_41 = 0;
    field_44 = 1.0;
    field_49 = 0;
    field_4C &= 0xFC;
    field_4A = 0;
    field_4E = 10;
    field_4F = 10;
    field_50 = (rand() & 7) + 2;
    field_94 = -1;
    field_95 = 0;
    m_pCarWeMakingSlowDownFor = 0;
    m_fMaxTrafficSpeed = 0.0;
}
