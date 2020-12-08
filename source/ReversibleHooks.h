#pragma once

#include <unordered_map>
#include <vector>
#include <string>


#pragma pack(push, 1)
struct SHookContent {
    unsigned char jumpOpCode = JUMP_OPCODE;
    unsigned int jumpLocation = 0;
    unsigned char possibleNops[52 - sizeof(jumpOpCode) - sizeof(jumpLocation)] = { 0 };
};
#pragma pack(pop)
static_assert(sizeof(SHookContent) == 0x34, "Incorrect struct size: SHookContent");

struct SReversibleHook {
    SHookContent m_HookContent;
    unsigned char m_OriginalFunctionContent[sizeof(m_HookContent)];
    unsigned int m_iHookedBytes;

    bool m_bIsHooked = false;
    std::string m_sIdentifier;
    std::string m_sFunctionName;
    unsigned int m_iLibFunctionAddress;
    unsigned int m_iRealHookedAddress;
};

class ReversibleHooks {
public:
    static ReversibleHooks& GetInstance() {
        static ReversibleHooks instance;
        return instance;
    }
    std::unordered_map<std::string, std::vector<SReversibleHook>> GetAllHooks() const { return m_HooksMap; }

    template<typename T>
    void HookInstall(const std::string& sIdentifier, const std::string& sFuncName, unsigned int installAddress, T addressToJumpTo, int iJmpCodeSize = 5);
    void HookSwitch(SReversibleHook& sHook);
    bool IsFunctionHooked(const std::string& sIdentifier, const std::string& sFuncName);
    SReversibleHook* GetHook(const std::string& sIdentifier, const std::string& sFuncName);

private:
    std::unordered_map<std::string, std::vector<SReversibleHook>> m_HooksMap;
    ReversibleHooks() = default;
};

template<typename T>
void HookInstallReversible(const std::string& sIdentifier, const std::string& sFuncName, DWORD installAddress, T addressToJumpTo, int iJmpCodeSize = 5) {
    auto& hooks = ReversibleHooks::GetInstance();
    hooks.HookInstall(sIdentifier, sFuncName, installAddress, addressToJumpTo, iJmpCodeSize);
}
