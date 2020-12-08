#include "StdInc.h"

template<typename T>
inline void ReversibleHooks::HookInstall(const std::string& sIdentifier, const std::string& sFuncName, unsigned int installAddress, T addressToJumpTo, int iJmpCodeSize)
{
    if (m_HooksMap.find(sIdentifier) == m_HooksMap.end())
        m_HooksMap.insert(sIdentifier, std::vector<SReversibleHook>());

    auto& usedVector = m_HooksMap[sIdentifier];

    unsigned int dwAddressToJumpTo = (unsigned int)FunctionPointerToVoidP(addressToJumpTo);
    const unsigned int x86FixedJumpSize = 5U;
    SReversibleHook theHook;
    theHook.m_iLibFunctionAddress = dwAddressToJumpTo;
    theHook.m_sIdentifier = sIdentifier;
    theHook.m_sFunctionName = sFuncName;
    theHook.m_iRealHookedAddress = installAddress;

    theHook.m_HookContent.jumpLocation = dwAddressToJumpTo - installAddress - x86FixedJumpSize;
    memset(theHook.m_HookContent.possibleNops, NOP_OPCODE, iJmpCodeSize - x86FixedJumpSize);

    const int maxBytesToProtect = std::max(iJmpCodeSize, 8);
    DWORD dwProtect[2] = { 0 };
    VirtualProtect((void*)installAddress, maxBytesToProtect, PAGE_EXECUTE_READWRITE, &dwProtect[0]);

    // workaround for hoodlum crashes due to securom protection.
    if (*(std::uint8_t*)installAddress == NOP_OPCODE) {
        bool bJumpFound = false;
        std::uint32_t i = 0;
        for (; i < 4; i++) {
            if (*(std::uint8_t*)(installAddress + i) == JUMP_OPCODE) {
                bJumpFound = true;
                break;
            }
        }
        if (bJumpFound) {
            DWORD newInstallAddress = installAddress + i;
            DWORD jumpDestination = *(DWORD*)(newInstallAddress + 1) + newInstallAddress + x86FixedJumpSize;
            theHook.m_iRealHookedAddress = jumpDestination;
            theHook.m_HookContent.jumpLocation = (DWORD)jumpDestination - (DWORD)installAddress - (DWORD)x86FixedJumpSize;
            theHook.m_iHookedBytes = 5U;
            DWORD dwProtectHoodlum[2] = { 0 };
            VirtualProtect((void*)installAddress, 5, PAGE_EXECUTE_READWRITE, &dwProtectHoodlum[0]);
            memcpy((void*)&theHook.m_OriginalFunctionContent, (void*)installAddress, 5U);
            memcpy((void*)installAddress, &theHook.m_HookContent, x86FixedJumpSize);
            VirtualProtect((void*)installAddress, 5, dwProtectHoodlum[0], &dwProtectHoodlum[1]);
            theHook.m_bIsHooked = true;
        }
        else {
            printf("HookInstall: Couldn't find the jump for address = %#.8x\n", installAddress);
            theHook.m_iHookedBytes = iJmpCodeSize;
            memcpy((void*)&theHook.m_OriginalFunctionContent, (void*)installAddress, iJmpCodeSize);
            memcpy((void*)installAddress, &theHook.m_HookContent, iJmpCodeSize);
            theHook.m_bIsHooked = true;
        }
    }
    else {
        theHook.m_iHookedBytes = iJmpCodeSize;
        memcpy((void*)&theHook.m_OriginalFunctionContent, (void*)installAddress, iJmpCodeSize);
        memcpy((void*)installAddress, &theHook.m_HookContent, iJmpCodeSize);
        theHook.m_bIsHooked = true;
    }
    VirtualProtect((void*)installAddress, maxBytesToProtect, dwProtect[0], &dwProtect[1]);

    usedVector.insert(std::move(theHook));
}

void ReversibleHooks::HookSwitch(SReversibleHook& sHook)
{
    DWORD dwProtect[2] = { 0 };
    auto pDst = (void*)sHook.m_iRealHookedAddress;
    auto pSrc = (void*)&sHook.m_OriginalFunctionContent;
    if (!sHook.m_bIsHooked)
        pSrc = (void*)&sHook.m_HookContent;


    VirtualProtect(pDst, sHook.m_iHookedBytes, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
    memcpy(pDst, pSrc, sHook.m_iHookedBytes);
    VirtualProtect(pDst, sHook.m_iHookedBytes, dwProtect[0], &dwProtect[1]);
    sHook.m_bIsHooked = !sHook.m_bIsHooked;
}

bool ReversibleHooks::IsFunctionHooked(const std::string& sIdentifier, const std::string& sFuncName)
{
    auto hook = GetHook(sIdentifier, sFuncName);
    if (hook == nullptr)
        return false;

    return hook->m_bIsHooked;
}

SReversibleHook* ReversibleHooks::GetHook(const std::string& sIdentifier, const std::string& sFuncName)
{
    if (m_HooksMap.find(sIdentifier) == m_HooksMap.end())
        return nullptr;

    auto& vecHooks = m_HooksMap[sIdentifier];
    auto findResult = std::find_if(vecHooks.begin(), vecHooks.end(), [&](const SReversibleHook& hook) {return hook.m_sFunctionName == sFuncName; });
    if (findResult == vecHooks.end())
        return nullptr;

    auto& refHook = *findResult;
    return &refHook;
}
