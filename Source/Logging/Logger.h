#pragma once
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define DEBUGLOG(str) do { Logger::Get()->outfile_ << str << std::endl; std::cout << str << std::endl; } while(false)
#define ERRORLOG(str) do { Logger::Get()->outfile_ << "Error: " << str << std::endl; std::cout << "Error: " << str << std::endl; } while(false)
#else
#define DEBUGLOG(str) do { } while(false)
#define ERRORLOG(str) do { std::cout << "Error: " << str << std::endl; } while(false) 
#endif

#ifdef _DEBUG
class Logger {
public:
    Logger();
    static Logger* logger_;
    std::ofstream outfile_;
    static Logger* Get();
    static void Close();
};
#endif
