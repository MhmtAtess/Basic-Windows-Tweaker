#pragma once
#include <windows.h>
#include <memory>
#include "../renderer/DX11Renderer.h"
#include "../ui/UIManager.h"

class Application {
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND windowHandle = nullptr;
    bool isRunning = false;

    std::unique_ptr<DX11Renderer> renderer;
    std::unique_ptr<UIManager> uiManager;
};
