#include "Controls.h"
#include "../core/ConfigManager.h"
#include "../ui/AnimationManager.h"

namespace UI {

bool Toggle(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    float height = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    
    float width = height * 2.0f;
    float radius = height * 0.50f;

    const ImRect total_bb(pos, pos + ImVec2(width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    float animState = AnimationManager::GetAnimatedState(id, *v, ConfigManager::Get().config.animationSpeed * 15.0f);
    
    ImU32 col_bg;
    if (hovered)
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.2f, 0.2f, 0.2f, 1.0f), ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered), animState));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.15f, 0.15f, 0.15f, 1.0f), ImGui::GetStyleColorVec4(ImGuiCol_Button), animState));

    window->DrawList->AddRectFilled(pos, pos + ImVec2(width, height), col_bg, radius);
    window->DrawList->AddCircleFilled(ImVec2(pos.x + radius + animState * (width - radius * 2.0f), pos.y + radius), radius - 2.0f, IM_COL32(255, 255, 255, 255));

    if (label_size.x > 0.0f)
        ImGui::RenderText(ImVec2(pos.x + width + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y), label);

    return pressed;
}

bool ButtonAnimated(const char* label, const ImVec2& size_arg) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + g.Style.FramePadding.x * 2.0f, label_size.y + g.Style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, g.Style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    float hoverAnim = AnimationManager::GetAnimatedState(id, hovered, 10.0f);
    
    ImU32 col = ImGui::GetColorU32(ImLerp(g.Style.Colors[ImGuiCol_Button], g.Style.Colors[ImGuiCol_ButtonHovered], hoverAnim));
    
    if (hoverAnim > 0.01f) {
        window->DrawList->AddRect(bb.Min - ImVec2(1,1), bb.Max + ImVec2(1,1), ImGui::GetColorU32(ImVec4(g.Style.Colors[ImGuiCol_ButtonHovered].x, g.Style.Colors[ImGuiCol_ButtonHovered].y, g.Style.Colors[ImGuiCol_ButtonHovered].z, hoverAnim * 0.5f)), g.Style.FrameRounding);
    }

    window->DrawList->AddRectFilled(bb.Min, bb.Max, col, g.Style.FrameRounding);
    ImGui::RenderTextClipped(bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, &label_size, g.Style.ButtonTextAlign, &bb);

    return pressed;
}

void CardBegin(const char* id, const ImVec2& size) {
    ImGui::BeginChild(id, size, false, ImGuiWindowFlags_NoScrollbar);
    ImVec2 pos = ImGui::GetWindowPos();
    ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + size, IM_COL32(30, 30, 35, 150), 8.0f);
    ImGui::GetWindowDrawList()->AddRect(pos, pos + size, IM_COL32(60, 60, 70, 100), 8.0f);
    ImGui::SetCursorPos(ImVec2(15, 15));
    ImGui::BeginGroup();
}

void CardEnd() {
    ImGui::EndGroup();
    ImGui::EndChild();
}

}
