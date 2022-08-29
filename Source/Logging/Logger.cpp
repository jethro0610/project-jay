#include "Logger.h"
#include <assert.h>

void Logger::InitLogger() {
    assert(logger_ == nullptr);
    logger_ = new Logger();
}