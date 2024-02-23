#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangent input) {
	PixelInput output;
	output.pos = mul(input.pos, world);
	
	output.viewDir = normalize(output.pos.xyz - inverse_view._41_42_43);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.uv  = input.uv;
	output.normal   = mul(input.normal, (float3x3) world);
	output.tangent  = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	return output;
}

Texture2D alphaMap : register(t10);
Texture2D secondDiffuseMap : register(t11);
Texture2D thirdDiffuseMap  : register(t12); 

Texture2D secondSpecularMap : register(t13);
Texture2D thirdSpecularMap  : register(t14); 

Texture2D secondNormalMap : register(t15);
Texture2D thirdNormalMap  : register(t16);

float4 PS(PixelInput input) : SV_TARGET {
    float4 alpha = alphaMap.Sample(samp, 1 - input.uv);
	
    float4 albedo = { 1.0f, 1.0f, 1.0f, 1.0f };
    if (hasDiffuse) {
        albedo = diffuseMap.Sample(samp, input.uv);
        float4 secondDiffuse = secondDiffuseMap.Sample(samp, input.uv);
        float4 thirdDiffuse  = thirdDiffuseMap .Sample(samp, input.uv);
        albedo = lerp(albedo, secondDiffuse, alpha.r);
        albedo = lerp(albedo, thirdDiffuse , alpha.g);
    }
	
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);
	
	float3 light = normalize(light_direction);
	
	float3 normal = N;
	if (hasNormal) {
		float3x3 TBN = float3x3(T, B, N);
		float4 normalMapping = normalMap.Sample(samp, input.uv);
		
        float4 secondNormal = secondNormalMap.Sample(samp, input.uv);
        float4 thirdNormal  = thirdNormalMap .Sample(samp, input.uv);
        normalMapping = lerp(normalMapping, secondNormal, alpha.r);
        normalMapping = lerp(normalMapping, thirdNormal , alpha.g);
		
		normal = normalMapping.rgb * 2.0f - 1.0f;
		normal = normalize(mul(normal, TBN));
	}
	
	float diffuse = saturate(dot(normal, -light));
	
	float4 specular = 0.0f;
	if (diffuse > 0.0f) {
		float3 reflection = normalize(reflect(light, normal));
        specular = saturate(dot(-reflection, input.viewDir));
        float4 specularIntensity = float4(0.0f, 0.0f, 0.0f, 0.0f);
        if (hasSpecular) {
            specularIntensity = specularMap.Sample(samp, input.uv);
			
            float4 secondSpecular = secondSpecularMap.Sample(samp, input.uv);
            float4 thirdSpecular  = thirdSpecularMap .Sample(samp, input.uv);
            specularIntensity = lerp(specularIntensity, secondSpecular, alpha.r);
            specularIntensity = lerp(specularIntensity, thirdSpecular , alpha.g);
        }
        specular = pow(specular, mShininess) * specularIntensity;
    }
	
	float4 diffuseColor = albedo * diffuse * mDiffuse;
	float4 ambientColor = albedo * ambient * mAmbient;
	float4 specularColor = specular * mSpecular;
	return diffuseColor + specularColor + ambientColor;
}