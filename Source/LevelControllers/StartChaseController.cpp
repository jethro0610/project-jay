#include "StartChaseController.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include "Entity/Chase.h"
#include "Entity/Medal.h"

void StartChaseController::Start() {
    allChase_.clear();
    starterChase_.clear();
    bigChase_ = nullptr;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_)
            continue;

        if (entity.typeId_ != Chase::TYPEID)
            continue;

        Chase* chase = (Chase*)&entity;
        allChase_.push_back(chase);

        std::string label = entity.label_;
        if (label.substr(0, 7) == "stchase")
            starterChase_.push_back(chase);

        if (label == "bigchase")
            bigChase_ = chase;
    }
    destroyedChases = 0;
    destroyedStartChases = 0;
}

void StartChaseController::Update() {
}

void StartChaseController::OnEntityDestroyed(Entity* entity) {
    for (int i = 0; i < allChase_.size(); i++) {
        if (entity == allChase_[i]) {
            allChase_[i] = nullptr;
            destroyedChases++;

            if (destroyedChases >= allChase_.size()) {
                Transform transform;
                transform.position = entity->transform_.position;
                Medal& medal = (Medal&)entities_->CreateEntity(Medal::TYPEID, transform);
                medal.collected_ = true;
            }
        }
    }

    for (int i = 0; i < starterChase_.size(); i++) {
        if (entity == starterChase_[i]) {
            starterChase_[i] = nullptr;
            destroyedStartChases++;
            if (destroyedStartChases >= starterChase_.size()) {
                Transform transform;
                transform.position = entity->transform_.position;
                Medal& medal = (Medal&)entities_->CreateEntity(Medal::TYPEID, transform);
                medal.collected_ = true;
            }
        }
    }
    
    if (entity == bigChase_) {
        bigChase_ = nullptr;
        Transform transform;
        transform.position = entity->transform_.position;
        Medal& medal = (Medal&)entities_->CreateEntity(Medal::TYPEID, transform);
        medal.collected_ = true;
    }
}

int StartChaseController::GetNumHiddenMedals() {
    // Defeat all bunnies
    // Fill all bumper holes
    // Hit all trees
    // Defeat big bunny
    // Defeat 3 starter bunnies
    // Defeat 1st set of bumpers

    return 6;
}
