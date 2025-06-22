#pragma once

class Clock;
class EntityList;
class ScoreKeeper;
class SeedManager;
class Terrain;

class LevelController {
public:
    LevelController();

    void DoStart();
    void DoUpdate();

    int id_;
    Clock* clock_;
    EntityList* entities_;
    ScoreKeeper* scoreKeeper_;
    SeedManager* seedManager_;
    Terrain* terrain_;

    static bool IsValidControllerId(int id);

protected:
    void Update() {};
    void Start() {};
};
