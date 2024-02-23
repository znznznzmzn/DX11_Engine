#pragma once

class GUI_Basic : public Transform {
private:
	typedef VertexUV VertexType;
	vector<VertexType> vertices;
	vector<UINT> indices;

	Mesh* mesh;
	MatrixBuffer* wBuffer;
	ColorBuffer* cBuffer;

	VertexShader* vShader = nullptr;
	PixelShader* pShader = nullptr;
protected:
	void init(wstring shader_path = L"Shaders/GUI/Basic.hlsl");
	Texture* texture = nullptr;
public:
	GUI_Basic(wstring image_path = L"Texture/Basic.png");
	~GUI_Basic();

	void SetTexture(wstring texture_path);
	void SetShader(wstring file);

	virtual void PostRender(); // 2D GUI Render
	virtual void GUIRender();
	float4& Color() { return cBuffer->data.color; }
};