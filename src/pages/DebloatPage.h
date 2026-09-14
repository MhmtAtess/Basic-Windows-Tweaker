#pragma once
#include "../components/Controls.h"
#include "../core/Logger.h"
#include "../tweaks/DebloatTweaks.h"
#include <vector>

class DebloatPage {
public:
    static void Render() {
        ImGui::Text("UWP Bloatware Removal & Software Installer (Batch Mode)");
        ImGui::Separator();
        ImGui::Spacing();

        static std::vector<bool> bloatSelected = { true, true, true, true, true, true, true, true, true, true };
        static std::vector<bool> softSelected = { false, false, false, false, false, false };

        ImGui::Text("Select Bloatware Packages to Remove:");
        ImGui::Spacing();

        auto apps = DebloatTweaks::GetBloatwareList();
        for (size_t i = 0; i < apps.size(); ++i) {
            ImGui::PushID((int)i);
            bool val = bloatSelected[i];
            if (ImGui::Checkbox(apps[i].displayName.c_str(), &val)) {
                bloatSelected[i] = val;
            }
            if ((i + 1) % 2 != 0) ImGui::SameLine(360);
            ImGui::PopID();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Select Essential Software to Install (WinGet):");
        ImGui::Spacing();

        struct SoftwareItem { const char* name; const char* id; };
        static SoftwareItem softs[] = {
            { "Google Chrome", "Google.Chrome" },
            { "Brave Browser", "Brave.Brave" },
            { "Discord", "Discord.Discord" },
            { "Steam", "Valve.Steam" },
            { "7-Zip", "7zip.7zip" },
            { "Notepad++", "Notepad++.Notepad++" }
        };

        for (int i = 0; i < 6; ++i) {
            ImGui::PushID(100 + i);
            bool val = softSelected[i];
            if (ImGui::Checkbox(softs[i].name, &val)) {
                softSelected[i] = val;
            }
            if ((i + 1) % 2 != 0) ImGui::SameLine(360);
            ImGui::PopID();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Select All Bloatware", ImVec2(160, 32))) {
            for (size_t i = 0; i < bloatSelected.size(); ++i) bloatSelected[i] = true;
        }
        ImGui::SameLine();
        if (UI::ButtonAnimated("Deselect All", ImVec2(120, 32))) {
            for (size_t i = 0; i < bloatSelected.size(); ++i) bloatSelected[i] = false;
            for (size_t i = 0; i < softSelected.size(); ++i) softSelected[i] = false;
        }

        ImGui::SameLine(400);

        if (UI::ButtonAnimated("APPLY SELECTED DEBLOAT & INSTALLS", ImVec2(270, 36))) {
            Logger::Get().AddLog("Starting batch application of Debloat & Software Installers...", LOG_INFO);

            for (size_t i = 0; i < apps.size(); ++i) {
                if (bloatSelected[i]) {
                    Logger::Get().AddLog("Removing package: " + apps[i].displayName + "...", LOG_INFO);
                    DebloatTweaks::RemovePackage(apps[i].packageName);
                    Logger::Get().AddLog("Package removed: " + apps[i].displayName, LOG_SUCCESS);
                }
            }

            for (size_t i = 0; i < 6; ++i) {
                if (softSelected[i]) {
                    Logger::Get().AddLog("Installing software via WinGet: " + std::string(softs[i].name) + "...", LOG_INFO);
                    DebloatTweaks::InstallWinGetSoftware(softs[i].id);
                    Logger::Get().AddLog("Software installed: " + std::string(softs[i].name), LOG_SUCCESS);
                }
            }

            Logger::Get().AddLog("Debloat & Software Installer batch execution finished.", LOG_SUCCESS);
        }
    }
};
