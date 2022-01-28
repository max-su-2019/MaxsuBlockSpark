#include "Functions.h"

namespace MaxsuBlockSpark
{

	bool SparkLocalizer::GetShieldSparkPos(const RE::NiPoint3& hitPos, RE::NiAVObject* shieldNode, RE::NiPoint3& result)
	{
		if (!shieldNode)
			return false;
		
		auto GetShieldShapeExtent = [](RE::NiAVObject* shieldNode, RE::NiPoint3& centerPos, float& z_extent, float& y_extent) -> bool {

			RE::bhkNiCollisionObject* shieldCollied = nullptr;
			if (shieldNode->collisionObject)
				shieldCollied = shieldNode->collisionObject->AsBhkNiCollisionObject();
			else {
				auto childNode = shieldNode->GetObjectByName("SHIELD");
				shieldCollied = childNode && childNode->collisionObject ? childNode->collisionObject->AsBhkNiCollisionObject() : nullptr;
			}

			if (!shieldCollied)
				return false;
			
			RE::bhkRigidBody* bhkShieldRigid = shieldCollied->body.get() ? shieldCollied->body.get()->AsBhkRigidBody() : nullptr;
			RE::hkpRigidBody* hkpShieldRigid = bhkShieldRigid ? skyrim_cast<RE::hkpRigidBody*>(bhkShieldRigid->referencedObject.get()) : nullptr;
			if (bhkShieldRigid && hkpShieldRigid) {

				centerPos = shieldNode->worldBound.center;
				RE::bhkRigidBodyT* bhkShieldRigidT = skyrim_cast<RE::bhkRigidBodyT*>(bhkShieldRigid);
				if (bhkShieldRigidT) {
					float bodyTrans[4];
					_mm_store_ps(bodyTrans, bhkShieldRigidT->translation.quad);
					const auto matrix = shieldNode->world.rotate;
					centerPos += matrix * RE::NiPoint3(bodyTrans[0], bodyTrans[1], bodyTrans[2]) * RE::bhkWorld::GetWorldScaleInverse();
				}
		
				const RE::hkpShape* shieldShape = hkpShieldRigid->collidable.GetShape();
				if (shieldShape) {
					float upExtent = shieldShape->GetMaximumProjection(RE::hkVector4{ 0.0f,0.0f,1.0f,0.0f }) * RE::bhkWorld::GetWorldScaleInverse();
					float downExtent = shieldShape->GetMaximumProjection(RE::hkVector4{ 0.0f,0.0f,-1.0f,0.0f }) * RE::bhkWorld::GetWorldScaleInverse();
					z_extent = upExtent + downExtent;

					float forwardExtent = shieldShape->GetMaximumProjection(RE::hkVector4{ 0.0f,1.0f,0.0f,0.0f }) * RE::bhkWorld::GetWorldScaleInverse();
					float backwardExtent = shieldShape->GetMaximumProjection(RE::hkVector4{ 0.0f,-1.0f,0.0f,0.0f }) * RE::bhkWorld::GetWorldScaleInverse();
					y_extent = (forwardExtent + backwardExtent) / 2.0f;

					return true;
				}
			}

			return false;
		};

		RE::NiPoint3 centerPos;
		float z_extent, y_extent;

		if (!GetShieldShapeExtent(shieldNode, centerPos, z_extent, y_extent)) {
			logger::warn("Not Shield Physical Value Found!");
			return false;
		}

		const auto matrix = shieldNode->world.rotate;
		RE::NiMatrix3 invMatrix;
		InverseMatrix(invMatrix.entry, matrix.entry);
		const auto localVector = invMatrix * (hitPos - centerPos);

		result = centerPos + matrix * RE::NiPoint3(0.f, std::clamp(localVector.y, -y_extent * shieldNode->world.scale, y_extent * shieldNode->world.scale), min(-z_extent, -10.f) * shieldNode->world.scale);

		return true;
	}

}