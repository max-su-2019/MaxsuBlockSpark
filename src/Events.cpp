#include "Events.h"
#include "Functions.h"

namespace MaxsuBlockSpark
{
	EventResult OnHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource)
	{
		using HitFlag = RE::TESHitEvent::Flag;
		using AttackState = RE::ATTACK_STATE_ENUM;

		if (!a_event || !a_eventSource) {
			logger::error("Event Source Not Found!");
			return EventResult::kContinue;
		}

		logger::debug("OnHit Event Trigger!");

		if (a_event->flags.any(HitFlag::kHitBlocked) && a_event->target) {
			auto defender = a_event->target->As<RE::Actor>();
			auto attackWeapon = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);
			if (!defender || !attackWeapon || !defender->currentProcess || !defender->currentProcess->high || !attackWeapon->IsMelee() || attackWeapon->IsHandToHandMelee() || !defender->Get3D())
				return EventResult::kContinue;

			auto attacker = a_event->cause ? a_event->cause->As<RE::Actor>(): nullptr;
			if (!attacker || !attacker->currentProcess || !attacker->currentProcess->high) {
				logger::debug("Attack Actor Not Found!");
				return EventResult::kContinue;
			}

			auto attackerData = attacker->currentProcess->high->attackData;
			if (!attackerData) {
				logger::debug("Attacker Attack Data Not Found!");
				return EventResult::kContinue;
			}
			 
			auto attackerNode = attackerData->IsLeftAttack() ? attacker->GetNodeByName("SHIELD") : attacker->GetNodeByName("WEAPON");
			if (!attackerNode) {
				logger::debug("Attacker Node Not Found!");
				return EventResult::kContinue;
			}

			auto GetBipeObjIndex = [](RE::TESForm* parryEquipment, bool rightHand) -> RE::BIPED_OBJECT {
				if (!parryEquipment)
					return RE::BIPED_OBJECT::kNone;

				if (parryEquipment->As<RE::TESObjectWEAP>() && rightHand) {
					switch (parryEquipment->As<RE::TESObjectWEAP>()->GetWeaponType()) {
					case RE::WEAPON_TYPE::kOneHandSword:
						return RE::BIPED_OBJECT::kOneHandSword;

					case RE::WEAPON_TYPE::kOneHandAxe:
						return RE::BIPED_OBJECT::kOneHandAxe;

					case RE::WEAPON_TYPE::kOneHandMace:
						return RE::BIPED_OBJECT::kOneHandMace;

					case RE::WEAPON_TYPE::kTwoHandAxe:
					case RE::WEAPON_TYPE::kTwoHandSword:
						return RE::BIPED_OBJECT::kTwoHandMelee;
					}
				} else
					return RE::BIPED_OBJECT::kShield;

				return RE::BIPED_OBJECT::kNone;
			};

			RE::BIPED_OBJECT BipeObjIndex;
			auto defenderLeftEquipped = defender->currentProcess->GetEquippedLeftHand();
			auto defenderData = defender->currentProcess->high->attackData;
			
			if (defenderData && defenderLeftEquipped) 
				BipeObjIndex = defenderData->IsLeftAttack() && (defenderLeftEquipped->IsWeapon() || defenderLeftEquipped->IsArmor()) ? GetBipeObjIndex(defenderLeftEquipped, false) : GetBipeObjIndex(defender->currentProcess->GetEquippedRightHand(), true);
			else 
				BipeObjIndex = defenderLeftEquipped && (defenderLeftEquipped->IsWeapon() || defenderLeftEquipped->IsArmor()) ? GetBipeObjIndex(defenderLeftEquipped, false) : GetBipeObjIndex(defender->currentProcess->GetEquippedRightHand(), true);

			if (BipeObjIndex == RE::BIPED_OBJECT::kNone) {
				logger::debug("BipeObj Not Found!");
				return EventResult::kContinue;
			}

			auto defenderNode = defender->GetCurrentBiped()->objects[BipeObjIndex].partClone;
			if (!defenderNode || !defenderNode->AsNode()) {
				logger::debug("Defender Node Not Found!");
				return EventResult::kContinue;
			}

			auto cell = defender->GetParentCell();

			const auto modelName = BipeObjIndex == RE::BIPED_OBJECT::kShield ? "Maxsu\\SimpleBlockSpark\\fxmetalsparkimpactshield.nif" : "Maxsu\\SimpleBlockSpark\\fxmetalsparkimpactweap.nif";

			RE::NiPoint3 sparkPos;
			RE::NiPoint3 hitPos = attackerNode->worldBound.center + attackerNode->world.rotate * RE::NiPoint3(0.f,0.5f * attackerNode->worldBound.radius,0.f);

			if (BipeObjIndex == RE::BIPED_OBJECT::kShield && defenderLeftEquipped && defenderLeftEquipped->IsArmor() && SparkLocalizer::GetShieldSparkPos(hitPos, defenderNode.get(), sparkPos))
				logger::debug("Get Shield Spark Position!");
			else {
				sparkPos = defenderNode->worldBound.center;
				logger::debug("Get Weapon Spark Position!");
			}

			if (cell->PlaceParticleEffect(0.0f, modelName, defenderNode->world.rotate, sparkPos, 1.0f, 4U, defenderNode.get())) {
				logger::debug("Play Spark Effect Successfully!");
				return EventResult::kContinue;
			}

			logger::debug("Play Spark Effect Fail!");
		}

		return EventResult::kContinue;
	}

}