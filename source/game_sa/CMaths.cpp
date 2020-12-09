#include "StdInc.h"

float* CMaths::ms_SinTable = (float*)0x59ACB6;

void CMaths::InjectHooks()
{
    ReversibleHooks::Install("CMaths", "InitMathsTables", 0x59AC90, &CMaths::InitMathsTables);
}

void CMaths::InitMathsTables()
{
    if (!ReversibleHooks::Hooked("CMaths", __func__)) {
        return plugin::Call<0x59AC90>();
    }

    for (int i = 0; i < 256; ++i)
        CMaths::ms_SinTable[i] = sin(static_cast<float>(i) * PI / 128.0F);
}
