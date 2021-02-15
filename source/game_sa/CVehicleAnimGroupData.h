#pragma once

// based on https://projectcerbera.com/gta/sa/tutorials/handling#animation

// If the flags are true, second anim group is used, otherwise first
union sVehicleAnimDataGroupFlags
{
    unsigned int intValue;
    struct
    {
        unsigned int bAlign : 1; // Unknown stuff
        unsigned int bOpenFrontDoorsOnExit : 1;
        unsigned int bOpenRearDoorsOnExit : 1;
        unsigned int bCanEnterFrontDoors : 1;
        unsigned int bCanEnterRearDoors : 1;
        unsigned int bCanPulloutPed : 1;
        unsigned int bCloseFrontDoorWhenInside : 1;
        unsigned int bCloseRearDoorWhenInside : 1;

        unsigned int bShuffle : 1; // Unkn purpose
        unsigned int bCanExitFrontDoors : 1;
        unsigned int bCanExitRearDoors : 1;
        unsigned int bPlayerCanBeJacked : 1;
        unsigned int bCloseFrontDoorWhenOutside : 1;
        unsigned int bCloseRearDoorWhenOutside : 1;
        unsigned int bCanJumpOutOfVehicle : 1;
        unsigned int bRollDownWindowOnDoorClose : 1;

        unsigned int bPlayerCanFallOut : 1;
        unsigned int bDoorLocked : 1;
    };

public:
    sVehicleAnimDataGroupFlags() { intValue = 0; }
};
VALIDATE_SIZE(sVehicleAnimDataGroupFlags, 0x4);

union sVehicleAnimDataSpecialFlags
{
    unsigned int intValue;
    struct
    {
        unsigned int bDontCloseDoorAfterGettingOut : 1;
        unsigned int bDontCloseDoorAfterGettingIn : 1;
        unsigned int bUseKartDriveAnims : 1;
        unsigned int bUseTruckDriveAnims : 1;
        unsigned int bUseHovercraftDriveAnims : 1;
        unsigned int bRunSpecialLockedDoor : 1; // Unknown
        unsigned int bDontOpenDoorWhenGettingIn : 1;
    };

public:
    sVehicleAnimDataSpecialFlags() { intValue = 0; }
};
VALIDATE_SIZE(sVehicleAnimDataSpecialFlags, 0x4);

struct sVehAnimGroupGeneralTiming
{
    float m_fGetInTime;
    float m_fJumpOutTime;
    float m_fGetOutTime;
    float m_fCarJackTime;
    float m_fFallOutTime;

public:
    sVehAnimGroupGeneralTiming() : m_fGetInTime(0.0F), m_fJumpOutTime(0.0F), m_fGetOutTime(0.0F), m_fCarJackTime(0.0F), m_fFallOutTime(0.0F) {}
};

struct sVehAnimGroupInOutTiming
{
    float m_fOpenOutTime;
    float m_fCloseInTime;
    float m_fOpenInTime;
    float m_fCloseOutTime;

public:
    sVehAnimGroupInOutTiming() : m_fOpenOutTime(0.0F), m_fCloseInTime(0.0F), m_fOpenInTime(0.0F), m_fCloseOutTime(0.0F) {}
};

enum eInOutTiming : unsigned char {
    TIMING_START = 0,
    TIMING_END = 1,
};

enum eVehAnimDoorOffset : unsigned char {
    ENTER_FRONT = 0,
    ENTER_REAR = 1,
    ENTER_BIKE_FRONT = 2,
    EXIT_FRONT = 3,
    EXIT_REAR = 4,
    JACK_PED_LEFT = 5,
    JACK_PED_RIGHT = 6,
};

// Most of the code was inlined in final exe, in android IDB 99% of the code is inlined,
// on PC version some methods are left, but without symbols the names aren't matching original source most likely
class CVehicleAnimGroup
{
public:
    CVehicleAnimGroup();

public:
    unsigned char m_ucFirstGroup; // see eAnimGroup
    unsigned char m_ucSecondGroup; // see eAnimGroup
private:
    unsigned char _align[2];

public:
    sVehicleAnimDataGroupFlags m_animFlags;
    sVehicleAnimDataSpecialFlags m_specialFlags;
    sVehAnimGroupGeneralTiming m_generalTiming;
    sVehAnimGroupInOutTiming m_aInOutTiming[2]; // Access using GetInOutTiming()
    CVector m_aVecDoorOffsets[7]; // Access using GetDoorOffset()

public:
    static void InjectHooks();

    void InitAnimGroup(unsigned char firstGroup, unsigned char secondGroup, int animFlags, int animSpecialFlags, sVehAnimGroupGeneralTiming* generalTiming, sVehAnimGroupInOutTiming* startTiming, sVehAnimGroupInOutTiming* endTiming);
    void CopyAnimGroup(CVehicleAnimGroup* src);
    int GetGroup(int animId);
    float ComputeCriticalBlendTime(int animId);
    CVector ComputeAnimDoorOffsets(eVehAnimDoorOffset doorId);

public:
    sVehAnimGroupInOutTiming& GetInOutTiming(const eInOutTiming timing) { return m_aInOutTiming[timing]; }
    CVector& GetDoorOffset(const eVehAnimDoorOffset door) { return m_aVecDoorOffsets[door]; }
};
VALIDATE_SIZE(CVehicleAnimGroup, 0x94);


class CVehicleAnimGroupData
{
public:
    static constexpr int NUM_VEH_ANIM_GROUPS = 30;
    static CVehicleAnimGroup(&m_vehicleAnimGroups)[NUM_VEH_ANIM_GROUPS];
};
