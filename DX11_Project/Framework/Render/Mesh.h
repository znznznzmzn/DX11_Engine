#pragma once

class Mesh {
private:
	VertexBuffer* vBuffer;
	IndexBuffer* iBuffer;
public:
	Mesh(void* vertexData, UINT vertexStride, UINT vertexCount,
		void* indexData, UINT indexCount);
	~Mesh();
	void IASet(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void UpdateVertex(void* data, UINT count);
};