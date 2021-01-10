#include "StdInc.h"

void C2dEffect::InjectHooks()
{
    ReversibleHooks::Install("C2dEffect", "Roadsign_GetNumLinesFromFlags", 0x6FA640, &C2dEffect::Roadsign_GetNumLinesFromFlags);
    ReversibleHooks::Install("C2dEffect", "Roadsign_GetNumLettersFromFlags", 0x6FA670, &C2dEffect::Roadsign_GetNumLettersFromFlags);
    ReversibleHooks::Install("C2dEffect", "Roadsign_GetPaletteIDFromFlags", 0x6FA6A0, &C2dEffect::Roadsign_GetPaletteIDFromFlags);
}

int C2dEffect::Roadsign_GetNumLinesFromFlags(CRoadsignAttrFlags flags)
{
    if (!flags.m_nNumOfLines)
        return 4;

    return flags.m_nNumOfLines;
}

int C2dEffect::Roadsign_GetNumLettersFromFlags(CRoadsignAttrFlags flags)
{
    if (!flags.m_nSymbolsPerLine)
        return 16;

    switch (flags.m_nSymbolsPerLine) {
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 8;
    }
}

int C2dEffect::Roadsign_GetPaletteIDFromFlags(CRoadsignAttrFlags flags)
{
    return flags.m_nTextColor;
}
