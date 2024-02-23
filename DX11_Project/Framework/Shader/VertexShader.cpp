#include "Framework.h"

VertexShader::VertexShader(wstring file) {
	this->file = file;
	const DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	// VS
	D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0", flags, 0, &blob, nullptr);
	DEVICE->CreateVertexShader(blob->GetBufferPointer(),
		blob->GetBufferSize(), nullptr, &shader_vertex);
	CreateInputLayout(); // IA
}

VertexShader::~VertexShader() {
	SAFE_RELEASE(shader_vertex);
	SAFE_RELEASE(layout_input);
	SAFE_RELEASE(blob);
	SAFE_RELEASE(reflection);
}

void VertexShader::Set() {
	DC->IASetInputLayout(layout_input);
	DC->VSSetShader(shader_vertex, nullptr, 0);
}

void VertexShader::CreateInputLayout() { // IA
	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), 
		IID_ID3D11ShaderReflection, (void**)&reflection);
	
	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);
	
	vector<D3D11_INPUT_ELEMENT_DESC> layouts;
	for (UINT i = 0; i < shaderDesc.InputParameters; i++) {
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);
		D3D11_INPUT_ELEMENT_DESC elemDesc;
		elemDesc.SemanticName = paramDesc.SemanticName;
		elemDesc.SemanticIndex = paramDesc.SemanticIndex;
		elemDesc.InputSlot = 0;
		elemDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elemDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elemDesc.InstanceDataStepRate = 0;
		if (paramDesc.Mask < 2) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elemDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elemDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elemDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask < 4) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elemDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elemDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elemDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask < 8) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elemDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elemDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elemDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask < 16) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elemDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elemDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elemDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		string tmp = paramDesc.SemanticName;
		if (tmp == "POSITION")
			elemDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		tmp = tmp.substr(0, tmp.find_first_of("_"));
		if (tmp == "INSTANCE") {
			elemDesc.InputSlot = 1;
			elemDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elemDesc.InstanceDataStepRate = 1;
		}

		layouts.push_back(elemDesc);
	}

	DEVICE->CreateInputLayout(layouts.data(), static_cast<UINT>(layouts.size()),
		blob->GetBufferPointer(), blob->GetBufferSize(), &layout_input);
}
