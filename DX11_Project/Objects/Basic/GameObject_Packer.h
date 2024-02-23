#pragma once

// 여러 종류의 GameObject 기반 Object들을 하나의 버텍스로 묶어주는 클래스
// 수천개가 되는 정적 게임 오브젝트들을 하나로 묶는다
class GameObject_Packer : public GameObject {
	vector<GameObject*> packs;
public:
	GameObject_Packer();
	~GameObject_Packer();
	void AddObject(GameObject* item) { packs.emplace_back(item); }
	void ClearPack();

	void Pack();
};