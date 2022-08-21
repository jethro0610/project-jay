#pragma once
#include "EntityConstants.h"
#include <string>

#define NO_MESH ""

struct StaticMeshComponents {
    std::string mesh[MAX_ENTITIES];

    StaticMeshComponents() {
        std::fill_n(mesh, MAX_ENTITIES, NO_MESH);
    };
    StaticMeshComponents(const StaticMeshComponents&) = delete;
    StaticMeshComponents& operator=(const StaticMeshComponents&) = delete;
};