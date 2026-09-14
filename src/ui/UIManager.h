#pragma once
#include <windows.h>
#include <d3d11.h>
#include "../effects/ParticleSystem.h"

class UIManager {
public:
    bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
    void Cleanup();
    void Render();

private:
    ParticleSystem particles;
    void RenderSidebar(float width);
    void RenderContent();
};
