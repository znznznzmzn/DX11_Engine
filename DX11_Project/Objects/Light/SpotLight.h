#pragma once

class SpotLight : public Light {
private:
	class SpotLightBuffer : public ConstantBuffer {
	public:
		struct Data {
			float3 spotPosition;
			float  spotRange;
			float3 spotDirection;
			float spotRadius;
			float4 spotColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		} data;
		SpotLightBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	}*slBuffer = nullptr;

	void UpdateData() override;
public:
	SpotLight();
	~SpotLight();

	void Set() override;
	void UpdateWorld() override;
	void GUIRender() override;
};