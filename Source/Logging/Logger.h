#pragma once
#include <fstream>
#include <iostream>

#ifdef _DEBUG
#define DEBUGLOG(str) do { Logger::Get()->outfile_ << str << std::endl; std::cout << str << std::endl; } while(false)
#else
#define DEBUGLOG(str) do { } while(false)
#endif

#ifdef _DEBUG
class Logger {
private:
    Logger();
    static Logger* logger_;

public:
    std::ofstream outfile_;
    static void Init();
    static Logger* Get();
    static void Close();
};
#endif
