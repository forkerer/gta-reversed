#include "StdInc.h"

void CScriptsForBrains::RequestAttractorScriptBrainWithThisName(char const* name)
{
    plugin::CallMethod<0x46AA80, CScriptsForBrains*, char const*>(this, name);
}
