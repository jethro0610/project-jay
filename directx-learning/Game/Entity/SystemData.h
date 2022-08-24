#pragma once
#include <vector>

template<class... T>
struct EntitySystemGroup {
    std::vector<int> componentIds;

    template<class T>
    void AddComponent(T
};