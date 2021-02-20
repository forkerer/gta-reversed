#include "StdInc.h"
#include "CAEAudioHardware.h"

CAEAudioHardware& AEAudioHardware = *reinterpret_cast<CAEAudioHardware*>(0xB5F8B8);

void CAEAudioHardware::InjectHooks()
{
}

float CAEAudioHardware::GetSoundHeadroom(ushort soundId, short bankSlotId)
{
    return plugin::CallMethodAndReturn<float, 0x4D8E30, CAEAudioHardware*, ushort, short>(this, soundId, bankSlotId);
}
