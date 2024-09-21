#pragma once
#include "Entity.h"

class Mower : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_mower"; }
    static EntityDependendies GetStaticDependencies();
    enum Mode {
        Cut,
        Sleep
    };

    Entity* target_;
    int numberOfSeeds_;
    Mode mode_;

    void Update(); 
    void OnDestroy();

    void SetMode(Mode mode);
    void ToggleMode();
};
