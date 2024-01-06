#pragma once
#include <string>

class EntityListS;
class LevelLoader;
class LevelProperties;
class ResourceManager;
class Terrain;

class EditorLevel {
public:
    EditorLevel(
        EntityListS& entities,
        LevelLoader& levelLoader,
        LevelProperties& levelProperties,
        ResourceManager& resourceManager,
        Terrain& terrain
    );

    std::string name_;
    bool hasLevel_;
    LevelProperties& properties_;

    void Save() { Save(name_); };
    void Save(const std::string& name, bool setAsNewLevel = true);
    void New(const std::string& name);
    bool Load(const std::string& name);
    void Reset(const std::string& resetSuffix = "");

private:
    EntityListS& entities_;
    LevelLoader& levelLoader_;
    ResourceManager& resourceManager_;
    Terrain& terrain_;
};
