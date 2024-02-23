#pragma once

class Scene_ModelRender : public Scene {
private:
	Model* model;
public:
	Scene_ModelRender();
	~Scene_ModelRender();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};