/*
Plugin-SDK (Grand Theft Auto San Andreas) source file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

void CAESound::InjectHooks()
{
    ReversibleHooks::Install("CAESound", "operator=", 0x4EF680, &CAESound::operator=);
    ReversibleHooks::Install("CAESound", "UnregisterWithPhysicalEntity", 0x4EF1A0, &CAESound::UnregisterWithPhysicalEntity);
}

CAESound::CAESound(CAESound& sound)
{
    m_nBankSlotId = sound.m_nBankSlotId;
    m_nSoundIdInSlot = sound.m_nSoundIdInSlot;
    m_pBaseAudio = sound.m_pBaseAudio;
    m_nEvent = sound.m_nEvent;
    m_fMaxVolume = sound.m_fMaxVolume;
    m_fVolume = sound.m_fVolume;
    m_fSoundDistance = sound.m_fSoundDistance;
    m_fSpeed = sound.m_fSpeed;
    field_20 = sound.field_20;
    m_vecCurrPosn = sound.m_vecCurrPosn;
    m_vecPrevPosn = sound.m_vecPrevPosn;
    m_nLastFrameUpdate = sound.m_nLastFrameUpdate;
    m_nCurrTimeUpdate = sound.m_nCurrTimeUpdate;
    m_nPrevTimeUpdate = sound.m_nPrevTimeUpdate;
    m_fCurrCamDist = sound.m_fCurrCamDist;
    m_fPrevCamDist = sound.m_fPrevCamDist;
    m_fTimeScale = sound.m_fTimeScale;
    field_54 = sound.field_54;
    m_nEnvironmentFlags = sound.m_nEnvironmentFlags;
    m_nIsUsed = sound.m_nIsUsed;
    m_nCurrentPlayPosition = sound.m_nCurrentPlayPosition;
    m_nHasStarted = sound.m_nHasStarted;
    m_fFinalVolume = sound.m_fFinalVolume;
    m_fFrequency = sound.m_fFrequency;
    m_nPlayingState = sound.m_nPlayingState;
    m_fSoundHeadRoom = sound.m_fSoundHeadRoom;
    m_nSoundLength = sound.m_nSoundLength;
    m_pPhysicalEntity = nullptr;

    if (sound.m_pPhysicalEntity)
    {
        m_pPhysicalEntity = sound.m_pPhysicalEntity;
        m_pPhysicalEntity->RegisterReference(&m_pPhysicalEntity);
    }
}

CAESound::CAESound(short bankSlotId, short sfxId, CAEAudioEntity *baseAudio, CVector posn,
    float volume, float fDistance, float speed, float timeScale, unsigned char arg9,
    unsigned short environmentFlags, float arg11)
{
    m_nBankSlotId = bankSlotId;
    m_nSoundIdInSlot = sfxId;
    m_pBaseAudio = baseAudio;
    m_vecPrevPosn.x = 0.0;
    m_vecPrevPosn.y = 0.0;
    m_vecPrevPosn.z = 0.0;
    m_pPhysicalEntity = nullptr;
    m_fMaxVolume = -1.0F;
    m_nEvent = -1;
    m_nLastFrameUpdate = 0;

    CAESound::SetPosition(posn);

    m_fVolume = volume;
    m_fSoundDistance = fDistance;
    m_fSpeed = speed;
    m_fTimeScale = timeScale;
    field_54 = arg9;
    m_nEnvironmentFlags = environmentFlags;
    m_nHasStarted = 0;
    m_nCurrentPlayPosition = 0;
    m_fSoundHeadRoom = 0.0F;
    field_20 = arg11;
    m_nIsUsed = 1;
    m_fFinalVolume = -100.0F;
    m_fFrequency = 1.0F;
    m_nSoundLength = -1;
}

CAESound::~CAESound()
{
    CAESound::UnregisterWithPhysicalEntity();
}

void CAESound::operator=(CAESound& sound)
{
    if (this == &sound)
        return;

    CAESound::UnregisterWithPhysicalEntity();
    m_nBankSlotId = sound.m_nBankSlotId;
    m_nSoundIdInSlot = sound.m_nSoundIdInSlot;
    m_pBaseAudio = sound.m_pBaseAudio;
    m_nEvent = sound.m_nEvent;
    m_fMaxVolume = sound.m_fMaxVolume;
    m_fVolume = sound.m_fVolume;
    m_fSoundDistance = sound.m_fSoundDistance;
    m_fSpeed = sound.m_fSpeed;
    field_20 = sound.field_20;
    m_vecCurrPosn = sound.m_vecCurrPosn;
    m_vecPrevPosn = sound.m_vecPrevPosn;
    m_nLastFrameUpdate = sound.m_nLastFrameUpdate;
    m_nCurrTimeUpdate = sound.m_nCurrTimeUpdate;
    m_nPrevTimeUpdate = sound.m_nPrevTimeUpdate;
    m_fCurrCamDist = sound.m_fCurrCamDist;
    m_fPrevCamDist = sound.m_fPrevCamDist;
    m_fTimeScale = sound.m_fTimeScale;
    field_54 = sound.field_54;
    m_nEnvironmentFlags = sound.m_nEnvironmentFlags;
    m_nIsUsed = sound.m_nIsUsed;
    m_nCurrentPlayPosition = sound.m_nCurrentPlayPosition;
    m_nHasStarted = sound.m_nHasStarted;
    m_fFinalVolume = sound.m_fFinalVolume;
    m_fFrequency = sound.m_fFrequency;
    m_nPlayingState = sound.m_nPlayingState;
    m_fSoundHeadRoom = sound.m_fSoundHeadRoom;
    m_nSoundLength = sound.m_nSoundLength;
    m_pPhysicalEntity = nullptr;

    if (sound.m_pPhysicalEntity)
    {
        m_pPhysicalEntity = sound.m_pPhysicalEntity;
        m_pPhysicalEntity->RegisterReference(&m_pPhysicalEntity);
    }
}

void CAESound::UnregisterWithPhysicalEntity()
{
    if (!m_pPhysicalEntity)
        return;

    m_pPhysicalEntity->CleanUpOldReference(&m_pPhysicalEntity);
    m_pPhysicalEntity = nullptr;
}

void CAESound::StopSound()
{
    //((void(__thiscall *)(CAESound *))0x4EF1C0)(this);
    m_nPlayingState = eSoundState::SOUND_STOPPED;
    CAESound::UnregisterWithPhysicalEntity();
}

void CAESound::SetIndividualEnvironment(unsigned short envFlag, unsigned short bEnabled)
{
    /*((void(__thiscall *)(CAESound *, unsigned short, unsigned short))0x4EF2B0)(this, environment,
        state);*/
    if (bEnabled)
        m_nEnvironmentFlags |= envFlag;
    else
        m_nEnvironmentFlags &= ~envFlag;

}

void CAESound::UpdatePlayTime(short soundLength, short newPlayPosition, short playProgress)
{
    //((void(__thiscall *)(CAESound *, short, short, short))0x4EF2E0)(this, soundLength, playPosition, playProgress);
    m_nSoundLength = soundLength;
    if (m_nHasStarted)
        return;

    if (m_nPlayingState != eSoundState::SOUND_ACTIVE)
    {
        m_nCurrentPlayPosition = -1;
        return;
    }

    m_nCurrentPlayPosition += static_cast<short>(static_cast<float>(playProgress) * m_fFrequency);
    if (m_nCurrentPlayPosition < soundLength)
        return;

    if (newPlayPosition == -1)
    {
        m_nCurrentPlayPosition = -1;
        return;
    }

    m_nCurrentPlayPosition = newPlayPosition + (m_nCurrentPlayPosition % soundLength);
}

void CAESound::GetRelativePosition(CVector *outPosn)
{
    //((void(__thiscall *)(CAESound *, CVector *))0x4EF350)(this, outPosn);
    if (!CAESound::GetFrontEnd())
        return CAEAudioEnvi
}

void CAESound::CalculateFrequency()
{
    ((void(__thiscall *)(CAESound *))0x4EF390)(this);
}

void CAESound::UpdateFrequency()
{
    ((void(__thiscall *)(CAESound *))0x4EF3E0)(this);
}

float CAESound::GetRelativePlaybackFrequencyWithDoppler()
{
    return ((float(__thiscall *)(CAESound *))0x4EF400)(this);
}

float CAESound::GetSlowMoFrequencyScalingFactor()
{
    return ((float(__thiscall *)(CAESound *))0x4EF440)(this);
}

void CAESound::NewVPSLentry()
{
    ((void(__thiscall *)(CAESound *))0x4EF7A0)(this);
}

void CAESound::RegisterWithPhysicalEntity(CEntity *entity)
{
    ((void(__thiscall *)(CAESound *, CEntity *))0x4EF820)(this, entity);
}

void CAESound::StopSoundAndForget()
{
    ((void(__thiscall *)(CAESound *))0x4EF850)(this);
}

void CAESound::SetPosition(CVector posn)
{
    ((void(__thiscall *)(CAESound *, CVector))0x4EF880)(this, posn);
}

void CAESound::CalculateVolume()
{
    ((void(__thiscall *)(CAESound *))0x4EFA10)(this);
}

void CAESound::Initialise(short bankSlotId, short sfxId, CAEAudioEntity *baseAudio, CVector posn,
    float volume, float maxDistance, float speed, float timeScale,
    unsigned char arg9, unsigned short environmentFlags, float arg11,
    short currPlayPosn)
{
    ((void(__thiscall *)(CAESound *, short, short, CAEAudioEntity *, CVector, float, float, float,
        float, unsigned char, unsigned short, float, short))0x4EFE50)(this, bankSlotId, sfxId,
            baseAudio, posn, volume, maxDistance, speed, timeScale, arg9, environmentFlags, arg11,
            currPlayPosn);
}

void CAESound::UpdateParameters(short arg1)
{
    ((void(__thiscall *)(CAESound *, short))0x4EFF50)(this, arg1);
}
