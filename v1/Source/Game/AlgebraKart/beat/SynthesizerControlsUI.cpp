#include "SynthesizerControlsUI.h"
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>



SynthesizerControlsUI::SynthesizerControlsUI(Context* context) :
        Object(context)
{
}

SynthesizerControlsUI::~SynthesizerControlsUI()
{
}

void SynthesizerControlsUI::Initialize()
{
    CreateSynthWindow();
    CreateOscillatorControls();
    CreateFilterControls();
    CreateEnvelopeControls();
    CreateEffectsControls();
    CreateOscilloscope();
    //CreatePresetControls();
}

void SynthesizerControlsUI::CreateSynthWindow()
{
    auto* ui = GetSubsystem<UI>();
    auto* graphics = GetSubsystem<Graphics>();
    auto* cache = GetSubsystem<ResourceCache>();

    // Create main synthesizer window
    synthWindow_ = new Window(context_);
    ui->GetRoot()->AddChild(synthWindow_);

    synthWindow_->SetName("SynthesizerControls");
    synthWindow_->SetSize(600, 700);
    synthWindow_->SetPosition(20, 20);
    synthWindow_->SetOpacity(0.95f);
    synthWindow_->SetMovable(true);
    synthWindow_->SetResizable(true);
    synthWindow_->SetLayout(LM_VERTICAL, 10, IntRect(10, 30, 10, 10));

    // Window title
    auto* titleBar = synthWindow_->CreateChild<UIElement>();
    titleBar->SetMinHeight(25);
    titleBar->SetMaxHeight(25);

    auto* titleText = titleBar->CreateChild<Text>();
    titleText->SetText("SYNTHESIZER CONTROLS");
    titleText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 14);
    titleText->SetColor(Color::WHITE);
    titleText->SetAlignment(HA_CENTER, VA_CENTER);

    synthWindow_->SetStyleAuto();
    synthWindow_->SetColor(Color(0.05f, 0.05f, 0.1f, 0.9f));
}

void SynthesizerControlsUI::CreateOscillatorControls()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create oscillator section
    oscillatorContainer_ = synthWindow_->CreateChild<UIElement>();
    oscillatorContainer_->SetLayoutMode(LM_VERTICAL);
    oscillatorContainer_->SetLayoutSpacing(5);
    oscillatorContainer_->SetMinHeight(150);

    // Section title
    auto* oscTitle = oscillatorContainer_->CreateChild<Text>();
    oscTitle->SetText("OSCILLATORS");
    oscTitle->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    oscTitle->SetColor(Color::CYAN);

    // Oscillator 1 controls
    auto* osc1Row = oscillatorContainer_->CreateChild<UIElement>();
    osc1Row->SetLayoutMode(LM_HORIZONTAL);
    osc1Row->SetLayoutSpacing(10);

    auto* osc1Label = osc1Row->CreateChild<Text>();
    osc1Label->SetText("OSC1:");
    osc1Label->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    osc1Label->SetColor(Color::WHITE);
    osc1Label->SetMinWidth(50);

    // Frequency slider
    auto* freqContainer1 = osc1Row->CreateChild<UIElement>();
    freqContainer1->SetLayoutMode(LM_VERTICAL);
    freqContainer1->SetMinWidth(100);

    auto* freqLabel1 = freqContainer1->CreateChild<Text>();
    freqLabel1->SetText("Freq: 440Hz");
    freqLabel1->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    freqLabel1->SetColor(Color::GRAY);

    osc1FreqSlider_ = freqContainer1->CreateChild<Slider>();
    osc1FreqSlider_->SetName("Osc1FreqSlider");
    osc1FreqSlider_->SetMinHeight(20);
    osc1FreqSlider_->SetRange(880.0f); // 20Hz to 900Hz (logarithmic would be better)
    osc1FreqSlider_->SetValue(420.0f); // 440Hz
    osc1FreqSlider_->SetStyleAuto();

    // Amplitude slider
    auto* ampContainer1 = osc1Row->CreateChild<UIElement>();
    ampContainer1->SetLayoutMode(LM_VERTICAL);
    ampContainer1->SetMinWidth(100);

    auto* ampLabel1 = ampContainer1->CreateChild<Text>();
    ampLabel1->SetText("Amp: 0.7");
    ampLabel1->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    ampLabel1->SetColor(Color::GRAY);

    osc1AmpSlider_ = ampContainer1->CreateChild<Slider>();
    osc1AmpSlider_->SetName("Osc1AmpSlider");
    osc1AmpSlider_->SetMinHeight(20);
    osc1AmpSlider_->SetRange(1.0f);
    osc1AmpSlider_->SetValue(0.7f);
    osc1AmpSlider_->SetStyleAuto();

    // Waveform selector
    auto* waveContainer1 = osc1Row->CreateChild<UIElement>();
    waveContainer1->SetLayoutMode(LM_VERTICAL);
    waveContainer1->SetMinWidth(80);

    auto* waveLabel1 = waveContainer1->CreateChild<Text>();
    waveLabel1->SetText("Wave:");
    waveLabel1->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    waveLabel1->SetColor(Color::GRAY);

    osc1WaveformList_ = waveContainer1->CreateChild<DropDownList>();
    osc1WaveformList_->SetName("Osc1WaveformList");
    osc1WaveformList_->SetMinHeight(20);
    osc1WaveformList_->SetStyleAuto();

    // Add waveform options
    const String waveforms[] = {"Sine", "Saw", "Square", "Noise"};
    for (int i = 0; i < 4; i++) {
        auto* item = new Text(context_);
        item->SetText(waveforms[i]);
        item->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
        item->SetColor(Color::WHITE);
        osc1WaveformList_->AddItem(item);
    }
    osc1WaveformList_->SetSelection(0);

    // Oscillator 2 controls (similar structure)
    auto* osc2Row = oscillatorContainer_->CreateChild<UIElement>();
    osc2Row->SetLayoutMode(LM_HORIZONTAL);
    osc2Row->SetLayoutSpacing(10);

    auto* osc2Label = osc2Row->CreateChild<Text>();
    osc2Label->SetText("OSC2:");
    osc2Label->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    osc2Label->SetColor(Color::WHITE);
    osc2Label->SetMinWidth(50);

    // Similar controls for OSC2...
    auto* freqContainer2 = osc2Row->CreateChild<UIElement>();
    freqContainer2->SetLayoutMode(LM_VERTICAL);
    freqContainer2->SetMinWidth(100);

    auto* freqLabel2 = freqContainer2->CreateChild<Text>();
    freqLabel2->SetText("Freq: 440Hz");
    freqLabel2->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    freqLabel2->SetColor(Color::GRAY);

    osc2FreqSlider_ = freqContainer2->CreateChild<Slider>();
    osc2FreqSlider_->SetName("Osc2FreqSlider");
    osc2FreqSlider_->SetMinHeight(20);
    osc2FreqSlider_->SetRange(880.0f);
    osc2FreqSlider_->SetValue(420.0f);
    osc2FreqSlider_->SetStyleAuto();

    // OSC Mix slider
    auto* mixRow = oscillatorContainer_->CreateChild<UIElement>();
    mixRow->SetLayoutMode(LM_HORIZONTAL);
    mixRow->SetLayoutSpacing(10);

    auto* mixLabel = mixRow->CreateChild<Text>();
    mixLabel->SetText("MIX:");
    mixLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    mixLabel->SetColor(Color::WHITE);
    mixLabel->SetMinWidth(50);

    oscMixSlider_ = mixRow->CreateChild<Slider>();
    oscMixSlider_->SetName("OscMixSlider");
    oscMixSlider_->SetMinHeight(20);
    oscMixSlider_->SetRange(1.0f);
    oscMixSlider_->SetValue(0.5f);
    oscMixSlider_->SetStyleAuto();

    // Subscribe to events
    SubscribeToEvent(osc1FreqSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleOscillatorChanged));
    SubscribeToEvent(osc2FreqSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleOscillatorChanged));
    SubscribeToEvent(oscMixSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleOscillatorChanged));
}

void SynthesizerControlsUI::CreateFilterControls()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create filter section
    filterContainer_ = synthWindow_->CreateChild<UIElement>();
    filterContainer_->SetLayoutMode(LM_VERTICAL);
    filterContainer_->SetLayoutSpacing(5);
    filterContainer_->SetMinHeight(100);

    // Section title
    auto* filterTitle = filterContainer_->CreateChild<Text>();
    filterTitle->SetText("FILTER");
    filterTitle->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    filterTitle->SetColor(Color::YELLOW);

    // Filter controls row
    auto* filterRow = filterContainer_->CreateChild<UIElement>();
    filterRow->SetLayoutMode(LM_HORIZONTAL);
    filterRow->SetLayoutSpacing(10);

    // Cutoff frequency
    auto* cutoffContainer = filterRow->CreateChild<UIElement>();
    cutoffContainer->SetLayoutMode(LM_VERTICAL);
    cutoffContainer->SetMinWidth(120);

    auto* cutoffLabel = cutoffContainer->CreateChild<Text>();
    cutoffLabel->SetText("Cutoff: 1000Hz");
    cutoffLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    cutoffLabel->SetColor(Color::GRAY);

    filterCutoffSlider_ = cutoffContainer->CreateChild<Slider>();
    filterCutoffSlider_->SetName("FilterCutoffSlider");
    filterCutoffSlider_->SetMinHeight(20);
    filterCutoffSlider_->SetRange(20000.0f); // 20Hz to 20kHz
    filterCutoffSlider_->SetValue(1000.0f);
    filterCutoffSlider_->SetStyleAuto();

    // Resonance
    auto* resonanceContainer = filterRow->CreateChild<UIElement>();
    resonanceContainer->SetLayoutMode(LM_VERTICAL);
    resonanceContainer->SetMinWidth(120);

    auto* resonanceLabel = resonanceContainer->CreateChild<Text>();
    resonanceLabel->SetText("Resonance: 0.5");
    resonanceLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    resonanceLabel->SetColor(Color::GRAY);

    filterResonanceSlider_ = resonanceContainer->CreateChild<Slider>();
    filterResonanceSlider_->SetName("FilterResonanceSlider");
    filterResonanceSlider_->SetMinHeight(20);
    filterResonanceSlider_->SetRange(1.0f);
    filterResonanceSlider_->SetValue(0.5f);
    filterResonanceSlider_->SetStyleAuto();

    // Filter type
    auto* typeContainer = filterRow->CreateChild<UIElement>();
    typeContainer->SetLayoutMode(LM_VERTICAL);
    typeContainer->SetMinWidth(100);

    auto* typeLabel = typeContainer->CreateChild<Text>();
    typeLabel->SetText("Type:");
    typeLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    typeLabel->SetColor(Color::GRAY);

    filterTypeList_ = typeContainer->CreateChild<DropDownList>();
    filterTypeList_->SetName("FilterTypeList");
    filterTypeList_->SetMinHeight(20);
    filterTypeList_->SetStyleAuto();

    const String filterTypes[] = {"Low Pass", "High Pass", "Band Pass"};
    for (int i = 0; i < 3; i++) {
        auto* item = new Text(context_);
        item->SetText(filterTypes[i]);
        item->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
        item->SetColor(Color::WHITE);
        filterTypeList_->AddItem(item);
    }
    filterTypeList_->SetSelection(0);

    SubscribeToEvent(filterCutoffSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleFilterChanged));
    SubscribeToEvent(filterResonanceSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleFilterChanged));
    SubscribeToEvent(filterTypeList_, E_ITEMSELECTED, URHO3D_HANDLER(SynthesizerControlsUI, HandleFilterChanged));
}

void SynthesizerControlsUI::CreateEnvelopeControls()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create envelope section
    envelopeContainer_ = synthWindow_->CreateChild<UIElement>();
    envelopeContainer_->SetLayoutMode(LM_VERTICAL);
    envelopeContainer_->SetLayoutSpacing(5);
    envelopeContainer_->SetMinHeight(100);

    // Section title
    auto* envTitle = envelopeContainer_->CreateChild<Text>();
    envTitle->SetText("ENVELOPE (ADSR)");
    envTitle->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    envTitle->SetColor(Color::GREEN);

    // ADSR controls
    auto* adsrRow = envelopeContainer_->CreateChild<UIElement>();
    adsrRow->SetLayoutMode(LM_HORIZONTAL);
    adsrRow->SetLayoutSpacing(10);

    // Attack
    auto* attackContainer = adsrRow->CreateChild<UIElement>();
    attackContainer->SetLayoutMode(LM_VERTICAL);
    attackContainer->SetMinWidth(100);

    auto* attackLabel = attackContainer->CreateChild<Text>();
    attackLabel->SetText("Attack: 0.1s");
    attackLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    attackLabel->SetColor(Color::GRAY);

    attackSlider_ = attackContainer->CreateChild<Slider>();
    attackSlider_->SetName("AttackSlider");
    attackSlider_->SetMinHeight(20);
    attackSlider_->SetRange(2.0f); // 0-2 seconds
    attackSlider_->SetValue(0.1f);
    attackSlider_->SetStyleAuto();

    // Decay
    auto* decayContainer = adsrRow->CreateChild<UIElement>();
    decayContainer->SetLayoutMode(LM_VERTICAL);
    decayContainer->SetMinWidth(100);

    auto* decayLabel = decayContainer->CreateChild<Text>();
    decayLabel->SetText("Decay: 0.3s");
    decayLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    decayLabel->SetColor(Color::GRAY);

    decaySlider_ = decayContainer->CreateChild<Slider>();
    decaySlider_->SetName("DecaySlider");
    decaySlider_->SetMinHeight(20);
    decaySlider_->SetRange(2.0f);
    decaySlider_->SetValue(0.3f);
    decaySlider_->SetStyleAuto();

    // Sustain
    auto* sustainContainer = adsrRow->CreateChild<UIElement>();
    sustainContainer->SetLayoutMode(LM_VERTICAL);
    sustainContainer->SetMinWidth(100);

    auto* sustainLabel = sustainContainer->CreateChild<Text>();
    sustainLabel->SetText("Sustain: 0.7");
    sustainLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    sustainLabel->SetColor(Color::GRAY);

    sustainSlider_ = sustainContainer->CreateChild<Slider>();
    sustainSlider_->SetName("SustainSlider");
    sustainSlider_->SetMinHeight(20);
    sustainSlider_->SetRange(1.0f);
    sustainSlider_->SetValue(0.7f);
    sustainSlider_->SetStyleAuto();

    // Release
    auto* releaseContainer = adsrRow->CreateChild<UIElement>();
    releaseContainer->SetLayoutMode(LM_VERTICAL);
    releaseContainer->SetMinWidth(100);

    auto* releaseLabel = releaseContainer->CreateChild<Text>();
    releaseLabel->SetText("Release: 0.5s");
    releaseLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    releaseLabel->SetColor(Color::GRAY);

    releaseSlider_ = releaseContainer->CreateChild<Slider>();
    releaseSlider_->SetName("ReleaseSlider");
    releaseSlider_->SetMinHeight(20);
    releaseSlider_->SetRange(3.0f);
    releaseSlider_->SetValue(0.5f);
    releaseSlider_->SetStyleAuto();

    SubscribeToEvent(attackSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEnvelopeChanged));
    SubscribeToEvent(decaySlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEnvelopeChanged));
    SubscribeToEvent(sustainSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEnvelopeChanged));
    SubscribeToEvent(releaseSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEnvelopeChanged));
}

void SynthesizerControlsUI::CreateEffectsControls()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create effects section
    effectsContainer_ = synthWindow_->CreateChild<UIElement>();
    effectsContainer_->SetLayoutMode(LM_VERTICAL);
    effectsContainer_->SetLayoutSpacing(5);
    effectsContainer_->SetMinHeight(120);

    // Section title
    auto* effectsTitle = effectsContainer_->CreateChild<Text>();
    effectsTitle->SetText("EFFECTS");
    effectsTitle->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    effectsTitle->SetColor(Color::MAGENTA);

    // Reverb
    auto* reverbRow = effectsContainer_->CreateChild<UIElement>();
    reverbRow->SetLayoutMode(LM_HORIZONTAL);
    reverbRow->SetLayoutSpacing(10);

    reverbCheckBox_ = reverbRow->CreateChild<CheckBox>();
    reverbCheckBox_->SetName("ReverbCheckBox");
    reverbCheckBox_->SetStyleAuto();

    auto* reverbLabel = reverbRow->CreateChild<Text>();
    reverbLabel->SetText("Reverb");
    reverbLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    reverbLabel->SetColor(Color::WHITE);
    reverbLabel->SetMinWidth(60);

    reverbAmountSlider_ = reverbRow->CreateChild<Slider>();
    reverbAmountSlider_->SetName("ReverbAmountSlider");
    reverbAmountSlider_->SetMinHeight(20);
    reverbAmountSlider_->SetRange(1.0f);
    reverbAmountSlider_->SetValue(0.3f);
    reverbAmountSlider_->SetStyleAuto();

    // Delay
    auto* delayRow = effectsContainer_->CreateChild<UIElement>();
    delayRow->SetLayoutMode(LM_HORIZONTAL);
    delayRow->SetLayoutSpacing(10);

    delayCheckBox_ = delayRow->CreateChild<CheckBox>();
    delayCheckBox_->SetName("DelayCheckBox");
    delayCheckBox_->SetStyleAuto();

    auto* delayLabel = delayRow->CreateChild<Text>();
    delayLabel->SetText("Delay");
    delayLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    delayLabel->SetColor(Color::WHITE);
    delayLabel->SetMinWidth(60);

    delayTimeSlider_ = delayRow->CreateChild<Slider>();
    delayTimeSlider_->SetName("DelayTimeSlider");
    delayTimeSlider_->SetMinHeight(20);
    delayTimeSlider_->SetRange(1.0f); // 0-1 second
    delayTimeSlider_->SetValue(0.25f);
    delayTimeSlider_->SetStyleAuto();

    delayFeedbackSlider_ = delayRow->CreateChild<Slider>();
    delayFeedbackSlider_->SetName("DelayFeedbackSlider");
    delayFeedbackSlider_->SetMinHeight(20);
    delayFeedbackSlider_->SetRange(0.9f); // 0-90% feedback
    delayFeedbackSlider_->SetValue(0.4f);
    delayFeedbackSlider_->SetStyleAuto();

    // Distortion
    auto* distortionRow = effectsContainer_->CreateChild<UIElement>();
    distortionRow->SetLayoutMode(LM_HORIZONTAL);
    distortionRow->SetLayoutSpacing(10);

    distortionCheckBox_ = distortionRow->CreateChild<CheckBox>();
    distortionCheckBox_->SetName("DistortionCheckBox");
    distortionCheckBox_->SetStyleAuto();

    auto* distortionLabel = distortionRow->CreateChild<Text>();
    distortionLabel->SetText("Distortion");
    distortionLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    distortionLabel->SetColor(Color::WHITE);
    distortionLabel->SetMinWidth(60);

    distortionAmountSlider_ = distortionRow->CreateChild<Slider>();
    distortionAmountSlider_->SetName("DistortionAmountSlider");
    distortionAmountSlider_->SetMinHeight(20);
    distortionAmountSlider_->SetRange(1.0f);
    distortionAmountSlider_->SetValue(0.2f);
    distortionAmountSlider_->SetStyleAuto();

    SubscribeToEvent(reverbCheckBox_, E_TOGGLED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEffectChanged));
    SubscribeToEvent(delayCheckBox_, E_TOGGLED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEffectChanged));
    SubscribeToEvent(distortionCheckBox_, E_TOGGLED, URHO3D_HANDLER(SynthesizerControlsUI, HandleEffectChanged));
}

void SynthesizerControlsUI::CreateOscilloscope()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create oscilloscope section
    oscilloscopeContainer_ = synthWindow_->CreateChild<UIElement>();
    oscilloscopeContainer_->SetLayoutMode(LM_VERTICAL);
    oscilloscopeContainer_->SetLayoutSpacing(5);
    oscilloscopeContainer_->SetMinHeight(150);

    // Section title
    auto* scopeTitle = oscilloscopeContainer_->CreateChild<Text>();
    scopeTitle->SetText("WAVEFORM DISPLAY");
    scopeTitle->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    scopeTitle->SetColor(Color::WHITE);

    // Waveform display area
    auto* waveformArea = oscilloscopeContainer_->CreateChild<UIElement>();
    waveformArea->SetMinHeight(120);
    waveformArea->SetLayoutMode(LM_FREE);
    waveformArea->SetClipBorder(IntRect(2, 2, 2, 2));

    // Create waveform points
    const int numPoints = 200;
    waveformPoints_.Resize(numPoints);

    for (int i = 0; i < numPoints; i++) {
        waveformPoints_[i] = waveformArea->CreateChild<UIElement>();
        waveformPoints_[i]->SetSize(2, 2);
        waveformPoints_[i]->SetColor(Color::GREEN);
        waveformPoints_[i]->SetPosition((float)i * 2.5f, 60.0f); // Center line
    }
}

void SynthesizerControlsUI::SetVisible(bool visible)
{
    if (synthWindow_) {
        synthWindow_->SetVisible(visible);
    }
}

void SynthesizerControlsUI::UpdateOscilloscope(const Vector<float>& waveformData)
{
    if (waveformData.Empty() || waveformPoints_.Empty()) return;

    int dataSize = waveformData.Size();
    int pointsSize = waveformPoints_.Size();

    // Display the waveform
    for (int i = 0; i < pointsSize; i++) {
        int dataIndex = (i * dataSize) / pointsSize;
        float sample = Clamp(waveformData[dataIndex], -1.0f, 1.0f);

        // Convert to screen coordinates
        float y = 60.0f - (sample * 50.0f); // Center at 60, ±50 range

        waveformPoints_[i]->SetPosition((float)i * 2.5f, y);

        // Color based on amplitude
        float intensity = Abs(sample);
        Color pointColor = Color::GREEN.Lerp(Color::RED, intensity);
        waveformPoints_[i]->SetColor(pointColor);
    }
}

// Event handlers
void SynthesizerControlsUI::HandleOscillatorChanged(StringHash eventType, VariantMap& eventData)
{
    UpdateSynthParameters();

    // Send parameters to synthesizer
    VariantMap synthEventData;
    synthEventData["Osc1Freq"] = synthParams_.osc1Freq;
    synthEventData["Osc2Freq"] = synthParams_.osc2Freq;
    synthEventData["Osc1Amp"] = synthParams_.osc1Amp;
    synthEventData["Osc2Amp"] = synthParams_.osc2Amp;
    synthEventData["OscMix"] = synthParams_.oscMix;
    SendEvent("SynthOscillatorChanged", synthEventData);
}

void SynthesizerControlsUI::HandleFilterChanged(StringHash eventType, VariantMap& eventData)
{
    UpdateSynthParameters();

    VariantMap synthEventData;
    synthEventData["FilterCutoff"] = synthParams_.filterCutoff;
    synthEventData["FilterResonance"] = synthParams_.filterResonance;
    synthEventData["FilterType"] = synthParams_.filterType;
    SendEvent("SynthFilterChanged", synthEventData);
}

void SynthesizerControlsUI::HandleEnvelopeChanged(StringHash eventType, VariantMap& eventData)
{
    UpdateSynthParameters();

    VariantMap synthEventData;
    synthEventData["Attack"] = synthParams_.attack;
    synthEventData["Decay"] = synthParams_.decay;
    synthEventData["Sustain"] = synthParams_.sustain;
    synthEventData["Release"] = synthParams_.release;
    SendEvent("SynthEnvelopeChanged", synthEventData);
}

void SynthesizerControlsUI::HandleEffectChanged(StringHash eventType, VariantMap& eventData)
{
    UpdateSynthParameters();

    VariantMap synthEventData;
    synthEventData["ReverbEnabled"] = synthParams_.reverbEnabled;
    synthEventData["ReverbAmount"] = synthParams_.reverbAmount;
    synthEventData["DelayEnabled"] = synthParams_.delayEnabled;
    synthEventData["DelayTime"] = synthParams_.delayTime;
    synthEventData["DelayFeedback"] = synthParams_.delayFeedback;
    synthEventData["DistortionEnabled"] = synthParams_.distortionEnabled;
    synthEventData["DistortionAmount"] = synthParams_.distortionAmount;
    SendEvent("SynthEffectsChanged", synthEventData);
}

void SynthesizerControlsUI::UpdateSynthParameters()
{
    // Update parameters from UI controls
    if (osc1FreqSlider_) {
        synthParams_.osc1Freq = osc1FreqSlider_->GetValue() + 20.0f; // 20Hz to 900Hz
        // Update label
        if (auto* label = osc1FreqSlider_->GetParent()->GetChild("Text", true)) {
            static_cast<Text*>(label)->SetText("Freq: " + String((int)synthParams_.osc1Freq) + "Hz");
        }
    }

    if (osc2FreqSlider_) {
        synthParams_.osc2Freq = osc2FreqSlider_->GetValue() + 20.0f;
    }

    if (oscMixSlider_) {
        synthParams_.oscMix = oscMixSlider_->GetValue();
    }

    if (filterCutoffSlider_) {
        synthParams_.filterCutoff = filterCutoffSlider_->GetValue() + 20.0f;
        if (auto* label = filterCutoffSlider_->GetParent()->GetChild("Text", true)) {
            static_cast<Text*>(label)->SetText("Cutoff: " + String((int)synthParams_.filterCutoff) + "Hz");
        }
    }

    if (filterResonanceSlider_) {
        synthParams_.filterResonance = filterResonanceSlider_->GetValue();
    }

    if (attackSlider_) {
        synthParams_.attack = attackSlider_->GetValue();
        if (auto* label = attackSlider_->GetParent()->GetChild("Text", true)) {
            static_cast<Text*>(label)->SetText("Attack: " + String(synthParams_.attack, 2) + "s");
        }
    }

    if (decaySlider_) {
        synthParams_.decay = decaySlider_->GetValue();
    }

    if (sustainSlider_) {
        synthParams_.sustain = sustainSlider_->GetValue();
    }

    if (releaseSlider_) {
        synthParams_.release = releaseSlider_->GetValue();
    }

    // Effects
    if (reverbCheckBox_) {
        synthParams_.reverbEnabled = reverbCheckBox_->IsChecked();
    }

    if (reverbAmountSlider_) {
        synthParams_.reverbAmount = reverbAmountSlider_->GetValue();
    }

    // Similar updates for other effects...
}

const SharedPtr<Window> &SynthesizerControlsUI::GetMainWindow() const {
    return synthWindow_;
}
