#pragma once

#ifdef WINDOWS
#include <fstream>
#endif

#ifdef _DEBUG
#define DEBUGLOG(text) Logger::Out(text)
#else
#define DEBUGLOG(text)
#endif

#ifdef _DEBUG
class Logger {
private:
    Logger();
    ~Logger();
    static Logger* logger_;

#ifdef WINDOWS
    std::ofstream* outfile_;
#endif

public:
    static void InitLogger();
    static void Out_P(std::string text);
};
#endif


