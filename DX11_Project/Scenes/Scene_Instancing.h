#pragma once

class Scene_Instancing : public Scene {
private:
	//const UINT COUNT = 1000;
	//ModelAnimator* model;
	//vector<Matrix> instanceData;
	//VertexBuffer* instanceBuffer;
	ModelAnimatorInstancing* modelInstancing;

	int idx = 0;
	int clip_idx = 0;
public:
	Scene_Instancing();
	~Scene_Instancing();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

};