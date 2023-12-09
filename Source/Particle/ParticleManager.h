#pragma once
#include "ParticleEmitter.h"
#include <array>
#include <deque>

class Renderer;

const int MAX_EMITTERS = 64;

class ParticleManager {
public:
    ParticleManager();
    std::array<ParticleEmitter, MAX_EMITTERS> emitters_; 
    std::deque<int> usableEmitters_;
    
    ParticleEmitter* RequestEmitter(EmitterProperties* properties);

    void Update(float deltaTime);
};
