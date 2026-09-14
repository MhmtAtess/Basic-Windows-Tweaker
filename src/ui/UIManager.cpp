#define IMGUI_DEFINE_MATH_OPERATORS
#include "UIManager.h"
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include "ThemeManager.h"
#include "../core/ConfigManager.h"
#include "AnimationManager.h"
#include "../pages/Dashboard.h"
#include "../pages/PerformancePage.h"
#include "../pages/GamingPage.h"
#include "../pages/PrivacyPage.h"
#include "../pages/UIPage.h"
#include "../pages/DebloatPage.h"
#include "../pages/CleanerPage.h"
#include "../pages/LogsPage.h"
#include "../pages/Settings.h"
#include <string>

bool UIManager::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);

    auto& cfg = ConfigManager::Get().config;
    ThemeManager::ApplyTheme(cfg.theme, cfg.accentColor);

    particles.Init(300);

    return true;
}

void UIManager::Cleanup() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::Render() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    auto& cfg = ConfigManager::Get().config;

    ImDrawList* bg_draw_list = ImGui::GetBackgroundDrawList();

    if (cfg.enableParticles) {
        ImVec2 size = ImGui::GetIO().DisplaySize;
        particles.UpdateAndDraw(bg_draw_list, ImVec2(0,0), size, ImGui::GetIO().DeltaTime, cfg.accentColor);
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    
    if (ImGui::Begin("MainOverlay", nullptr, flags)) {
        
        float sidebarWidth = 210.0f;
        RenderSidebar(sidebarWidth);
        
        ImGui::SameLine();
        
        ImGui::BeginGroup();
        RenderContent();
        ImGui::EndGroup();

    }
    ImGui::End();
    ImGui::PopStyleVar(2);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::RenderSidebar(float width) {
    ImGui::BeginChild("Sidebar", ImVec2(width, 0), false);
    
    ImGui::Text("WIN TWEAKER");
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "v1.0.0 Premium Suite");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    const char* tabs[] = { 
        "Dashboard", 
        "Performance", 
        "Gaming & Latency", 
        "Privacy", 
        "UI & Explorer", 
        "Debloat & Apps", 
        "Cleaner", 
        "Operation Logs", 
        "Settings" 
    };
    auto& cfg = ConfigManager::Get().config;

    for (int i = 0; i < 9; ++i) {
        bool selected = (cfg.selectedTab == i);
        
        float anim = AnimationManager::GetAnimatedState(ImGui::GetID(tabs[i]), selected, 15.0f);
        
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(width - 25.0f, 32.0f);

        if (ImGui::Selectable(std::string(std::string("  ") + tabs[i]).c_str(), selected, 0, size)) {
            cfg.selectedTab = i;
        }

        if (anim > 0.01f) {
            ImU32 col = ImGui::GetColorU32(ImGuiCol_Button);
            ImGui::GetWindowDrawList()->AddRectFilled(
                pos, ImVec2(pos.x + 3.0f, pos.y + size.y), col, 2.0f);
            
            ImGui::GetWindowDrawList()->AddRectFilled(
                pos, ImVec2(pos.x + size.x, pos.y + size.y), 
                ImGui::GetColorU32(ImVec4(cfg.accentColor[0], cfg.accentColor[1], cfg.accentColor[2], 0.2f * anim)), 4.0f);
        }
    }
    
    ImGui::EndChild();
}

void UIManager::RenderContent() {
    ImGui::BeginChild("Content", ImVec2(0, 0), false);
    
    auto& cfg = ConfigManager::Get().config;
    
    switch (cfg.selectedTab) {
        case 0: Dashboard::Render(); break;
        case 1: PerformancePage::Render(); break;
        case 2: GamingPage::Render(); break;
        case 3: PrivacyPage::Render(); break;
        case 4: UIPage::Render(); break;
        case 5: DebloatPage::Render(); break;
        case 6: CleanerPage::Render(); break;
        case 7: LogsPage::Render(); break;
        case 8: Settings::Render(); break;
        default: Dashboard::Render(); break;
    }
    
    ImGui::EndChild();
}
