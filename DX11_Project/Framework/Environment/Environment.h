#pragma once
class Environment : public Singleton<Environment> {
private:
	void CreateViewport();
	void CreateProjection();
	void CreateState();

	Camera* main_camera;
	Tween * main_tween;

	Matrix projection;
	MatrixBuffer* perspectiveBuffer;
	MatrixBuffer* orthographicBuffer;
	MatrixBuffer* GUIBuffer;

	LightBuffer * lightBuffer;

	D3D11_VIEWPORT viewPort;

	SamplerState* samplerState;
	RasterizerState* rasterizerState;
	DepthStencilState* depthState[2];

	bool isWireMode = false;
	bool isClipMouse = false;
public:
	Environment();
	~Environment();

	void Update();
	void GUIRender();

	void Set();
	void PostSet();

	void LockMouse(bool isLock, RECT lockArea = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

	Matrix GetProjection() { return projection; }
	Camera*& GetMainCamera() { return main_camera; }
	Tween *& GetMainTween () { return main_tween; }
	LightBuffer*& GetLightData() { return lightBuffer; }
};