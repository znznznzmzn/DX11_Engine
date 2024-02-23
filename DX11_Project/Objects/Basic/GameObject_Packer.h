#pragma once

// ���� ������ GameObject ��� Object���� �ϳ��� ���ؽ��� �����ִ� Ŭ����
// ��õ���� �Ǵ� ���� ���� ������Ʈ���� �ϳ��� ���´�
class GameObject_Packer : public GameObject {
	vector<GameObject*> packs;
public:
	GameObject_Packer();
	~GameObject_Packer();
	void AddObject(GameObject* item) { packs.emplace_back(item); }
	void ClearPack();

	void Pack();
};