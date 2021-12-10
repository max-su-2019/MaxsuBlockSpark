#include "LoadGame.h"
#include "DataHandler.h"
#include "SimpleIni.h"
#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\MaxsuBlockSpark.ini"

namespace MaxsuBlockSpark
{
	DataHandler::DataHandler()
	{
		CSimpleIniA ini;
		ini.LoadFile(SETTINGFILE_PATH);

		std::uint32_t  triggerChance = ini.GetLongValue("Main", "TriggerChance", 100);
		bool		   checkMaterial = ini.GetBoolValue("Main", "CheckMaterial");

		settings = std::make_unique<INIData>(std::clamp<std::uint32_t>(triggerChance,0,100), checkMaterial);

		if (ini.GetBoolValue("Main", "EnableDebugLog")) {
			spdlog::set_level(spdlog::level::debug);
			logger::debug("Enable Debug Log!");
		}
		
		OnHitEventHandler::RegisterOnHitEvent();
	}
	
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
			INFO("Data Load CallBack Trigger!");
			DataHandler::GetSingleton();
		}
	}

}