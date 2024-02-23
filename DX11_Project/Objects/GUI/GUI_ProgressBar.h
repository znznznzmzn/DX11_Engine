#pragma once
class GUI_ProgressBar : public GUI_Basic {
private:
	ValueBuffer_Float* vBuffer;
	Texture* textureBackground = nullptr;
	ColorBuffer* cBufferBG;
	void Set();
public:
	GUI_ProgressBar(wstring front_img_path, wstring back_img_path = L"");
	~GUI_ProgressBar();

	void PostRender();
	void GUIRender();

	void SetValue(float value);
	float4& Color_BG() { return cBufferBG->data.color; }
};