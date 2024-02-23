#pragma once
class Spark : public Particle {
private:
	struct VertexInput {
		float3 pos = { 0.0f, 0.0f, 0.0f };
		float2 size = { 0.0f, 0.0f };
		float3 velocity = { 0.0f, 0.0f, 0.0f };
	} * vertices;

	class SparkBuffer : public ConstantBuffer {
	public:
		struct Data {
			float3 direction = { 0, 0, 0 };
			float duration = 1.0f;

			float time = 0.0f;
			float3 padding = { 0, 0, 0 };
		} data;
		SparkBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	}* sparkBuffer;
	ColorBuffer* startColorBuffer;
	ColorBuffer* endColorBuffer;

	float minRadius = 5.0f;
	float maxRadius = 10.0f;

	float minSize = 0.1f;
	float maxSize = 1.0f;

	void Create();
	void UpdateParticle();
public:
	Spark();
	~Spark();

	// Particle을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

	virtual void Play(Vector3 pos) override;
};