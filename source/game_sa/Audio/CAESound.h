/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"
#include "CVector.h"
class CAEAudioEntity;

enum  eSoundEnvironment : unsigned short {
    SOUND_FRONT_END                        = 0x1,
    SOUND_UNCANCELLABLE                    = 0x2,
    SOUND_REQUEST_UPDATES                  = 0x4,
    SOUND_PLAY_PHYSICALLY                  = 0x8,
    SOUND_UNPAUSABLE                       = 0x10,
    SOUND_START_PERCENTAGE                 = 0x20,
    SOUND_MUSIC_MASTERED                   = 0x40,
    SOUND_LIFESPAN_TIED_TO_PHYSICAL_ENTITY = 0x80,
    SOUND_UNDUCKABLE                       = 0x100,
    SOUND_UNCOMPRESSABLE                   = 0x200,
    SOUND_ROLLED_OFF                       = 0x400,
    SOUND_SMOOTH_DUCKING                   = 0x800,
    SOUND_FORCED_FRONT                     = 0x1000
};

enum eSoundState : short {
    SOUND_ACTIVE = 0,
    SOUND_STOPPED = 1,
};

class  CAESound {
public:
    CAESound() { m_pPhysicalEntity = nullptr; }
    CAESound(CAESound& sound);
    CAESound(short bankSlotId, short sfxId, CAEAudioEntity* baseAudio, CVector posn, float volume,
        float fDistance, float speed, float timeScale, unsigned char arg9,
        unsigned short environmentFlags, float speedVariability);
    ~CAESound();
    CAESound& operator=(CAESound const& sound);
public:
    short                 m_nBankSlotId;
    short                 m_nSoundIdInSlot;
    CAEAudioEntity       *m_pBaseAudio;
    class CEntity        *m_pPhysicalEntity;
    unsigned int          m_nEvent; // see eAudioEvents
    float                 m_fMaxVolume;
    float                 m_fVolume;
    float                 m_fSoundDistance;
    float                 m_fSpeed;
    float                 m_fSpeedVariability;
    CVector               m_vecCurrPosn;
    CVector               m_vecPrevPosn;
    int                   m_nLastFrameUpdate;
    int                   m_nCurrTimeUpdate;
    int                   m_nPrevTimeUpdate;
    float                 m_fCurrCamDist;
    float                 m_fPrevCamDist;
    float                 m_fTimeScale;
    char field_54;
    char field_55;
    union {
        unsigned short m_nEnvironmentFlags;
        struct {
            unsigned short m_bFrontEnd : 1;
            unsigned short m_bUncancellable : 1;
            unsigned short m_bRequestUpdates : 1;
            unsigned short m_bPlayPhysically : 1;
            unsigned short m_bUnpausable : 1;
            unsigned short m_bStartPercentage : 1;
            unsigned short m_bMusicMastered : 1;
            unsigned short m_bLifespanTiedToPhysicalEntity : 1;

            unsigned short m_bUnduckable : 1;
            unsigned short m_bUncompressable : 1;
            unsigned short m_bRolledOff : 1;
            unsigned short m_bSmoothDucking : 1;
            unsigned short m_bForcedFront : 1;
        };
    };
    unsigned short        m_nIsUsed;
    short field_5A;
    short                 m_nCurrentPlayPosition;
    short                 m_nHasStarted;
    float                 m_fFinalVolume;
    float                 m_fFrequency;
    short                 m_nPlayingState; // see eSoundState
    char field_6A[2];
    float                 m_fSoundHeadRoom;
    short m_nSoundLength;
    short field_72;

public:
    static void InjectHooks();
    
    void UnregisterWithPhysicalEntity();
    void StopSound();
    bool GetUncancellable() { return m_bUncancellable; }
    bool GetFrontEnd() { return m_bFrontEnd; }
    bool GetRequestUpdates() { return m_bRequestUpdates; }
    bool GetUnpausable() { return m_bUnpausable; }
    bool GetPlayPhysically() { return m_bPlayPhysically; };
    bool GetStartPercentage() { return m_bStartPercentage; }
    bool GetMusicMastered() { return m_bMusicMastered; }
    bool GetLifespanTiedToPhysicalEntity() { return m_bLifespanTiedToPhysicalEntity; }
    bool GetUnduckable() { return m_bUnduckable; }
    bool GetUncompressable() { return m_bUncompressable; }
    bool GetRolledOff() { return m_bRolledOff; }
    bool GetSmoothDucking() { return m_bSmoothDucking; }
    bool GetForcedFront() { return m_bForcedFront; }
    void SetIndividualEnvironment(unsigned short envFlag, unsigned short bEnabled); // pass eSoundEnvironment as envFlag
    void UpdatePlayTime(short soundLength, short newPlayPosition, short playProgress);
    void GetRelativePosition(CVector *outPosn);
    void CalculateFrequency();
    void UpdateFrequency();
    float GetRelativePlaybackFrequencyWithDoppler();
    float GetSlowMoFrequencyScalingFactor();
    void NewVPSLentry();
    void RegisterWithPhysicalEntity(CEntity *entity);
    void StopSoundAndForget();
    void SetPosition(CVector vecPos);
    void CalculateVolume();
    void Initialise(short bankSlotId, short sfxId, CAEAudioEntity *baseAudio, CVector posn,
        float volume, float maxDistance, float speed, float timeScale, unsigned char arg9,
        unsigned short environmentFlags, float speedVariability, short currPlayPosn);
    void UpdateParameters(short curPlayPos);
    void SoundHasFinished();

public:
    static constexpr float fSlowMoFrequencyScalingFactor = 0.5F;
};
VALIDATE_SIZE(CAESound, 0x74);

