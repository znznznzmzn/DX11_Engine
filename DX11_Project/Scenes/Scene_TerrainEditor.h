#pragma once
class Scene_TerrainEditor : public Scene {
private:
	TerrainEditor * te;
	//Robot * robot;
	Vector3 pickedPos;
	Vector3 cameraOffset = { 0.0f, 25.0f, -20.0f}; 

	//void ByteAddress();
public:
	Scene_TerrainEditor();
	~Scene_TerrainEditor();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};