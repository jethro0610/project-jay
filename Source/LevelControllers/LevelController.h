#pragma once

class Clock;
class Entity;
class EntityList;
class SeedManager;
class Terrain;

class LevelController {
public:
    LevelController();

    void DoStart();
    void DoUpdate();

    int numMedals_;
    int maxNumMedals_;

    int id_;
    Clock* clock_;
    EntityList* entities_;
    SeedManager* seedManager_;
    Terrain* terrain_;

    static bool IsValidControllerId(int id);

    void DoEntityHurt(Entity* entity);
    void DoEntityDestroyed(Entity* entity);

protected:
    void Update() {};
    void Start() {};
    void OnEntityHurt(Entity* entity) {};
    void OnEntityDestroyed(Entity* entity) {};
    int GetNumHiddenMedals() { return 0; }
};
