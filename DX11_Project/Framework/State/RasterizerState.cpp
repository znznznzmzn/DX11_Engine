#include "Framework.h"

RasterizerState::RasterizerState() {
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.MultisampleEnable = true;
	Changed();
}

RasterizerState::~RasterizerState() {
	SAFE_RELEASE(state);
}

void RasterizerState::SetState() {
	DC->RSSetState(state);
}

void RasterizerState::FillMode(D3D11_FILL_MODE value) {
	desc.FillMode = value;
	Changed();
}

void RasterizerState::FrontCountClockwise(bool value) {
	desc.FrontCounterClockwise = value;
	Changed();
}

void RasterizerState::Changed() {
	SAFE_RELEASE(state);
	DEVICE->CreateRasterizerState(&desc, &state);
}
