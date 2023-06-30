#include <chrono>
#include <math.h>
#include "Time.h"
#include "../Constants/TimeConstants.h"

GlobalTime* GlobalTime::time_ = nullptr;

void GlobalTime::Init() {
    time_ = new GlobalTime();
    time_->deltaTime_ = 0.0f;
    time_->elapsedTime_ = 0.0f;
    time_->currentTimeUSec_ = duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    time_->lastTimeUSec_ = time_->currentTimeUSec_;
}

void GlobalTime::UpdateTime() {
    time_->lastTimeUSec_ = time_->currentTimeUSec_;
    time_->currentTimeUSec_ = duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    time_->deltaTime_ = (time_->currentTimeUSec_ - time_->lastTimeUSec_) * 0.000001f;
    time_->deltaTime_ = std::min<float>(MAX_DELTA_TIME, time_->deltaTime_);
    time_->elapsedTime_ += time_->deltaTime_;
}

float GlobalTime::GetDeltaTime() {
    return time_->deltaTime_;
}

float GlobalTime::GetTime() {
    return time_->elapsedTime_;
}
