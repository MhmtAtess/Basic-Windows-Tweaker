#include "DebloatTweaks.h"
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
        WaitForSingleObject(pi.hProcess, 15000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    return false;
}

std::vector<AppPackage> DebloatTweaks::GetBloatwareList() {
    return {
        { "Xbox App Suite", "*Microsoft.XboxApp*" },
        { "Xbox Game Overlay", "*Microsoft.XboxGamingOverlay*" },
        { "3D Builder", "*Microsoft.3DBuilder*" },
        { "Bing Weather", "*Microsoft.BingWeather*" },
        { "Bing News", "*Microsoft.BingNews*" },
        { "Solitaire Collection", "*Microsoft.MicrosoftSolitaireCollection*" },
        { "Cortana", "*Microsoft.549981C62B0B5*" },
        { "Get Help", "*Microsoft.GetHelp*" },
        { "Your Phone", "*Microsoft.YourPhone*" },
        { "Windows Maps", "*Microsoft.WindowsMaps*" }
    };
}

bool DebloatTweaks::RemovePackage(const std::string& packageName) {
    std::string psCmd = "powershell -Command \"Get-AppxPackage " + packageName + " | Remove-AppxPackage\"";
    return RunCommandHidden(psCmd.c_str());
}

bool DebloatTweaks::RemoveAllBloatware() {
    auto list = GetBloatwareList();
    for (const auto& item : list) {
        RemovePackage(item.packageName);
    }
    return true;
}

bool DebloatTweaks::InstallWinGetSoftware(const std::string& packageId) {
    std::string cmd = "winget install --id " + packageId + " -e --silent --accept-source-agreements --accept-package-agreements";
    return RunCommandHidden(cmd.c_str());
}
