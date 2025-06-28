#include "LevelController.h"
#include "LevelControllerIds.h"
#include "Entity/EntityList.h"
#include "Entity/Medal.h"

void LevelController::DoStart() {
    maxNumMedals_ = 0;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (entity.typeId_ == Medal::TYPEID)
            maxNumMedals_++;
    }

    switch(id_) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: maxNumMedals_ += ((TYPE*)(this))->GetNumHiddenMedals();
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }

    switch(id_) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: ((TYPE*)(this))->Start();
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }
}

void LevelController::DoUpdate() {
    switch(id_) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: ((TYPE*)(this))->Update();
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }
}

void LevelController::DoEntityHurt(Entity* entity) {
    switch(id_) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: ((TYPE*)(this))->OnEntityHurt(entity);
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }
}

void LevelController::DoEntityDestroyed(Entity* entity) {
    if (entity->typeId_ == Medal::TYPEID)
        numMedals_++;

    switch(id_) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: ((TYPE*)(this))->OnEntityDestroyed(entity);
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }
}

bool LevelController::IsValidControllerId(int id) {
    switch(id) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: return true;
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }
    return false;
}

int LevelController::GetTotalSeeds() {
    int totalSeeds = 0;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = (*entities_)[i];
        if (!entity.alive_)
            continue;

        int tot = entity.DoGetSeeds();
        if (tot != 0)
            DEBUGLOG(tot);
        totalSeeds += entity.DoGetSeeds();
    }
    DEBUGLOG("");
    return totalSeeds;
}
