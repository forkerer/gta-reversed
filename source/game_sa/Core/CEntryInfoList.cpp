#include "StdInc.h"

void CEntryInfoList::InjectHooks()
{
    ReversibleHooks::Install("CEntryInfoList", "Flush", 0x536E10, &CEntryInfoList::Flush);
}

void CEntryInfoList::Flush()
{
    CEntryInfoNode* pCurNode = &m_pNode;
    if (!pCurNode->m_pDoubleLinkList)
        return;

    while (pCurNode) {
        auto pNextNode = pCurNode->m_pNext;
        if (pCurNode->m_pDoubleLinkList == m_pNode.m_pDoubleLinkList)
            m_pNode.m_pDoubleLinkList = pNextNode->m_pDoubleLinkList;

        if (pCurNode->m_pPrevious)
            pCurNode->m_pPrevious->m_pNext = pCurNode->m_pNext;

        if (pCurNode->m_pNext)
            pCurNode->m_pNext->m_pPrevious = pCurNode->m_pPrevious;

        delete pCurNode;
        pCurNode = pNextNode;
    }
}
