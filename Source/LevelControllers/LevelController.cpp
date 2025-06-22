#include "LevelController.h"
#include "LevelControllerIds.h"
#include "Logging/Logger.h"

void LevelController::DoStart() {
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

bool LevelController::IsValidControllerId(int id) {
    switch(id) {
        #define LVCONTROLLER(TYPE, VAR, ID) case ID: return true;
        EXPANDCONTROLLERS
        #undef LVCONTROLLER
    }
    return false;
}
