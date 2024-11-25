#pragma once

#ifndef MM_RECORDER_H
#define MM_RECORDER_H

#include <Urho3D/Container/Vector.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Scene/LogicComponent.h>

//s#define algebrakart_SERVER

#ifdef MAYASCAPE_SERVER
#include <Urho3D/Database/Database.h>
#else
// Skip Urho3D database defs for client
#endif

#include "Beat.h"
#include "BufferData.h"
#include "Urho3D/Core/Context.h"

using namespace Urho3D;

/*
  __   __
 /  \/  \
/__/ /__/ AYA SCAPE

 BEAT: RECORDER

Written by Ajay Bhaga 2022
*/
class Recorder : public Object {
URHO3D_OBJECT(Recorder, Object);
protected:
    String id_;
    long seqId_ = 0;
public:
    const String &GetId() const;

protected:
    bool odbcConnected_;
    SharedPtr<SoundSource3D> playSource_;

#ifdef MAYASCAPE_SERVER
    SharedPtr<Database> db_;
    SharedPtr<DbConnection> cxn_;
#endif
    String schema_;
    int currSeqId_;
    bool sequence_;

    Context* storeContext_;
    Vector<SharedPtr<BufferData>> data_;

    // Persist markers
    float pStartTime;
    float pEndTime;

public:

    Recorder(Context* context);
    ~Recorder();
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    void Reset(Context *context);
    void Capture(Beat* channel1_, Beat* channel2_, Beat* channel3_, float currTime_, float beatTime_, float barTime_);
    void SetPlaySource(SoundSource3D* playSource);
    int GetBufferSize();
    bool IsODBCConnected();
    void Persist();
    void CreateSequence(String name); // ms_sequence
    int GetSequence(); // ms_sequence
    void CreateTimeCode(float timeCode, float beat); // ms_time_code
    void CreatePattern(Beat* channel1_, Beat* channel2_, Beat* channel3_, float currTime_, float beatTime_, float barTime_); // ms_pattern
    void HandleDBCursor(StringHash eventType, VariantMap& eventData);
};



#endif //MM_RECORDER_H
