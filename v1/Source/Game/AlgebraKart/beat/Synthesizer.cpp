#include "Synthesizer.h"


#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Audio/BufferedSoundStream.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>


//=============================================================================
// SYNTHESIZER
//=============================================================================

void Synthesizer::RegisterObject(Context *context)
{
    context->RegisterFactory<Synthesizer>();
}

Synthesizer::Synthesizer(Context *context) : Object(context) {
    filter_ = 0.1f;
    accumulator_ = 0.0f;
    osc1_ = 0.0f;
    osc2_= 180.0f;
}

Synthesizer::~Synthesizer() {
    if (playSource_)
        playSource_->ReleaseRef();
}

Vector<Sound*> Synthesizer::GetSampleQueue() {
    return sampleQueue_;
}

void Synthesizer::Reset(Scene* scene) {
    node_ = scene;
}

void Synthesizer::Load(Sound* sample) {
    // Store sample
    sampleQueue_.Push(sample);
    loaded_ = true;
}

void Synthesizer::Stop() {

}

bool Synthesizer::Loaded() {
    return loaded_;
}

void Synthesizer::SetPlaySource(SoundSource3D *playSource) {
    playSource_ = playSource;
}

void Synthesizer::CreateSound()
{
    if (!soundStream_) {
        // Sound source needs a node so that it is considered enabled
        auto *source = node_->CreateComponent<SoundSource3D>(LOCAL);
        soundStream_ = new BufferedSoundStream();
        // Set format: 44100 Hz, sixteen bit, mono
        soundStream_->SetFormat(44100, true, false);


        source->SetNearDistance(1);  // distance up to where the volume is 100%
        source->SetFarDistance(6000);  // distance from where the volume is at 0%
        source->SetSoundType(SOUND_EFFECT);
        source->SetGain(0.001);
        source->SetAutoRemoveMode(REMOVE_COMPONENT);

        // Start playback. We don't have data in the stream yet, but the SoundSource will wait until there is data,
        // as the stream is by default in the "don't stop at end" mode
        source->Play(soundStream_);
    }
}

void Synthesizer::UpdateSound()
{
    // Try to keep 1/10 seconds of sound in the buffer, to avoid both dropouts and unnecessary latency
    float targetLength = 1.0f / 10.0f;
    float requiredLength = targetLength - soundStream_->GetBufferLength();
    if (requiredLength < 0.0f)
        return;

    auto numSamples = (unsigned)(soundStream_->GetFrequency() * requiredLength);
    if (!numSamples)
        return;

    // Allocate a new buffer and fill it with a simple two-oscillator algorithm. The sound is over-amplified
    // (distorted), clamped to the 16-bit range, and finally lowpass-filtered according to the coefficient
    SharedArrayPtr<signed short> newData(new signed short[numSamples]);
    for (unsigned i = 0; i < numSamples; ++i)
    {
        osc1_ = fmodf(osc1_ + 1.0f, 360.0f);
        osc2_ = fmodf(osc2_ + 1.002f, 360.0f);

        float newValue = Clamp((Sin(osc1_) + Sin(osc2_)) * 100000.0f, -32767.0f, 32767.0f);
        accumulator_ = Lerp(accumulator_, newValue, filter_);
        newData[i] = (int)accumulator_;
    }

    // Queue buffer to the stream for playback
    soundStream_->AddData(newData, numSamples * sizeof(signed short));
}
