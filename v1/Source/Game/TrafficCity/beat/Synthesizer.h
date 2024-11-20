#pragma once

#ifndef MM_SYNTHESIZER_H
#define MM_SYNTHESIZER_H


namespace Urho3D
{

    class Node;
    class BufferedSoundStream;

}

#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Audio/BufferedSoundStream.h>

using namespace Urho3D;

/*
  __   __
 /  \/  \
/__/ /__/ AYA SCAPE

 BEAT: SYNTHESIZER

Written by Ajay Bhaga 2022
*/
class Synthesizer : public Object {
URHO3D_OBJECT(Synthesizer, Object);
protected:
    String id_;
public:
    const String &GetId() const;

protected:
    Vector<Sound*> sampleQueue_;
    bool loaded_;

    // Current state
    int playSampleIdx_; // Current beat
    float currTime_; // Current time
    float freqMod_; // Frequency modulator
    SharedPtr<SoundSource3D> playSource_;

    /// Scene node for the sound component.
    SharedPtr<Node> node_;
    /// Sound stream that we update.
    SharedPtr<BufferedSoundStream> soundStream_;
    /// Filter coefficient for the sound.
    float filter_;
    /// Synthesis accumulator.
    float accumulator_;
    /// First oscillator.
    float osc1_;
    /// Second oscillator.
    float osc2_;

public:

    Synthesizer(Context* context);
    ~Synthesizer();
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    Vector<Sound*> GetSampleQueue();
    void Reset(Scene* scene);
    void Load(Sound* sample);
    void Stop();
    bool Loaded();
    void SetPlaySource(SoundSource3D* playSource);

    void CreateSound();
    void UpdateSound();

};

#endif //MM_SYNTHESIZER_H
