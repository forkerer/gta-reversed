/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"

class  CGarages
{
public:
	// static char MessageIDString[8];
	static char *MessageIDString;

    static void Init();
    static void Shutdown();
    static void Init_AfterRestart();
    static void Update();
    static void Save();
    static void Load();
    static void TriggerMessage(char* cTagMsg, short wMsgMin, unsigned short ucTime, short wMsgMax);
    static void PrintMessages();
    static int FindGarageIndex(char*);
    static int FindGarageForObject(CObject* pObject);
    static float FindDoorHeightForMI(int16_t);
    static int FindSafeHouseIndexForGarageType(int32_t);
    static void AddOne(float, float, float, float, char, int, char* src, int);
    static bool IsModelIndexADoor(int nModelIndex);
    static void ChangeGarageType(int16_t, uint8_t, uint32_t);
    static bool IsCarSprayable(CVehicle* pVehicle);
    static void SetTargetCarForMissionGarage(int16_t, CAutomobile* pCar);
    static bool HasCarBeenDroppedOffYet(int16_t);
    static void DeActivateGarage(int16_t);
    static void ActivateGarage(int16_t);
    static bool IsGarageOpen(int16_t);
    static bool IsGarageClosed(int16_t);
    static bool HasResprayHappened(int16_t);
    static bool CameraShouldBeOutside(int16_t);
    static void GivePlayerDetonator();
    static void PlayerArrestedOrDied();
    static bool IsPointInAGarageCameraZone(CVector vecPos);
    static bool IsThisCarWithingGarageArea(int16_t, CEntity* pEntity);
    static void StopCarFromBlowingUp(CAutomobile* pCar);
    static bool IsPointWithinHideOutGarage(CVector& vecPos);
    static bool IsPointWithinAnyGarage(CVector& vecPos);
    static int CountCarsInHideoutGarage(uint8_t);
    static void CloseHideOutGaragesBeforeSave();
    static void StoreCarInNearestImpoundingGarage(CVehicle* pVehicle);
    static void AllRespraysCloseOrOpen(bool);
};
