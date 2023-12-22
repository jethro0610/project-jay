#pragma once
#include "EntityID.h"
#include "EntityDescription.h"
#include "SpawnPromise.h"
#include "Types/Transform.h"
#include <vector_const.h>

class EntityManager;

class EntitySpawner {
    struct SpawnDescription {
        EntityDescription* description;
        Transform transform;
    };

public:
    static const int MAX_SPAWNS = 64;
    void Spawn(EntityDescription* description, const Transform& transform = Transform(), SpawnPromise* promise = nullptr) {
        spawns_.push_back({description, transform}); 
        if (promise != nullptr) {
            promise->waiting = true;
            promise->spawned = false;
            promise->id = NULL_ENTITY;
        }
        promises_.push_back(promise);
    }

private:
    friend EntityManager;
    vector_const<SpawnDescription , MAX_SPAWNS> spawns_;
    vector_const<SpawnPromise*, MAX_SPAWNS> promises_;
};
