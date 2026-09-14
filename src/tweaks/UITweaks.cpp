#include "UITweaks.h"
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

static void RestartExplorer() {
    RunCommandHidden("taskkill /f /im explorer.exe & start explorer.exe");
}

bool UITweaks::SetWin11ClassicContextMenu(bool enable) {
    HKEY hKey;
    const char* path = "Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\\InprocServer32";
    if (enable) {
        if (RegCreateKeyExA(HKEY_CURRENT_USER, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "", 0, REG_SZ, (BYTE*)"", 1);
            RegCloseKey(hKey);
            RestartExplorer();
            return true;
        }
    } else {
        RegDeleteKeyA(HKEY_CURRENT_USER, path);
        RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\CLSID\\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}");
        RestartExplorer();
        return true;
    }
    return false;
}

bool UITweaks::SetTakeOwnershipContextMenu(bool enable) {
    if (enable) {
        HKEY hKey;
        if (RegCreateKeyExA(HKEY_CLASSES_ROOT, "*\\shell\\runas", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
            const char* title = "Take Ownership";
            const char* neverDefault = "";
            RegSetValueExA(hKey, "", 0, REG_SZ, (BYTE*)title, strlen(title) + 1);
            RegSetValueExA(hKey, "NoWorkingDirectory", 0, REG_SZ, (BYTE*)neverDefault, 1);
            RegCloseKey(hKey);
        }
        HKEY hKeyCmd;
        if (RegCreateKeyExA(HKEY_CLASSES_ROOT, "*\\shell\\runas\\command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKeyCmd, NULL) == ERROR_SUCCESS) {
            const char* cmd = "cmd.exe /c takeown /f \"%1\" && icacls \"%1\" /grant administrators:F";
            RegSetValueExA(hKeyCmd, "", 0, REG_SZ, (BYTE*)cmd, strlen(cmd) + 1);
            RegCloseKey(hKeyCmd);
        }
    } else {
        RegDeleteKeyA(HKEY_CLASSES_ROOT, "*\\shell\\runas\\command");
        RegDeleteKeyA(HKEY_CLASSES_ROOT, "*\\shell\\runas");
    }
    return true;
}

bool UITweaks::SetTaskbarAlignment(int align) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = align == 0 ? 0 : 1;
        RegSetValueExA(hKey, "TaskbarAl", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
        RestartExplorer();
        return true;
    }
    return false;
}

bool UITweaks::SetShowHiddenFiles(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = enable ? 1 : 2;
        RegSetValueExA(hKey, "Hidden", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
        return true;
    }
    return false;
}

bool UITweaks::SetShowFileExtensions(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD val = enable ? 0 : 1;
        RegSetValueExA(hKey, "HideFileExt", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
        RegCloseKey(hKey);
        return true;
    }
    return false;
}
