#include "Framework.h"

GUI_ProgressBar::GUI_ProgressBar(wstring front_img_path, wstring back_img_path) : 
	GUI_Basic(front_img_path) {
	if(back_img_path.length() > 0)
		textureBackground = Texture::Add(back_img_path);
	SetShader(L"Shaders/GUI/ProgressBar.hlsl");
	vBuffer = new ValueBuffer_Float();
	vBuffer->values[0] = 1.0f;
	cBufferBG = new ColorBuffer();
}

GUI_ProgressBar::~GUI_ProgressBar() { 
	SAFE_DELETE(vBuffer); 
	SAFE_DELETE(cBufferBG);
}

void GUI_ProgressBar::Set() {
	if (!Active()) return;
	vBuffer->SetPSBuffer(10);
	cBufferBG->SetPSBuffer(11);
	if (textureBackground) textureBackground->PSSet(10);
}

void GUI_ProgressBar::PostRender() {
	Set();
	GUI_Basic::PostRender();
}

void GUI_ProgressBar::GUIRender() {
	GUI_Basic::GUIRender();
	ImGui::ColorEdit4("BG_Color", (float*)&cBufferBG->data.color);
	if (EditorGUI::ImageButton_Dialog_LoadTexture("BG_Texture", "Load BG Texture", textureBackground))
		SetTexture(textureBackground->GetPath());
	ImGui::SameLine();
	if (ImGui::Button("BX", { 24, 24 })) textureBackground = Texture::Add(L"");
	ImGui::SliderFloat("value", &vBuffer->values[0], 0.0f, 1.0f);
}

void GUI_ProgressBar::SetValue(float value) { 
	vBuffer->values[0] = value; 
	UpdateWorld();
}
