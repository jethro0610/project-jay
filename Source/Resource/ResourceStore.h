#include <vector_holed.h>
#include <unordered_map>
#include <string>
#include "Helpers/MapCheck.h"

template <typename T, const int alloc_size_>
class ResourceStore {
private:
    vector_holed<T, alloc_size_> resources_;
    std::unordered_map<std::string, int> resourceMap_;

public:
    T& Add(const std::string& name) {
        ForceMapUnique(resourceMap_, name, "Already loaded resource " + name);
        int next = resources_.push_back();
        resourceMap_[name] = resources_[next];
        return resources_[next];
    }
    T& Get(const std::string& name) { 
        int resourceIndex = GetFromMap<int>(resourceMap_, name, "Tried to get unloaded resource " + name); 
        return resources_[resourceIndex];
    }
    void Remove(const std::string& name) {
        int resourceIndex = GetFromMap<int>(resourceMap_, name, "Tried to remove unloaded resource " + name); 
        resources_.remove(resourceIndex);
        resourceMap_.erase(name);
    }
    const std::unordered_map<std::string, int>& GetMap() const {
        return resourceMap_;
    }
};
