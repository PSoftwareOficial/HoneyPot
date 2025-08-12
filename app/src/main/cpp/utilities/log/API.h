
#pragma once
#include <fstream>
#include <cstring>
#include <mutex>


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
    }

}

    // Append to log
    void log(const std::string& message)
{
	std::lock_guard<std::mutex> lock(mtx);
    if (outFile.is_open()) {
        outFile << message << '\n';
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
};

