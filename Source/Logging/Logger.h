#pragma once

#ifdef _WINDOWS
#include <fstream>
#endif

#ifdef _DEBUG
#define DEBUGLOG(text) Logger::Out_P(text)
#else
#define DEBUGLOG(text)
#endif

#ifdef _DEBUG
class Logger {
private:
    Logger();
    ~Logger();
    static Logger* logger_;

#ifdef _WINDOWS
    std::ofstream* outfile_;
#endif

public:
    static void InitLogger();
    static void Out_P(std::string text);
};
#endif


