#include <string>
#include "../Logging/Logger.h"

#ifdef _DEBUG
template <class T, class M>
T GetFromMap(M map, std::string key, std::string error) {
    auto element = map.find(key);
    if (element == map.end()) {
        DEBUGLOG("Error: " << error);
        abort();
    }
    return element->second;
}

template <class M>
void ForceMapUnique(M map, std::string key, std::string error) {
    if (map.contains(key)) {
        DEBUGLOG("Error: " << error);
        abort();
    }
}
#endif
