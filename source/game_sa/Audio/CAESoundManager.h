#pragma once
#include "PluginBase.h"
#include "CAESound.h"
#include "CPhysical.h"

class CAESoundManager
{
public:
    CAESoundManager();
    ~CAESoundManager();

public:
    int16_t  m_nNumAvailableChannels;
    int16_t  m_nChannel;
    CAESound m_aSounds[300];
    int16_t* m_aChannelSoundTable;
    int16_t* m_aChannelSoundPosition;
    int16_t* m_aChannelSoundUncancellable;
    int16_t  m_nSoundLength[300];
    int16_t  m_nSoundLoopStartTime[300];
    uint32_t m_nUpdateTime;
    bool     m_bPauseTimeInUse;
    bool     m_bManuallyPaused;
    uint8_t  field_8CB6[2];
    uint32_t m_nPauseUpdateTime;

public:
    static void InjectHooks();

    void Initialise();
    void Terminate();
    void Reset();
    void PauseManually(uchar bPause);
    void Service();
    void RequestNewSound(CAESound*);
    void AreSoundsPlayingInBankSlot(short bankSlot);
    void AreSoundsOfThisEventPlayingForThisEntity(short eventId, CAEAudioEntity* audioEntity);
    void AreSoundsOfThisEventPlayingForThisEntityAndPhysical(short eventId, CAEAudioEntity* audioEntity, CPhysical* physical);
    void CancelSoundsOfThisEventPlayingForThisEntity(short eventId, CAEAudioEntity* audioEntity);
    void CancelSoundsOfThisEventPlayingForThisEntityAndPhysical(short eventId, CAEAudioEntity* audioEntity, CPhysical* physical);
    void CancelSoundsInBankSlot(short bankSlot, uchar bFullStop);
    void CancelSoundsOwnedByAudioEntity(CAEAudioEntity* audioEntity, uchar bFullStop);
    void GetVirtualChannelForPhysicalChannel(short physicalChannel);    
};

extern CAESoundManager& AESoundManager;
