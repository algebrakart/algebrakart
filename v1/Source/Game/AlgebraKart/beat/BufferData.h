#pragma once

#ifndef URHO3D_BUFFERDATA_H
#define URHO3D_BUFFERDATA_H


#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#ifdef algebrakart_SERVER
#include <Urho3D/Database/Database.h>
#endif
#include "BeatTime.h"
#include "Beat.h"

using namespace Urho3D;

/*
  __   __
 /  \/  \
/__/ /__/ AYA SCAPE

 BEAT: BUFFER DATA

Written by Ajay Bhaga 2022
*/
class BufferData : public Object {
URHO3D_OBJECT(BufferData, Object);
protected:
    String id_;
    HashMap<BeatTime*,Vector<Beat*>> bufferBeat_;
    HashMap<BeatTime*,Vector<BeatTime*>> bufferTime_;
    bool onLongStore_;

public:

    BufferData(Context* context);
    ~BufferData();
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    void SetData(Beat* channel1_, Beat* channel2_, Beat* channel3_, BeatTime *time_);
    HashMap<BeatTime*,Vector<Beat*>> GetBeatData();
    HashMap<BeatTime*,Vector<BeatTime*>> GetTimeData();
    const String &GetId() const;
    void SetOnLongStore(bool onLongStore);
    bool IsOnLongStore() const;

};

#endif //URHO3D_BUFFERDATA_H