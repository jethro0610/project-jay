#include "LevelController.h"
#include <vector_const.h>

class Chase;

class StartChaseController : public LevelController {
public:
    void Update();
    void Start();
    int GetNumHiddenMedals();

    void OnEntityDestroyed(Entity* entity);

    vector_const<Chase*, 16> allChase_;
    vector_const<Chase*, 16> starterChase_;
    Chase* bigChase_;

    int destroyedChases;
    int destroyedStartChases;
};
