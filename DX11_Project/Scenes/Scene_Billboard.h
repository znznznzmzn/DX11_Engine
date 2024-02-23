#pragma once
class Scene_Billboard : public Scene {
private:
	Terrain* t;
	//vector<Quad*> trees;
	const UINT COUNT = 1000;

	Material* mat;
	VertexBuffer* vBuffer;
	GeometryShader* gShader;

	DepthStencilState* depthState[2];
public:
	Scene_Billboard();
	~Scene_Billboard();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void GUIRender() override;
};