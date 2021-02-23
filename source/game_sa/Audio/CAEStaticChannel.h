#pragma once
#include "CAEAudioChannel.h"

class CAEStaticChannel : public CAEAudioChannel
{
public:
    CAEStaticChannel(unsigned short, unsigned int, unsigned short);
    ~CAEStaticChannel() override;

public:


public:
    void Service() override;
    bool IsSoundPlaying() override;
    uint16_t GetPlayTime() override;
    uint16_t GetLength() override;
    void Play(short, signed char, float) override;
    void SynchPlayback() override;
    void Stop() override;
    void SetFrequencyScalingFactor(float factor) override;
};
