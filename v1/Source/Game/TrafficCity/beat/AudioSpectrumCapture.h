#pragma once

#ifndef MM_AUDIOSPECTRUMCAPTURE_H
#define MM_AUDIOSPECTRUMCAPTURE_H

#include <vector>
#include <string>
#include <iostream>

using namespace Urho3D;



#include <Urho3D/Container/Vector.h>
//=============================================================================
// AUDIO SPECTRUM CAPTURE
//=============================================================================

// Derived from https://github.com/agehama/minimal_spectrum_analyzer
// Agehama (https://twitter.com/agehama_)
class AudioSpectrumCapture
{
public:

    AudioSpectrumCapture() = default;

    AudioSpectrumCapture(size_t inputSampleSize)
    {
        init(inputSampleSize);
    }

    ~AudioSpectrumCapture()
    {
        data->buffer->Clear();
        if (data->buffer)
            delete data->buffer;

        if (data)
            delete data;

    }

    void init(size_t inputSampleSize)
    {
        data = new SoundData();
        data->buffer = new Vector<float>();
        data->buffer->Resize(inputSampleSize);
    }

    Vector<float>* getBuffer() const
    {
        return data->buffer;
    }

    size_t bufferHeadIndex() const
    {
        return data->bufferHeadIndex;
    }

    size_t bufferReadCount() const
    {
        return data->readCount;
    }

    struct SoundData
    {
        Vector<float>* buffer = nullptr; // Sound buffer
        size_t bufferHeadIndex = 0;
        size_t readCount = 0;

    };

    SoundData* data; // Sound data
};

#endif // MM_AUDIOSPECTRUMCAPTURE_H