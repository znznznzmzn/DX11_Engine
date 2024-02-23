#pragma once

class BlendState {
private:
	D3D11_BLEND_DESC desc = {};
	ID3D11BlendState* state = nullptr;
public:
	BlendState();
	~BlendState();

	void SetState();

	void Alpha(bool const& value);
	void Additive();
	void AlphaToCoverage(bool const& value);

	void Changed();
};