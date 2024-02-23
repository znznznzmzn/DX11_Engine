#pragma once

class Scene_Animation : public Scene {
private:
	ModelAnimator* model;
public:
	Scene_Animation();
	~Scene_Animation();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};