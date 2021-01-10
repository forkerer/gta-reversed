#pragma once
#include "PluginBase.h"
#include "CEntryInfoNode.h"

class CEntryInfoList {
public:
    CEntryInfoNode m_pNode;

public:
    static void InjectHooks();

    void Flush(); //0x536E10
};
VALIDATE_SIZE(CEntryInfoList, 0x14);
