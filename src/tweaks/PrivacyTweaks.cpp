#include "PrivacyTweaks.h"
#include <windows.h>
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

bool PrivacyTweaks::SetPauseWindowsUpdates(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS ||
        RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD valOptions = enable ? 2 : 4;
        DWORD valNoAuto = enable ? 1 : 0;
        RegSetValueExA(hKey, "AUOptions", 0, REG_DWORD, (BYTE*)&valOptions, sizeof(valOptions));
        RegSetValueExA(hKey, "NoAutoUpdate", 0, REG_DWORD, (BYTE*)&valNoAuto, sizeof(valNoAuto));
        RegCloseKey(hKey);
    }
    if (enable) {
        RunCommandHidden("sc config wuauserv start= disabled");
        RunCommandHidden("sc stop wuauserv");
    } else {
        RunCommandHidden("sc config wuauserv start= demand");
    }
    return true;
}

bool PrivacyTweaks::SetDisableAdsAndSuggestions(bool enable) {
    HKEY hKeyContent;
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\ContentDeliveryManager", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKeyContent, NULL) == ERROR_SUCCESS) {
        DWORD val = enable ? 0 : 1;
        RegSetValueExA(hKeyContent, "SystemPaneSuggestionsEnabled", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegSetValueExA(hKeyContent, "SubscribedContent-338388Enabled", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegSetValueExA(hKeyContent, "SubscribedContent-338389Enabled", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegSetValueExA(hKeyContent, "SubscribedContent-353696Enabled", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKeyContent);
    }
    return true;
}

bool PrivacyTweaks::SetDisableLocationAndTracking(bool enable) {
    HKEY hKeyLoc;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\LocationAndSensors", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKeyLoc, NULL) == ERROR_SUCCESS) {
        DWORD val = enable ? 1 : 0;
        RegSetValueExA(hKeyLoc, "DisableLocation", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKeyLoc);
    }
    return true;
}

bool PrivacyTweaks::SetDefenderEnabled(bool enable) {
    if (enable) {
        RunCommandHidden("powershell -Command \"Set-MpPreference -DisableRealtimeMonitoring $false\"");
    } else {
        RunCommandHidden("powershell -Command \"Set-MpPreference -DisableRealtimeMonitoring $true\"");
    }
    return true;
}
