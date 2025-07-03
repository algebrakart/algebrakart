#include "Sampler.h"
#include "Sequencer.h"
#include "Beat.h"

#include "Urho3D/Resource/ResourceCache.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>


//=============================================================================
// SAMPLER
//=============================================================================

void Sampler::RegisterObject(Context *context)
{
    context->RegisterFactory<Sampler>();
}

Sampler::Sampler(Context *context) : Object(context) {
}

Sampler::~Sampler() {
    if (playSource_)
        playSource_->ReleaseRef();
}

Vector<Sound*> Sampler::GetSampleQueue() {
    return sampleQueue_;
}

void Sampler::Reset(Scene* scene) {

    node_ = scene;
}

void Sampler::Play(int sampleIdx) {

    if (sampleQueue_.Size() <= sampleIdx)
        return;

// This is where we play a sample
/*
    if (sampleQueue_[sampleIdx] != nullptr) {
        auto *cache = GetSubsystem<ResourceCache>();
        auto *source = node_->CreateComponent<SoundSource3D>(LOCAL);

        source->SetNearDistance(1);  // distance up to where the volume is 100%
        source->SetFarDistance(6000);  // distance from where the volume is at 0%
        source->SetSoundType(SOUND_EFFECT);
//        source->SetGain(0.2);
        source->SetGain(0.002);


        // Play sample
        source->SetAutoRemoveMode(REMOVE_COMPONENT);
        source->Play(sampleQueue_[sampleIdx]);
    }*/
}

void Sampler::Load(Sound* sample) {
    // Store sample
    sampleQueue_.Push(sample);
    loaded_ = true;
}

void Sampler::Stop() {

}

bool Sampler::Loaded() {
    return loaded_;
}

void Sampler::SetPlaySource(SoundSource3D *playSource) {
    playSource_ = playSource;
}