#pragma once

class ConstantBuffer {
protected:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();
	void Update();
public:
	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);
	void SetGSBuffer(UINT slot);
private:
	ID3D11Buffer* buffer_constant = nullptr;

	void* data;
	UINT dataSize = 0;

	D3D11_MAPPED_SUBRESOURCE subResource = {};
};