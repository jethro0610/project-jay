#include "Time.h"
#include <chrono>

GlobalTime* GlobalTime::time_ = nullptr;

GlobalTime::GlobalTime() {
    time_ = this;
    deltaTime_ = 0.0f;
    elapsedTime_ = 0.0f;
    currentTimeUSec_ = duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
}   

void GlobalTime::UpdateTime() {
    float cummulativeDelta = 0.0f;

    while (cummulativeDelta < 1/90.0f) {
        time_->lastTimeUSec_ = time_->currentTimeUSec_;
        time_->currentTimeUSec_ = duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        float curDeltaTime = (time_->currentTimeUSec_ - time_->lastTimeUSec_) * 0.000001f;
        cummulativeDelta += curDeltaTime;
    }

    time_->deltaTime_ = cummulativeDelta;
    time_->deltaTime_ = std::min<float>(MAX_DELTA_TIME, time_->deltaTime_);
    time_->elapsedTime_ += time_->deltaTime_;
}

float GlobalTime::GetDeltaTime() {
    return time_->deltaTime_;
}

float GlobalTime::GetTime() {
    return time_->elapsedTime_;
}
