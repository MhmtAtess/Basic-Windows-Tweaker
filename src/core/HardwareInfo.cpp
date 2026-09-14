#include "HardwareInfo.h"
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <cstdio>

SystemHardwareSpecs HardwareInfo::DetectSpecs() {
    SystemHardwareSpecs specs;

    HKEY hKeyCPU;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKeyCPU) == ERROR_SUCCESS) {
        char cpuBuf[256] = { 0 };
        DWORD bufSize = sizeof(cpuBuf);
        if (RegQueryValueExA(hKeyCPU, "ProcessorNameString", NULL, NULL, (LPBYTE)cpuBuf, &bufSize) == ERROR_SUCCESS) {
            specs.cpuName = cpuBuf;
        } else {
            specs.cpuName = "Generic x64 Processor";
        }
        RegCloseKey(hKeyCPU);
    } else {
        specs.cpuName = "Intel / AMD Processor";
    }

    while (!specs.cpuName.empty() && specs.cpuName.front() == ' ') {
        specs.cpuName.erase(specs.cpuName.begin());
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory))) {
        Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
        if (SUCCEEDED(factory->EnumAdapters(0, &adapter))) {
            DXGI_ADAPTER_DESC desc;
            if (SUCCEEDED(adapter->GetDesc(&desc))) {
                char gpuBuf[128];
                WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, gpuBuf, sizeof(gpuBuf), NULL, NULL);
                specs.gpuName = gpuBuf;
            }
        }
    }
    if (specs.gpuName.empty()) {
        specs.gpuName = "DirectX 11 Graphics Device";
    }

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalGB = (double)memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
        char ramBuf[64];
        snprintf(ramBuf, sizeof(ramBuf), "%.1f GB Physical RAM", totalGB);
        specs.totalRAM = ramBuf;
    } else {
        specs.totalRAM = "16 GB System Memory";
    }

    specs.osVersion = "Windows 11 / 10 64-bit";

    ULARGE_INTEGER freeBytesCaller, totalBytes, totalFreeBytes;
    if (GetDiskFreeSpaceExA("C:\\", &freeBytesCaller, &totalBytes, &totalFreeBytes)) {
        double freeGB = (double)totalFreeBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
        double totalGB = (double)totalBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
        char driveBuf[128];
        snprintf(driveBuf, sizeof(driveBuf), "C: Drive - %.1f GB Free / %.1f GB", freeGB, totalGB);
        specs.driveSpace = driveBuf;
    } else {
        specs.driveSpace = "C: Primary Storage Device";
    }

    return specs;
}
