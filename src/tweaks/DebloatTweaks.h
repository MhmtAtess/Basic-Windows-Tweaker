#pragma once
#include <string>
#include <vector>

struct AppPackage {
    std::string displayName;
    std::string packageName;
};

class DebloatTweaks {
public:
    static std::vector<AppPackage> GetBloatwareList();
    static bool RemovePackage(const std::string& packageName);
    static bool RemoveAllBloatware();
    static bool InstallWinGetSoftware(const std::string& packageId);
};
