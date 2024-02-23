#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormal input) {
	PixelInput output;
	output.pos = mul(input.pos, world);
	
	output.viewDir = normalize(output.pos.xyz - inverse_view._41_42_43);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.uv  = input.uv;
	output.normal = mul(input.normal, (float3x3)world);
	return output;
}

float4 PS(PixelInput input) : SV_TARGET {
	float4 albedo = diffuseMap.Sample(samp, input.uv);
	
	float3 normal = normalize(input.normal);
	float3 light = normalize(light_direction);
	
	float diffuse = saturate(dot(normal, -light));
	
	float4 specular = 0;
	if (diffuse > 0.0f) {
		float3 reflection = normalize(reflect(light, normal));
		specular = saturate(dot(-reflection, input.viewDir));
		float4 specularIntensity = specularMap.Sample(samp, input.uv);
        specular = pow(specular, mShininess) * specularIntensity;
    }
	
	float4 diffuseColor = albedo * diffuse;
	float4 ambientColor = albedo * ambient;
	float4 specularColor = specular;
	
	return diffuseColor + specularColor + ambientColor;
}