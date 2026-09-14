#pragma once
#include <string>

class PerformanceTweaks {
public:
    static bool SetUltimatePowerPlan(bool enable);
    static bool SetDisableTelemetry(bool enable);
    static bool TrimWorkingSetRAM();
    static bool SetCPUPriorityBoost(bool enable);
    static bool SetServiceState(const std::string& serviceName, bool disable);
    static bool SetHAGS(bool enable);

    static bool IsUltimatePowerPlanActive();
    static bool IsTelemetryDisabled();
    static bool IsHAGSEnabled();
};
