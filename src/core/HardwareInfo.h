#pragma once
#include <string>

struct SystemHardwareSpecs {
    std::string cpuName;
    std::string gpuName;
    std::string totalRAM;
    std::string osVersion;
    std::string driveSpace;
};

class HardwareInfo {
public:
    static SystemHardwareSpecs DetectSpecs();
};
