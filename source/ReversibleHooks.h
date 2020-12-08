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
    bool m_bImguiHooked = false;
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

    static void Install(const std::string& sIdentifier, const std::string& sFuncName, DWORD installAddress, void* addressToJumpTo, int iJmpCodeSize = 5) {
        ReversibleHooks::GetInstance().HookInstall(sIdentifier, sFuncName, installAddress, addressToJumpTo, iJmpCodeSize);
    }
    static void Switch(SReversibleHook& sHook) {
        ReversibleHooks::GetInstance().HookSwitch(sHook);
    }
    static bool Hooked(const std::string& sIdentifier, const std::string& sFuncName) {
        return ReversibleHooks::GetInstance().IsFunctionHooked(sIdentifier, sFuncName);
    }
    static std::unordered_map<std::string, std::vector<SReversibleHook>>& GetAllHooks() {
        return ReversibleHooks::GetInstance().m_HooksMap;
    }

private:
    void HookInstall(const std::string& sIdentifier, const std::string& sFuncName, unsigned int installAddress, void* addressToJumpTo, int iJmpCodeSize = 5);
    void HookSwitch(SReversibleHook& sHook);
    bool IsFunctionHooked(const std::string& sIdentifier, const std::string& sFuncName);
    SReversibleHook* GetHook(const std::string& sIdentifier, const std::string& sFuncName);

private:
    std::unordered_map<std::string, std::vector<SReversibleHook>> m_HooksMap;
    ReversibleHooks() = default;
    ReversibleHooks(ReversibleHooks const&) = delete;
    void operator=(ReversibleHooks const&) = delete;
};
