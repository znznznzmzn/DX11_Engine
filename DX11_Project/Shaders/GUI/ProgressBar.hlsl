#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput {
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input) {
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.uv = input.uv;
    return output;
}

cbuffer ValueBuffer : register(b10) { float value; }
cbuffer ColorBuffer_Background : register(b11) { float4 color_backround; }
Texture2D backImage : register(t10);

float4 PS(PixelInput input) : SV_TARGET {
    if (input.uv.x < value) return diffuseMap.Sample(samp, input.uv) * color;
    else return backImage.Sample(samp, input.uv) * color_backround;
}