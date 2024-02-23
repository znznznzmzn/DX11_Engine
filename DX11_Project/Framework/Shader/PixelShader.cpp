#include "Framework.h"

PixelShader::PixelShader(wstring file) {// PS
	this->file = file;
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"PS", "ps_5_0", flags, 0, &blob, nullptr);
	DEVICE->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader_pixel);
}

PixelShader::~PixelShader() { 
	SAFE_RELEASE(shader_pixel); 
	SAFE_RELEASE(blob);
}

void PixelShader::Set() { DC->PSSetShader(shader_pixel, nullptr, 0); }
