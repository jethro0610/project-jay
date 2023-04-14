#ifdef _DEBUG
#include <assert.h>
#include <ctime>
#include "Logger.h"
#include "../Windows/WindowsInc.h"

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

void Logger::Out_P(std::string text) {
    *logger_->outfile_ << text << '\n';
    OutputDebugString((text + '\n').c_str());
}
#endif
