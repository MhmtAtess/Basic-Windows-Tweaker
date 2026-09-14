#pragma once

class GamingTweaks {
public:
    static bool SetTimerResolutionHigh(bool enable);
    static bool SetMouseAccelerationFix(bool enable);
    static bool SetNagleAlgorithmFix(bool enable);
    static bool SetGameMode(bool enable);
    static bool ClearGPUShaderCache();
};
