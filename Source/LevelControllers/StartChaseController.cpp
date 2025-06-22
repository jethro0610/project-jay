#include "StartChaseController.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include "Entity/Chase.h"

void StartChaseController::Start() {
    bunnies_.clear();

    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_)
            continue;

        if (entity.typeId_ == Chase::TYPEID)
            bunnies_.push_back((Chase*)&entity);
    }

    for (int i = 0; i < 16; i++) {
        deadBunnies_[i] = false;
    }
}

void StartChaseController::Update() {
    bool anyAlive = true;
    for (int i = 0; i < bunnies_.size(); i++) {
        if (deadBunnies_[i])
            continue;

        Chase* bunny = bunnies_[i];
        if (!bunny->alive_)
            deadBunnies_[i] = true;
        else
            anyAlive = false;
    }
}
