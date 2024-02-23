#pragma once

class Light : public Transform {
protected:
	MatrixBuffer* wBuffer = nullptr;
	
	bool IsGizmo = true;
	GUI_Basic* Bulb = nullptr;

	virtual void UpdateData() = 0;
public:
	virtual void Set() = 0;
	void Render() { Set(); }
	void PostRender() { if (IsGizmo) Bulb->PostRender(); }
	void OnGizmo(bool IsGizmo = true) { this->IsGizmo = IsGizmo; }
};