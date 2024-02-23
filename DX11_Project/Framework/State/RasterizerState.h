#pragma once
class RasterizerState {
private:
	D3D11_RASTERIZER_DESC desc = {};
	ID3D11RasterizerState* state = nullptr;
public:
	RasterizerState();
	~RasterizerState();

	void SetState();
	void FillMode(D3D11_FILL_MODE value);
	void FrontCountClockwise(bool value);
	void Changed();
};