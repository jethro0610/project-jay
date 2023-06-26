#pragma once
#ifdef _DEBUG
#define ASSERT(condition, message) do {     \
    if (!condition) {                       \
        ERRORLOG(message);                  \
        abort();                            \
    }                                       \
} while(0)
#endif
