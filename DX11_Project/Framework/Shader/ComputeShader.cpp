#include "Framework.h"

ComputeShader::ComputeShader(wstring file) {
	this->file = file;
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"CS", "cs_5_0", flags, 0, &blob, nullptr);
	DEVICE->CreateComputeShader
		(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader_compute);
}

ComputeShader::~ComputeShader() {
	SAFE_RELEASE(shader_compute);
	SAFE_RELEASE(blob);
}

void ComputeShader::Set() { DC->CSSetShader(shader_compute, nullptr, 0); }