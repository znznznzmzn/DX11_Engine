// ModelAnimator
#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput : VertexUVNormalTangentBlend {
    matrix transform : INSTANCE_TRANSFORM;
};

struct PixelInput {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;
};

matrix SkinWorld(int instanceID, float4 indices, float4 weights) {
	matrix transform = 0;
    matrix cur, next;
    matrix curAnim, nextAnim;
	
    float4 c[4];
    float4 n[4];
	
    for (int i = 0; i < 4; i++) {
        int curFrame = motions[instanceID].cur.curFrame;
        int clip = motions[instanceID].cur.clip;
		
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
		
        curAnim = lerp(cur, next, motions[instanceID].cur.time);
        curFrame = motions[instanceID].next.curFrame;
        clip = motions[instanceID].next.clip;
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
            nextAnim = lerp(cur, next, motions[instanceID].next.time);
			
            curAnim = lerp(curAnim, nextAnim, motions[instanceID].tweenTime);
        }
        transform += mul(weights[i], curAnim);
    }
	
    return transform;
}

PixelInput VS(VertexModelInstansing input) {
	PixelInput output;
    matrix transform = mul(SkinWorld(input.index, input.indices, input.weights), input.transform);
    
    output.pos = mul(input.pos, transform);
	
	output.viewDir = normalize(output.pos.xyz - inverse_view._41_42_43);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.uv  = input.uv;
	output.normal   = mul(input.normal , (float3x3) transform);
	output.tangent  = mul(input.tangent, (float3x3) transform);
	output.binormal = cross(output.normal, output.tangent);
	return output;
}

float4 PS(PixelInput input) : SV_TARGET {
	float4 albedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (hasDiffuse) albedo = diffuseMap.Sample(samp, input.uv);
	
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);
	
	float3 normal = N;
	if (hasNormal) {
		float3x3 TBN = float3x3(T, B, N);
		float4 normalMapping = normalMap.Sample(samp, input.uv);
		normal = normalMapping.rgb * 2.0f - 1.0f;
		normal = normalize(mul(normal, TBN));
	}
	
    float3 light = normalize(light_direction);
	float diffuse = saturate(dot(normal, -light));
	float4 specular = 0.0f;
	if (diffuse > 0.0f) {
		float3 reflection = normalize(reflect(light, normal));
		specular = saturate(dot(-reflection, input.viewDir));
		float4 specularIntensity = float4(0.0f, 0.0f, 0.0f, 0.0f);
		if (hasSpecular)
			specularIntensity = specularMap.Sample(samp, input.uv);
        specular = pow(specular, mShininess) * specularIntensity;
    }
	
	float4 diffuseColor = albedo * diffuse * mDiffuse;
	float4 ambientColor = albedo * ambient * mAmbient;
	float4 specularColor = specular * mSpecular;
	return diffuseColor + specularColor + ambientColor;
}