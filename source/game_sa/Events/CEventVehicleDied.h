#include "CEvent.h"

class CVehicle;

class CEventVehicleDied : public CEvent
{
public:
    CVehicle* m_vehicle;
    bool m_IsOccupantLeavingCar;
private:
    char padding[3];
public:
    static void InjectHooks();

    CEventVehicleDied (CVehicle* vehicle);
    ~CEventVehicleDied ();
private:
    CEventVehicleDied * Constructor(CVehicle* vehicle);
public:
    eEventType GetEventType() override { return EVENT_VEHICLE_DIED; }
    int GetEventPriority() override { return 72; }
    int GetLifeTime() override { return 0; }
    CEvent* Clone() override;
    bool AffectsPed(CPed* ped) override;

    CEvent* Clone_Reversed();
    bool AffectsPed_Reversed(CPed* ped);
};

VALIDATE_SIZE(CEventVehicleDied, 0x14);
