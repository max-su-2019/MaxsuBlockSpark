#pragma once

namespace MaxsuBlockSpark
{
	using EventResult = RE::BSEventNotifyControl;

	class OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
	{
	public:
		virtual EventResult ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource);

		static bool RegisterOnHitEvent()
		{
			static OnHitEventHandler g_hiteventhandler;

			auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

			if (!ScriptEventSource) {
				logger::error("ScriptEventSource not found!");
				return false;
			}

			ScriptEventSource->AddEventSink(&g_hiteventhandler);

			logger::info("Register OnHit Event Handler!");

			return true;
		}

	private:
		OnHitEventHandler() = default;

		~OnHitEventHandler() = default;

		OnHitEventHandler(const OnHitEventHandler&) = delete;

		OnHitEventHandler(OnHitEventHandler&&) = delete;

		OnHitEventHandler& operator=(const OnHitEventHandler&) = delete;

		OnHitEventHandler& operator=(OnHitEventHandler&&) = delete;
	};

}