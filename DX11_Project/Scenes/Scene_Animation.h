#pragma once

class Scene_Animation : public Scene {
private:
	ModelAnimator* model;
public:
	Scene_Animation();
	~Scene_Animation();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};