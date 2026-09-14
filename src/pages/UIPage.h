#pragma once
#include "../components/Controls.h"
#include "../core/ConfigManager.h"
#include "../core/Logger.h"
#include "../tweaks/UITweaks.h"

class UIPage {
public:
    static void Render() {
        auto& config = ConfigManager::Get().config;

        ImGui::Text("Windows 11 Context Menu & Explorer (Batch Mode)");
        ImGui::Separator();
        ImGui::Spacing();

        static bool selClassicMenu = config.classicContextMenu;
        static bool selTakeOwner = config.takeOwnershipAdded;
        static bool selShowHidden = true;
        static bool selShowExt = true;
        static int selTaskbarAlign = 1;

        ImGui::Checkbox("Restore Win11 Classic Context Menu", &selClassicMenu);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Restores traditional Windows 10 right-click menu instantly.");

        ImGui::Spacing();
        ImGui::Checkbox("Add 'Take Ownership' Right-Click Menu", &selTakeOwner);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Adds instant file/folder admin ownership option to context menu.");

        ImGui::Spacing();
        ImGui::Checkbox("Show Hidden Files & Folders", &selShowHidden);

        ImGui::Spacing();
        ImGui::Checkbox("Show File Extensions", &selShowExt);

        ImGui::Spacing();
        ImGui::Combo("Taskbar Alignment", &selTaskbarAlign, "Left\0Center\0");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Select All", ImVec2(120, 32))) {
            selClassicMenu = selTakeOwner = selShowHidden = selShowExt = true;
        }
        ImGui::SameLine();
        if (UI::ButtonAnimated("Deselect All", ImVec2(120, 32))) {
            selClassicMenu = selTakeOwner = selShowHidden = selShowExt = false;
        }

        ImGui::SameLine(400);

        if (UI::ButtonAnimated("APPLY SELECTED TWEAKS", ImVec2(240, 36))) {
            Logger::Get().AddLog("Starting batch application of UI Tweaks...", LOG_INFO);

            config.classicContextMenu = selClassicMenu;
            UITweaks::SetWin11ClassicContextMenu(selClassicMenu);
            Logger::Get().AddLog(std::string("Win11 Classic Context Menu ") + (selClassicMenu ? "Restored" : "Default"), LOG_SUCCESS);

            config.takeOwnershipAdded = selTakeOwner;
            UITweaks::SetTakeOwnershipContextMenu(selTakeOwner);
            Logger::Get().AddLog(std::string("Take Ownership Context Menu ") + (selTakeOwner ? "Added" : "Removed"), LOG_SUCCESS);

            UITweaks::SetShowHiddenFiles(selShowHidden);
            Logger::Get().AddLog(std::string("Show Hidden Files ") + (selShowHidden ? "Enabled" : "Disabled"), LOG_SUCCESS);

            UITweaks::SetShowFileExtensions(selShowExt);
            Logger::Get().AddLog(std::string("Show File Extensions ") + (selShowExt ? "Enabled" : "Disabled"), LOG_SUCCESS);

            UITweaks::SetTaskbarAlignment(selTaskbarAlign);
            Logger::Get().AddLog(std::string("Taskbar Alignment set to ") + (selTaskbarAlign == 0 ? "Left" : "Center"), LOG_SUCCESS);

            ConfigManager::Get().Save();
            Logger::Get().AddLog("UI Tweaks batch execution finished.", LOG_SUCCESS);
        }
    }
};
