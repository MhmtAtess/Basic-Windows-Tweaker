#pragma once

class PrivacyTweaks {
public:
    static bool SetPauseWindowsUpdates(bool enable);
    static bool SetDisableAdsAndSuggestions(bool enable);
    static bool SetDisableLocationAndTracking(bool enable);
    static bool SetDefenderEnabled(bool enable);
};
