#include "Functions.h"

namespace MaxsuBlockSpark
{

	bool SparkLocalizer::GetShieldSparkPos(const RE::NiPoint3& hitPos, RE::NiAVObject* shieldNode, RE::NiPoint3& result)
	{
		if (!shieldNode)
			return false;

		auto const centerPos = shieldNode->worldBound.center;
		auto const matrix = shieldNode->world.rotate;
		auto const radius = shieldNode->worldBound.radius * 0.75f;

		result = centerPos + matrix * RE::NiPoint3(0.f, std::clamp(hitPos.z - centerPos.z, -radius, radius) , -10.0f);

		return true;
	}

}