#pragma once
namespace MaxsuBlockSpark
{
	class DataHandler
	{
		struct INIData
		{
			INIData() = delete;

			INIData(const INIData&) = delete;

			INIData(INIData&&) = delete;

			INIData& operator=(const INIData&) = delete;

			INIData& operator=(INIData&&) = delete;
			
			const std::uint32_t		triggerChance;
			const bool				checkMaterial;
			
			explicit INIData(const std::uint32_t a_triggerChance = 100, const bool a_checkMaterial = false) :
				triggerChance(a_triggerChance), checkMaterial(a_checkMaterial)
			{
				INFO(FMT_STRING("Set triggerChance is {}, checkMaterial is {}"),triggerChance, checkMaterial);
			}

		};

	public:
		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;
			return  std::addressof(singleton);
		}

	private:
		DataHandler();

		~DataHandler() = default;

		DataHandler(const DataHandler&) = delete;

		DataHandler(DataHandler&&) = delete;

		DataHandler& operator=(const DataHandler&) = delete;

		DataHandler& operator=(DataHandler&&) = delete;

	public:
		std::unique_ptr<INIData> settings;

	};


}