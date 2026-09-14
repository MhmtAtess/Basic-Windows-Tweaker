#include "core/Application.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Application app;
    if (app.Initialize()) {
        app.Run();
    }
    return 0;
}
