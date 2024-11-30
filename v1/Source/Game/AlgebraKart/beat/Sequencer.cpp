#include "Sequencer.h"
#include "Beat.h"

#include <vector>
#include <string>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Container/HashMap.h>

//=============================================================================
// SEQUENCER
//=============================================================================

#define SAMPLE_REST -1
#define SAMPLE_KICK 0
#define SAMPLE_SNARE 1
#define SAMPLE_HH 2

//#define LONG_STORE_WRITE_TIME 120.0f
//#define LONG_STORE_WRITE_TIME 30.0f
#define LONG_STORE_WRITE_TIME MAXFLOAT // infinite, no long store logging

std::vector<std::string> SAMPLE_PACK = {
        "samples/SAMPLE1-BASSKICK.wav",
        "samples/SAMPLE1-SNARE.wav",
        "samples/SAMPLE1-HH.wav"
};


void Sequencer::RegisterObject(Context *context)
{
    context->RegisterFactory<Sequencer>();

    URHO3D_ATTRIBUTE("id", String, id_, String::EMPTY, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("mute", bool, mute_, false, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("currTime", bool, currTime_, true, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("beatTime", bool, beatTime_, true, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("barTime", bool, barTime_, true, AM_DEFAULT | AM_NET);

    //URHO3D_ATTRIBUTE("Color Index", int, colorIdx_, 0, AM_DEFAULT | AM_NET);
    //URHO3D_ATTRIBUTE("Position", Vector3, position_, Vector3(0,0,0), AM_DEFAULT | AM_NET);
}

void Sequencer::Start()
{

    auto *cache = GetSubsystem<ResourceCache>();
    playSource_ = node_->CreateComponent<SoundSource3D>(LOCAL);
    playSource_->SetNearDistance(1);  // distance up to where the volume is 100%
    playSource_->SetFarDistance(6000);  // distance from where the volume is at 0%
    playSource_->SetSoundType(SOUND_MUSIC);
    sampler_->SetPlaySource(playSource_);
    //synth_->SetPlaySource(playSource_);
    beatTime_ = 0.0f;

}


void Sequencer::FixedUpdate(float timeStep)
{
    // SERVER & CLIENT CODE
    SharedPtr<Recorder> recorder_ = nullptr;

    if (isServer_) {
        recorder_ = serverRec_;
    }

    // Sequencer update

    // Play on fixed update time step
    Play(timeStep, recorder_);
}

Sequencer::Sequencer(Context *context) : LogicComponent(context), length_(16) {

    // Setup client sequencer

    // Init parameters
    Reset();

    // Create a new sampler
    sampler_ = context->CreateObject<Sampler>();

    // Create a new synth
    //synth_ = context->CreateObject<Synthesizer>();

    int idx;

    // Default mute playback
//    mute_ = true;
    mute_ = false;

    // Generate sequence -> instruction set to time beat

    // Clear sequence
    sequenceByBeat_.Clear();

    Vector<Beat*> channel_;

    // Generate drum sequence
    for (int i = 0; i < length_; i++) {
        if ((i % 4) == 0) {
            idx = SAMPLE_KICK;
        } else {
            idx = SAMPLE_REST;
        }
        Beat *b = new Beat(1 / beatsPerBar_, sampler_, idx);
        channel_.Push(b);
    }
    sequenceByBeat_.Populate("KICK",channel_);
    channel_.Clear();

    for (int i = 0; i < length_; i++) {
        if ((i % 3) == 0) {
            idx = SAMPLE_SNARE;
        } else {
            idx = SAMPLE_REST;
        }
        Beat *b = new Beat(1 / beatsPerBar_, sampler_, idx);
        channel_.Push(b);
    }
    sequenceByBeat_.Populate("SNARE",channel_);
    channel_.Clear();

    for (int i = 0; i < length_; i++) {
        idx = 2;
        Beat *b = new Beat(1 / beatsPerBar_, sampler_, idx);
        channel_.Push(b);
    }
    sequenceByBeat_.Populate("HH",channel_);



    URHO3D_LOGDEBUGF("**SEQUENCER ** -> beatsPerBar_=%d,beatTimeStep_=%f", beatsPerBar_, beatTimeStep_);

    // Register
    SetUpdateEventMask(USE_FIXEDUPDATE);

    // Load default samples
    LoadSamples();

}

void Sequencer::SetId(const String &id) {
    id_ = id;
}

Sequencer::~Sequencer() {
    if (playSource_)
        playSource_->ReleaseRef();

    //if (synth_)
    //    synth_->ReleaseRef();

    if (sampler_)
        sampler_->ReleaseRef();

}

void Sequencer::Reset() {
    currTime_ = 0.0f;
    barTime_ = 0.0f;
    bar_ = 0;
    beat_ = 0;
    bpm_ = 90;
    lastLongStoreWrite_ = 0.0f;
    URHO3D_LOGDEBUGF("**SEQUENCER [%s] ** RESTART -> time %f, bar %d, beat %d", id_.CString(), currTime_, bar_, beat_);

    // Quarter note by default
    beatsPerBar_ = 4;

    // 1 s = 1000 ms by beats in bar = beat time
    //beatTimeStep_ = (1.0f / beatsPerBar_) * 4.0f; // 60 bpm
    beatTimeStep_ = (1.0f / beatsPerBar_) * 2.0f; // 120 bpm
    // 8/4 beats/sec -> 2 beats/sec
    // 4/4 beats/sec -> 1 beats/sec -> 60 beats in min -> 60 bpm

    // 4/4 = 250 ms time segment

}

void Sequencer::LoadSamples() {
    // LOAD DEFAULT SAMPLES INTO SAMPLER

    // Load sequencer samples for client
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    String prefix = "Sounds/";
    String soundName;
    Sound *sample;
    soundName = SAMPLE_PACK[SAMPLE_KICK].c_str();
    sample = cache->GetResource<Sound>(prefix + soundName);
    sampler_->Load(sample);

    soundName = SAMPLE_PACK[SAMPLE_SNARE].c_str();
    sample = cache->GetResource<Sound>(prefix + soundName);
    sampler_->Load(sample);

    soundName = SAMPLE_PACK[SAMPLE_HH].c_str();
    sample = cache->GetResource<Sound>(prefix + soundName);
    sampler_->Load(sample);

}

// Sequencer Play: Move forward a time step
void Sequencer::Play(float timeStep, SharedPtr<Recorder> recorder_) {

    // Assigns scene node to sound modules for creating SoundSource
    sampler_->Reset(GetScene());
    //synth_->Reset(GetScene());

    // Create base sound in buffer, if not existing
    //synth_->CreateSound();

    // Sequencer moves forward
    // Mapping of instruction set to timeStep (beat)

    // Play a time step
    currTime_ += timeStep;
    beatTime_ += timeStep;
    barTime_ += timeStep;

    // Update sound buffer
    //synth_->UpdateSound();


    // If time accumulation is past
    if (beatTime_ >= beatTimeStep_) {
        beat_++; // Increment beat

        if (!mute_) {
            // Play beat sample

            Vector<Beat *> channel1_, channel2_, channel3_;
            // Play each channel

            //URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER ** -> recorder_->GetBufferSize()=%d", recorder_->GetBufferSize());

            // KICK
            channel1_ = sequenceByBeat_.Find("KICK")->second_;

            // SNARE
            channel2_ = sequenceByBeat_.Find("SNARE")->second_;

            // HI-HAT
            channel3_ = sequenceByBeat_.Find("HH")->second_;

/*            URHO3D_LOGDEBUGF(
                    "[%f] ** SEQUENCER [%s]: PLAY ** [timeStep: %f, beat: %d] -> channel1=%d, channel2=%d, channel3=%d",
                    currTime_, id_.CString(), timeStep, beat_, channel1_[beat_]->GetBeatSampleIdx(),
                    channel2_[beat_]->GetBeatSampleIdx(), channel3_[beat_]->GetBeatSampleIdx());*/

            if (channel1_[beat_]->GetBeatSampleIdx() > SAMPLE_REST) {
                channel1_[beat_]->Play();
               // URHO3D_LOGDEBUG("** SEQUENCER: SAMPLE PLAY ** -> CHANNEL 1 [KICK]");

            }

            if (channel2_[beat_]->GetBeatSampleIdx() > SAMPLE_REST) {
                channel2_[beat_]->Play();
              //  URHO3D_LOGDEBUG("** SEQUENCER: SAMPLE PLAY ** -> CHANNEL 2 [SNARE]");
            }

            if (channel3_[beat_]->GetBeatSampleIdx() > SAMPLE_REST) {
                channel3_[beat_]->Play();
              //  URHO3D_LOGDEBUG("** SEQUENCER: SAMPLE PLAY ** -> CHANNEL 3 [HH]");
            }


            if (recorder_) {
                // CAPTURE RECORDING to memory buffer - short store
                recorder_->Capture(channel1_[beat_], channel2_[beat_], channel3_[beat_], currTime_, beatTime_,
                                   barTime_);

                // After 10 seconds persist
                if ((currTime_ - lastLongStoreWrite_) > LONG_STORE_WRITE_TIME) {
                    // Call persist to write to long store
                    recorder_->Persist();
                    // Store last write
                    lastLongStoreWrite_ = currTime_;
                }
            }
        }

        //URHO3D_LOGDEBUGF("** SEQUENCER: RECORDER ** -> recorder_->GetBufferSize()=%d", recorder_->GetBufferSize());

        if (beat_ > beatsPerBar_) {
            // Over bar limit
            bar_++;
            beat_ = 1;
            barTime_ = 0.0f;
        } else {
 /*           URHO3D_LOGDEBUGF("**SEQUENCER [%s] ** -> time %f, beat time: %f, bar time: %f, bar %d, beat %d", id_.CString(), currTime_,
                             beatTime_, barTime_, bar_, beat_);*/
        }

        beatTime_ = 0.0f; // Reset timer
    }
}

float Sequencer::GetBeatTime() const {
    return beatTime_;
}

int Sequencer::GetSize() const {
    return length_;
}

int Sequencer::GetBeat() const {
    return beat_;
}

float Sequencer::GetTime() const {
    return currTime_;
}

int Sequencer::GetBpm() const {
    return bpm_;
}

int Sequencer::GetBeatsPerBar() const {
    return beatsPerBar_;
}

float Sequencer::GetBeatTimeStep() const {
    return beatTimeStep_;
}

const String &Sequencer::GetId() const {
    return id_;
}

Sampler *Sequencer::GetSampler() {
    return sampler_;
}

void Sequencer::SetPlaySource(SoundSource3D* playSource) {
    playSource_ = WeakPtr(playSource);
}

bool Sequencer::IsServer() const {
    return isServer_;
}

void Sequencer::SetIsServer(bool isServer) {
    isServer_ = isServer;
}

const SharedPtr<Recorder> &Sequencer::GetServerRec() const {
    return serverRec_;
}

void Sequencer::SetServerRec(const SharedPtr<Recorder> &serverRec) {
    serverRec_ = serverRec;
}

void Sequencer::SetMute(bool mute) {
    mute_ = mute;
}

bool Sequencer::IsMute() const {
    return mute_;
}

const SharedPtr<SoundSource3D> &Sequencer::GetPlaySource() const {
    return playSource_;
}
