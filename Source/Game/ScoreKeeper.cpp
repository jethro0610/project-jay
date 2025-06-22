#include "ScoreKeeper.h"

ScoreKeeper::ScoreKeeper(
    Clock& clock, 
    SeedManager& seedManager
) : 
    seedManager_(seedManager),
    clock_(clock)
{

}

void ScoreKeeper::Reset() {
    numMedals_ = 0;
}
