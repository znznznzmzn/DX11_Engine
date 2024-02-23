#pragma once
class Scene_Light : public Scene{
private: 
	Cube* obj_1;
	Sphere* obj_2;
	Cylinder* obj_3;

	SpotLight* spight;
public:

	Scene_Light();
	~Scene_Light();
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};