#include "Functions.h"

namespace MaxsuBlockSpark
{

	bool SparkLocalizer::GetShieldSparkPos(const RE::NiPoint3& hitPos, RE::NiAVObject* shieldNode, RE::NiPoint3& result)
	{
		if (!shieldNode)
			return false;
		
		auto GetShieldShapeExtent = [](RE::NiAVObject* shieldNode) -> std::optional<float> {
			std::optional<float> result;

			RE::bhkNiCollisionObject* shieldCollied = nullptr;
			if (shieldNode->collisionObject)
				shieldCollied = shieldNode->collisionObject->AsBhkNiCollisionObject();
			else {
				auto childNode = shieldNode->GetObjectByName("SHIELD");
				shieldCollied = childNode && childNode->collisionObject ? childNode->collisionObject->AsBhkNiCollisionObject() : nullptr;
			}
			
			RE::hkpRigidBody* shieldRigid = shieldCollied && shieldCollied->body.get() ? skyrim_cast<RE::hkpRigidBody*>(shieldCollied->body.get()->referencedObject.get()) : nullptr;
			const RE::hkpShape* shieldShape = shieldRigid ? shieldRigid->collidable.GetShape() : nullptr;
			if (shieldShape) {
				float upExtent = shieldShape->GetMaximumProjection(RE::hkVector4{ 0.0f,0.0f,1.0f,0.0f }) * RE::bhkWorld::GetWorldScaleInverse();
				float downExtent = shieldShape->GetMaximumProjection(RE::hkVector4{ 0.0f,0.0f,-1.0f,0.0f }) * RE::bhkWorld::GetWorldScaleInverse();
				result.emplace(upExtent + downExtent);
			}
			
			return result;
		};

		auto shieldShapeExtent = GetShieldShapeExtent(shieldNode);
		if (!shieldShapeExtent.has_value()) {
			logger::warn("Not Shield hkpShape Found!");
			return false;
		}

		const float z_offset = min(-shieldShapeExtent.value(), -10.f) * shieldNode->world.scale;
		logger::debug("Z offset is {}", z_offset);

		const auto centerPos = shieldNode->worldBound.center;
		const auto matrix = shieldNode->world.rotate;
		const auto radiusY = shieldNode->worldBound.radius * 0.8f;
		
		RE::NiMatrix3 invMatrix;
		InverseMatrix(invMatrix.entry, matrix.entry);
		const auto localVector = invMatrix * (hitPos - centerPos) * shieldNode->world.scale;

		result = centerPos + matrix * RE::NiPoint3(0.f, std::clamp(localVector.y, -radiusY, radiusY), z_offset);

		return true;
	}

}