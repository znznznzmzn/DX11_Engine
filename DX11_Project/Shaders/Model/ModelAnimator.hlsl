// ModelAnimator
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

matrix SkinWorld(float4 indices, float4 weights) {
	matrix transform = 0;
    matrix cur, next;
    matrix curAnim, nextAnim;
	
    float4 c[4];
    float4 n[4];
	
    for (int i = 0; i < 4; i++) {
        int curFrame = motion.cur.curFrame;
        int clip = motion.cur.clip;
		
        c[0] = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c[1] = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c[2] = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c[3] = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        cur = matrix(c[0], c[1], c[2], c[3]);
		
        n[0] = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
        n[1] = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
        n[2] = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
        n[3] = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
        next = matrix(n[0], n[1], n[2], n[3]);
		
        curAnim = lerp(cur, next, motion.cur.time);
		
		
        curFrame = motion.next.curFrame;
        clip = motion.next.clip;
        if (clip > -1) {
            c[0] = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
			c[1] = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
			c[2] = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
			c[3] = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
			cur = matrix(c[0], c[1], c[2], c[3]);
		
			n[0] = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
			n[1] = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
			n[2] = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
			n[3] = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
            next = matrix(n[0], n[1], n[2], n[3]);
            nextAnim = lerp(cur, next, motion.next.time);
			
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
        transform += mul(weights[i], curAnim);
    }
	
    return mul(transform, world);
}

PixelInput VS(VertexUVNormalTangentBlend input) {
	PixelInput output;
    matrix transform = SkinWorld(input.indices, input.weights);
    output.pos = mul(input.pos, transform);
    output.viewDir = normalize(output.pos.xyz - inverse_view._41_42_43);
    output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.uv  = input.uv;
	output.normal   = mul(input.normal, (float3x3) transform);
	output.tangent  = mul(input.tangent, (float3x3) transform);
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
    result += Lighting_Point(albedo, specularIntensity, normal, input.viewDir, input.worldPos);
	
    return result + ambientColor;
}