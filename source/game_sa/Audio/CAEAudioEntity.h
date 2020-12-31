/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"
#include "CAESound.h"

class CEntity;

class  CAEAudioEntity {
protected:
    CAEAudioEntity();
    ~CAEAudioEntity();
public:
    CEntity *m_pEntity;
    CAESound m_tempSound;

    // vtable
    virtual void UpdateParameters(CAESound *sound, short arg2);

private:
    void UpdateParameters_Reversed(CAESound* sound, short arg2);
};

VALIDATE_SIZE(CAEAudioEntity, 0x7C);
