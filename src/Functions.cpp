#include "Functions.h"

namespace MaxsuBlockSpark
{

	bool SparkLocalizer::GetShieldSparkPos(const RE::NiPoint3& hitPos, RE::NiAVObject* shieldNode, RE::NiPoint3& result)
	{
		if (!shieldNode)
			return false;

		const auto centerPos = shieldNode->worldBound.center;
		const auto matrix = shieldNode->world.rotate;
		const auto radiusY = shieldNode->worldBound.radius * 0.8f;

		RE::NiMatrix3 invMatrix;
		InverseMatrix(invMatrix.entry, matrix.entry);

		const auto localVector = invMatrix * (hitPos - centerPos);

		result = centerPos + matrix * RE::NiPoint3(0.f, std::clamp(localVector.y, -radiusY, radiusY), std::clamp(localVector.z, -10.f, 0.f) -10.f);

		return true;
	}

}