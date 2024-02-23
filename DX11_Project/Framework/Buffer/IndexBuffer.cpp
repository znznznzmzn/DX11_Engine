#include "Framework.h"

IndexBuffer::IndexBuffer(void* data, UINT count) {
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(UINT) * count;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource = {};
	resource.pSysMem = data;
	DEVICE->CreateBuffer(&bufferDesc, &resource, &buffer_index);
}
IndexBuffer::~IndexBuffer() { SAFE_RELEASE(buffer_index); }
void IndexBuffer::IASet() { DC->IASetIndexBuffer(buffer_index, DXGI_FORMAT_R32_UINT, 0); }
