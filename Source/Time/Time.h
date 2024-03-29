#pragma once

class GlobalTime {
public:
    GlobalTime();
    static constexpr float MAX_DELTA_TIME = 1.0f / 15.0f;
    // Changing the timestep will affect accelerations, but velocities will still have the same effect
    // This is done so speeds in fixed timestep will line up with speeds in the unlocked timestep
    // ie. cameraSpeed 30 = entitySpeed 30
    static constexpr int TIMESTEP_RATE = 60;
    static constexpr float TIMESTEP = 1.0f / TIMESTEP_RATE;

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
