#include "LevelController.h"
#include <vector_const.h>

class Chase;

class StartChaseController : public LevelController {
public:
    void Update();
    void Start();

    vector_const<Chase*, 16> bunnies_;
    bool deadBunnies_[16];
};
