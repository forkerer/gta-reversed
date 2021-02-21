#include "StdInc.h"
#include "CAESoundManager.h"

#include "CAEAudioHardware.h"

CAESoundManager& AESoundManager = *(CAESoundManager*)0xB62CB0;

void CAESoundManager::InjectHooks()
{
    ReversibleHooks::Install("CAESoundManager", "Initialise", 0x5B9690, &CAESoundManager::Initialise);
    ReversibleHooks::Install("CAESoundManager", "Terminate", 0x4EFAA0, &CAESoundManager::Terminate);
    ReversibleHooks::Install("CAESoundManager", "Reset", 0x4EF4D0, &CAESoundManager::Reset);
    ReversibleHooks::Install("CAESoundManager", "PauseManually", 0x4EF510, &CAESoundManager::PauseManually);
    ReversibleHooks::Install("CAESoundManager", "RequestNewSound", 0x4EFB10, &CAESoundManager::RequestNewSound);
    ReversibleHooks::Install("CAESoundManager", "AreSoundsPlayingInBankSlot", 0x4EF520, &CAESoundManager::AreSoundsPlayingInBankSlot);
    ReversibleHooks::Install("CAESoundManager", "AreSoundsOfThisEventPlayingForThisEntity", 0x4EF570, &CAESoundManager::AreSoundsOfThisEventPlayingForThisEntity);
    ReversibleHooks::Install("CAESoundManager", "AreSoundsOfThisEventPlayingForThisEntityAndPhysical", 0x4EF5D0, &CAESoundManager::AreSoundsOfThisEventPlayingForThisEntityAndPhysical);
    ReversibleHooks::Install("CAESoundManager", "GetVirtualChannelForPhysicalChannel", 0x4EF630, &CAESoundManager::GetVirtualChannelForPhysicalChannel);
    ReversibleHooks::Install("CAESoundManager", "CancelSoundsInBankSlot", 0x4EFC60, &CAESoundManager::CancelSoundsInBankSlot);
    ReversibleHooks::Install("CAESoundManager", "CancelSoundsOfThisEventPlayingForThisEntity", 0x4EFB90, &CAESoundManager::CancelSoundsOfThisEventPlayingForThisEntity);
    ReversibleHooks::Install("CAESoundManager", "CancelSoundsOfThisEventPlayingForThisEntityAndPhysical", 0x4EFBF0, &CAESoundManager::CancelSoundsOfThisEventPlayingForThisEntityAndPhysical);
}

bool CAESoundManager::Initialise()
{
    const auto availChannels = AEAudioHardware.GetNumAvailableChannels();
    if (availChannels <= 10)
        return false;

    m_nNumAvailableChannels = availChannels >= MAX_NUM_SOUNDS ? MAX_NUM_SOUNDS : availChannels;
    m_nChannel = AEAudioHardware.AllocateChannels(m_nNumAvailableChannels);
    if (m_nChannel == -1)
        return false;

    //BUG? There's some short weird logic in the original code, i simplified it to what's actually used i think
    m_aChannelSoundTable = new int16_t[m_nNumAvailableChannels];
    m_aChannelSoundPosition = new int16_t[m_nNumAvailableChannels];
    m_aChannelSoundUncancellable = new int16_t[m_nNumAvailableChannels];

    for (auto& sound : m_aSounds)
    {
        sound.m_nIsUsed = 0;
        sound.m_nHasStarted = 0;
    }

    for (auto i = 0; i < m_nNumAvailableChannels; ++i)
        m_aChannelSoundTable[i] = -1;

    m_nUpdateTime = CTimer::m_snTimeInMilliseconds;
    m_bPauseTimeInUse = false;
    m_nPauseUpdateTime = 0;
    m_bManuallyPaused = false;

    printf("Initialised SoundManager\n");
    return true;
}

void CAESoundManager::Terminate()
{
    delete[] m_aChannelSoundTable;
    delete[] m_aChannelSoundPosition;
    delete[] m_aChannelSoundUncancellable;

    m_aChannelSoundTable = nullptr;
    m_aChannelSoundPosition = nullptr;
    m_aChannelSoundUncancellable = nullptr;
}

void CAESoundManager::Reset()
{
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed)
            continue;

        sound.StopSound();
    }
}

void CAESoundManager::PauseManually(uchar bPause)
{
    m_bManuallyPaused = bPause;
}

CAESound* CAESoundManager::RequestNewSound(CAESound* pSound)
{
    int16_t firstFree;
    for (firstFree = 0; firstFree < MAX_NUM_SOUNDS; ++firstFree)
    {
        if (!m_aSounds[firstFree].m_nIsUsed)
            break;
    }

    if (firstFree >= MAX_NUM_SOUNDS)
        return nullptr;

    auto& newSound = m_aSounds[firstFree];
    newSound = *pSound;
    pSound->UnregisterWithPhysicalEntity();
    newSound.NewVPSLentry();
    AEAudioHardware.RequestVirtualChannelSoundInfo(firstFree, newSound.m_nSoundIdInSlot, newSound.m_nBankSlotId);

    return &newSound;

}

int16_t CAESoundManager::AreSoundsPlayingInBankSlot(short bankSlot)
{
    auto nPlaying = eSoundPlayingStatus::SOUND_NOT_PLAYING;
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_nBankSlotId != bankSlot)
            continue;

        if (sound.m_nHasStarted)
            return eSoundPlayingStatus::SOUND_HAS_STARTED;

        nPlaying = eSoundPlayingStatus::SOUND_PLAYING;
    }

    return nPlaying;
}

int16_t CAESoundManager::AreSoundsOfThisEventPlayingForThisEntity(short eventId, CAEAudioEntity* audioEntity)
{
    auto nPlaying = eSoundPlayingStatus::SOUND_NOT_PLAYING;
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_nEvent != eventId || sound.m_pBaseAudio != audioEntity)
            continue;

        if (sound.m_nHasStarted)
            return eSoundPlayingStatus::SOUND_HAS_STARTED;

        nPlaying = eSoundPlayingStatus::SOUND_PLAYING;
    }

    return nPlaying;
}

int16_t CAESoundManager::AreSoundsOfThisEventPlayingForThisEntityAndPhysical(short eventId,
                                                                             CAEAudioEntity* audioEntity,
                                                                             CPhysical* physical)
{
    bool nPlaying = eSoundPlayingStatus::SOUND_NOT_PLAYING;
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_nEvent != eventId || sound.m_pBaseAudio != audioEntity || sound.m_pPhysicalEntity != physical)
            continue;

        if (sound.m_nHasStarted)
            return eSoundPlayingStatus::SOUND_HAS_STARTED;

        nPlaying = eSoundPlayingStatus::SOUND_PLAYING;
    }

    return nPlaying;
}

void CAESoundManager::CancelSoundsOfThisEventPlayingForThisEntity(short eventId, CAEAudioEntity* audioEntity)
{
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_nEvent != eventId || sound.m_pBaseAudio != audioEntity)
            continue;

            sound.StopSoundAndForget();
    }
}

void CAESoundManager::CancelSoundsOfThisEventPlayingForThisEntityAndPhysical(short eventId,
                                                                             CAEAudioEntity* audioEntity,
                                                                             CPhysical* physical)
{
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_nEvent != eventId || sound.m_pBaseAudio != audioEntity || sound.m_pPhysicalEntity != physical)
            continue;

        sound.StopSoundAndForget();
    }
}

void CAESoundManager::CancelSoundsInBankSlot(short bankSlot, uchar bFullStop)
{
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_nBankSlotId != bankSlot)
            continue;

        if (bFullStop)
            sound.StopSoundAndForget();
        else
            sound.StopSound();
    }
}

void CAESoundManager::CancelSoundsOwnedByAudioEntity(CAEAudioEntity* audioEntity, uchar bFullStop)
{
    for (auto& sound : m_aSounds)
    {
        if (!sound.m_nIsUsed || sound.m_pBaseAudio != audioEntity)
            continue;

        if (bFullStop)
            sound.StopSoundAndForget();
        else
            sound.StopSound();
    }
}

int16_t CAESoundManager::GetVirtualChannelForPhysicalChannel(short physicalChannel)
{
    const auto channel = physicalChannel - m_nChannel;
    if (channel < 0 || channel >= m_nNumAvailableChannels)
        return -1;

    return m_aChannelSoundTable[channel];
}
