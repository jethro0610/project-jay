#pragma once
const float MAX_DELTA_TIME = 1.0f / 15.0f;

// Changing the timestep will affect accelerations, but velocities will still have the same effect
// This is done so speeds in fixed timestep will line up with speeds in the unlocked timestep
// ie. cameraSpeed 30 = entitySpeed 30
const uint8_t TIMESTEP_RATE = 60;
const float TIMESTEP = 1.0f / TIMESTEP_RATE;
