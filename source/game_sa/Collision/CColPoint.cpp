#include "StdInc.h"

float tColLighting::GetCurrentLighting() const
{
	const auto fDay = static_cast<float>(day) / 30.0F;
	const auto fNight = static_cast<float>(night) / 30.0F;
	return lerp(fDay, fNight, CCustomBuildingDNPipeline::m_fDNBalanceParam);
}
