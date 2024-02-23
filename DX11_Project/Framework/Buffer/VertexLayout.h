#pragma once

struct Vertex {
	float3 pos;
	Vertex() : pos(0, 0, 0) { }
	Vertex(float3 pos) : pos(pos) { }
	Vertex(float x, float y, float z) : pos(x, y, z) { }
};

struct VertexColor {
	float3 pos;
	float4 color;
	VertexColor() : pos(0, 0, 0), color(1, 1, 1, 1) { }
	VertexColor(float x, float y, float z) : pos(x, y, z), color(1, 1, 1, 1) { }
	VertexColor(float x, float y, float z, float r, float g, float b) : pos(x, y, z), color(r, g, b, 1) { }
	VertexColor(float x, float y, float z, float4 color) : pos(x, y, z), color(color) { }
	VertexColor(float3 pos, float4 color) : pos(pos), color(color) { }
};

struct VertexUV {
	float3 pos;
	float2 uv;
	VertexUV() : pos(0, 0, 0), uv(0, 0) { }
	VertexUV(float x, float y, float z, float u, float v) : pos(x, y, z), uv(u, v) { }
};

struct VertexUVNormal {
	float3 pos;
	float2 uv;
	float3 normal;
	VertexUVNormal() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0) { }
	VertexUVNormal(float3 pos, float2 uv, float3 normal) : pos(pos), uv(uv), normal(normal) { }
	VertexUVNormal(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), uv(u, v), normal(nx, ny, nz) { }
};

struct VertexUVNormalTangent {
	float3 pos;
	float2 uv;
	float3 normal;
	float3 tangent;
	VertexUVNormalTangent() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0) { }
	VertexUVNormalTangent(float3 pos) : pos(pos), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0) { }
	VertexUVNormalTangent(float3 pos, float2 uv)
		: pos(pos), uv(uv), normal(0, 0, 0), tangent(0, 0, 0) {}
	VertexUVNormalTangent(float3 pos, float2 uv, float3 normal, float3 tangent) 
		: pos(pos), uv(uv), normal(normal), tangent(tangent) { }
	VertexUVNormalTangent(float x, float y, float z, float u, float v)
		: pos(x, y, z), uv(u, v), normal(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f) { }
	VertexUVNormalTangent(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: pos(x, y, z), uv(u, v), normal(nx, ny, nz), tangent(0.0f, 0.0f, 0.0f) { }
	VertexUVNormalTangent(float x, float y, float z, float u, float v, 
		float nx, float ny, float nz, float tx, float ty, float tz)
		: pos(x, y, z), uv(u, v), normal(nx, ny, nz), tangent(tx, ty, tz) { }
};

struct VertexUVNormalTangentBlend {
	float3 pos;
	float2 uv;
	float3 normal;
	float3 tangent;
	float4 indices;
	float4 weights;
	VertexUVNormalTangentBlend() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0),
		indices(0, 0, 0, 0), weights(0, 0, 0, 0) { }
};

struct VertexSize {
	float3 pos;
	float2 size;
	VertexSize() : pos(0, 0, 0), size(0, 0) { }
};