#include "StdInc.h"
#include "CAEAudioChannel.h"

#include "CAEAudioUtility.h"

void CAEAudioChannel::InjectHooks()
{
    ReversibleHooks::Install("CAEAudioChannel", "SetPosition", 0x4D7950, &CAEAudioChannel::SetPosition);
    ReversibleHooks::Install("CAEAudioChannel", "UpdateStatus", 0x4D7BD0, &CAEAudioChannel::UpdateStatus);
    ReversibleHooks::Install("CAEAudioChannel", "Lost", 0x4D7A10, &CAEAudioChannel::Lost);
    ReversibleHooks::Install("CAEAudioChannel", "ConvertFromBytesToMS", 0x4D79D0, &CAEAudioChannel::ConvertFromBytesToMS);
    ReversibleHooks::Install("CAEAudioChannel", "ConvertFromMsToBytes", 0x4D79F0, &CAEAudioChannel::ConvertFromMsToBytes);
    ReversibleHooks::Install("CAEAudioChannel", "SetFrequency", 0x4D7A50, &CAEAudioChannel::SetFrequency);
    ReversibleHooks::Install("CAEAudioChannel", "SetOriginalFrequency", 0x4D7A70, &CAEAudioChannel::SetOriginalFrequency);
    ReversibleHooks::Install("CAEAudioChannel", "GetCurrentPlaybackPosition", 0x4D79A0, &CAEAudioChannel::GetCurrentPlaybackPosition);
}

void CAEAudioChannel::SetPosition(CVector* vecPos)
{
    if (!m_pDirectSoundBuffer)
        return;

    if (!m_pDirectSound3DBuffer)
        return;

    m_pDirectSound3DBuffer->SetPosition(vecPos->x, vecPos->y, vecPos->z, DS3D_DEFERRED);
}

int CAEAudioChannel::GetCurrentPlaybackPosition()
{
    if (!m_pDirectSoundBuffer)
        return 0;

    uint32_t outPos;
    m_pDirectSoundBuffer->GetCurrentPosition(reinterpret_cast<LPDWORD>(&outPos), nullptr);
    return outPos;
}

uint32_t CAEAudioChannel::ConvertFromBytesToMS(uint32_t bytes)
{
    return CAEAudioUtility::ConvertFromBytesToMS(bytes, m_dwBufferFrequency, m_wFrequencyMult);
}

uint32_t CAEAudioChannel::ConvertFromMsToBytes(uint32_t ms)
{
    return CAEAudioUtility::ConvertFromMSToBytes(ms, m_dwBufferFrequency, m_wFrequencyMult);
}

void CAEAudioChannel::SetFrequency(uint32_t freq)
{
    if (m_dwFrequency == freq)
        return;

    m_dwFrequency = freq;
    if (m_pDirectSoundBuffer)
        m_pDirectSoundBuffer->SetFrequency(freq);
}

void CAEAudioChannel::SetOriginalFrequency(uint32_t freq)
{
    CAEAudioChannel::SetFrequency(freq);
    m_dwOriginalFrequency = freq;
}

void CAEAudioChannel::UpdateStatus()
{
    m_pDirectSoundBuffer->GetStatus(reinterpret_cast<LPDWORD>(&m_dwBufferStatus));
    if (m_dwBufferStatus & DSBSTATUS_BUFFERLOST)
        CAEAudioChannel::Lost();
}

bool CAEAudioChannel::Lost()
{
    while (m_pDirectSoundBuffer->Restore() == DSERR_BUFFERLOST) //BUG: Infinite loop if we don't restore
        Sleep(10);

    return true;
}

