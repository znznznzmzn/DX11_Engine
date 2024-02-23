#include "Framework.h"

GUI_FPS_CrossHair::GUI_FPS_CrossHair(wstring Base_Image, wstring Rebound_Image)
	: GUI_Basic(Base_Image) {
	Scale() = { Size, Size, 0.0f };
	Position() = { CENTER_X, CENTER_Y, 0.0f };
	UpdateWorld();

	ReboundCross = new GUI_Basic(Rebound_Image);
	ReboundCross->Scale() = { Size, Size, 0.0f };
	ReboundCross->Position() = { CENTER_X, CENTER_Y, 0.0f };
	ReboundCross->UpdateWorld();
}
GUI_FPS_CrossHair::~GUI_FPS_CrossHair() {
	SAFE_DELETE(ReboundCross);
}

void GUI_FPS_CrossHair::PostRender() {
	GUI_Basic::PostRender();
	ReboundCross->PostRender();
}
void GUI_FPS_CrossHair::GUIRender() {
	GUI_Basic::GUIRender();
}

void GUI_FPS_CrossHair::SetSize(float const& Size) {
	this->Size = Size;
	Scale() = { Size, Size, 0.0f };
}
void GUI_FPS_CrossHair::SetReboundSize(float const& Size) {
	ReboundCross->Scale() = { Size, Size, 0.0f };
	ReboundCross->UpdateWorld();
}
