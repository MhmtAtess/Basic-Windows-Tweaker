#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

enum LogType {
    LOG_INFO,
    LOG_SUCCESS,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry {
    std::string timestamp;
    std::string message;
    LogType type;
};

class Logger {
public:
    static Logger& Get() {
        static Logger instance;
        return instance;
    }

    void AddLog(const std::string& message, LogType type = LOG_INFO) {
        std::lock_guard<std::mutex> lock(logMutex);
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        struct tm tm_buf;
        localtime_s(&tm_buf, &in_time_t);
        ss << std::put_time(&tm_buf, "%H:%M:%S");

        LogEntry entry;
        entry.timestamp = ss.str();
        entry.message = message;
        entry.type = type;
        logs.push_back(entry);
    }

    std::vector<LogEntry> GetLogs() {
        std::lock_guard<std::mutex> lock(logMutex);
        return logs;
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(logMutex);
        logs.clear();
    }

private:
    std::vector<LogEntry> logs;
    std::mutex logMutex;
};
