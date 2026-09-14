#include "Application.h"
#include "ConfigManager.h"
#include "Logger.h"
#include <backends/imgui_impl_win32.h>
#include <dwmapi.h>
#include <windowsx.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Application::Application() {
    renderer = std::make_unique<DX11Renderer>();
    uiManager = std::make_unique<UIManager>();
}

Application::~Application() {
    uiManager->Cleanup();
    renderer->Cleanup();
    DestroyWindow(windowHandle);
    UnregisterClassA("PremiumOverlayClass", GetModuleHandle(nullptr));
}

bool Application::Initialize() {
    ConfigManager::Get().Load("config.json");
    Logger::Get().AddLog("Application initialized successfully.", LOG_INFO);

    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = LoadIconA(wc.hInstance, MAKEINTRESOURCEA(1));
    wc.hIconSm = LoadIconA(wc.hInstance, MAKEINTRESOURCEA(1));
    wc.lpszClassName = "PremiumOverlayClass";
    RegisterClassExA(&wc);

    int width = 960;
    int height = 580;
    
    windowHandle = CreateWindowExA(
        WS_EX_APPWINDOW | WS_EX_LAYERED,
        wc.lpszClassName, "Premium Overlay",
        WS_POPUP | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
        width, height, nullptr, nullptr, wc.hInstance, this);

    SetLayeredWindowAttributes(windowHandle, 0, 255, LWA_ALPHA);

    MARGINS margins = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(windowHandle, &margins);

    if (!renderer->Initialize(windowHandle)) return false;
    if (!uiManager->Initialize(windowHandle, renderer->GetDevice(), renderer->GetContext())) return false;

    isRunning = true;
    return true;
}

void Application::Run() {
    MSG msg = {};
    while (isRunning) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) isRunning = false;
        }
        
        if (!isRunning) break;

        renderer->BeginFrame();
        uiManager->Render();
        renderer->EndFrame(ConfigManager::Get().config.vsync);
    }
}

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Application* app = nullptr;
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        app = (Application*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
    } else {
        app = (Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    
    if (app) return app->HandleMessage(hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Application::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg) {
        case WM_SIZE:
            if (wParam != SIZE_MINIMIZED && renderer) {
                renderer->HandleResize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
            }
            return 0;
        case WM_ENTERSIZEMOVE:
            SetTimer(hwnd, 1, 16, NULL);
            return 0;
        case WM_EXITSIZEMOVE:
            KillTimer(hwnd, 1);
            return 0;
        case WM_TIMER:
            if (wParam == 1 && renderer && uiManager) {
                renderer->BeginFrame();
                uiManager->Render();
                renderer->EndFrame(ConfigManager::Get().config.vsync);
            }
            return 0;
        case WM_NCHITTEST: {
            LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);
            if (hit == HTCLIENT) {
                POINT pt;
                pt.x = GET_X_LPARAM(lParam);
                pt.y = GET_Y_LPARAM(lParam);
                ScreenToClient(hwnd, &pt);
                if (pt.y < 40) return HTCAPTION;
            }
            return hit;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
