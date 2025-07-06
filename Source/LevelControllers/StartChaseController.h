#include "LevelController.h"
#include <vector_const.h>

class Chase;
class MeteorHole;

class StartChaseController : public LevelController {
public:
    void Update();
    void Start();
    int GetNumHiddenMedals();

    void OnEntityDestroyed(Entity* entity);
    void OnAddSeed();

    vector_const<Chase*, 16> allChase_;
    vector_const<Chase*, 16> starterChase_;
    vector_const<MeteorHole*, 16> meteorHoles_;
    Chase* bigChase_;

    int destroyedChases;
    int destroyedStartChases;
    bool didMeteors_;
};
