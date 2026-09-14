#pragma once
#include <string>

class SystemRestore {
public:
    static bool CreateRestorePoint(const std::string& description);
};
