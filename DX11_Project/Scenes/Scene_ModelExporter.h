#pragma once

class Scene_ModelExporter : public Scene {
public:
	Scene_ModelExporter();
	~Scene_ModelExporter();
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};