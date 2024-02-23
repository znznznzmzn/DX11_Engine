#include "Framework.h"

void StructuredBuffer::CreateInput() {
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC inputDesc = { };
	inputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = inputStride * inputCount;
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	inputDesc.StructureByteStride = inputStride;
	D3D11_SUBRESOURCE_DATA data = { };
	data.pSysMem = inputData;
	DEVICE->CreateBuffer(&inputDesc, &data, &buffer);
	input = (ID3D11Resource*)buffer;
}
void StructuredBuffer::CreateSRV() {
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Buffer.NumElements = inputCount;
	DEVICE->CreateShaderResourceView(input, &srvDesc, &srv);
}

void StructuredBuffer::CreateOutput() {
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = outputStride * outputCount;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = outputStride;

	DEVICE->CreateBuffer(&bufferDesc, nullptr, &buffer);

	output = (ID3D11Resource*)buffer;
}
void StructuredBuffer::CreateUAV() {
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = outputCount;
	DEVICE->CreateUnorderedAccessView(buffer, &uavDesc, &uav);
}

void StructuredBuffer::CreateResult() {
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

StructuredBuffer::StructuredBuffer(void* inputData,
	UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount) :
	inputData(inputData), 
	inputStride(inputStride), inputCount(inputCount),
	outputStride(outputStride), outputCount(outputCount) {
	CreateInput();
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}
StructuredBuffer::~StructuredBuffer() {
	SAFE_RELEASE(input);
	SAFE_RELEASE(srv);
	SAFE_RELEASE(output);
	SAFE_RELEASE(uav);
	SAFE_RELEASE(result);
}

void StructuredBuffer::Copy(void* data, UINT size) {
	DC->CopyResource(result, output);
	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(result, 0, D3D11_MAP_READ, 0, &subResource);
	memcpy(data, subResource.pData, size);
	DC->Unmap(result, 0);
}

void StructuredBuffer::UpdateInput(void* inputData) {
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(input, 0, D3D11_MAP_WRITE, 0, &subResource);
	memcpy(subResource.pData, inputData, inputStride * inputCount);
	DC->Unmap(input, 0);
}