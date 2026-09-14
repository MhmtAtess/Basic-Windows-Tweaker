#pragma once
#include "../components/Controls.h"
#include "../core/Logger.h"
#include "../tweaks/CleanerTweaks.h"

class CleanerPage {
public:
    static void Render() {
        ImGui::Text("Junk Files Cleaner & Maintenance (Batch Mode)");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Estimated Junk Files Size: ~%llu MB", CleanerTweaks::CalculateJunkSize());
        ImGui::Spacing();

        static bool selTempFiles = true;
        static bool selPrefetch = true;
        static bool selUpdateCache = true;
        static bool selSFCScan = false;
        static bool selDISMRepair = false;
        static bool selDriveDefrag = false;

        ImGui::Text("Select Cleaning Tasks:");
        ImGui::Spacing();

        ImGui::Checkbox("Clean %TEMP% & Windows Temp Files", &selTempFiles);
        ImGui::Spacing();
        ImGui::Checkbox("Clean Prefetch Files Cache", &selPrefetch);
        ImGui::Spacing();
        ImGui::Checkbox("Clean Windows Update Download Cache", &selUpdateCache);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Select System Maintenance Tasks:");
        ImGui::Spacing();

        ImGui::Checkbox("Run SFC System File Checker Scan (sfc /scannow)", &selSFCScan);
        ImGui::Spacing();
        ImGui::Checkbox("Run DISM Online Image Health Repair", &selDISMRepair);
        ImGui::Spacing();
        ImGui::Checkbox("Run SSD/HDD TRIM & Drive Defragmentation", &selDriveDefrag);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Select All Cleaners", ImVec2(160, 32))) {
            selTempFiles = selPrefetch = selUpdateCache = selSFCScan = selDISMRepair = selDriveDefrag = true;
        }
        ImGui::SameLine();
        if (UI::ButtonAnimated("Deselect All", ImVec2(120, 32))) {
            selTempFiles = selPrefetch = selUpdateCache = selSFCScan = selDISMRepair = selDriveDefrag = false;
        }

        ImGui::SameLine(400);

        if (UI::ButtonAnimated("APPLY SELECTED CLEANING & MAINTENANCE", ImVec2(280, 36))) {
            Logger::Get().AddLog("Starting batch application of Junk Cleaning & Maintenance...", LOG_INFO);

            if (selTempFiles || selPrefetch || selUpdateCache) {
                CleanerTweaks::CleanJunkFiles();
                Logger::Get().AddLog("Temp, Prefetch, and Cache files cleanup completed.", LOG_SUCCESS);
            }

            if (selSFCScan) {
                Logger::Get().AddLog("Executing SFC System File Check in background...", LOG_INFO);
                CleanerTweaks::RunSFCScan();
                Logger::Get().AddLog("SFC scan launched successfully.", LOG_SUCCESS);
            }

            if (selDISMRepair) {
                Logger::Get().AddLog("Executing DISM Image Repair in background...", LOG_INFO);
                CleanerTweaks::RunDISMRepair();
                Logger::Get().AddLog("DISM repair launched successfully.", LOG_SUCCESS);
            }

            if (selDriveDefrag) {
                Logger::Get().AddLog("Executing Drive TRIM / Optimization in background...", LOG_INFO);
                CleanerTweaks::RunDriveOptimize();
                Logger::Get().AddLog("Drive TRIM / Optimization launched successfully.", LOG_SUCCESS);
            }

            Logger::Get().AddLog("Cleaner & Maintenance batch execution finished.", LOG_SUCCESS);
        }
    }
};
