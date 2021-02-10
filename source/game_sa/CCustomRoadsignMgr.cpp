#include "StdInc.h"

RwTexture*& CCustomRoadsignMgr::pCharsetTex = *(RwTexture**)0xC3EF84;
RwUInt8*& CCustomRoadsignMgr::pCharsetLockedRaster = *(RwUInt8**)0xC3EF88;
RwUInt8*& CCustomRoadsignMgr::pCharsetLockedPallete = *(RwUInt8**)0xC3EF8C;

void CCustomRoadsignMgr::InjectHooks()
{
    ReversibleHooks::Install("CCustomRoadsignMgr", "Initialise", 0x6FE120, &CCustomRoadsignMgr::Initialise);
    ReversibleHooks::Install("CCustomRoadsignMgr", "Shutdown", 0x6FE180, &CCustomRoadsignMgr::Shutdown);
    ReversibleHooks::Install("CCustomRoadsignMgr", "CreateRoadsignTexture", 0x6FECA0, &CCustomRoadsignMgr::CreateRoadsignTexture);
    ReversibleHooks::Install("CCustomRoadsignMgr", "CreateRoadsignAtomicA", 0x6FEDA0, &CCustomRoadsignMgr::CreateRoadsignAtomicA);
    ReversibleHooks::Install("CCustomRoadsignMgr", "CreateRoadsignAtomic", 0x6FF2D0, &CCustomRoadsignMgr::CreateRoadsignAtomic);
    ReversibleHooks::Install("CCustomRoadsignMgr", "RenderRoadsignAtomic", 0x6FF350, &CCustomRoadsignMgr::RenderRoadsignAtomic);
    ReversibleHooks::Install("CCustomRoadsignMgr", "SetupRoadsignAtomic", 0x6FED60, &CCustomRoadsignMgr::SetupRoadsignAtomic);
    ReversibleHooks::Install("CCustomRoadsignMgr", "SetAtomicAlpha", 0x6FE240, &CCustomRoadsignMgr::SetAtomicAlpha);
}

bool CCustomRoadsignMgr::Initialise()
{
    const auto iTxdSlot = CTxdStore::FindTxdSlot("particle");
    CTxdStore::PushCurrentTxd();
    CTxdStore::SetCurrentTxd(iTxdSlot);

    auto* pTexture = RwTextureRead("roadsignfont", nullptr);
    RwTextureSetFilterMode(pTexture, rwFILTERNEAREST);
    RwTextureSetAddressing(pTexture, rwTEXTUREADDRESSCLAMP);
    CCustomRoadsignMgr::pCharsetTex = pTexture;

    CTxdStore::PopCurrentTxd();
    CCustomRoadsignMgr::pCharsetLockedRaster = RwRasterLock(RwTextureGetRaster(pTexture), 0, rwRASTERLOCKREAD);
    return true;
}

void CCustomRoadsignMgr::Shutdown()
{
    if (!CCustomRoadsignMgr::pCharsetTex)
        return;

    auto* pRaster = RwTextureGetRaster(CCustomRoadsignMgr::pCharsetTex);

    if (CCustomRoadsignMgr::pCharsetLockedPallete)
    {
        RwRasterUnlockPalette(pRaster);
        CCustomRoadsignMgr::pCharsetLockedPallete = nullptr;
    }

    if(CCustomRoadsignMgr::pCharsetLockedRaster)
    {
        RwRasterUnlock(pRaster);
        CCustomRoadsignMgr::pCharsetLockedRaster = nullptr;
    }

    RwTextureDestroy(CCustomRoadsignMgr::pCharsetTex);
    CCustomRoadsignMgr::pCharsetTex = nullptr;
}

RwTexture* CCustomRoadsignMgr::CreateRoadsignTexture(char* pName, int numOfChars)
{
    auto* pRaster = RwRasterCreate(8 * numOfChars, 16, 32, rwRASTERFORMAT8888 | rwRASTERPIXELLOCKEDWRITE);
    assert(pRaster); //TODO: Remove if crash cause is found
    if (!pRaster)
        return nullptr;

    auto* pCharsetRaster = RwTextureGetRaster(CCustomRoadsignMgr::pCharsetTex);
    assert(pCharsetRaster); //TODO: Remove if crash cause is found
    if (!pCharsetRaster)
    {
        RwRasterDestroy(pRaster);
        return nullptr;
    }

    const auto bGenerated = RoadsignGenerateTextRaster(pName, numOfChars, pCharsetRaster, 0, pRaster);
    assert(bGenerated); //TODO: Remove if crash cause is found
    if (!bGenerated)
    {
        RwRasterDestroy(pRaster);
        return nullptr;
    }

    auto* pTexture = RwTextureCreate(pRaster);
    assert(pTexture); //TODO: Remove if crash cause is found
    if (!pTexture)
    {
        RwRasterDestroy(pRaster);
        return nullptr;
    }

    char nameCopy[12] {};
    strncpy(nameCopy, pName, 10);
    RwTextureSetName(pTexture, nameCopy);
    RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);
    return pTexture;
}

RwTexture* CCustomRoadsignMgr::SetupRoadsignAtomic(RpAtomic* pAtomic, char* pName, int numOfChars)
{
    auto* pTexture = CCustomRoadsignMgr::CreateRoadsignTexture(pName, numOfChars);
    if (!pTexture)
        return nullptr;

    RpGeometryForAllMaterials(RpAtomicGetGeometry(pAtomic), RoadsignSetMaterialTextureCB, pTexture);
    return reinterpret_cast<RwTexture*>(pAtomic); //BUG? This method isn't used anywhere anyways
}

RpAtomic* CCustomRoadsignMgr::SetAtomicAlpha(RpAtomic* pAtomic, unsigned char alpha)
{
    RpGeometryForAllMaterials(RpAtomicGetGeometry(pAtomic), RoadsignSetMaterialAlphaCB, reinterpret_cast<void*>(alpha));
    return pAtomic;
}

RpAtomic* CCustomRoadsignMgr::CreateRoadsignAtomicA(float xScale, float yScale, signed int numLines, char* pLine1, char* pLine2, char* pLine3, char* pLine4, int lettersPerLine, unsigned char ucPallete)
{
    char* apLines[]{ pLine1, pLine2, pLine3, pLine4 };
    RpMaterial* apMaterials[4]{};

    bool bFailed = false;
    for (auto ind = 0; ind < numLines; ++ind)
    {
        auto* pMaterial = RpMaterialCreate();
        assert(pMaterial); //TODO: Remove if crash cause is found
        if (!pMaterial)
        {
            bFailed = true;
            break;
        }

        auto color = CRGBA();
        color.a = 255u;
        if (ucPallete == 1)
            color.Set(0u, 0u, 0u);
        else if (ucPallete == 2)
            color.Set(128u, 128u, 128u);
        else if (ucPallete == 3)
            color.Set(255u, 0u, 0u);
        else
            color.Set(255u, 255u, 255u);

        auto rwColor = color.ToRwRGBA();
        RpMaterialSetColor(pMaterial, &rwColor);

        auto* pTexture = CCustomRoadsignMgr::CreateRoadsignTexture(apLines[ind], lettersPerLine);
        assert(pTexture); //TODO: Remove if crash cause is found
        if (!pTexture)
        {
            bFailed = true;
            break;
        }

        RpMaterialSetTexture(pMaterial, pTexture);
        RwTextureDestroy(pTexture); // decrement refcounter so the material is sole owner of the texture
        apMaterials[ind] = pMaterial;

        auto surfProps = RwSurfaceProperties{ 0.3F, 0.3F, 0.7F };
        RpMaterialSetSurfaceProperties(pMaterial, &surfProps);
    }

    if (!bFailed)
    {
        do
        {
            auto* pGeometry = RpGeometryCreate(4 * numLines, 2 * numLines, rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED | rpGEOMETRYPOSITIONS);
            if (!pGeometry)
                break; // Go to cleanup

            auto* pMorphTarget = RpGeometryGetMorphTarget(pGeometry, 0);
            const auto fLineY = yScale / static_cast<float>(numLines);
            const auto fNegHalfX = xScale * -0.5F;
            for (auto iLine = 0 ; iLine < numLines; ++iLine)
            {
                auto* pVerts = &RpMorphTargetGetVertices(pMorphTarget)[iLine * 4];
                //TODO/BUG? This seems like base init of values, that is overriden completely just a bit later
                /*pVerts[0] = { 0.0F, 0.0F, 0.0F };
                pVerts[1] = { xScale, 0.0F, 0.0F };
                pVerts[2] = { xScale, fLineY * 0.95F, 0.0F };
                pVerts[3] = { 0.0F, fLineY * 0.95F , 0.0F };*/ 

                auto fNewY = 0.0F;
                switch(numLines)
                {
                case 1:
                    fNewY = fLineY * -0.5F;
                    break;

                case 2:
                    if (iLine == 0)
                        fNewY = 0.0F;
                    else if (iLine == 1)
                        fNewY = fLineY * -1.0F;
                    break;

                case 3:
                    if (iLine == 0)
                        fNewY = fLineY * 0.5F;
                    else if (iLine == 1)
                        fNewY = fLineY * -0.5F;
                    else if (iLine == 2)
                        fNewY = fLineY * -1.5F;
                    break;

                case 4:
                    if (iLine == 0)
                        fNewY = fLineY;
                    else if (iLine == 1)
                        fNewY = 0.0F;
                    else if (iLine == 2)
                        fNewY = fLineY * -1.0F;
                    else if (iLine == 3)
                        fNewY = fLineY * -2.0F;
                    break;
                }

                // Adjust the verts based on the above calculations
                pVerts[0] = { fNegHalfX, fNewY, 0.0F };
                pVerts[1] = { fNegHalfX + xScale, fNewY, 0.0F };
                pVerts[2] = { fNegHalfX + xScale, fNewY + fLineY * 0.95F, 0.0F };
                pVerts[3] = { fNegHalfX, fNewY + fLineY * 0.95F, 0.0F };
            }

            for (auto iLine = 0; iLine < numLines; ++iLine)
            {
                auto* pTexCoords = &RpGeometryGetVertexTexCoords(pGeometry, 1)[iLine * 4];
                pTexCoords[0] = { 0.0F, 1.0F };
                pTexCoords[1] = { 1.0F, 1.0F };
                pTexCoords[2] = { 1.0F, 0.0F };
                pTexCoords[3] = { 0.0F, 0.0F };
            }

            for (auto iLine = 0; iLine < numLines; ++iLine)
            {
                auto* pColors = &RpGeometryGetPreLightColors(pGeometry)[iLine * 4];
                pColors[0] = { 255u, 255u, 255u, 255u };
                pColors[1] = { 255u, 255u, 255u, 255u };
                pColors[2] = { 255u, 255u, 255u, 255u };
                pColors[3] = { 255u, 255u, 255u, 255u };
            }

            for (auto iLine = 0; iLine < numLines; ++iLine)
            {
                const auto iVert = iLine * 4;
                auto* pTriangles = &RpGeometryGetTriangles(pGeometry)[iLine * 2];
                RpGeometryTriangleSetVertexIndices(pGeometry, &pTriangles[0], iVert, iVert + 1, iVert + 2);
                RpGeometryTriangleSetMaterial(pGeometry, &pTriangles[0], apMaterials[iLine]);
                RpGeometryTriangleSetVertexIndices(pGeometry, &pTriangles[1], iVert, iVert + 2, iVert + 3);
                RpGeometryTriangleSetMaterial(pGeometry, &pTriangles[1], apMaterials[iLine]);

                RpMaterialDestroy(apMaterials[iLine]);
                apMaterials[iLine] = nullptr;
            }

            RwSphere bndSphere;
            RpMorphTargetCalcBoundingSphere(pMorphTarget, &bndSphere);
            RpMorphTargetSetBoundingSphere(pMorphTarget, &bndSphere);
            RpGeometryUnlock(pGeometry);
            auto* pAtomic = RpAtomicCreate();
            if (!pAtomic)
            {
                RpGeometryDestroy(pGeometry);
                break; // Go to cleanup
            }

            if (!RpAtomicSetGeometry(pAtomic, pGeometry, 0))
            {
                RpGeometryDestroy(pGeometry);
                RpAtomicDestroy(pAtomic);
                break; // Go to cleanup
            }

            RpGeometryDestroy(pGeometry);
            RpAtomicSetFlags(pAtomic, rpATOMICRENDER);
            auto* pFrame = RwFrameCreate();
            RwFrameSetIdentity(pFrame);
            RpAtomicSetFrame(pAtomic, pFrame);
            return pAtomic;

        } while (false); // So we can easily break out of the logic to destroy materials, would be nice to make it cleaner
    }


    for(auto& pMaterial : apMaterials)
        if (pMaterial)
            RpMaterialDestroy(pMaterial);

    return nullptr;
}

RpAtomic* CCustomRoadsignMgr::CreateRoadsignAtomic(float xScale, float yScale, signed int numLines, char* pLine1, char* pLine2, char* pLine3, char* pLine4, int lettersPerLine, unsigned char ucPallete)
{
    char dummy = ' ';
    auto* usedLine1 = pLine1 ? pLine1 : &dummy;
    auto* usedLine2 = pLine2 ? pLine2 : &dummy;
    auto* usedLine3 = pLine3 ? pLine3 : &dummy;
    auto* usedLine4 = pLine4 ? pLine4 : &dummy;

    auto* pAtomic = CCustomRoadsignMgr::CreateRoadsignAtomicA(xScale, yScale, numLines, usedLine1, usedLine2, usedLine3, usedLine4, lettersPerLine, ucPallete);
    assert(pAtomic); //TODO: Remove if crash cause is found
    return pAtomic;


}

RpAtomic* CCustomRoadsignMgr::RenderRoadsignAtomic(RpAtomic* pAtomic, CVector const& vecPos)
{
    auto* pFrame = RpAtomicGetFrame(pAtomic);
    auto vecDist = vecPos - *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
    const auto fDistSquared = vecDist.SquaredMagnitude();
    if (fDistSquared > 250000.0F) // 500 units away
        return pAtomic;

    if (fDistSquared >= 1600.0F) { // Fade out above 40 units away
        const auto fFade =  1.0F - invLerp(1600, 250000, fDistSquared);
        const auto ucAlpha = static_cast<RwUInt8>(lerp(0.0F, 254.0F, fFade));
        CCustomRoadsignMgr::SetAtomicAlpha(pAtomic, ucAlpha);
        RpAtomicRender(pAtomic);
        return pAtomic;
    }

    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)100u);
    CCustomRoadsignMgr::SetAtomicAlpha(pAtomic, 255u);
    RpAtomicRender(pAtomic);
    return pAtomic;

}

RpMaterial* RoadsignSetMaterialAlphaCB(RpMaterial* material, void* data)
{
    RpMaterialGetColor(material)->alpha = reinterpret_cast<RwUInt8>(data);
    return material;
}

RpMaterial* RoadsignSetMaterialTextureCB(RpMaterial* material, void* data)
{
    RpMaterialSetTexture(material, static_cast<RwTexture*>(data));
    return material;
}

bool RoadsignGenerateTextRaster(char* roadName, int numLetters, RwRaster* charsetRaster, int unused, RwRaster* signRaster)
{
    return plugin::CallAndReturn<bool, 0x6FEB70, char*, int, RwRaster*, int, RwRaster*>
        (roadName, numLetters, charsetRaster, unused, signRaster);
}
