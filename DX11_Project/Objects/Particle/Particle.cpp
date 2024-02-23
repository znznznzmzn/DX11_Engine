#include "Framework.h"

Particle::Particle() : particleCount(MAX_COUNT) {
	material = new Material();

	wBuffer = new MatrixBuffer();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

Particle::~Particle() {
	SAFE_DELETE(material);

	SAFE_DELETE(vBuffer);
	SAFE_DELETE(wBuffer);

	SAFE_DELETE(blendState[0]);
	SAFE_DELETE(blendState[1]);
	SAFE_DELETE(depthState[0]);
	SAFE_DELETE(depthState[1]);
}

void Particle::Render() {
	if (!Active()) return;

	depthState[1]->SetState();
	blendState[1]->SetState();

	wBuffer->SetVSBuffer(0);
	vBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	material->Set();
	gShader->Set();
	
	DC->Draw(particleCount, 0);
	DC->GSSetShader(nullptr, nullptr, 0);

	depthState[0]->SetState();
	blendState[0]->SetState();
}

void Particle::Play(Vector3 pos) {
	SetActive(true);
	Position() = pos;
	UpdateWorld();
}
void Particle::Stop() { SetActive(false); }
