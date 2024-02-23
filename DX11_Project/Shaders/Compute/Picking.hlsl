//Picking.hlsl
cbuffer RayBuffer : register(b0) {
	float3 position;
    float triangleSize;

	float3 direction;
}

struct InputDesc {
	float3 v0;
	float3 v1;
	float3 v2;
};
StructuredBuffer<InputDesc> input : register(t0);

struct OutputDesc{
	int isPicked;
    float distance;
};
RWStructuredBuffer<OutputDesc> output : register(u0);

void Intersection(uint index) {
    float3 A = input[index].v0;
    float3 B = input[index].v1;
    float3 C = input[index].v2;
	
    float3 e1 = B - A;
    float3 e2 = C - A;
	
    float3 P, T, Q;
    P = cross(direction, e2);
    float d = 1.0f / dot(e1, P);
    float u, v;
    T = position - A;
    u = dot(T, P) * d;
    Q = cross(T, e1);
    v = dot(direction, Q) * d;
    output[index].distance = dot(e2, Q) * d;
    bool Picked = (u >= 0.0f) && (v >= 0.0f) && 
                  (u + v <= 1.0f) && (output[index].distance >= 0.0f);
    output[index].isPicked = Picked ? 1 : 0;
}

[numthreads(64, 1, 1)]
void CS(uint3 DTid : SV_DispatchThreadID) {
    uint index = DTid.x;
    if (triangleSize > index) Intersection(index);
}