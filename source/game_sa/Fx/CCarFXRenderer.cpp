#include "StdInc.h"

bool CCarFXRenderer::IsCBPCPipelineAttached(RpAtomic* pAtomic)
{
	return plugin::CallAndReturn<bool, 0x5D5B80, RpAtomic*>(pAtomic);
}

bool CCarFXRenderer::IsCCPCPipelineAttached(RpAtomic* pAtomic)
{
    return plugin::CallAndReturn<bool, 0x5D5B80, RpAtomic*>(pAtomic);
}

void CCarFXRenderer::CustomCarPipeAtomicSetup(RpAtomic* pAtomic)
{
    plugin::Call<0x5D5B20, RpAtomic*>(pAtomic);
}

void CCarFXRenderer::SetCustomFXAtomicRenderPipelinesVMICB(RpAtomic* pAtomic, void* data)
{
    plugin::Call<0x5D5B60, RpAtomic*, void*>(pAtomic, data);
}

float CCarFXRenderer::SetFxEnvMapLightMult(float multiplier)
{
    return plugin::CallAndReturn<float, 0x5D5BA0, float>(multiplier);
}
