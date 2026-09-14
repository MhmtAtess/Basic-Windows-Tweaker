#pragma once
#include "../components/Controls.h"
#include "../core/ConfigManager.h"
#include "../core/Logger.h"
#include "../tweaks/PerformanceTweaks.h"

class PerformancePage {
public:
    static void Render() {
        auto& config = ConfigManager::Get().config;

        ImGui::Text("Power & CPU Optimizations (Batch Mode)");
        ImGui::Separator();
        ImGui::Spacing();

        static bool selPower = config.ultimatePowerPlan;
        static bool selTelemetry = config.disableTelemetry;
        static bool selCPUBoost = true;
        static bool selSysMain = false;
        static bool selWSearch = false;

        ImGui::Checkbox("Ultimate Performance Power Plan", &selPower);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Unlocks Windows hidden maximum performance power scheme.");

        ImGui::Spacing();
        ImGui::Checkbox("Disable Windows Telemetry Services", &selTelemetry);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Stops DiagTrack and background tracking data collection.");

        ImGui::Spacing();
        ImGui::Checkbox("Apply CPU Priority & Responsiveness Boost", &selCPUBoost);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Optimizes MMCSS system profile for maximum gaming priority.");

        ImGui::Spacing();
        ImGui::Checkbox("Disable SysMain (Superfetch)", &selSysMain);

        ImGui::Spacing();
        ImGui::Checkbox("Disable Windows Search Indexer", &selWSearch);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Select All", ImVec2(120, 32))) {
            selPower = selTelemetry = selCPUBoost = selSysMain = selWSearch = true;
        }
        ImGui::SameLine();
        if (UI::ButtonAnimated("Deselect All", ImVec2(120, 32))) {
            selPower = selTelemetry = selCPUBoost = selSysMain = selWSearch = false;
        }

        ImGui::SameLine(400);

        if (UI::ButtonAnimated("APPLY SELECTED TWEAKS", ImVec2(240, 36))) {
            Logger::Get().AddLog("Starting batch application of Performance Tweaks...", LOG_INFO);

            config.ultimatePowerPlan = selPower;
            PerformanceTweaks::SetUltimatePowerPlan(selPower);
            Logger::Get().AddLog(std::string("Ultimate Power Plan ") + (selPower ? "Activated" : "Deactivated"), LOG_SUCCESS);

            config.disableTelemetry = selTelemetry;
            PerformanceTweaks::SetDisableTelemetry(selTelemetry);
            Logger::Get().AddLog(std::string("Telemetry Services ") + (selTelemetry ? "Disabled" : "Enabled"), LOG_SUCCESS);

            if (selCPUBoost) {
                PerformanceTweaks::SetCPUPriorityBoost(true);
                Logger::Get().AddLog("CPU Priority Boost applied successfully.", LOG_SUCCESS);
            }

            PerformanceTweaks::SetServiceState("SysMain", selSysMain);
            if (selSysMain) Logger::Get().AddLog("SysMain Service Disabled.", LOG_SUCCESS);

            PerformanceTweaks::SetServiceState("WSearch", selWSearch);
            if (selWSearch) Logger::Get().AddLog("Windows Search Service Disabled.", LOG_SUCCESS);

            ConfigManager::Get().Save();
            Logger::Get().AddLog("Performance Tweaks batch execution finished.", LOG_SUCCESS);
        }
    }
};
