#pragma once
#include "ParticleEmitter.h"
#include <array>
#include <deque>

class Terrain;

const int MAX_EMITTERS = 64;

class ParticleManager {
public:
    ParticleManager(Terrain& terrain);
    std::array<ParticleEmitter, MAX_EMITTERS> emitters_; 
    std::deque<int> usableEmitters_;
    
    ParticleEmitter* RequestEmitter();

    void Update(float deltaTime);

    void Reset();

private:
    Terrain& terrain_;
};
