#include "Framework.h"

ConstantBuffer::ConstantBuffer(void* data, UINT dataSize) : data(data), dataSize(dataSize) {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = dataSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer_constant);
}
ConstantBuffer::~ConstantBuffer() { SAFE_RELEASE(buffer_constant); }
void ConstantBuffer::Update() { 
	DC->Map(buffer_constant, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); 
	memcpy(subResource.pData, data, dataSize);
	DC->Unmap(buffer_constant, 0);
}

void ConstantBuffer::SetVSBuffer(UINT slot) {
	Update();
	DC->VSSetConstantBuffers(slot, 1, &buffer_constant);
}

void ConstantBuffer::SetPSBuffer(UINT slot) {
	Update();
	DC->PSSetConstantBuffers(slot, 1, &buffer_constant);
}

void ConstantBuffer::SetCSBuffer(UINT slot) {
	Update();
	DC->CSSetConstantBuffers(slot, 1, &buffer_constant);
}

void ConstantBuffer::SetGSBuffer(UINT slot) {
	Update();
	DC->GSSetConstantBuffers(slot, 1, &buffer_constant);
}
