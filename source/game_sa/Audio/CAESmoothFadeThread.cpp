#include "StdInc.h"
#include "CAESmoothFadeThread.h"

CAESmoothFadeThread& AESmoothFadeThread = *(CAESmoothFadeThread*)0xB608D0;

void CAESmoothFadeThread::InjectHooks()
{
    ReversibleHooks::Install("CAESmoothFadeThread", "Start", 0x4EEA10, &CAESmoothFadeThread::Start);
    ReversibleHooks::Install("CAESmoothFadeThread", "Stop", 0x4EEA30, &CAESmoothFadeThread::Stop);
    ReversibleHooks::Install("CAESmoothFadeThread", "WaitForExit", 0x4EEA40, &CAESmoothFadeThread::WaitForExit);
    ReversibleHooks::Install("CAESmoothFadeThread", "Initialise", 0x4EEEC0, &CAESmoothFadeThread::Initialise);
    ReversibleHooks::Install("CAESmoothFadeThread", "InitialiseRequestSlots", 0x4EE9F0, &CAESmoothFadeThread::InitialiseRequestSlots);
    ReversibleHooks::Install("CAESmoothFadeThread", "CancelFade", 0x4EEA50, &CAESmoothFadeThread::CancelFade);
    ReversibleHooks::Install("CAESmoothFadeThread", "SetBufferVolume", 0x4EEC80, &CAESmoothFadeThread::SetBufferVolume);
    ReversibleHooks::Install("CAESmoothFadeThread", "SmoothFadeProc", 0x4EEE90, &CAESmoothFadeThread::SmoothFadeProc);
}

CAESmoothFadeThread::CAESmoothFadeThread()
{
    m_threadHandle = (HANDLE)-1;
    m_dwThreadId = 0;
    m_bThreadCreated = false;
    m_bActive = false;
    m_wUnkn = 0;
    m_wUnkn2 = 0;
    m_nLastServiceTime = 0;
    m_nUnused = 0;
}

void CAESmoothFadeThread::Initialise()
{
    CAESmoothFadeThread::InitialiseRequestSlots();
    m_threadHandle = CreateThread(nullptr, 0, &CAESmoothFadeThread::SmoothFadeProc, this, CREATE_SUSPENDED, &m_dwThreadId);
    if (m_threadHandle == (HANDLE)-1)
        m_bThreadCreated = false;
    else
    {
        SetThreadPriority(m_threadHandle, 0);
        m_bThreadCreated = true;
        m_bThreadInvalid = false;
        //NOTSA: Added thread name to ease debugging
        SetThreadDescription(m_threadHandle, L"CAESmoothFadeThread");
    }
}

void CAESmoothFadeThread::InitialiseRequestSlots()
{
    for (auto& entry : m_aEntries)
    {
        entry.m_nStatus = eSmoothFadeEntryStatus::STATE_INACTIVE;
        entry.m_pSoundBuffer = nullptr;
    }
}

void CAESmoothFadeThread::Start()
{
    m_bActive = true;
    ResumeThread(m_threadHandle);
}

void CAESmoothFadeThread::Stop()
{
    m_bActive = false;
}

void CAESmoothFadeThread::WaitForExit()
{
    WaitForSingleObject(m_threadHandle, -1);
}

void CAESmoothFadeThread::Service()
{
    plugin::CallMethod<0x4EED10, CAESmoothFadeThread*>(this);
}

void CAESmoothFadeThread::CancelFade(IDirectSoundBuffer* buffer)
{
    if (m_bThreadInvalid)
        return;

    for (auto& entry : m_aEntries)
        if (entry.m_nStatus == eSmoothFadeEntryStatus::STATE_ACTIVE && entry.m_pSoundBuffer == buffer)
            entry.m_nStatus = eSmoothFadeEntryStatus::STATE_CANCELLED;
}

void CAESmoothFadeThread::SetBufferVolume(IDirectSoundBuffer* buffer, float volume)
{
    auto dwVolume = static_cast<LONG>(volume * 100.0F);
    if (m_bThreadInvalid)
    {
        buffer->SetVolume(dwVolume);
        return;
    }

    int entryInd;
    for (entryInd = 0; entryInd < NUM_SMOOTHFADE_ENTRIES; ++entryInd)
    {
        auto& entry = m_aEntries[entryInd];
        if (entry.m_nStatus == eSmoothFadeEntryStatus::STATE_ACTIVE && entry.m_pSoundBuffer == buffer)
            break;
    }

    if (entryInd >= NUM_SMOOTHFADE_ENTRIES)
        buffer->SetVolume(dwVolume);
    else
    {
        auto& entry = m_aEntries[entryInd];
        if (entry.m_bUnkn || volume == entry.m_fTargetVolume || approxEqual(volume, entry.m_fTargetVolume, 0.01F))
            return;

        entry.m_nStatus = eSmoothFadeEntryStatus::STATE_CANCELLED;
        buffer->SetVolume(dwVolume);
    }
}

DWORD WINAPI CAESmoothFadeThread::SmoothFadeProc(void* pSmoothFade)
{
    auto* pFade = static_cast<CAESmoothFadeThread*>(pSmoothFade);
    while (pFade->m_bActive)
    {
        pFade->Service();
        Sleep(1);
    }

    return 0;
}
