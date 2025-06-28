#include "BeatSequencerUI.h"
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <cstdlib>

// Implementation
BeatSequencerUI::BeatSequencerUI(Context* context) :
        Object(context),
        currentBeat_(0),
        isPlaying_(false),
        isRecording_(false),
        tempo_(120.0f)
{
    // Initialize pattern
    for (int i = 0; i < NUM_CHANNELS; i++) {
        for (int j = 0; j < NUM_STEPS; j++) {
            beatPattern_[i][j] = false;
        }
    }

    // Set up some default patterns
    // Kick on 1, 5, 9, 13
    beatPattern_[0][0] = beatPattern_[0][4] = beatPattern_[0][8] = beatPattern_[0][12] = true;

    // Snare on 5, 13
    beatPattern_[1][4] = beatPattern_[1][12] = true;

    // Hi-hat on all steps
    for (int i = 0; i < NUM_STEPS; i++) {
        beatPattern_[2][i] = true;
    }
}

BeatSequencerUI::~BeatSequencerUI()
{
}

void BeatSequencerUI::Initialize()
{
    CreateSequencerWindow();
    CreateBeatGrid();
    CreateTransportControls();
    CreateChannelStrips();
    CreateSpectrumAnalyzer();
    CreateVUMeters();
}

void BeatSequencerUI::CreateSequencerWindow()
{
    auto* ui = GetSubsystem<UI>();
    auto* graphics = GetSubsystem<Graphics>();
    auto* cache = GetSubsystem<ResourceCache>();

    // Create main sequencer window
    mainWindow_ = new Window(context_);
    ui->GetRoot()->AddChild(mainWindow_);

    mainWindow_->SetName("BeatSequencer");
    mainWindow_->SetSize(800, 500);
    mainWindow_->SetPosition(graphics->GetWidth() - 820, 20);
    mainWindow_->SetOpacity(0.95f);
    mainWindow_->SetMovable(true);
    mainWindow_->SetResizable(true);
    mainWindow_->SetLayout(LM_VERTICAL, 10, IntRect(10, 30, 10, 10));

    // Window title
    auto* titleBar = mainWindow_->CreateChild<UIElement>();
    titleBar->SetMinHeight(25);
    titleBar->SetMaxHeight(25);

    auto* titleText = titleBar->CreateChild<Text>();
    titleText->SetText("BEAT SEQUENCER v2.0");
    titleText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 14);
    titleText->SetColor(Color::WHITE);
    titleText->SetAlignment(HA_CENTER, VA_CENTER);

    // Set window style
    mainWindow_->SetStyleAuto();
    mainWindow_->SetColor(Color(0.1f, 0.1f, 0.15f, 0.9f));
}

void BeatSequencerUI::CreateBeatGrid()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create beat grid container
    beatGrid_ = mainWindow_->CreateChild<UIElement>();
    beatGrid_->SetLayoutMode(LM_VERTICAL);
    beatGrid_->SetLayoutSpacing(2);
    beatGrid_->SetMinHeight(200);

    // Resize beat pads array
    beatPads_.Resize(NUM_CHANNELS);
    for (int i = 0; i < NUM_CHANNELS; i++) {
        beatPads_[i].Resize(NUM_STEPS);
    }

    // Create header with step numbers
    auto* headerRow = beatGrid_->CreateChild<UIElement>();
    headerRow->SetLayoutMode(LM_HORIZONTAL);
    headerRow->SetLayoutSpacing(2);
    headerRow->SetMinHeight(20);

    // Channel label space
    auto* emptyLabel = headerRow->CreateChild<Text>();
    emptyLabel->SetText("");
    emptyLabel->SetMinWidth(80);

    // Step numbers
    for (int step = 0; step < NUM_STEPS; step++) {
        auto* stepLabel = headerRow->CreateChild<Text>();
        stepLabel->SetText(String(step + 1));
        stepLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
        stepLabel->SetColor(Color::GRAY);
        stepLabel->SetMinWidth(40);
        stepLabel->SetTextAlignment(HA_CENTER);
    }

    // Create rows for each channel
    for (int channel = 0; channel < NUM_CHANNELS; channel++) {
        auto* channelRow = beatGrid_->CreateChild<UIElement>();
        channelRow->SetLayoutMode(LM_HORIZONTAL);
        channelRow->SetLayoutSpacing(2);
        channelRow->SetMinHeight(40);

        // Channel label
        auto* channelLabel = channelRow->CreateChild<Text>();
        channelLabel->SetText(channelNames_[channel]);
        channelLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
        channelLabel->SetColor(GetChannelColor(channel));
        channelLabel->SetMinWidth(80);
        channelLabel->SetTextAlignment(HA_CENTER);
        channelLabel->SetVerticalAlignment(VA_CENTER);

        // Beat pads for this channel
        for (int step = 0; step < NUM_STEPS; step++) {
            auto* beatPad = channelRow->CreateChild<Button>();
            beatPad->SetName(String("BeatPad_") + String(channel) + "_" + String(step));
            beatPad->SetMinSize(35, 35);
            beatPad->SetMaxSize(35, 35);

            // Set initial appearance
            UpdateBeatPad(channel, step, beatPattern_[channel][step], false);

            // Store reference
            beatPads_[channel][step] = beatPad;

            // Subscribe to click events
            SubscribeToEvent(beatPad, E_RELEASED, URHO3D_HANDLER(BeatSequencerUI, HandleBeatPadPressed));
        }
    }
}

void BeatSequencerUI::CreateTransportControls()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create transport container
    transportControls_ = mainWindow_->CreateChild<UIElement>();
    transportControls_->SetLayoutMode(LM_HORIZONTAL);
    transportControls_->SetLayoutSpacing(10);
    transportControls_->SetMinHeight(50);

    // Play button
    playButton_ = transportControls_->CreateChild<Button>();
    playButton_->SetName("PlayButton");
    playButton_->SetMinSize(60, 40);
    auto* playText = playButton_->CreateChild<Text>();
    playText->SetText("PLAY");
    playText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    playText->SetAlignment(HA_CENTER, VA_CENTER);
    playText->SetColor(Color::WHITE);
    playButton_->SetStyleAuto();
    SubscribeToEvent(playButton_, E_RELEASED, URHO3D_HANDLER(BeatSequencerUI, HandlePlayPressed));

    // Stop button
    stopButton_ = transportControls_->CreateChild<Button>();
    stopButton_->SetName("StopButton");
    stopButton_->SetMinSize(60, 40);
    auto* stopText = stopButton_->CreateChild<Text>();
    stopText->SetText("STOP");
    stopText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    stopText->SetAlignment(HA_CENTER, VA_CENTER);
    stopText->SetColor(Color::WHITE);
    stopButton_->SetStyleAuto();
    SubscribeToEvent(stopButton_, E_RELEASED, URHO3D_HANDLER(BeatSequencerUI, HandleStopPressed));

    // Record button
    recordButton_ = transportControls_->CreateChild<Button>();
    recordButton_->SetName("RecordButton");
    recordButton_->SetMinSize(60, 40);
    auto* recordText = recordButton_->CreateChild<Text>();
    recordText->SetText("REC");
    recordText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    recordText->SetAlignment(HA_CENTER, VA_CENTER);
    recordText->SetColor(Color::WHITE);
    recordButton_->SetStyleAuto();
    SubscribeToEvent(recordButton_, E_RELEASED, URHO3D_HANDLER(BeatSequencerUI, HandleRecordPressed));

    // Tempo controls
    auto* tempoContainer = transportControls_->CreateChild<UIElement>();
    tempoContainer->SetLayoutMode(LM_VERTICAL);
    tempoContainer->SetMinWidth(120);

    tempoText_ = tempoContainer->CreateChild<Text>();
    tempoText_->SetText("BPM: 120");
    tempoText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    tempoText_->SetColor(Color::WHITE);

    tempoSlider_ = tempoContainer->CreateChild<Slider>();
    tempoSlider_->SetName("TempoSlider");
    tempoSlider_->SetMinHeight(20);
    tempoSlider_->SetRange(60.0f);  // 60-180 BPM
    tempoSlider_->SetValue(60.0f);  // 120 BPM (60 offset)
    tempoSlider_->SetStyleAuto();
}

void BeatSequencerUI::CreateChannelStrips()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create channel strips container
    channelStrips_ = mainWindow_->CreateChild<UIElement>();
    channelStrips_->SetLayoutMode(LM_HORIZONTAL);
    channelStrips_->SetLayoutSpacing(5);
    channelStrips_->SetMinHeight(100);

    // Resize arrays
    volumeSliders_.Resize(NUM_CHANNELS);
    muteButtons_.Resize(NUM_CHANNELS);
    soloButtons_.Resize(NUM_CHANNELS);
    channelLabels_.Resize(NUM_CHANNELS);

    for (int i = 0; i < NUM_CHANNELS; i++) {
        auto* channelStrip = channelStrips_->CreateChild<UIElement>();
        channelStrip->SetLayoutMode(LM_VERTICAL);
        channelStrip->SetLayoutSpacing(5);
        channelStrip->SetMinWidth(80);

        // Channel label
        channelLabels_[i] = channelStrip->CreateChild<Text>();
        channelLabels_[i]->SetText(channelNames_[i]);
        channelLabels_[i]->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
        channelLabels_[i]->SetColor(GetChannelColor(i));
        channelLabels_[i]->SetTextAlignment(HA_CENTER);

        // Volume slider
        volumeSliders_[i] = channelStrip->CreateChild<Slider>();
        volumeSliders_[i]->SetName(String("VolumeSlider_") + String(i));
        volumeSliders_[i]->SetOrientation(O_VERTICAL);
        volumeSliders_[i]->SetMinHeight(60);
        volumeSliders_[i]->SetRange(1.0f);
        volumeSliders_[i]->SetValue(0.8f);
        volumeSliders_[i]->SetStyleAuto();

        // Button container
        auto* buttonContainer = channelStrip->CreateChild<UIElement>();
        buttonContainer->SetLayoutMode(LM_HORIZONTAL);
        buttonContainer->SetLayoutSpacing(2);

        // Mute button
        muteButtons_[i] = buttonContainer->CreateChild<Button>();
        muteButtons_[i]->SetName(String("MuteButton_") + String(i));
        muteButtons_[i]->SetMinSize(25, 20);
        auto* muteText = muteButtons_[i]->CreateChild<Text>();
        muteText->SetText("M");
        muteText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
        muteText->SetAlignment(HA_CENTER, VA_CENTER);
        muteButtons_[i]->SetStyleAuto();

        // Solo button
        soloButtons_[i] = buttonContainer->CreateChild<Button>();
        soloButtons_[i]->SetName(String("SoloButton_") + String(i));
        soloButtons_[i]->SetMinSize(25, 20);
        auto* soloText = soloButtons_[i]->CreateChild<Text>();
        soloText->SetText("S");
        soloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
        soloText->SetAlignment(HA_CENTER, VA_CENTER);
        soloButtons_[i]->SetStyleAuto();
    }
}

void BeatSequencerUI::CreateSpectrumAnalyzer()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create spectrum container
    spectrumContainer_ = mainWindow_->CreateChild<UIElement>();
    spectrumContainer_->SetLayoutMode(LM_HORIZONTAL);
    spectrumContainer_->SetLayoutSpacing(1);
    spectrumContainer_->SetMinHeight(80);
    spectrumContainer_->SetMaxHeight(80);

    // Create spectrum analyzer title
    auto* spectrumTitle = spectrumContainer_->CreateChild<Text>();
    spectrumTitle->SetText("SPECTRUM");
    spectrumTitle->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    spectrumTitle->SetColor(Color::WHITE);
    spectrumTitle->SetMinWidth(80);
    spectrumTitle->SetTextAlignment(HA_CENTER);
    spectrumTitle->SetVerticalAlignment(VA_TOP);

    // Create spectrum bars container
    auto* barsContainer = spectrumContainer_->CreateChild<UIElement>();
    barsContainer->SetLayoutMode(LM_HORIZONTAL);
    barsContainer->SetLayoutSpacing(1);

    // Create spectrum bars
    spectrumBars_.Resize(NUM_SPECTRUM_BARS);
    for (int i = 0; i < NUM_SPECTRUM_BARS; i++) {
        spectrumBars_[i] = barsContainer->CreateChild<BorderImage>();
        spectrumBars_[i]->SetMinWidth(15);
        spectrumBars_[i]->SetMaxWidth(15);
        spectrumBars_[i]->SetMinHeight(5);
        spectrumBars_[i]->SetVerticalAlignment(VA_BOTTOM);

        // Color gradient from green to red
        float ratio = (float)i / NUM_SPECTRUM_BARS;
        Color barColor;
        if (ratio < 0.5f) {
            barColor = Color::GREEN.Lerp(Color::YELLOW, ratio * 2.0f);
        } else {
            barColor = Color::YELLOW.Lerp(Color::RED, (ratio - 0.5f) * 2.0f);
        }
        spectrumBars_[i]->SetColor(barColor);
    }
}

void BeatSequencerUI::CreateVUMeters()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create VU meter container
    vuMeterContainer_ = mainWindow_->CreateChild<UIElement>();
    vuMeterContainer_->SetLayoutMode(LM_HORIZONTAL);
    vuMeterContainer_->SetLayoutSpacing(10);
    vuMeterContainer_->SetMinHeight(80);
    vuMeterContainer_->SetMaxHeight(80);

    // Left channel VU meter
    auto* leftContainer = vuMeterContainer_->CreateChild<UIElement>();
    leftContainer->SetLayoutMode(LM_VERTICAL);
    leftContainer->SetMinWidth(30);

    vuLabelLeft_ = leftContainer->CreateChild<Text>();
    vuLabelLeft_->SetText("L");
    vuLabelLeft_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    vuLabelLeft_->SetColor(Color::WHITE);
    vuLabelLeft_->SetTextAlignment(HA_CENTER);

    auto* leftBarsContainer = leftContainer->CreateChild<UIElement>();
    leftBarsContainer->SetLayoutMode(LM_VERTICAL);
    leftBarsContainer->SetLayoutSpacing(1);

    vuBarsLeft_.Resize(NUM_VU_BARS);
    for (int i = 0; i < NUM_VU_BARS; i++) {
        vuBarsLeft_[NUM_VU_BARS - 1 - i] = leftBarsContainer->CreateChild<BorderImage>();
        vuBarsLeft_[NUM_VU_BARS - 1 - i]->SetMinSize(25, 3);
        vuBarsLeft_[NUM_VU_BARS - 1 - i]->SetMaxSize(25, 3);

        // Color gradient
        Color barColor;
        if (i < NUM_VU_BARS * 0.7f) {
            barColor = Color::GREEN;
        } else if (i < NUM_VU_BARS * 0.9f) {
            barColor = Color::YELLOW;
        } else {
            barColor = Color::RED;
        }
        vuBarsLeft_[NUM_VU_BARS - 1 - i]->SetColor(barColor);
    }

    // Right channel VU meter
    auto* rightContainer = vuMeterContainer_->CreateChild<UIElement>();
    rightContainer->SetLayoutMode(LM_VERTICAL);
    rightContainer->SetMinWidth(30);

    vuLabelRight_ = rightContainer->CreateChild<Text>();
    vuLabelRight_->SetText("R");
    vuLabelRight_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    vuLabelRight_->SetColor(Color::WHITE);
    vuLabelRight_->SetTextAlignment(HA_CENTER);

    auto* rightBarsContainer = rightContainer->CreateChild<UIElement>();
    rightBarsContainer->SetLayoutMode(LM_VERTICAL);
    rightBarsContainer->SetLayoutSpacing(1);

    vuBarsRight_.Resize(NUM_VU_BARS);
    for (int i = 0; i < NUM_VU_BARS; i++) {
        vuBarsRight_[NUM_VU_BARS - 1 - i] = rightBarsContainer->CreateChild<BorderImage>();
        vuBarsRight_[NUM_VU_BARS - 1 - i]->SetMinSize(25, 3);
        vuBarsRight_[NUM_VU_BARS - 1 - i]->SetMaxSize(25, 3);

        // Color gradient
        Color barColor;
        if (i < NUM_VU_BARS * 0.7f) {
            barColor = Color::GREEN;
        } else if (i < NUM_VU_BARS * 0.9f) {
            barColor = Color::YELLOW;
        } else {
            barColor = Color::RED;
        }
        vuBarsRight_[NUM_VU_BARS - 1 - i]->SetColor(barColor);
    }
}

void BeatSequencerUI::UpdateSequencer(float timeStep, int currentBeat, float beatTime, bool isPlaying)
{
    currentBeat_ = currentBeat % NUM_STEPS;
    isPlaying_ = isPlaying;

    // Update beat pad visuals
    for (int channel = 0; channel < NUM_CHANNELS; channel++) {
        for (int step = 0; step < NUM_STEPS; step++) {
            bool isCurrent = (step == currentBeat_) && isPlaying_;
            UpdateBeatPad(channel, step, beatPattern_[channel][step], isCurrent);
        }
    }

    // Update transport button states
    if (isPlaying_) {
        playButton_->GetChild("Text", true)->SetColor(Color::GREEN);
        stopButton_->GetChild("Text", true)->SetColor(Color::WHITE);
    } else {
        playButton_->GetChild("Text", true)->SetColor(Color::WHITE);
        stopButton_->GetChild("Text", true)->SetColor(Color::WHITE);
    }

    if (isRecording_) {
        recordButton_->GetChild("Text", true)->SetColor(Color::RED);
    } else {
        recordButton_->GetChild("Text", true)->SetColor(Color::WHITE);
    }

    // Update tempo display
    if (tempoSlider_) {
        tempo_ = tempoSlider_->GetValue() + 60.0f;
        tempoText_->SetText("BPM: " + String((int)tempo_));
    }
}

void BeatSequencerUI::UpdateSpectrum(const Vector<float>& spectrumData)
{
    if (spectrumData.Empty() || spectrumBars_.Empty()) return;

    int dataSize = spectrumData.Size();
    for (int i = 0; i < NUM_SPECTRUM_BARS && i < dataSize; i++) {
        float level = Clamp(spectrumData[i], 0.0f, 1.0f);
        int height = (int)(level * 75.0f) + 5;
        spectrumBars_[i]->SetMaxHeight(height);
        spectrumBars_[i]->SetMinHeight(height);

        // Update opacity based on level
        Color currentColor = spectrumBars_[i]->GetColor(Corner::C_TOPLEFT);
        currentColor.a_ = 0.3f + (level * 0.7f);
        spectrumBars_[i]->SetColor(currentColor);
    }
}

void BeatSequencerUI::UpdateVUMeters(float leftLevel, float rightLevel)
{
    leftLevel = Clamp(leftLevel, 0.0f, 1.0f);
    rightLevel = Clamp(rightLevel, 0.0f, 1.0f);

    int leftBars = (int)(leftLevel * NUM_VU_BARS);
    int rightBars = (int)(rightLevel * NUM_VU_BARS);

    // Update left channel
    for (int i = 0; i < NUM_VU_BARS; i++) {
        vuBarsLeft_[i]->SetVisible(i < leftBars);
    }

    // Update right channel
    for (int i = 0; i < NUM_VU_BARS; i++) {
        vuBarsRight_[i]->SetVisible(i < rightBars);
    }
}

void BeatSequencerUI::UpdateBeatPad(int channel, int step, bool active, bool current)
{
    if (channel >= NUM_CHANNELS || step >= NUM_STEPS) return;

    Urho3D::SharedPtr<Urho3D::Button> button = beatPads_[channel][step];
    if (!button) return;

    Color baseColor = GetChannelColor(channel);

    if (current) {
        // Current beat - bright white/yellow
        button->SetColor(Color::YELLOW);
    } else if (active) {
        // Active beat - channel color
        button->SetColor(baseColor);
    } else {
        // Inactive beat - dark gray
        button->SetColor(Color(0.2f, 0.2f, 0.2f, 0.8f));
    }

    // Emphasize quarter notes
    if (step % 4 == 0) {
        // Make quarter note boundaries slightly brighter
        Color currentColor = button->GetColor(Corner::C_TOPLEFT);
        currentColor = currentColor * 1.1f;
        button->SetColor(currentColor);
    }
}

Color BeatSequencerUI::GetChannelColor(int channel)
{
    switch (channel) {
        case 0: return Color::RED;      // Kick
        case 1: return Color::BLUE;     // Snare
        case 2: return Color::GREEN;    // Hi-hat
        case 3: return Color::MAGENTA;  // Synth
        default: return Color::WHITE;
    }
}

void BeatSequencerUI::SetVisible(bool visible)
{
    if (mainWindow_) {
        mainWindow_->SetVisible(visible);
    }
}

void BeatSequencerUI::HandleBeatPadPressed(StringHash eventType, VariantMap& eventData)
{
    auto* button = static_cast<Button*>(eventData[Released::P_ELEMENT].GetPtr());
    if (!button) return;

    // Parse button name to get channel and step
    String name = button->GetName();
    Vector<String> parts = name.Split('_');
    if (parts.Size() < 3) return;

    int channel = std::atoi(parts[1].CString());
    int step = std::atoi(parts[2].CString());

    if (channel >= 0 && channel < NUM_CHANNELS && step >= 0 && step < NUM_STEPS) {
        // Toggle beat
        beatPattern_[channel][step] = !beatPattern_[channel][step];

        // Update visual immediately
        UpdateBeatPad(channel, step, beatPattern_[channel][step], false);

        // TODO: Send event to your sequencer system
        // Example:
        // VariantMap eventData;
        // eventData["Channel"] = channel;
        // eventData["Step"] = step;
        // eventData["Active"] = beatPattern_[channel][step];
        // SendEvent("BeatToggled", eventData);
    }
}

void BeatSequencerUI::HandlePlayPressed(StringHash eventType, VariantMap& eventData)
{
    isPlaying_ = !isPlaying_;

    // TODO: Send play/pause event to your sequencer
    // Example:
    // VariantMap eventData;
    // eventData["Playing"] = isPlaying_;
    // SendEvent("SequencerPlayToggle", eventData);
}

void BeatSequencerUI::HandleStopPressed(StringHash eventType, VariantMap& eventData)
{
    isPlaying_ = false;
    currentBeat_ = 0;

    // TODO: Send stop event to your sequencer
    // VariantMap eventData;
    // SendEvent("SequencerStop", eventData);
}

void BeatSequencerUI::HandleRecordPressed(StringHash eventType, VariantMap& eventData)
{
    isRecording_ = !isRecording_;

    // TODO: Send record event to your sequencer
    // VariantMap eventData;
    // eventData["Recording"] = isRecording_;
    // SendEvent("SequencerRecordToggle", eventData);
}

const SharedPtr<Window> &BeatSequencerUI::GetMainWindow() const {
    return mainWindow_;
}
