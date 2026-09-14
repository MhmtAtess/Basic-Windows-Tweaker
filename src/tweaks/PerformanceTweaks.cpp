#include "PerformanceTweaks.h"
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdlib>
#include <cstdio>

static bool RunCommandHidden(const char* cmd) {
    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;
    char cmdBuf[512];
    snprintf(cmdBuf, sizeof(cmdBuf), "cmd.exe /c %s", cmd);
    if (CreateProcessA(NULL, cmdBuf, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, 5000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    return false;
}

bool PerformanceTweaks::SetUltimatePowerPlan(bool enable) {
    if (enable) {
        RunCommandHidden("powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61");
        RunCommandHidden("powercfg -setactive e9a42b02-d5df-448d-aa00-03f14749eb61");
    } else {
        RunCommandHidden("powercfg -setactive 381b4222-f694-41f0-9685-ff5bb260df2e");
    }
    return true;
}

bool PerformanceTweaks::SetDisableTelemetry(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = enable ? 0 : 3;
        RegSetValueExA(hKey, "AllowTelemetry", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
    }
    if (enable) {
        RunCommandHidden("sc config DiagTrack start= disabled");
        RunCommandHidden("sc stop DiagTrack");
        RunCommandHidden("sc config dmwappushservice start= disabled");
        RunCommandHidden("sc stop dmwappushservice");
    } else {
        RunCommandHidden("sc config DiagTrack start= auto");
        RunCommandHidden("sc start DiagTrack");
    }
    return true;
}

bool PerformanceTweaks::TrimWorkingSetRAM() {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &pe)) {
        do {
            HANDLE hProc = OpenProcess(PROCESS_SET_QUOTA, FALSE, pe.th32ProcessID);
            if (hProc) {
                SetProcessWorkingSetSize(hProc, (SIZE_T)-1, (SIZE_T)-1);
                CloseHandle(hProc);
            }
        } while (Process32Next(hSnap, &pe));
    }
    CloseHandle(hSnap);
    return true;
}

bool PerformanceTweaks::SetCPUPriorityBoost(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD valSystem = enable ? 26 : 14;
        DWORD valNetwork = enable ? 0 : 10;
        RegSetValueExA(hKey, "SystemResponsiveness", 0, REG_DWORD, (BYTE*)&valSystem, sizeof(valSystem));
        RegSetValueExA(hKey, "NetworkThrottlingIndex", 0, REG_DWORD, (BYTE*)&valNetwork, sizeof(valNetwork));
        RegCloseKey(hKey);
    }
    return true;
}

bool PerformanceTweaks::SetServiceState(const std::string& serviceName, bool disable) {
    std::string cmd = "sc config " + serviceName + (disable ? " start= disabled" : " start= auto");
    RunCommandHidden(cmd.c_str());
    if (disable) {
        std::string stopCmd = "sc stop " + serviceName;
        RunCommandHidden(stopCmd.c_str());
    } else {
        std::string startCmd = "sc start " + serviceName;
        RunCommandHidden(startCmd.c_str());
    }
    return true;
}

bool PerformanceTweaks::SetHAGS(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = enable ? 2 : 1;
        RegSetValueExA(hKey, "HwSchMode", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

bool PerformanceTweaks::IsUltimatePowerPlanActive() { return false; }
bool PerformanceTweaks::IsTelemetryDisabled() { return false; }
bool PerformanceTweaks::IsHAGSEnabled() { return false; }
