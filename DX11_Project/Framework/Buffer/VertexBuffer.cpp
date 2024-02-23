#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count) : stride(stride), offset(0) {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = stride * count;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = data;
	DEVICE->CreateBuffer(&bufferDesc, &resource, &buffer_vertex);
}

VertexBuffer::~VertexBuffer() { SAFE_RELEASE(buffer_vertex); }

void VertexBuffer::IASet(D3D11_PRIMITIVE_TOPOLOGY type, UINT slot) {
	DC->IASetVertexBuffers(slot, 1, &buffer_vertex, &stride, &offset);
	DC->IASetPrimitiveTopology(type);
}

void VertexBuffer::Update(void* data, UINT count) { 
	DC->UpdateSubresource(buffer_vertex, 0, nullptr, data, stride, count); 
}
