#pragma once

class PointLight : public Light {
private:
	class PointLightBuffer : public ConstantBuffer {
	public:
		struct Data {
			float3 pointPosition;
			float  pointRange;
			float4 pointColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		} data;
		PointLightBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	}*plBuffer = nullptr;

	void UpdateData() override;
public:
	PointLight();
	~PointLight();

	void Set() override;
	void UpdateWorld() override;
	void GUIRender() override;
};