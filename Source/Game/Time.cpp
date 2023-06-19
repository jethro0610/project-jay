#include <chrono>
#include <math.h>
#include "Time.h"
#include "../Constants/TimeConstants.h"

Time* Time::time_ = nullptr;

void Time::Init() {
    time_ = new Time();
    time_->deltaTime_ = 0.0f;
    time_->elapsedTime_ = 0.0f;
    time_->currentTimeUSec_ = duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    time_->lastTimeUSec_ = time_->currentTimeUSec_;
}

void Time::UpdateTime() {
    time_->lastTimeUSec_ = time_->currentTimeUSec_;
    time_->currentTimeUSec_ = duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    time_->deltaTime_ = (time_->currentTimeUSec_ - time_->lastTimeUSec_) * 0.000001f;
    time_->deltaTime_ = std::min<float>(MAX_DELTA_TIME, time_->deltaTime_);
    time_->elapsedTime_ += time_->deltaTime_;
}

float Time::GetDeltaTime() {
    return time_->deltaTime_;
}

float Time::GetTime() {
    return time_->elapsedTime_;
}
