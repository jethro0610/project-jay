#include <string>
#include <nlohmann/json.hpp>

class EntityManager;

class LevelLoader {
public:
    LevelLoader(EntityManager& entityManager);
    void UnloadLevel();
    void LoadLevel(const std::string& name);

private:
    EntityManager& entityManager_;
};
