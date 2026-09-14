#pragma once
#include <string>
#include <vector>

struct StartupItem {
    std::string name;
    std::string path;
    bool enabled;
    std::string location;
};

class CleanerTweaks {
public:
    static unsigned long long CalculateJunkSize();
    static bool CleanJunkFiles();
    static bool RunSFCScan();
    static bool RunDISMRepair();
    static bool RunDriveOptimize();
    static std::vector<StartupItem> GetStartupItems();
    static bool ToggleStartupItem(const StartupItem& item, bool enable);
};
