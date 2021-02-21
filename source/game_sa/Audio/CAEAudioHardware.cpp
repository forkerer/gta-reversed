#include "StdInc.h"
#include "CAEAudioHardware.h"

CAEAudioHardware& AEAudioHardware = *reinterpret_cast<CAEAudioHardware*>(0xB5F8B8);

void CAEAudioHardware::InjectHooks()
{
    ReversibleHooks::Install("CAEAudioHardware", "RequestVirtualChannelSoundInfo", 0x4D8E60, &CAEAudioHardware::RequestVirtualChannelSoundInfo);
    ReversibleHooks::Install("CAEAudioHardware", "AllocateChannels", 0x5B9340, &CAEAudioHardware::AllocateChannels);
}

void CAEAudioHardware::LoadSoundBank(ushort bankId, short bankSlotId)
{
    plugin::CallMethod<0x4D88A0, CAEAudioHardware*, ushort, short>(this, bankId, bankSlotId);
}

float CAEAudioHardware::GetSoundHeadroom(ushort soundId, short bankSlotId)
{
    return plugin::CallMethodAndReturn<float, 0x4D8E30, CAEAudioHardware*, ushort, short>(this, soundId, bankSlotId);
}

void CAEAudioHardware::RequestVirtualChannelSoundInfo(ushort soundIndex, ushort soundIdInSlot, ushort bankSlotId)
{
    if (soundIndex >= MAX_NUM_SOUNDS)
        return;

    m_aBankSlotIds[soundIndex] = bankSlotId;
    m_aSoundIdsInSlots[soundIndex] = soundIdInSlot;
}

int16_t CAEAudioHardware::AllocateChannels(ushort numChannels)
{
    if (!numChannels || numChannels > m_wNumAvailableChannels)
        return -1;

    auto slot = 0;
    while (m_anNumChannelsInSlot[slot] && slot < MAX_NUM_AUDIO_CHANNELS)
        slot += m_anNumChannelsInSlot[slot];

    if (slot >= MAX_NUM_AUDIO_CHANNELS)
        return -1;

    m_anNumChannelsInSlot[slot] = numChannels;
    m_wNumAvailableChannels -= numChannels;
    return slot;
}
