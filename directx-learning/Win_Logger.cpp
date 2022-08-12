#ifdef _DEBUG
#include "Logger.h"
#include <assert.h>
#include <ctime>

Logger* Logger::logger_ = nullptr;

Logger::Logger() {
    time_t systime;
    tm local;

    time(&systime);
    localtime_s(&local, &systime);

    char filename[128];
    std::strftime(filename, 128, "logs/[%m-%d-%y] %H--%M--%S.txt", &local);

    outfile_ = new std::ofstream(filename);
}

Logger::~Logger() {
    outfile_->close();
    delete outfile_;
}

void Logger::InitLogger() {
    assert(logger_ == nullptr);
    logger_ = new Logger();
}

void Logger::Out(std::string text) {
    *logger_->outfile_ << "Log: " << text << '\n';
}
#endif