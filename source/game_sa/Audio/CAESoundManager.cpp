#include "StdInc.h"
#include "CAESoundManager.h"

CAESoundManager& AESoundManager = *(CAESoundManager*)0xB62CB0;

void CAESoundManager::InjectHooks()
{
    ReversibleHooks::Install("CAESoundManager", "CancelSoundsInBankSlot", 0x4EFC60, &CAESoundManager::CancelSoundsInBankSlot);
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
