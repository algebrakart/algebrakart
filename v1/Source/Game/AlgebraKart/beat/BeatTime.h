#pragma once

#ifndef URHO3D_TIME_H
#define URHO3D_TIME_H

#include <Urho3D/Container/Vector.h>

using namespace Urho3D;
class BeatTime {

protected:
    float currTime_;
    float beatTime_;
    float barTime_;

public:
    BeatTime(float currTime_, float beatTime_, float barTime_);
    ~BeatTime();

    float GetCurrentTime();
    float GetBeatTime();
    float GetBarTime();
};


#endif //URHO3D_TIME_H
