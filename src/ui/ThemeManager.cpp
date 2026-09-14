#include "ThemeManager.h"

void ThemeManager::ApplyTheme(int themeId, const float* accent) {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 10.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding = 6.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(16, 16);

    switch (themeId) {
        case 1: SetupMidnight(accent); break;
        case 2: SetupNeon(accent); break;
        case 0: 
        default: SetupDark(accent); break;
    }
}

void ThemeManager::SetupDark(const float* accent) {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.09f, 0.96f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.22f, 0.50f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    
    ImVec4 ac(accent[0], accent[1], accent[2], accent[3]);
    colors[ImGuiCol_Button] = ac;
    colors[ImGuiCol_ButtonHovered] = ImVec4(ac.x * 1.2f, ac.y * 1.2f, ac.z * 1.2f, ac.w);
    colors[ImGuiCol_ButtonActive] = ImVec4(ac.x * 0.8f, ac.y * 0.8f, ac.z * 0.8f, ac.w);
    colors[ImGuiCol_SliderGrab] = ac;
    colors[ImGuiCol_SliderGrabActive] = colors[ImGuiCol_ButtonHovered];
    colors[ImGuiCol_Header] = ac;
    colors[ImGuiCol_HeaderHovered] = colors[ImGuiCol_ButtonHovered];
    colors[ImGuiCol_HeaderActive] = colors[ImGuiCol_ButtonActive];
    colors[ImGuiCol_CheckMark] = ac;
}

void ThemeManager::SetupMidnight(const float* accent) {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.05f, 0.08f, 0.98f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_Border] = ImVec4(0.15f, 0.18f, 0.25f, 0.40f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.10f, 0.15f, 1.00f);
    ImVec4 ac(accent[0], accent[1], accent[2], accent[3]);
    colors[ImGuiCol_CheckMark] = ac;
    colors[ImGuiCol_SliderGrab] = ac;
    colors[ImGuiCol_Button] = ac;
}

void ThemeManager::SetupNeon(const float* accent) {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.95f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImVec4 ac(accent[0], accent[1], accent[2], accent[3]);
    colors[ImGuiCol_Border] = ImVec4(ac.x, ac.y, ac.z, 0.3f);
    colors[ImGuiCol_CheckMark] = ac;
    colors[ImGuiCol_SliderGrab] = ac;
    colors[ImGuiCol_Button] = ac;
}
