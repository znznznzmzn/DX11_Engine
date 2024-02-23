#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    float3 worldPos : WORLDPOS;
};

PixelInput VS(VertexUVNormalTangent input) {
	PixelInput output;
	output.pos = mul(input.pos, world);
	
    output.worldPos = output.pos;
    output.viewDir = normalize(output.pos.xyz - inverse_view._41_42_43);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.uv  = input.uv;
	output.normal   = mul(input.normal, (float3x3) world);
	output.tangent  = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	return output;
}

float4 PS(PixelInput input) : SV_TARGET {
	// diffuse
	float4 albedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    [flatten]
    if (hasDiffuse) albedo = diffuseMap.Sample(samp, input.uv);
	
	// Normal
    float3 normal = Mapping_Normal(input.tangent, input.binormal, input.normal, input.uv);
	
	// specular
    float4 specularIntensity = Mapping_Specular(input.uv) * mSpecular;
    float4 ambientColor = albedo * ambient * mAmbient;
	
	// Lightings
    float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);
    result += Lighting_Directional(albedo, specularIntensity, normal, input.viewDir);
    //result += Lighting_Point(albedo, specularIntensity, normal, input.viewDir, input.worldPos);
    result += Lighting_Spot(albedo, specularIntensity, normal, input.viewDir, input.worldPos);
	
    return result + ambientColor;
}