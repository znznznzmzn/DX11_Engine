#include "Framework.h"

namespace Gloval {
	void DataManager::LoadData() {
		ifstream loadStream("TextData/Tables/MonsterTable.csv", ios::in);
		string str;

		//- Incoding Type Consider -> ANSI

		loadStream >> str; // �� ���� �а�
		while (!str.empty()) {
			vector<string> tmp = SplitString(str, ",", true); // ,�� �и��ؼ� tmp�� ����
			
			//--���⼭ �ε��ϰ���� �����ͺ��� ��ȯ
			// stoi, stof Ȱ��
			
			loadStream >> str; // �ٿ��� �б�
		}
	}
	void DataManager::SaveData() {
	}

	DataManager::DataManager() {
	}
	DataManager::~DataManager() {
	}
}
