#pragma once
class Material {
private:
	string file = "";
	string name = "Default";

	VertexShader* vShader = nullptr;
	PixelShader* pShader = nullptr;

	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;

	MaterialBuffer* mBuffer;

	void SelectShader();

public:
	Material();
	Material(wstring shaderFile);
	~Material();

	void Set();
	
	void GUIRender();

	void SetDiffuseMap (wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap  (wstring file);

	void SetShader(wstring file);

	void SetName(string name) { this->name = name; }
	string GetName() { return name; }

	MaterialBuffer::Data& GetData() { return mBuffer->data; }

	string GetPath() { return file; }
	wstring GetDiffusePath ();
	wstring GetSpecularPath();
	wstring GetNormalPath  ();

	void SaveMaterial(string path);
	void LoadMaterial(string path);
};