#include "Framework.h"

void RawBuffer::CreateOutput() {
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);

	output = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateUAV() {
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = byteWidth / 4;

	DEVICE->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
}

void RawBuffer::CreateResult() {
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC bufferDesc = { };
	((ID3D11Buffer*)output)->GetDesc(&bufferDesc);
	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);

	result = (ID3D11Resource*)buffer;
}

RawBuffer::RawBuffer(UINT byteWidth)  : byteWidth(byteWidth) {
	CreateOutput();
	CreateUAV();
	CreateResult();
}

RawBuffer::~RawBuffer() {
	SAFE_RELEASE(output);
	SAFE_RELEASE(uav);
	SAFE_RELEASE(result);
}

void RawBuffer::Copy(void* data, UINT size) {
	DC->CopyResource(result, output);
	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	memcpy(data, subResource.pData, size);
	DC->Unmap(result, 0);
}
