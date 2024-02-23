#pragma once

class MatrixBuffer : public ConstantBuffer {
private: struct Data { Matrix matrix; } data;
public:
	MatrixBuffer() : ConstantBuffer(&data, sizeof(Data)) { data.matrix = XMMatrixIdentity(); }
	void Set(Matrix value) { data.matrix = XMMatrixTranspose(value); }
};

class ViewBuffer : public ConstantBuffer {
private: struct Data { Matrix view; Matrix inverse_view; } data; //- 바이트 맞추기
public:
	ViewBuffer() : ConstantBuffer(&data, sizeof(Data)) { 
		data.view = XMMatrixIdentity();
		data.inverse_view = XMMatrixIdentity();
	}
	void Set(Matrix view, Matrix inverse_view) {
		data.view = XMMatrixTranspose(view);
		data.inverse_view = XMMatrixTranspose(inverse_view);
	}
};

class LightBuffer : public ConstantBuffer {
private: struct Data { 
	float3 light_direction; 
	float shininess;
	float4 ambient;
	float4 light_color;
} data;
public:
	LightBuffer() : ConstantBuffer(&data, sizeof(Data)) { 
		data.light_direction = { 0.0f, -1.0f, 1.0f }; 
		data.shininess = 24.0f;
		data.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		data.light_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	float3& GetLightDirection() { return data.light_direction; }
	float& GetShininess() { return data.shininess; }
	float4& GetAmbient() { return data.ambient; }
	float4& GetLightColor() { return data.light_color; }
};

class MaterialBuffer : public ConstantBuffer {
public:
	struct Data {
		float4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		float4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		float4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		float shininess = 24.0f;
		int hasDiffuse = false;
		int hasSpecular = false;
		int hasNormal = false;
	} data;
	MaterialBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
};

class ColorBuffer : public ConstantBuffer {
public:
	struct Data { float4 color; } data;
	ColorBuffer() : ConstantBuffer(&data, sizeof(Data))
	{ data.color = { 1.0f, 1.0f, 1.0f, 1.0f }; }
};

class ValueBuffer_Float : public ConstantBuffer {
public:
	float values[4] = {};
	ValueBuffer_Float() : ConstantBuffer(values, sizeof(float) * 4) { }
};

/*
class MaterialBuffer : public ConstantBuffer {
private: struct Data {
	float4 diffuse;
	float4 specular;
	float4 ambient;
	int hasDiffuse;
	int hasSpecular;
	int hasNormal;
	int padding;
} data;
public:
	MaterialBuffer() : ConstantBuffer(&data, sizeof(Data)) {
		data.diffuse =  { 1.0f, 1.0f, 1.0f, 1.0f };
		data.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		data.ambient =  { 1.0f, 1.0f, 1.0f, 1.0f };
		data.hasDiffuse  = true;
		data.hasSpecular = true;
		data.hasNormal   = true;
		data.padding    = 0;
	}
	float4& GetDiffuse () { return data.diffuse;  }
	float4& GetSpecular() { return data.specular; }
	float4& GetAmbient () { return data.ambient;  }

	int& IsDiffuse()  { return data.hasDiffuse;  }
	int& IsSpecular() { return data.hasSpecular; }
	int& IsNormal()   { return data.hasNormal;   }
};
*/

