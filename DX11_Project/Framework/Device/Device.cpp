#include "Framework.h"

Device::Device() {
	CreateDeviceAndSwapChain();
	CreateBuffer();
}

Device::~Device() {
	SAFE_RELEASE(view);
	SAFE_RELEASE(dView);
	SAFE_RELEASE(device);
	SAFE_RELEASE(context);
	SAFE_RELEASE(swapChain);
}

void Device::Clear() {
	const float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	context->ClearRenderTargetView(view, clearColor);
	context->ClearDepthStencilView(dView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present() { swapChain->Present(0, 0); }

void Device::CreateDeviceAndSwapChain() {
	UINT width  = SCREEN_WIDTH;
	UINT height = SCREEN_HEIGHT;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // 주사율 N / D
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1; // MSAA
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true; // 창모드 유무

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device,
		nullptr, &context);
}

void Device::CreateBuffer() {
	{ // Render Target View
		ID3D11Texture2D* backBuffer;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		device->CreateRenderTargetView(backBuffer, nullptr, &view);
		SAFE_RELEASE(backBuffer);
	}
	{ // Depth Stencil View
		ID3D11Texture2D* depthBuffer;
		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = SCREEN_WIDTH;
		depthDesc.Height = SCREEN_HEIGHT;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = depthDesc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		device->CreateDepthStencilView(depthBuffer, &dsvDesc, &dView);
	}
	context->OMSetRenderTargets(1, &view, dView);
}
