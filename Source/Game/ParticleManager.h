#pragma once
#include "ParticleEmitter.h"
#include <deque>
#include <tuple>
#include <unordered_map>

class Renderer;

const int MAX_EMITTERS = 64;

class ParticleManager {
public:
    ParticleManager();
    std::array<ParticleEmitter, MAX_EMITTERS> emitters_; 
    std::deque<int> usableEmitters_;

    std::unordered_map<std::string, EmitterProperties> emitterProps_;
    
    ParticleEmitter* RequestEmitter(std::string name);

    void Update(float deltaTime);
    void LoadEmitterProperty(std::string name, Renderer& renderer);
};
