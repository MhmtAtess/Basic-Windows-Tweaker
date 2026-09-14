#include "SystemRestore.h"
#include <windows.h>
#include <cstdio>

static bool RunCommandHidden(const char* cmd) {
    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    PROCESS_INFORMATION pi;
    char cmdBuf[1024];
    snprintf(cmdBuf, sizeof(cmdBuf), "cmd.exe /c %s", cmd);
    if (CreateProcessA(NULL, cmdBuf, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, 10000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    return false;
}

bool SystemRestore::CreateRestorePoint(const std::string& description) {
    std::string psCmd = "powershell -Command \"Enable-ComputerRestore -Drive 'C:\\'; Checkpoint-Computer -Description '" + description + "' -RestorePointType 'MODIFY_SETTINGS'\"";
    return RunCommandHidden(psCmd.c_str());
}
