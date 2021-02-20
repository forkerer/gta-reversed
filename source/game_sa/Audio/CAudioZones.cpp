#include "StdInc.h"

int(&CAudioZones::m_aActiveBoxes)[10] = *(int(*)[10])0xB6DC6C;
int(&CAudioZones::m_aActiveSpheres)[10] = *(int(*)[10])0xB6DC94;

uint32_t& CAudioZones::m_NumActiveBoxes = *(uint32_t*)0xB6DCBC;
uint32_t& CAudioZones::m_NumActiveSpheres = *(uint32_t*)0xB6DCC0;
uint32_t& CAudioZones::m_NumBoxes = *(uint32_t*)0xB6DCC4;
uint32_t& CAudioZones::m_NumSpheres = *(uint32_t*)0xB6DCC8;

tAudioZoneBox(&CAudioZones::m_aBoxes)[NUM_AUDIO_BOXES] = *(tAudioZoneBox(*)[NUM_AUDIO_BOXES])0xB6DCD0;
tAudioZoneSphere(&CAudioZones::m_aSpheres)[NUM_AUDIO_SPHERES] = *(tAudioZoneSphere(*)[NUM_AUDIO_SPHERES])0xB6EBA8;

void CAudioZones::InjectHooks()
{
}

void CAudioZones::Init()
{
    plugin::Call<0x5081A0>();
}
