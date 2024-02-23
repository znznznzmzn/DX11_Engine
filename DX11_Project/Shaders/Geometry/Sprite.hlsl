#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput{    
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

VertexSize VS(VertexSize input) {
    VertexSize output;
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
void GS(point VertexSize input[1], inout TriangleStream<PixelInput> output) {
    float3 up = inverse_view._21_22_23;
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
	
    for (int i = 0; i < 4; i++) {
        pixelInput.pos = mul(vertices[i], view);
        pixelInput.pos = mul(pixelInput.pos, projection);
		
        pixelInput.uv = TEXCOORD[i];
		
        output.Append(pixelInput);
    }
}

cbuffer SpriteBuffer : register(b10) {
    float2 maxFrame;
    float2 curFrame;
}

float4 PS(PixelInput input) : SV_TARGET {
    float2 uv = (input.uv / maxFrame) + (curFrame / maxFrame);
    return diffuseMap.Sample(samp, uv) * mDiffuse;
}