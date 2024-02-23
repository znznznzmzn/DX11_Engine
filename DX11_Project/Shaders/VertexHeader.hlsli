// Vertex Header
#define MAX_INSTANCE 128

// Vertex Constant Buffer
cbuffer WorldBuffer		 : register(b0) { matrix world; }
cbuffer ViewBuffer		 : register(b1)	{ 
	matrix view; 
	matrix inverse_view; 
}
cbuffer ProjectionBuffer : register(b2) { matrix projection; }

struct Frame {
    int clip;
    uint curFrame;
    float time;
    float speed;
};

struct Motion {
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    Frame cur;
    Frame next;
};

cbuffer FrameBuffer : register(b3) { Motion motion; }
cbuffer FrameInstancingBuffer : register(b4) { Motion motions[MAX_INSTANCE]; }

Texture2DArray transformMap : register(t0);

// Vertex Layout
struct Vertex { float4 pos : POSITION; };
struct VertexUV : Vertex { float2 uv : UV; };
struct VertexUVNormal : VertexUV { float3 normal : NORMAL; };
struct VertexUVNormalTangent : VertexUVNormal { float3 tangent : TANGENT; };
struct VertexUVNormalTangentBlend : VertexUVNormalTangent {
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
};

struct VertexModelInstansing : VertexUVNormalTangentBlend {
    matrix transform : INSTANCE_TRANSFORM;
    int index : INSTANCE_INDEX;
};

struct VertexSize { 
    float3 pos : POSITIONT;
    float2 size : SIZE; 
 };

