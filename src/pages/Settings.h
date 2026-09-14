#pragma once
#include "../components/Controls.h"
#include "../core/ConfigManager.h"
#include "../core/Logger.h"
#include "../ui/ThemeManager.h"

class Settings {
public:
    static void Render() {
        auto& config = ConfigManager::Get().config;

        ImGui::Text("General Settings & Appearance");
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Combo("Theme", &config.theme, "Dark\0Midnight\0Neon\0")) {
            ThemeManager::ApplyTheme(config.theme, config.accentColor);
            ConfigManager::Get().Save();
            Logger::Get().AddLog("Applied Theme ID: " + std::to_string(config.theme), LOG_INFO);
        }

        ImGui::Spacing();
        if (ImGui::ColorEdit4("Accent Color", config.accentColor, ImGuiColorEditFlags_NoInputs)) {
            ThemeManager::ApplyTheme(config.theme, config.accentColor);
            ConfigManager::Get().Save();
            Logger::Get().AddLog("Updated Accent Color.", LOG_INFO);
        }

        ImGui::Spacing();
        if (ImGui::SliderFloat("Animation Speed", &config.animationSpeed, 0.1f, 3.0f)) {
            ConfigManager::Get().Save();
        }

        ImGui::Spacing();
        if (UI::Toggle("Enable VSync", &config.vsync)) {
            ConfigManager::Get().Save();
            Logger::Get().AddLog(std::string("VSync ") + (config.vsync ? "Enabled" : "Disabled"), LOG_INFO);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Visual Effects");
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::Toggle("Enable Particles", &config.enableParticles)) {
            ConfigManager::Get().Save();
            Logger::Get().AddLog(std::string("Background Particles ") + (config.enableParticles ? "Enabled" : "Disabled"), LOG_INFO);
        }

        if (config.enableParticles) {
            ImGui::Spacing();
            if (ImGui::SliderInt("Particle Density", &config.particleDensity, 10, 500)) {
                ConfigManager::Get().Save();
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Save Configuration", ImVec2(180, 35))) {
            ConfigManager::Get().Save();
            Logger::Get().AddLog("All Configuration Settings Saved Successfully.", LOG_SUCCESS);
        }

        ImGui::SameLine();

        if (UI::ButtonAnimated("Reset to Defaults", ImVec2(180, 35))) {
            config.theme = 0;
            config.accentColor[0] = 0.45f;
            config.accentColor[1] = 0.0f;
            config.accentColor[2] = 1.0f;
            config.accentColor[3] = 1.0f;
            config.animationSpeed = 1.0f;
            config.enableParticles = true;
            config.particleDensity = 100;
            config.vsync = true;
            ThemeManager::ApplyTheme(config.theme, config.accentColor);
            ConfigManager::Get().Save();
            Logger::Get().AddLog("Settings reset to defaults.", LOG_WARNING);
        }
    }
};
