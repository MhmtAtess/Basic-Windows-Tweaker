#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

namespace UI {
    bool Toggle(const char* label, bool* v);
    bool ButtonAnimated(const char* label, const ImVec2& size = ImVec2(0, 0));
    void CardBegin(const char* id, const ImVec2& size);
    void CardEnd();
}
