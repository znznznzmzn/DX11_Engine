#pragma once
class SamplerState {
private:
	D3D11_SAMPLER_DESC desc = {};
	ID3D11SamplerState* state = nullptr;
public:
	SamplerState();
	~SamplerState();

	void PSSetState(UINT slot = 0);
	void Filter(D3D11_FILTER value);
	void Changed();
};