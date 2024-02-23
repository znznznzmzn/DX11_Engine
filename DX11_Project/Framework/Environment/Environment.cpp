#include "Framework.h"

Environment::Environment() {
	CreateViewport();
	CreateProjection();
	CreateState();

	main_camera = new Camera;
	main_camera->Tag() = "MainCamera";
	main_camera->Load();

	main_tween = new Tween();

	lightBuffer = new LightBuffer();
	GUIBuffer   = new MatrixBuffer();
}

Environment::~Environment() {
	main_camera->Save();
	SAFE_DELETE(samplerState);
	SAFE_DELETE(rasterizerState);

	SAFE_DELETE(perspectiveBuffer);
	SAFE_DELETE(orthographicBuffer);
	SAFE_DELETE(GUIBuffer);

	SAFE_DELETE(lightBuffer);

	SAFE_DELETE(depthState[0]);
	SAFE_DELETE(depthState[1]);

	SAFE_DELETE(main_camera);
	SAFE_DELETE(main_tween);
}

void Environment::Update() {
	main_tween->Update();
	main_camera->Update();
	//F1 = freemode
	if (KEY_DOWN(ImGuiKey::ImGuiKey_F2)) {
		if (isWireMode) {
			rasterizerState->FillMode(D3D11_FILL_SOLID);
			isWireMode = false;
		}
		else {
			rasterizerState->FillMode(D3D11_FILL_WIREFRAME);
			isWireMode = true;
		}
		rasterizerState->SetState();
	}
	if (KEY_DOWN(ImGuiKey::ImGuiKey_F3)) LockMouse(!isClipMouse); // 마우스 고정
	if (KEY_DOWN(ImGuiKey::ImGuiKey_F4)) main_camera->SetActive(!main_camera->Active()); // 메인 카메라 중단
}

void Environment::GUIRender() {
	string fps = "FPS : " + to_string((int)ImGui::GetIO().Framerate);
	ImGui::Text(fps.c_str());

	main_camera->GUIRender();
	ImGui::SliderFloat3("Light Direction", (float*)&lightBuffer->GetLightDirection(), -Calc::PI, Calc::PI);
	ImGui::ColorEdit3("Light Ambient", (float*)&lightBuffer->GetAmbient());
	ImGui::ColorEdit3("Light Color", (float*)&lightBuffer->GetLightColor());
}

void Environment::Set() {
	main_camera->SetViewBuffer();
	lightBuffer->SetPSBuffer(0);
	perspectiveBuffer->SetVSBuffer(2);
	perspectiveBuffer->SetGSBuffer(2);

	depthState[0]->SetState();
}

void Environment::PostSet() {
	GUIBuffer->SetVSBuffer(1);
	orthographicBuffer->SetVSBuffer(2);

	depthState[1]->SetState();
}

void Environment::LockMouse(bool isLock, RECT lockArea) {
	isClipMouse = isLock;
	if (isLock) {
		RECT rc = lockArea;
		
		POINT lt = { rc.left, rc.top };
		POINT rb = { rc.right, rc.bottom };
		ClientToScreen(hWnd, &lt);
		ClientToScreen(hWnd, &rb);
		
		rc = { lt.x, lt.y, rb.x, rb.y };
		ClipCursor(&rc);
	}
	else ClipCursor(nullptr);
}

void Environment::CreateViewport() {
	D3D11_VIEWPORT viewPort;
	viewPort.Width = SCREEN_WIDTH;
	viewPort.Height = SCREEN_HEIGHT;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	DC->RSSetViewports(1, &viewPort);
}

void Environment::CreateProjection() {
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	perspectiveBuffer = new MatrixBuffer();
	perspectiveBuffer->Set(projection);

	orthographicBuffer = new MatrixBuffer();
	orthographicBuffer->Set(
		XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f));
}

void Environment::CreateState() {
	samplerState = new SamplerState();
	samplerState->PSSetState(0);

	rasterizerState = new RasterizerState();

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);
}
