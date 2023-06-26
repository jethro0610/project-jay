#pragma once
#ifdef _DEBUG
#define ASSERT(condition, message) do {  \
    if (!condition) {                       \
        DEBUGLOG("Error: " << message);     \
        abort();                            \
    }                                       \
} while(0)
#endif
