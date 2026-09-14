#include "GamingTweaks.h"
#include <windows.h>
#include <cstdio>
#include <string>

typedef NTSTATUS(NTAPI* NtSetTimerResolution_t)(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG ActualResolution);

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

bool GamingTweaks::SetTimerResolutionHigh(bool enable) {
    HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
    if (!hNtDll) return false;
    NtSetTimerResolution_t NtSetTimerResolution = (NtSetTimerResolution_t)GetProcAddress(hNtDll, "NtSetTimerResolution");
    if (!NtSetTimerResolution) return false;

    ULONG actualRes = 0;
    if (enable) {
        NtSetTimerResolution(5000, TRUE, &actualRes);
    } else {
        NtSetTimerResolution(156250, TRUE, &actualRes);
    }
    return true;
}

bool GamingTweaks::SetMouseAccelerationFix(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        const char* speed = enable ? "0" : "1";
        const char* t1 = enable ? "0" : "6";
        const char* t2 = enable ? "0" : "10";
        RegSetValueExA(hKey, "MouseSpeed", 0, REG_SZ, (BYTE*)speed, (DWORD)strlen(speed) + 1);
        RegSetValueExA(hKey, "MouseThreshold1", 0, REG_SZ, (BYTE*)t1, (DWORD)strlen(t1) + 1);
        RegSetValueExA(hKey, "MouseThreshold2", 0, REG_SZ, (BYTE*)t2, (DWORD)strlen(t2) + 1);
        RegCloseKey(hKey);
        SystemParametersInfoA(SPI_SETMOUSE, 0, NULL, SPIF_SENDCHANGE);
        return true;
    }
    return false;
}

bool GamingTweaks::SetNagleAlgorithmFix(bool enable) {
    HKEY hKeyInterfaces;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces", 0, KEY_READ, &hKeyInterfaces) == ERROR_SUCCESS) {
        char subKeyName[256];
        DWORD index = 0;
        DWORD size = sizeof(subKeyName);
        while (RegEnumKeyExA(hKeyInterfaces, index++, subKeyName, &size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            std::string path = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\" + std::string(subKeyName);
            HKEY hSubKey;
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path.c_str(), 0, KEY_SET_VALUE, &hSubKey) == ERROR_SUCCESS) {
                DWORD val = enable ? 1 : 0;
                RegSetValueExA(hSubKey, "TcpAckFrequency", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
                RegSetValueExA(hSubKey, "TCPNoDelay", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
                RegCloseKey(hSubKey);
            }
            size = sizeof(subKeyName);
        }
        RegCloseKey(hKeyInterfaces);
        return true;
    }
    return false;
}

bool GamingTweaks::SetGameMode(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\GameBar", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = enable ? 1 : 0;
        RegSetValueExA(hKey, "AllowAutoGameMode", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegSetValueExA(hKey, "AutoGameModeEnabled", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

bool GamingTweaks::ClearGPUShaderCache() {
    RunCommandHidden("del /f /s /q \"%LOCALAPPDATA%\\NVIDIA\\DXCache\\*.*\" 2>nul");
    RunCommandHidden("del /f /s /q \"%LOCALAPPDATA%\\AMD\\DxCache\\*.*\" 2>nul");
    RunCommandHidden("del /f /s /q \"%LOCALAPPDATA%\\D3DSCache\\*.*\" 2>nul");
    return true;
}
