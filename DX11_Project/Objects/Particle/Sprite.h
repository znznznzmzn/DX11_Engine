#pragma once

class Sprite : public Particle {
private:
	class SpriteBuffer : public ConstantBuffer {
	public:
		struct Data {
			float2 maxFrame = { 0, 0 };
			float2 curFrame = { 0, 0 };
		} data;
		SpriteBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	} *spriteBuffer;
	VertexSize* vertices;

	float time = 0.0f;
	float speed = 1.0f;

	UINT curFrameCount;
	UINT maxFrameCount;

	float2 size;

	void Create();
public:
	Sprite(wstring file, float2 maxFrame, float2 size = { 1, 1 }, bool isAdditive = true);
	~Sprite();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

	virtual void Play(Vector3 position) override;
	virtual void SetAdditive(bool isAdditive = true);
};