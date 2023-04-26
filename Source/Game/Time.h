#pragma once

class Time {
public:
    static void Init();
    static void UpdateTime();
    static float GetDeltaTime();
    static float GetTime();

private:
    static Time* time_;
    long long lastTimeUSec_;
    long long currentTimeUSec_;
    float deltaTime_;
    float elapsedTime_;
};
