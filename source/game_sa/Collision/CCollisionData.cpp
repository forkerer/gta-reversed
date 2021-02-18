#include "StdInc.h"

void CCollisionData::InjectHooks()
{
    ReversibleHooks::Install("CCollisionData", "RemoveCollisionVolumes", 0x40F070, &CCollisionData::RemoveCollisionVolumes);
    ReversibleHooks::Install("CCollisionData", "Copy", 0x40F120, &CCollisionData::Copy);
}

CCollisionData::CCollisionData()
{
    bUsesDisks = false;
    bNotEmpty = false;
    bHasShadowInfo = false;

    m_nNumSpheres = 0;
    m_nNumBoxes = 0;
    m_nNumTriangles = 0;
    m_nNumLines = 0;

    m_pSpheres = nullptr;
    m_pBoxes = nullptr;
    m_pLines = nullptr;
    m_pVertices = nullptr;
    m_pTriangles = nullptr;
    m_pTrianglePlanes = nullptr;

    m_nNumShadowTriangles = 0;
    m_nNumShadowVertices = 0;
    m_pShadowTriangles = nullptr;
    m_pShadowVertices = nullptr;
}

void CCollisionData::RemoveCollisionVolumes()
{
    CMemoryMgr::Free(m_pSpheres);
    CMemoryMgr::Free(m_pLines);
    CMemoryMgr::Free(m_pBoxes);
    CMemoryMgr::Free(m_pVertices);
    CMemoryMgr::Free(m_pTriangles);
    CMemoryMgr::Free(m_pShadowTriangles);
    CMemoryMgr::Free(m_pShadowVertices);

    CCollision::RemoveTrianglePlanes(this);

    m_nNumSpheres = 0;
    m_nNumLines = 0;
    m_nNumBoxes = 0;
    m_nNumTriangles = 0;
    m_pSpheres = nullptr;
    m_pLines = nullptr;
    m_pBoxes = nullptr;
    m_pVertices = nullptr;
    m_pTriangles = nullptr;
    m_pShadowTriangles = nullptr;
    m_pShadowVertices = nullptr;
}

void CCollisionData::Copy(CCollisionData const& src)
{
// ----- SPHERES -----
    if (m_nNumSpheres != src.m_nNumSpheres || !src.m_nNumSpheres)
    {
        m_nNumSpheres = src.m_nNumSpheres;
        CMemoryMgr::Free(m_pSpheres);
        m_pSpheres = nullptr;

        if (src.m_nNumSpheres)
            m_pSpheres = static_cast<CColSphere*>(CMemoryMgr::Malloc(m_nNumSpheres * sizeof(CColSphere)));
    }
    for (auto i = 0; i < m_nNumSpheres; ++i)
        m_pSpheres[i] = src.m_pSpheres[i];

// ----- LINES / DISKS -----
    if (m_nNumLines != src.m_nNumLines || !src.m_nNumLines || bUsesDisks != src.bUsesDisks)
    {
        bUsesDisks = src.bUsesDisks;
        m_nNumLines = src.m_nNumLines;
        CMemoryMgr::Free(m_pLines);
        m_pLines = nullptr;

        if (src.m_nNumLines)
        {
            if (src.bUsesDisks)
                m_pDisks = static_cast<CColDisk*>(CMemoryMgr::Malloc(m_nNumLines * sizeof(CColDisk)));
            else
                m_pLines = static_cast<CColLine*>(CMemoryMgr::Malloc(m_nNumLines * sizeof(CColLine)));
        }
    }
    for (auto i = 0; i < m_nNumLines; ++i)
    {
        if (bUsesDisks)
            m_pDisks[i] = src.m_pDisks[i];
        else
            m_pLines[i] = src.m_pLines[i];
        
    }

// ----- BOXES -----
    if (m_nNumBoxes != src.m_nNumBoxes || !src.m_nNumBoxes)
    {
        m_nNumBoxes = src.m_nNumBoxes;
        CMemoryMgr::Free(m_pBoxes);
        m_pBoxes = nullptr;

        if (src.m_nNumBoxes)
            m_pBoxes = static_cast<CColBox*>(CMemoryMgr::Malloc(m_nNumBoxes * sizeof(CColBox)));
    }
    for (auto i = 0; i < m_nNumBoxes; ++i)
        m_pBoxes[i] = src.m_pBoxes[i];

// ----- TRIANGLES & VERTICES -----
    if (m_nNumTriangles != src.m_nNumTriangles || !src.m_nNumTriangles)
    {
        m_nNumTriangles = src.m_nNumTriangles;
        CMemoryMgr::Free(m_pTriangles);
        CMemoryMgr::Free(m_pVertices);
        m_pTriangles = nullptr;
        m_pVertices = nullptr;

        if (src.m_nNumTriangles)
        {
            // Get number of vertices
            unsigned short iVertsCount = 0;
            for (auto i = 0; i < src.m_nNumTriangles; ++i)
                iVertsCount = std::max({iVertsCount,
                                        src.m_pTriangles[i].m_nVertA,
                                        src.m_pTriangles[i].m_nVertB,
                                        src.m_pTriangles[i].m_nVertC});

            if (iVertsCount)
                m_pVertices = static_cast<CompressedVector*>(CMemoryMgr::Malloc((iVertsCount + 1) * sizeof(CompressedVector)));

            for (auto i = 0; i < iVertsCount; ++i)
                m_pVertices[i] = src.m_pVertices[i];

            m_pTriangles = static_cast<CColTriangle*>(CMemoryMgr::Malloc(m_nNumTriangles * sizeof(CColTriangle)));
        }
    }
    for (auto i = 0; i < m_nNumTriangles; ++i)
        m_pTriangles[i] = src.m_pTriangles[i];

// ----- SHADOW TRIANGLES & SHADOW VERTICES -----
    if (m_nNumShadowTriangles != src.m_nNumShadowTriangles || !src.m_nNumShadowTriangles || bHasShadowInfo != src.bHasShadowInfo)
    {
        bHasShadowInfo = src.bHasShadowInfo;
        m_nNumShadowTriangles = src.m_nNumShadowTriangles;
        m_nNumShadowVertices = src.m_nNumShadowVertices;
        CMemoryMgr::Free(m_pShadowTriangles);
        CMemoryMgr::Free(m_pShadowVertices);
        m_pShadowTriangles = nullptr;
        m_pShadowVertices = nullptr;

        if (src.m_nNumShadowTriangles)
        {
            // Get number of vertices
            unsigned short iVertsCount = 0;
            for (auto i = 0; i < src.m_nNumShadowTriangles; ++i)
                iVertsCount = std::max({ iVertsCount,
                                        src.m_pShadowTriangles[i].m_nVertA,
                                        src.m_pShadowTriangles[i].m_nVertB,
                                        src.m_pShadowTriangles[i].m_nVertC });

            if (iVertsCount)
                m_pShadowVertices = static_cast<CompressedVector*>(CMemoryMgr::Malloc((iVertsCount + 1) * sizeof(CompressedVector)));

            for (auto i = 0; i < iVertsCount; ++i)
                m_pShadowVertices[i] = src.m_pShadowVertices[i];

            m_pShadowTriangles = static_cast<CColTriangle*>(CMemoryMgr::Malloc(m_nNumShadowTriangles * sizeof(CColTriangle)));
        }
    }
    for (auto i = 0; i < m_nNumShadowTriangles; ++i)
        m_pShadowTriangles[i] = src.m_pShadowTriangles[i];
}

void CCollisionData::CalculateTrianglePlanes()
{
    plugin::CallMethod<0x40F590, CCollisionData*>(this);
}

void CCollisionData::RemoveTrianglePlanes()
{
    plugin::CallMethod<0x40F6A0, CCollisionData*>(this);
}
