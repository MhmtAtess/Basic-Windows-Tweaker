#pragma once
#include <imgui.h>

class ThemeManager {
public:
    static void ApplyTheme(int themeId, const float* accent);
private:
    static void SetupDark(const float* accent);
    static void SetupMidnight(const float* accent);
    static void SetupNeon(const float* accent);
};
