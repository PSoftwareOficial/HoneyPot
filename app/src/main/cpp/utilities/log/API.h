
#pragma once
#include <fstream>
#include <cstring>
#include <mutex>
#include <chrono>
#include <sstream>

#define LOG(msg) Logger::getInstance().log(msg)
#define LOGI(msg) Logger::getInstance().log(msg)
#define LOGE(msg) Logger::getInstance().log(msg)


class Logger {
public:
    static Logger& getInstance() {
	static Logger instance;
	return instance;
	}

    // Initialize once with the android_app path
    void init(const std::string& basePath) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!outFile.is_open()) {
            std::string filePath = basePath + "/native_log.txt";
            
        outFile.open(filePath, std::ios::binary | std::ios::trunc);
        tpInit = std::chrono::steady_clock::now();
    }

}

    // Append to log
    void log(const std::string& message)
    {
        auto tpNow = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tpNow - tpStart).count();
        int hours = static_cast<int>(ms / (1000 * 60 * 60));
        ms %= (1000 * 60 * 60);
        int minutes = static_cast<int>(ms / (1000 * 60));
        ms %= (1000 * 60);
        int seconds = static_cast<int>(ms / 1000);
        ms %= 1000;
        std::ostringstream oss;
        if (hours > 0) {
            oss << hours << "::"
                << std::setw(2) << std::setfill('0') << minutes << "::"
                << std::setw(2) << std::setfill('0') << seconds << "::"
                << std::setw(3) << std::setfill('0') << ms;
        } else if (minutes > 0) {
            oss << minutes << "::"
                << std::setw(2) << std::setfill('0') << seconds << "::"
                << std::setw(3) << std::setfill('0') << ms;
        } else {
            oss << seconds << "::"
                << std::setw(3) << std::setfill('0') << ms;
        }

	std::lock_guard<std::mutex> lock(mtx);
    if (outFile.is_open()) {
        outFile << oss.str() << " : " <<message << '\n';
        outFile.flush(); // Ensure it is written to disk
    }
}

private:
    Logger() = default;
    ~Logger() {
    std::lock_guard<std::mutex> lock(mtx);
    if (outFile.is_open()) {
        outFile.close();
    }
}

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream outFile;
    std::mutex mtx;
    std::chrono::steady_clock::time_point tpInit;
};

