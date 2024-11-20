#include "Beat.h"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>

//=============================================================================
// BEAT
//=============================================================================

Beat::Beat(float beatLength_, Sampler *sampler_, int beatSampleIdx_) {
    // REST
    if (beatSampleIdx_ == -1) {
        // NO PLAY
    }

    this->beatLength_ = beatLength_;
    this->sampler_ = sampler_;
    this->beatSampleIdx_ = beatSampleIdx_;
}

Beat::~Beat() {

}

void Beat::Play() {
    if (sampler_->Loaded())
        sampler_->Play( beatSampleIdx_);
}

int Beat::GetBeatSampleIdx() const {
    return beatSampleIdx_;
}

