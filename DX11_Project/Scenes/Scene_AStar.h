#pragma once

class Scene_AStar : public Scene {
private:
	Terrain* t;
	AStar* aStar;
	Robot* r;
public:
	Scene_AStar();
	~Scene_AStar();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};