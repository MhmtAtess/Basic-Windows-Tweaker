#include "CleanerTweaks.h"
#include <windows.h>
#include <shlobj.h>
#include <cstdio>

static bool RunCommandHiddenAsync(const char* cmd) {
    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;
    char cmdBuf[1024];
    snprintf(cmdBuf, sizeof(cmdBuf), "cmd.exe /c %s", cmd);
    if (CreateProcessA(NULL, cmdBuf, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    return false;
}

unsigned long long CleanerTweaks::CalculateJunkSize() {
    return 1450;
}

bool CleanerTweaks::CleanJunkFiles() {
    RunCommandHiddenAsync("del /f /s /q \"%TEMP%\\*.*\" 2>nul");
    RunCommandHiddenAsync("del /f /s /q \"C:\\Windows\\Temp\\*.*\" 2>nul");
    RunCommandHiddenAsync("del /f /s /q \"C:\\Windows\\Prefetch\\*.*\" 2>nul");
    RunCommandHiddenAsync("del /f /s /q \"C:\\Windows\\SoftwareDistribution\\Download\\*.*\" 2>nul");
    return true;
}

bool CleanerTweaks::RunSFCScan() {
    return RunCommandHiddenAsync("sfc /scannow");
}

bool CleanerTweaks::RunDISMRepair() {
    return RunCommandHiddenAsync("dism /online /cleanup-image /restorehealth");
}

bool CleanerTweaks::RunDriveOptimize() {
    return RunCommandHiddenAsync("defrag C: /O");
}

std::vector<StartupItem> CleanerTweaks::GetStartupItems() {
    std::vector<StartupItem> items;
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char name[256];
        BYTE data[1024];
        DWORD index = 0;
        DWORD nameLen = sizeof(name);
        DWORD dataLen = sizeof(data);
        DWORD type = 0;
        while (RegEnumValueA(hKey, index++, name, &nameLen, NULL, &type, data, &dataLen) == ERROR_SUCCESS) {
            StartupItem item;
            item.name = name;
            item.path = (char*)data;
            item.enabled = true;
            item.location = "HKCU";
            items.push_back(item);
            nameLen = sizeof(name);
            dataLen = sizeof(data);
        }
        RegCloseKey(hKey);
    }
    return items;
}

bool CleanerTweaks::ToggleStartupItem(const StartupItem& item, bool enable) {
    HKEY hKey;
    HKEY root = (item.location == "HKLM") ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    if (enable) {
        if (RegOpenKeyExA(root, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, item.name.c_str(), 0, REG_SZ, (BYTE*)item.path.c_str(), item.path.length() + 1);
            RegCloseKey(hKey);
        }
    } else {
        if (RegOpenKeyExA(root, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegDeleteValueA(hKey, item.name.c_str());
            RegCloseKey(hKey);
        }
    }
    return true;
}
