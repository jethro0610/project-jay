#include "ParticleEmitter.h"
#include <deque>
#include <tuple>
#include <unordered_map>

const int MAX_EMITTERS = 64;
const int NULL_EMITTER = -1;
typedef int EmitterID;

class ParticleManager {
public:
    ParticleManager();

    std::array<ParticleEmitter, MAX_EMITTERS> emitters_; 
    std::array<bool, MAX_EMITTERS> alive_;
    std::deque<EmitterID> usableEmitters_;

    std::unordered_map<std::string, EmitterProperties> emitterProps_;
    
    EmitterID RequestEmitter(std::string name);
    void ReleaseEmitter(EmitterID emitter);

    void Update(float deltaTime);
    void Emmit(EmitterID emitter);
    void SetActive(EmitterID emitter, bool active);
    void SetTransform(EmitterID emitter, const Transform& transform);
    void SetProperties(EmitterID emitter, std::string name);

    void LoadEmitterProperty(std::string name);
};
