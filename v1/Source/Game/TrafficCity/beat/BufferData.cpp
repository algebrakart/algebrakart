#include "BufferData.h"
#include "BeatTime.h"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>

//=============================================================================
// BUFFER DATA
//=============================================================================

void BufferData::RegisterObject(Context *context)
{
    context->RegisterFactory<BufferData>();
}

BufferData::BufferData(Context *context) : Object(context), onLongStore_(false) {
}

BufferData::~BufferData() {

}


void BufferData::SetData(Beat *channel1_, Beat *channel2_, Beat *channel3_, BeatTime *time_) {

    Vector<Beat*> beatData;
    beatData.Push(channel1_);
    beatData.Push(channel2_);
    beatData.Push(channel3_);
    bufferBeat_.Populate(time_, beatData);

    Vector<BeatTime*> timeData;
    timeData.Push(time_);
    bufferTime_.Populate(time_, timeData);
}

HashMap<BeatTime*,Vector<Beat*>> BufferData::GetBeatData() {
    return bufferBeat_;
}

HashMap<BeatTime*,Vector<BeatTime*>> BufferData::GetTimeData() {
    return bufferTime_;
}

void BufferData::SetOnLongStore(bool onLongStore) {
    onLongStore_ = onLongStore;
}

bool BufferData::IsOnLongStore() const {
    return onLongStore_;
}
