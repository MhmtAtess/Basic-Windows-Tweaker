#pragma once
#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>

class DX11Renderer {
public:
    bool Initialize(HWND hwnd);
    void Cleanup();
    void BeginFrame();
    void EndFrame(bool vsync);
    void HandleResize(UINT width, UINT height);

    ID3D11Device* GetDevice() const { return device.Get(); }
    ID3D11DeviceContext* GetContext() const { return context.Get(); }

private:
    void CreateRenderTarget();
    void CleanupRenderTarget();

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    HWND windowHandle = nullptr;
};
