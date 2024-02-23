#pragma once

class GUI_FPS_CrossHair : public GUI_Basic {
private:
	GUI_Basic* ReboundCross = nullptr; 
	float Size = 32.0f;
public:
	GUI_FPS_CrossHair(wstring Base_Image = L"Textures/GUI/CrossHair.png", 
		wstring Rebound_Image = L"Textures/GUI/CrossHair.png");
	~GUI_FPS_CrossHair();

	void PostRender();
	void GUIRender();

	void SetSize(float const& Size);
	void SetReboundSize(float const& Size);
};