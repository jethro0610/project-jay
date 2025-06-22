#pragma once

class SeedManager;
class Clock;

class ScoreKeeper {
public:
    ScoreKeeper(Clock& clock, SeedManager& seedManager);
    void Reset();

    int numMedals_;

private:
    Clock& clock_;
    SeedManager& seedManager_;
};
