#pragma once
#include "../components/Controls.h"
#include "../tweaks/PerformanceTweaks.h"
#include "../tweaks/SystemRestore.h"
#include "../core/HardwareInfo.h"
#include "../core/Logger.h"
#include <windows.h>
#include <psapi.h>

class Dashboard {
public:
    static void Render() {
        ImGui::Text("System Overview & Quick Actions");
        ImGui::Spacing();
        
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORD ramUsagePercent = memInfo.dwMemoryLoad;
        float availRAMGB = memInfo.ullAvailPhys / (1024.0f * 1024.0f * 1024.0f);

        ImGui::BeginGroup();
        
        UI::CardBegin("fps_card", ImVec2(220, 95));
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "FRAMERATE");
        ImGui::SetWindowFontScale(1.8f);
        ImGui::Text("%.0f FPS", ImGui::GetIO().Framerate);
        ImGui::SetWindowFontScale(1.0f);
        UI::CardEnd();
        
        ImGui::SameLine();
        
        UI::CardBegin("ram_card", ImVec2(220, 95));
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "RAM USAGE");
        ImGui::SetWindowFontScale(1.8f);
        ImGui::Text("%lu%%", ramUsagePercent);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%.1f GB Free", availRAMGB);
        UI::CardEnd();

        ImGui::SameLine();

        UI::CardBegin("sys_status", ImVec2(220, 95));
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "SYSTEM STATE");
        ImGui::SetWindowFontScale(1.4f);
        ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.4f, 1.0f), "OPTIMIZED");
        ImGui::SetWindowFontScale(1.0f);
        UI::CardEnd();

        ImGui::EndGroup();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Quick Actions");
        ImGui::Spacing();

        if (UI::ButtonAnimated("Create System Restore Point", ImVec2(230, 36))) {
            Logger::Get().AddLog("Creating Windows System Restore Point...", LOG_INFO);
            if (SystemRestore::CreateRestorePoint("PremiumOverlay_AutoBackup")) {
                Logger::Get().AddLog("System Restore Point created successfully.", LOG_SUCCESS);
            } else {
                Logger::Get().AddLog("Failed to create Restore Point (Requires Admin privileges).", LOG_WARNING);
            }
        }

        ImGui::SameLine();

        if (UI::ButtonAnimated("Flush RAM Cache", ImVec2(180, 36))) {
            PerformanceTweaks::TrimWorkingSetRAM();
            Logger::Get().AddLog("Standby RAM cache trimmed across system processes.", LOG_SUCCESS);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("System Hardware Specifications");
        ImGui::Spacing();

        static SystemHardwareSpecs specs = HardwareInfo::DetectSpecs();

        UI::CardBegin("hw_specs_card", ImVec2(680, 160));
        
        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "PROCESSOR (CPU):");
        ImGui::SameLine(180);
        ImGui::Text("%s", specs.cpuName.c_str());

        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "GRAPHICS (GPU):");
        ImGui::SameLine(180);
        ImGui::Text("%s", specs.gpuName.c_str());

        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "MEMORY (RAM):");
        ImGui::SameLine(180);
        ImGui::Text("%s", specs.totalRAM.c_str());

        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "SYSTEM OS:");
        ImGui::SameLine(180);
        ImGui::Text("%s", specs.osVersion.c_str());

        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "STORAGE:");
        ImGui::SameLine(180);
        ImGui::Text("%s", specs.driveSpace.c_str());

        UI::CardEnd();
    }
};
