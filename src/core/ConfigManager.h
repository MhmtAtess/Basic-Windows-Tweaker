#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

struct AppConfig {
    int theme = 0;
    float uiScale = 1.0f;
    float accentColor[4] = { 0.45f, 0.0f, 1.0f, 1.0f };
    float animationSpeed = 1.0f;
    bool enableParticles = true;
    int particleDensity = 100;
    int selectedTab = 0;
    bool vsync = true;

    bool ultimatePowerPlan = false;
    bool disableTelemetry = false;
    bool timerResHigh = false;
    bool mouseAccelFix = false;
    bool nagleAlgoFix = false;
    bool classicContextMenu = false;
    bool takeOwnershipAdded = false;
    bool pauseWindowsUpdates = false;
    bool disableAds = false;
};

class ConfigManager {
public:
    static ConfigManager& Get() {
        static ConfigManager instance;
        return instance;
    }

    AppConfig config;

    void Load(const std::string& path) {
        configPath = path;
        std::ifstream file(path);
        if (!file.is_open()) {
            config.selectedTab = 0;
            Save();
            return;
        }

        try {
            nlohmann::json j;
            file >> j;
            if (j.contains("theme")) config.theme = j["theme"];
            if (j.contains("uiScale")) config.uiScale = j["uiScale"];
            if (j.contains("accentColor")) {
                auto arr = j["accentColor"];
                for (int i = 0; i < 4; ++i) config.accentColor[i] = arr[i];
            }
            if (j.contains("animationSpeed")) config.animationSpeed = j["animationSpeed"];
            if (j.contains("enableParticles")) config.enableParticles = j["enableParticles"];
            if (j.contains("particleDensity")) config.particleDensity = j["particleDensity"];
            if (j.contains("vsync")) config.vsync = j["vsync"];

            if (j.contains("ultimatePowerPlan")) config.ultimatePowerPlan = j["ultimatePowerPlan"];
            if (j.contains("disableTelemetry")) config.disableTelemetry = j["disableTelemetry"];
            if (j.contains("timerResHigh")) config.timerResHigh = j["timerResHigh"];
            if (j.contains("mouseAccelFix")) config.mouseAccelFix = j["mouseAccelFix"];
            if (j.contains("nagleAlgoFix")) config.nagleAlgoFix = j["nagleAlgoFix"];
            if (j.contains("classicContextMenu")) config.classicContextMenu = j["classicContextMenu"];
            if (j.contains("takeOwnershipAdded")) config.takeOwnershipAdded = j["takeOwnershipAdded"];
            if (j.contains("pauseWindowsUpdates")) config.pauseWindowsUpdates = j["pauseWindowsUpdates"];
            if (j.contains("disableAds")) config.disableAds = j["disableAds"];
        } catch (...) {
            config = AppConfig();
        }

        config.selectedTab = 0;
    }

    void Save() {
        nlohmann::json j;
        j["theme"] = config.theme;
        j["uiScale"] = config.uiScale;
        j["accentColor"] = { config.accentColor[0], config.accentColor[1], config.accentColor[2], config.accentColor[3] };
        j["animationSpeed"] = config.animationSpeed;
        j["enableParticles"] = config.enableParticles;
        j["particleDensity"] = config.particleDensity;
        j["selectedTab"] = config.selectedTab;
        j["vsync"] = config.vsync;

        j["ultimatePowerPlan"] = config.ultimatePowerPlan;
        j["disableTelemetry"] = config.disableTelemetry;
        j["timerResHigh"] = config.timerResHigh;
        j["mouseAccelFix"] = config.mouseAccelFix;
        j["nagleAlgoFix"] = config.nagleAlgoFix;
        j["classicContextMenu"] = config.classicContextMenu;
        j["takeOwnershipAdded"] = config.takeOwnershipAdded;
        j["pauseWindowsUpdates"] = config.pauseWindowsUpdates;
        j["disableAds"] = config.disableAds;

        std::ofstream file(configPath);
        if (file.is_open()) {
            file << j.dump(4);
        }
    }

private:
    std::string configPath = "config.json";
};
