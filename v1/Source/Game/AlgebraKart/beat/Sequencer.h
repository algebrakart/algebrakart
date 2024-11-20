#pragma once

#ifndef MM_SEQUENCER_H
#define MM_SEQUENCER_H

#include "Beat.h"
#include "Recorder.h"
#include "Synthesizer.h"
#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

/*
  __   __
 /  \/  \
/__/ /__/ AYA SCAPE

 BEAT: SEQUENCER

Written by Ajay Bhaga 2022
*/

class Sequencer : public LogicComponent {

    URHO3D_OBJECT(Sequencer, LogicComponent);
public:

protected:
    String id_;
    HashMap<String,Vector<Beat*>> sequenceByBeat_;
    HashMap<String,Beat*> sequenceByTime_;
    SharedPtr<Sampler> sampler_;
    SharedPtr<Synthesizer> synth_;
    int length_; // 16 beats
    SharedPtr<SoundSource3D> playSource_;

    // Current state
    int beat_; // Current beat
    float currTime_; // Current time
    float beatTime_; // Current beat time
    float barTime_; // Current bar time
    int bar_; // Current bar

    int bpm_; // Beats per minute
    int beatsPerBar_; // Beats per bar
    float beatTimeStep_; // Calculated beat time step

    float lastLongStoreWrite_; // Last buffer data to long store write

    bool isServer_;
    SharedPtr<Recorder> serverRec_;
    bool mute_;

public:
    // Mute playback
    void SetMute(bool mute);
    bool IsMute() const;


    Sequencer(Context* context);
    ~Sequencer();
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    /// Handle startup. Called by LogicComponent base class.
    void Start();
    /// Handle physics world update. Called by LogicComponent base class.
    void FixedUpdate(float timeStep);

    bool IsServer() const;
    void SetIsServer(bool isServer);

    const SharedPtr<Recorder> &GetServerRec() const;
    void SetServerRec(const SharedPtr<Recorder> &serverRec);

    const SharedPtr<SoundSource3D> &GetPlaySource() const;

    Vector<Beat*> GetSequence();
    Sampler *GetSampler();
    const String &GetId() const;
    void Reset();
    void Play(float timeStep, SharedPtr<Recorder> recorder_);
    int GetSize() const;
    int GetBeat() const;
    float GetTime() const;
    int GetBpm() const;
    int GetBeatsPerBar() const;
    float GetBeatTimeStep() const;
    void SetId(const String &id);
    float GetBeatTime() const;
    void SetPlaySource(SoundSource3D* playSource);
    void LoadSamples();
};


#endif //MM_SEQUENCER_H
