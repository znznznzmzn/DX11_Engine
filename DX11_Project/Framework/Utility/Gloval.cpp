#include "Framework.h"

namespace Gloval {
	void DataManager::LoadData() {
		ifstream loadStream("TextData/Tables/MonsterTable.csv", ios::in);
		string str;

		//- Incoding Type Consider -> ANSI

		loadStream >> str; // 한 줄을 읽고
		while (!str.empty()) {
			vector<string> tmp = SplitString(str, ",", true); // ,로 분리해서 tmp에 저장
			
			//--여기서 로드하고싶은 데이터별로 변환
			// stoi, stof 활용
			
			loadStream >> str; // 다움줄 읽기
		}
	}
	void DataManager::SaveData() {
	}

	DataManager::DataManager() {
	}
	DataManager::~DataManager() {
	}
}
