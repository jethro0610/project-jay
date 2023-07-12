#pragma once

class GlobalTime {
public:
    static void Init();
    static void UpdateTime();
    static float GetDeltaTime();
    static float GetTime();
    static float GetFrameTime();

private:
    static GlobalTime* time_;
    long long lastTimeUSec_;
    long long currentTimeUSec_;
    float deltaTime_;
    float elapsedTime_;
};
