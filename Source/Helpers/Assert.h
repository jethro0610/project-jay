#pragma once
#ifdef _DEBUG
#include "Logging/Logger.h"
#define ASSERT(condition, message) do {     \
    if (!condition) {                       \
        ERRORLOG(message);                  \
        abort();                            \
    }                                       \
} while(0)
#endif
