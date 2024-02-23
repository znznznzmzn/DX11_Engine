#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexOutput {
	float3 pos : POSITION;
	float2 size : SIZE;
	float2 distance : DISTANCE;
	float4 color : COLOR;
	float3 velocity : VELOCITY;
};

struct PixelInput {
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float4 color : COLOR;
	float alpha : ALPHA;
};

cbuffer RainBuffer : register(b10) {
	float3 velocity;
	float drawDistance;
	
	float4 rainColor;
	
	float3 origin;
	float time;
	
	float3 size;
}
VertexOutput VS(VertexSize input) {
	VertexOutput output;
	
	float3 v = velocity;
	v.xz /= input.size.y * 0.1f;
	output.velocity = v;
	
	float3 displace = time * v;
	input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);
	
	float alpha = cos(time + (input.pos.x + input.pos.z));
	alpha = saturate(1.5f + alpha / drawDistance * 2.0f);
	
	output.distance.x = drawDistance;
	output.distance.y = alpha;
	
    output.color = rainColor;
	
	output.pos = input.pos;
	output.size = input.size;
	
	return output;
}

static const float2 TEXCOORD[4] = {
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> output) {
	float3 up = normalize(-input[0].velocity);
	float3 forward = input[0].pos - inverse_view._41_42_43;
	forward = normalize(forward);
	float3 right = normalize(cross(up, forward));
	
	float2 halfSize = input[0].size * 0.5f;
	
	float4 vertices[4];
	vertices[0] = float4(input[0].pos - halfSize.x * right - halfSize.y * up, 1.0f);
	vertices[1] = float4(input[0].pos - halfSize.x * right + halfSize.y * up, 1.0f);
	vertices[2] = float4(input[0].pos + halfSize.x * right - halfSize.y * up, 1.0f);
	vertices[3] = float4(input[0].pos + halfSize.x * right + halfSize.y * up, 1.0f);

	PixelInput pixelInput;
	pixelInput.color = input[0].color;
	
	for (int i = 0; i < 4; i++) {
		pixelInput.pos = mul(vertices[i], view);
		pixelInput.pos = mul(pixelInput.pos, projection);
		
		pixelInput.uv = TEXCOORD[i];
		
		pixelInput.alpha = 0.2f * saturate(1 - pixelInput.pos.z / input[0].distance.x)
		 * input[0].distance.y;
		
		output.Append(pixelInput);
	}
}

float4 PS(PixelInput input) : SV_TARGET {
	float4 result = diffuseMap.Sample(samp, input.uv) * mDiffuse;
	
	result.rgb *= input.color.rgb * (1 + input.alpha) * 2.0f;
	result.a *= input.alpha;
	
	return result;
}