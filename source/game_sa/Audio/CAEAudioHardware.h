#pragma once
#include "PluginBase.h"
#include "CVector.h"

#ifdef PlaySound
#undef PlaySound
#endif

// DirectSound8 structure
struct DSCAPS
{
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwMinSecondarySampleRate;
    uint32_t dwMaxSecondarySampleRate;
    uint32_t dwPrimaryBuffers;
    uint32_t dwMaxHwMixingAllBuffers;
    uint32_t dwMaxHwMixingStaticBuffers;
    uint32_t dwMaxHwMixingStreamingBuffers;
    uint32_t dwFreeHwMixingAllBuffers;
    uint32_t dwFreeHwMixingStaticBuffers;
    uint32_t dwFreeHwMixingStreamingBuffers;
    uint32_t dwMaxHw3DAllBuffers;
    uint32_t dwMaxHw3DStaticBuffers;
    uint32_t dwMaxHw3DStreamingBuffers;
    uint32_t dwFreeHw3DAllBuffers;
    uint32_t dwFreeHw3DStaticBuffers;
    uint32_t dwFreeHw3DStreamingBuffers;
    uint32_t dwTotalHwMemBytes;
    uint32_t dwFreeHwMemBytes;
    uint32_t dwMaxContigFreeHwMemBytes;
    uint32_t dwUnlockTransferRateHwBuffers;
    uint32_t dwPlayCpuOverheadSwBuffers;
    uint32_t dwReserved1;
    uint32_t dwReserved2;
};
VALIDATE_SIZE(DSCAPS, 0x60);


class CAEAudioHardware
{
public:
    CAEAudioHardware();

public:
    uint8_t field_0;
    bool m_bDisableEffectsLoading;
    uint8_t field_2;
    uint8_t field_3;
    uint8_t field_4;
    uint8_t m_nReverbEnvironment;
    uint8_t m_awChannelFlags[1][100];
    uint8_t gap6A[28];
    uint16_t field_86;
    int32_t m_nReverbDepth;
    int16_t m_wNumAvailableChannels;
    int16_t m_nNumChannels;
    uint8_t m_anNumSoundsInSlot[100];
    uint8_t gapF4[28];
    float m_afChannelVolumes[64];
    uint8_t field_210[100];
    uint8_t gap274[156];
    float m_afChannelsFrqScalingFactor[64];
    float m_fMusicMasterScalingFactor;
    float m_fEffectMasterScalingFactor;
    float m_fMusicFaderScalingFactor;
    float m_fEffectsFaderScalingFactor;
    float m_fStreamFaderScalingFactor;
    float m_fNonStreamFaderScalingFactor;
    float field_428;
    float field_42C;
    uint8_t m_aBankSlotIds[100];
    uint8_t gap494[500];
    uint8_t m_aSoundTypes[100];
    uint8_t gap6EC[500];
    uint8_t m_anVirtualChannelSoundLoopStartTimes[1][100];
    uint8_t gap944[500];
    uint8_t m_anVirtualChannelSoundLengths[100];
    uint8_t gapB9C[500];
    uint8_t m_nBassSet;
    uint8_t __pad1__[3];
    float m_fBassEqGain;
    struct CAEMP3BankLoader* m_pMP3BankLoader;
    void* m_pMP3TrackLoader;
    struct IDirectSound8* m_pDirectSound;
    void* m_dwSpeakerConfig;
    void* m_n3dEffectsQueryResult;
    DSCAPS m_dsCaps;
    struct IDirectSound3DListener* m_pDirectSound3dListener;
    void* m_pStreamingChannel;
    struct CAEStreamThread* m_pStreamThread;
    uint8_t gapE14[76];
    void* m_aChannels[64];
    uint8_t field_F64[100];
    uint8_t gapFC8[60];
    int32_t field_1004;
    int32_t field_1008;
    int32_t field_100C;
    uint8_t field_1010;
    int32_t field_1014;

public:
    static void InjectHooks();

    // Return types aren't real, I've just copied the signatures for now
    void GetActualNumberOfHardwareChannels();
    void InitOpenALListener();
    void Initialise();
    void Terminate();
    void PlaySound(short, ushort, ushort, ushort, short, short, float);
    void GetNumAvailableChannels();
    void GetChannelPlayTimes(short, short*);
    void SetChannelVolume(short, ushort, float, uchar);
    void LoadSoundBank(ushort bankId, short bankSlotId);
    void IsSoundBankLoaded(ushort, short);
    void GetSoundBankLoadingStatus(ushort, short);
    void StopSound(short, ushort);
    void SetChannelPosition(short, ushort, CVector*, uchar);
    void SetChannelFrequencyScalingFactor(short, ushort, float);
    void RescaleChannelVolumes();
    void Service();
    void UpdateReverbEnvironment();
    float GetSoundHeadroom(ushort soundId, short bankSlotId);
    void EnableEffectsLoading();
    void DisableEffectsLoading();
    void RequestVirtualChannelSoundInfo(ushort, ushort, ushort);
    void GetVirtualChannelSoundLengths(short*);
    void GetVirtualChannelSoundLoopStartTimes(short*);
    void LoadSound(ushort, ushort, short);
    void IsSoundLoaded(ushort, ushort, short);
    void GetSoundLoadingStatus(ushort, ushort, short);
    void PlayTrack(uint, int, uint, char, bool, bool);
    void StartTrackPlayback();
    void StopTrack();
    void GetTrackPlayTime();
    void GetTrackLengthMs();
    void GetActiveTrackID();
    void GetPlayingTrackID();
    void GetBeatInfo(struct tBeatInfo*);
    void SetBassSetting(signed char, float);
    void DisableBassEq();
    void EnableBassEq();
    void SetChannelFlags(short, ushort, short);
    void SetMusicMasterScalingFactor(float);
    void SetEffectsMasterScalingFactor(float);
    void SetMusicFaderScalingFactor(float);
    void SetEffectsFaderScalingFactor(float);
    void GetEffectsMasterScalingFactor();
    void GetMusicMasterScalingFactor();
    void GetEffectsFaderScalingFactor();
    void SetStreamFaderScalingFactor(float);
    void SetNonStreamFaderScalingFactor(float);
    void IsStreamingFromDVD();
    void GetDVDDriveLetter();
    void CheckDVD();
    void PauseAllSounds();
    void ResumeAllSounds();
    void AllocateChannels(ushort);

};
VALIDATE_SIZE(CAEAudioHardware, 0x1018);

extern CAEAudioHardware& AEAudioHardware;
