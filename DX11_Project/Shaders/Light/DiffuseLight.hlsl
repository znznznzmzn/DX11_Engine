cbuffer WorldBuffer		 : register(b0) { matrix world; }
cbuffer ViewBuffer		 : register(b1)	{ matrix view;  }
cbuffer ProjectionBuffer : register(b2) { matrix projection; }
cbuffer LightBuffer      : register(b3) { float3 light_direction; }

struct VertexInput {
	float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInput {
	float4 pos : SV_POSITION;
    float2 uv : UV;
    float diffuse : DIFFUSE;
};

PixelInput VS(VertexInput input) {
	PixelInput output;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
    output.uv = input.uv;
	
    float3 normal = normalize(mul(input.normal, (float3x3)world));
	float3 light  = normalize(light_direction);
	
    output.diffuse = saturate(dot(normal, -light));
	
	return output;
}

cbuffer ColorBuffer : register(b0) { float4 color; };

Texture2D diffuseMap : register(t0);
SamplerState sample : register(s0);

float4 PS(PixelInput input) : SV_TARGET { 
    float4 albedo = diffuseMap.Sample(sample, input.uv);
    float4 diffuse = albedo * input.diffuse * color;
    float4 ambient = albedo * 0.1f;
    return diffuse + ambient;
}