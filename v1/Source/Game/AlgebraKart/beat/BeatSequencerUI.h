#pragma once

#include <Urho3D/Engine/Application.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/BorderImage.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Slider.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Core/Context.h>

using namespace Urho3D;

class BeatSequencerUI : public Object
{
URHO3D_OBJECT(BeatSequencerUI, Object);

public:
    BeatSequencerUI(Context* context);
    ~BeatSequencerUI();

    /// Initialize the sequencer UI
    void Initialize();

    /// Update the UI with current sequencer state
    void UpdateSequencer(float timeStep, int currentBeat, float beatTime, bool isPlaying);

    /// Update audio spectrum visualization
    void UpdateSpectrum(const Vector<float>& spectrumData);

    /// Update VU meters
    void UpdateVUMeters(float leftLevel, float rightLevel);

    /// Set visibility
    void SetVisible(bool visible);

    /// Handle beat pad clicks
    void HandleBeatPadPressed(StringHash eventType, VariantMap& eventData);

    /// Handle transport controls
    void HandlePlayPressed(StringHash eventType, VariantMap& eventData);
    void HandleStopPressed(StringHash eventType, VariantMap& eventData);
    void HandleRecordPressed(StringHash eventType, VariantMap& eventData);

private:
    /// Create the main sequencer window
    void CreateSequencerWindow();

    /// Create the beat grid
    void CreateBeatGrid();

    /// Create transport controls
    void CreateTransportControls();

    /// Create channel strips
    void CreateChannelStrips();

    /// Create spectrum analyzer
    void CreateSpectrumAnalyzer();

    /// Create VU meters
    void CreateVUMeters();

    /// Update beat pad visuals
    void UpdateBeatPad(int channel, int step, bool active, bool current);

    /// Get color for channel
    Color GetChannelColor(int channel);

protected:
    // UI Elements
    SharedPtr<Window> mainWindow_;
public:
    const SharedPtr<Window> &GetMainWindow() const;

protected:
    SharedPtr<UIElement> beatGrid_;
    SharedPtr<UIElement> transportControls_;
    SharedPtr<UIElement> channelStrips_;
    SharedPtr<UIElement> spectrumContainer_;
    SharedPtr<UIElement> vuMeterContainer_;

    // Beat grid buttons (4 channels x 16 steps)
    Vector<Vector<SharedPtr<Button>>> beatPads_;

    // Transport controls
    SharedPtr<Button> playButton_;
    SharedPtr<Button> stopButton_;
    SharedPtr<Button> recordButton_;
    SharedPtr<Slider> tempoSlider_;
    SharedPtr<Text> tempoText_;

    // Channel controls
    Vector<SharedPtr<Slider>> volumeSliders_;
    Vector<SharedPtr<Button>> muteButtons_;
    Vector<SharedPtr<Button>> soloButtons_;
    Vector<SharedPtr<Text>> channelLabels_;

    // Spectrum analyzer bars
    Vector<SharedPtr<BorderImage>> spectrumBars_;

    // VU meter elements
    Vector<SharedPtr<BorderImage>> vuBarsLeft_;
    Vector<SharedPtr<BorderImage>> vuBarsRight_;
    SharedPtr<Text> vuLabelLeft_;
    SharedPtr<Text> vuLabelRight_;

    // Pattern data (4 channels x 16 steps)
    bool beatPattern_[4][16];

    // Current state
    int currentBeat_;
    bool isPlaying_;
    bool isRecording_;
    float tempo_;

    // Constants
    static const int NUM_CHANNELS = 4;
    static const int NUM_STEPS = 16;
    static const int NUM_SPECTRUM_BARS = 32;
    static const int NUM_VU_BARS = 20;

    // Channel names
    const String channelNames_[NUM_CHANNELS] = {
            "KICK", "SNARE", "HI-HAT", "SYNTH"
    };
};
