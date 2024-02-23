#pragma once

class Scene_ModelRender : public Scene {
private:
	Model* model;
public:
	Scene_ModelRender();
	~Scene_ModelRender();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};