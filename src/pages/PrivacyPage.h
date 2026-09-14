#pragma once
#include "../components/Controls.h"
#include "../core/ConfigManager.h"
#include "../core/Logger.h"
#include "../tweaks/PrivacyTweaks.h"

class PrivacyPage {
public:
    static void Render() {
        auto& config = ConfigManager::Get().config;

        ImGui::Text("Windows Updates & Privacy (Batch Mode)");
        ImGui::Separator();
        ImGui::Spacing();

        static bool selUpdates = config.pauseWindowsUpdates;
        static bool selAds = config.disableAds;
        static bool selLocation = true;
        static bool selDefender = false;

        ImGui::Checkbox("Pause Automatic Windows Updates", &selUpdates);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Prevents automatic update reboots and sets updates to notify-only.");

        ImGui::Spacing();
        ImGui::Checkbox("Disable Windows Ads & Suggestions", &selAds);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Removes Start menu ads, lock screen suggestions, and consumer promos.");

        ImGui::Spacing();
        ImGui::Checkbox("Disable Location & Sensor Tracking", &selLocation);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "  Blocks Windows background location logging.");

        ImGui::Spacing();
        ImGui::Checkbox("Disable Windows Defender Realtime Monitoring", &selDefender);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (UI::ButtonAnimated("Select All", ImVec2(120, 32))) {
            selUpdates = selAds = selLocation = selDefender = true;
        }
        ImGui::SameLine();
        if (UI::ButtonAnimated("Deselect All", ImVec2(120, 32))) {
            selUpdates = selAds = selLocation = selDefender = false;
        }

        ImGui::SameLine(400);

        if (UI::ButtonAnimated("APPLY SELECTED TWEAKS", ImVec2(240, 36))) {
            Logger::Get().AddLog("Starting batch application of Privacy Tweaks...", LOG_INFO);

            config.pauseWindowsUpdates = selUpdates;
            PrivacyTweaks::SetPauseWindowsUpdates(selUpdates);
            Logger::Get().AddLog(std::string("Windows Updates ") + (selUpdates ? "Paused" : "Enabled"), LOG_SUCCESS);

            config.disableAds = selAds;
            PrivacyTweaks::SetDisableAdsAndSuggestions(selAds);
            Logger::Get().AddLog(std::string("Windows Ads & Suggestions ") + (selAds ? "Disabled" : "Enabled"), LOG_SUCCESS);

            PrivacyTweaks::SetDisableLocationAndTracking(selLocation);
            Logger::Get().AddLog(std::string("Location Tracking ") + (selLocation ? "Disabled" : "Enabled"), LOG_SUCCESS);

            if (selDefender) {
                PrivacyTweaks::SetDefenderEnabled(false);
                Logger::Get().AddLog("Windows Defender Realtime Monitoring Disabled.", LOG_WARNING);
            }

            ConfigManager::Get().Save();
            Logger::Get().AddLog("Privacy Tweaks batch execution finished.", LOG_SUCCESS);
        }
    }
};
