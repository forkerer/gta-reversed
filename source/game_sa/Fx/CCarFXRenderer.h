#pragma once

class CCarFXRenderer
{
public:
    static bool IsCBPCPipelineAttached(RpAtomic* pAtomic);
    static void SetCustomFXAtomicRenderPipelinesVMICB(RpAtomic* pAtomic, void* data);
    static float SetFxEnvMapLightMult(float multiplier);
};
