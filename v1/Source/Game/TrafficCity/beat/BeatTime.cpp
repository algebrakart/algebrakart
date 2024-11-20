#include "BeatTime.h"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>

//=============================================================================
// BEAT TIME
//=============================================================================

BeatTime::BeatTime(float currTime_, float beatTime_, float barTime_) {
    this->currTime_ = currTime_;
    this->beatTime_ = beatTime_;
    this->barTime_ = barTime_;
}

BeatTime::~BeatTime() {

}

float BeatTime::GetCurrentTime() {
    return currTime_;
}

float BeatTime::GetBeatTime() {
    return beatTime_;
}

float BeatTime::GetBarTime() {
    return barTime_;
}
