#pragma once
class Particle : public Transform {
protected:
	const UINT MAX_COUNT = 1000;

	Material* material;
	GeometryShader* gShader;
	
	VertexBuffer* vBuffer;
	MatrixBuffer* wBuffer;

	UINT particleCount;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];
public:
	Particle();
	virtual ~Particle();
	virtual void Update() = 0;
	virtual void Render();
	virtual void GUIRender() = 0;

	virtual void Play(Vector3 pos);
	void Stop();

};