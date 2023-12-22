#pragma once
#include <string>

class EntityManager;
class LevelLoader;
class LevelProperties;
class ResourceManager;
class Terrain;

class EditorLevel {
public:
    EditorLevel(
        EntityManager& entityManager,
        LevelLoader& levelLoader,
        LevelProperties& levelProperties,
        ResourceManager& resourceManager,
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
    ResourceManager& resourceManager_;
    Terrain& terrain_;
};
