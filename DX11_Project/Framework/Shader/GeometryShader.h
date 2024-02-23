#pragma once
class GeometryShader : public Shader{
public:
	GeometryShader(wstring file);
	~GeometryShader();
	void Set();
private:
	ID3D11GeometryShader* shader_geometry;
	ID3DBlob* blob;
};