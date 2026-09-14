#include "DX11Renderer.h"

bool DX11Renderer::Initialize(HWND hwnd) {
    windowHandle = hwnd;
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2,
        D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &context);
        
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

void DX11Renderer::Cleanup() {
    CleanupRenderTarget();
    swapChain.Reset();
    context.Reset();
    device.Reset();
}

void DX11Renderer::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) {
        device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
        pBackBuffer->Release();
    }
}

void DX11Renderer::CleanupRenderTarget() {
    if (renderTargetView) {
        renderTargetView.Reset();
    }
}

void DX11Renderer::HandleResize(UINT width, UINT height) {
    if (swapChain) {
        CleanupRenderTarget();
        swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }
}

void DX11Renderer::BeginFrame() {
    const float clear_color_with_alpha[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
    context->ClearRenderTargetView(renderTargetView.Get(), clear_color_with_alpha);
}

void DX11Renderer::EndFrame(bool vsync) {
    swapChain->Present(vsync ? 1 : 0, 0);
}
