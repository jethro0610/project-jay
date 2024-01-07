#include "Logger.h"
#include <assert.h>
#include <iostream>
#include <filesystem>
Logger* Logger::logger_ = nullptr;

Logger::Logger() {
    logger_ = this;
    bool logsToMove[4] = { false, false, false };
    const std::filesystem::path rootPath("./");

    for (const auto& entry : std::filesystem::directory_iterator(rootPath)) {
        std::filesystem::path path = entry.path();
        if (path.extension() == ".txt") {
            std::string name = path.stem().string();
            if (name.substr(0, 3) != "log")
                continue;

            int logNumber = name[3] - '0';
            if (logNumber >= 3)
                std::filesystem::remove(path);
            else
                logsToMove[logNumber] = true;
        }
    }

    for(int i = 3; i >= 0; i--) {
        if (logsToMove[i] == false)
            continue;

        std::filesystem::path oldPath = rootPath / ("log" + std::to_string(i) + ".txt");
        std::filesystem::path newPath = rootPath / ("log" + std::to_string(i + 1) + ".txt");
        std::filesystem::rename(oldPath , newPath);
    }

    outfile_.open("log0.txt", std::ios::out);
}

void Logger::Close() {
    logger_->outfile_.close();
}

Logger* Logger::Get() {
    return logger_;
}
