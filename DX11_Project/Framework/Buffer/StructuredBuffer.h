#pragma once
class StructuredBuffer {
private:
	ID3D11Resource* input; // CPU -> intput
	ID3D11ShaderResourceView* srv;

	ID3D11Resource* output;	// GPU -> output
	ID3D11UnorderedAccessView* uav;

	ID3D11Resource* result; // result -> CPU

	void* inputData;
	UINT inputStride, inputCount; 
	UINT outputStride, outputCount;

	void CreateInput();
	void CreateOutput();
	void CreateSRV();
	void CreateUAV();
	void CreateResult();

public:
	ID3D11UnorderedAccessView*& GetUAV() { return uav; }
	ID3D11ShaderResourceView*&  GetSRV() { return srv; }

	StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount, UINT outputStride, UINT outputCount);
	~StructuredBuffer();

	void Copy(void* data, UINT size);
	void UpdateInput(void* inputData);
};