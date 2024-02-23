#pragma once

struct Status {
	float MaxHP;
	float HP;

	float MaxSP;
	float SP;
};

namespace Gloval{
	class DataManager : public Singleton<DataManager> {
	private:
		void LoadData();
		void SaveData();

	public:
		DataManager();
		~DataManager();

	};
}