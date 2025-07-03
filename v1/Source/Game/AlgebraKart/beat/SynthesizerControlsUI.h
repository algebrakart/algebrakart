// Additional synthesizer controls and effects UI
// This complements the beat sequencer with synthesis parameters

#pragma once

#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Slider.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/DropDownList.h>

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

class SynthesizerControlsUI : public Object
{
URHO3D_OBJECT(SynthesizerControlsUI, Object);

public:
    SynthesizerControlsUI(Context* context);
    ~SynthesizerControlsUI();

    void Initialize();
    void SetVisible(bool visible);
    void UpdateOscilloscope(const Vector<float>& waveformData);
    void UpdateSynthParameters();
    const SharedPtr<Window> &GetMainWindow() const;

private:
    void CreateSynthWindow();
    void CreateOscillatorControls();
    void CreateFilterControls();
    void CreateEnvelopeControls();
    void CreateEffectsControls();
    void CreateOscilloscope();
    void CreatePresetControls();

    // Event handlers
    void HandleOscillatorChanged(StringHash eventType, VariantMap& eventData);
    void HandleFilterChanged(StringHash eventType, VariantMap& eventData);
    void HandleEnvelopeChanged(StringHash eventType, VariantMap& eventData);
    void HandleEffectChanged(StringHash eventType, VariantMap& eventData);
    void HandlePresetChanged(StringHash eventType, VariantMap& eventData);

    // UI Elements
    SharedPtr<Window> synthWindow_;

    // Oscillator controls
    SharedPtr<UIElement> oscillatorContainer_;
    SharedPtr<Slider> osc1FreqSlider_;
    SharedPtr<Slider> osc2FreqSlider_;
    SharedPtr<Slider> osc1AmpSlider_;
    SharedPtr<Slider> osc2AmpSlider_;
    SharedPtr<DropDownList> osc1WaveformList_;
    SharedPtr<DropDownList> osc2WaveformList_;
    SharedPtr<Slider> oscMixSlider_;

    // Filter controls
    SharedPtr<UIElement> filterContainer_;
    SharedPtr<Slider> filterCutoffSlider_;
    SharedPtr<Slider> filterResonanceSlider_;
    SharedPtr<DropDownList> filterTypeList_;
    SharedPtr<Slider> filterEnvAmountSlider_;

    // Envelope controls
    SharedPtr<UIElement> envelopeContainer_;
    SharedPtr<Slider> attackSlider_;
    SharedPtr<Slider> decaySlider_;
    SharedPtr<Slider> sustainSlider_;
    SharedPtr<Slider> releaseSlider_;

    // Effects controls
    SharedPtr<UIElement> effectsContainer_;
    SharedPtr<CheckBox> reverbCheckBox_;
    SharedPtr<Slider> reverbAmountSlider_;
    SharedPtr<CheckBox> delayCheckBox_;
    SharedPtr<Slider> delayTimeSlider_;
    SharedPtr<Slider> delayFeedbackSlider_;
    SharedPtr<CheckBox> distortionCheckBox_;
    SharedPtr<Slider> distortionAmountSlider_;

    // Oscilloscope
    SharedPtr<UIElement> oscilloscopeContainer_;
    Vector<SharedPtr<UIElement>> waveformPoints_;

    // Presets
    SharedPtr<UIElement> presetContainer_;
    SharedPtr<DropDownList> presetList_;
    SharedPtr<Button> savePresetButton_;
    SharedPtr<Button> loadPresetButton_;

    // Text labels
    Vector<SharedPtr<Text>> parameterLabels_;

    // Synthesis parameters
    struct SynthParams {
        float osc1Freq = 440.0f;
        float osc2Freq = 440.0f;
        float osc1Amp = 0.7f;
        float osc2Amp = 0.3f;
        int osc1Waveform = 0; // 0=sine, 1=saw, 2=square, 3=noise
        int osc2Waveform = 1;
        float oscMix = 0.5f;

        float filterCutoff = 1000.0f;
        float filterResonance = 0.5f;
        int filterType = 0; // 0=lowpass, 1=highpass, 2=bandpass
        float filterEnvAmount = 0.3f;

        float attack = 0.1f;
        float decay = 0.3f;
        float sustain = 0.7f;
        float release = 0.5f;

        bool reverbEnabled = false;
        float reverbAmount = 0.3f;
        bool delayEnabled = false;
        float delayTime = 0.25f;
        float delayFeedback = 0.4f;
        bool distortionEnabled = false;
        float distortionAmount = 0.2f;
    } synthParams_;
};