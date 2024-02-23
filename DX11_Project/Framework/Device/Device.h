#pragma once

class Device : public Singleton<Device> {
public:
	Device();
	~Device();
	void Clear();
	void Present();
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return context; }
private:
	void CreateDeviceAndSwapChain();
	void CreateBuffer();
private:
	ID3D11Device *			device;		// cpu
	ID3D11DeviceContext *	context;	// gpu
	IDXGISwapChain *		swapChain;	// ram  백버퍼
	ID3D11RenderTargetView * view;		// vram 백버퍼
	ID3D11DepthStencilView * dView;		// vram 깊이버퍼
};