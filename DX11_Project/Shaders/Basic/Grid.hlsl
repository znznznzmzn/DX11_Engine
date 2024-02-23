// Vertex Shader
// 
// sementic = 매개변수 및 반환값의 정보를 어떻게 정하는지 이름을 붇이는 것
// SV = System Value -> RS로 넘기라는 의미

cbuffer WorldBuffer		 : register(b0) { matrix world; }
cbuffer ViewBuffer		 : register(b1)	{ matrix view;  }
cbuffer ProjectionBuffer : register(b2) { matrix projection; }

struct VertexInput {
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PixelInput {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PixelInput VS(VertexInput input) {
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.color = input.color;
	return output;
}
float4 PS(PixelInput input) : SV_TARGET { return input.color; }
