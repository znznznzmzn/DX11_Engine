#pragma once

class VertexBuffer {
public:
	VertexBuffer(void* data, UINT stride, UINT count);
	~VertexBuffer();

	void IASet(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, UINT slot = 0);
	void Update(void* data, UINT count);
private:
	ID3D11Buffer* buffer_vertex;
	UINT stride;
	UINT offset;
};