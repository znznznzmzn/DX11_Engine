#pragma once

class VertexShader : public Shader{
public:
	VertexShader(wstring file);
	~VertexShader();
	void Set();
private:
	void CreateInputLayout();
private:
	ID3D11VertexShader * shader_vertex;
	ID3D11InputLayout  * layout_input;
	ID3DBlob * blob;
	ID3D11ShaderReflection * reflection;
};
