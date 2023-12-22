#pragma once
#include <string>

class EntityManager;
class LevelLoader;
class LevelProperties;
class Terrain;

class EditorLevel {
public:
    EditorLevel(
        EntityManager& entityManager,
        LevelLoader& levelLoader,
        LevelProperties& levelProperties,
        Terrain& terrain
    );

    std::string name_;
    bool hasLevel_;
    LevelProperties& properties_;

    void Save();
    void New(const std::string& name);
    bool Load(const std::string& name);
    void Reset();

private:
    EntityManager& entityManager_;
    LevelLoader& levelLoader_;
    Terrain& terrain_;
};
