#pragma once

#ifndef MM_SAMPLER_H
#define MM_SAMPLER_H

#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

/*
  __   __
 /  \/  \
/__/ /__/ AYA SCAPE

 BEAT: SAMPLER

Written by Ajay Bhaga 2022
*/
class Sampler : public Object {
    URHO3D_OBJECT(Sampler, Object);
protected:
    String id_;
public:
    const String &GetId() const;

protected:
    Vector<Sound*> sampleQueue_;
    bool loaded_;
    /// Scene node for the sound component.
    SharedPtr<Node> node_;

    // Current state
    int playSampleIdx_; // Current beat
    float currTime_; // Current time
    float freqMod_; // Frequency modulator
    SharedPtr<SoundSource3D> playSource_;


public:

    Sampler(Context* context);
    ~Sampler();
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    Vector<Sound*> GetSampleQueue();
    void Reset(Scene* scene);
//    void Play(int sampleIdx);
    void Play(int sampleIdx);
    void Load(Sound* sample);
    void Stop();
    bool Loaded();
    void SetPlaySource(SoundSource3D* playSource);

};


#endif //MM_SAMPLER_H