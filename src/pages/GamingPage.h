#pragma once
#include "../components/Controls.h"
#include "../core/ConfigManager.h"
#include "../core/Logger.h"
#include "../tweaks/GamingTweaks.h"

class GamingPage {
public:
    static void Render() {
        auto& config = ConfigManager::Get().config;

        ImGui::Text("Input Latency & Gaming Optimizations (Batch Mode)");
        ImGui::Separator();
        ImGui::Spacing();

        static bool selTimer = config.timerResHigh;
        static bool selMouse = config.mouseAccelFix;
        static bool selNagle = config.nagleAlgoFix;
        static bool selGameMode = true;
        static bool selShaderClear = false;

        ImGui::Checkbox("High Precision Timer Resolution (0.5ms)", &selTimer);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Reduces system timer resolution to 0.5ms for lowest input latency.");

        ImGui::Spacing();
        ImGui::Checkbox("Mouse Acceleration 1:1 Fix", &selMouse);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Disables Enhance Pointer Precision and applies 1:1 raw mouse tracking.");

        ImGui::Spacing();
        ImGui::Checkbox("Network Latency Fix (Nagle's Algorithm)", &selNagle);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Disables TCP delay/ack throttling for zero packet delay in online games.");

        ImGui::Spacing();
        ImGui::Checkbox("Enable Windows Game Mode", &selGameMode);

        ImGui::Spacing();
        ImGui::Checkbox("Clear GPU Shader Cache", &selShaderClear);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Clears Nvidia, AMD, and DirectX shader caches to fix stuttering.");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Select All", ImVec2(120, 32))) {
            selTimer = selMouse = selNagle = selGameMode = selShaderClear = true;
        }
        ImGui::SameLine();
        if (UI::ButtonAnimated("Deselect All", ImVec2(120, 32))) {
            selTimer = selMouse = selNagle = selGameMode = selShaderClear = false;
        }

        ImGui::SameLine(400);

        if (UI::ButtonAnimated("APPLY SELECTED TWEAKS", ImVec2(240, 36))) {
            Logger::Get().AddLog("Starting batch application of Gaming Tweaks...", LOG_INFO);

            config.timerResHigh = selTimer;
            GamingTweaks::SetTimerResolutionHigh(selTimer);
            Logger::Get().AddLog(std::string("Timer Resolution (0.5ms) ") + (selTimer ? "Enabled" : "Disabled"), LOG_SUCCESS);

            config.mouseAccelFix = selMouse;
            GamingTweaks::SetMouseAccelerationFix(selMouse);
            Logger::Get().AddLog(std::string("Mouse Acceleration 1:1 Fix ") + (selMouse ? "Applied" : "Reverted"), LOG_SUCCESS);

            config.nagleAlgoFix = selNagle;
            GamingTweaks::SetNagleAlgorithmFix(selNagle);
            Logger::Get().AddLog(std::string("Nagle Algorithm Network Fix ") + (selNagle ? "Applied" : "Reverted"), LOG_SUCCESS);

            GamingTweaks::SetGameMode(selGameMode);
            Logger::Get().AddLog(std::string("Windows Game Mode ") + (selGameMode ? "Enabled" : "Disabled"), LOG_SUCCESS);

            if (selShaderClear) {
                GamingTweaks::ClearGPUShaderCache();
                Logger::Get().AddLog("GPU Shader Cache cleared successfully.", LOG_SUCCESS);
            }

            ConfigManager::Get().Save();
            Logger::Get().AddLog("Gaming Tweaks batch execution finished.", LOG_SUCCESS);
        }
    }
};
