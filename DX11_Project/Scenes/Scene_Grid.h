#pragma once
class Scene_Grid : public Scene {
public:
	Scene_Grid();
	~Scene_Grid();
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	Grid* grid;
};