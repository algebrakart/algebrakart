#include "GenomeVisualization.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Scene/Scene.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Resource/ResourceCache.h"
#include "Urho3D/UI/UI.h"
#include "Urho3D/UI/UIEvents.h"
#include "Urho3D/UI/Font.h"
#include "Urho3D/UI/Button.h"
#include "Urho3D/UI/CheckBox.h"
#include "Urho3D/UI/Slider.h"
#include "Urho3D/Graphics/Renderer.h"
#include "Urho3D/Graphics/Camera.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/Graphics/RenderPath.h"
#include "Urho3D/Graphics/Light.h"
#include "AlgebraKart/ai/evolution_manager.h"

GenomeVisualization::GenomeVisualization(Context* context) :
        Component(context),
        vizScale_(1.0f),
        isVisible_(false),
        highlightedLayer_(-1),
        animationTime_(0.0f),
        pulseSpeed_(2.0f),
        needsUpdate_(true),
        updateTimer_(0.0f)
{
    // Initialize topology based on the neural network structure
    topology_.Push(9);  // Input layer
    topology_.Push(10); // Hidden layer 1
    topology_.Push(8);  // Hidden layer 2
    topology_.Push(3);  // Output layer
}

GenomeVisualization::~GenomeVisualization() {
    ClearVisualization();
}

void GenomeVisualization::RegisterObject(Context* context) {
    context->RegisterFactory<GenomeVisualization>();
}

void GenomeVisualization::OnNodeSet(Node* node) {
    if (node) {
        vizRootNode_ = node->CreateChild("GenomeVizRoot");
        Initialize();
    }
}

void GenomeVisualization::Initialize() {
    CreateUIPanel();
    CreateNetworkVisualization();
}

void GenomeVisualization::SetAgent(std::shared_ptr<Agent> agent) {
    currentAgent_ = agent;
    needsUpdate_ = true;

    if (agent) {
        UpdateUIInfo();
        UpdateNeuronColors();
        UpdateConnectionColors();
    }
}

void GenomeVisualization::CreateUIPanel() {
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    // Create the main window
    uiWindow_ = new Window(context_);
    ui->GetRoot()->AddChild(uiWindow_);

    // Set window properties
    uiWindow_->SetName("GenomeVisualizationWindow");
    uiWindow_->SetSize(400, 600);
    uiWindow_->SetPosition(20, 20);
    uiWindow_->SetStyleAuto();
    uiWindow_->SetMovable(true);
    uiWindow_->SetResizable(true);

    // Create title bar
    auto* titleBar = uiWindow_->CreateChild<UIElement>();
    titleBar->SetMinHeight(24);
    titleBar->SetVerticalAlignment(VA_TOP);
    titleBar->SetLayoutMode(LM_HORIZONTAL);

    auto* titleText = titleBar->CreateChild<Text>();
    titleText->SetName("TitleText");
    titleText->SetText("AI Bot Genome Visualization");
    titleText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 16);

    // Create close button
    auto* closeButton = titleBar->CreateChild<Button>();
    closeButton->SetName("CloseButton");
    closeButton->SetSize(24, 24);
    closeButton->SetAlignment(HA_RIGHT, VA_CENTER);
    auto* closeButtonText = closeButton->CreateChild<Text>();
    closeButtonText->SetText("X");
    closeButtonText->SetAlignment(HA_CENTER, VA_CENTER);
    SubscribeToEvent(closeButton, E_RELEASED, [this](StringHash, VariantMap&) {
        SetVisible(false);
    });

    // Create content area
    auto* content = uiWindow_->CreateChild<UIElement>();
    content->SetLayoutMode(LM_VERTICAL);
    content->SetLayoutSpacing(10);
    content->SetLayoutBorder(IntRect(10, 40, 10, 10));

    // Agent info section
    agentNameText_ = content->CreateChild<Text>();
    agentNameText_->SetText("Agent: None selected");
    agentNameText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 14);

    fitnessText_ = content->CreateChild<Text>();
    fitnessText_->SetText("Fitness: 0.0");
    fitnessText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);

    generationText_ = content->CreateChild<Text>();
    generationText_->SetText("Generation: 0");
    generationText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);

    paramCountText_ = content->CreateChild<Text>();
    paramCountText_->SetText("Parameters: 0");
    paramCountText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);

    // Separator
    auto* separator1 = content->CreateChild<UIElement>();
    separator1->SetMinHeight(2);
    separator1->SetMaxHeight(2);
    separator1->SetColor(Color(0.5f, 0.5f, 0.5f));

    // Network topology info
    layerInfoText_ = content->CreateChild<Text>();
    layerInfoText_->SetText("Network: 9-10-8-3");
    layerInfoText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);

    // Stats section
    weightStatsText_ = content->CreateChild<Text>();
    weightStatsText_->SetText("Weights: Min: 0.0, Max: 0.0, Avg: 0.0");
    weightStatsText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 11);
    weightStatsText_->SetWordwrap(true);

    activationStatsText_ = content->CreateChild<Text>();
    activationStatsText_->SetText("Activations: Active neurons: 0/30");
    activationStatsText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 11);

    // Separator
    auto* separator2 = content->CreateChild<UIElement>();
    separator2->SetMinHeight(2);
    separator2->SetMaxHeight(2);
    separator2->SetColor(Color(0.5f, 0.5f, 0.5f));

    // Controls section
    auto* controlsLabel = content->CreateChild<Text>();
    controlsLabel->SetText("Visualization Controls:");
    controlsLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);

    // Scale slider
    auto* scaleContainer = content->CreateChild<UIElement>();
    scaleContainer->SetLayoutMode(LM_HORIZONTAL);
    scaleContainer->SetLayoutSpacing(10);

    auto* scaleLabel = scaleContainer->CreateChild<Text>();
    scaleLabel->SetText("Scale:");
    scaleLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 11);
    scaleLabel->SetMinWidth(50);

    auto* scaleSlider = scaleContainer->CreateChild<Slider>();
    scaleSlider->SetRange(0.5f);
    scaleSlider->SetValue(1.0f);
    scaleSlider->SetSize(200, 20);
    SubscribeToEvent(scaleSlider, E_SLIDERCHANGED, [this](StringHash, VariantMap& eventData) {
        using namespace SliderChanged;
        float value = eventData[P_VALUE].GetFloat();
        SetVisualizationScale(0.5f + value * 2.0f);
    });

    // Animation speed slider
    auto* speedContainer = content->CreateChild<UIElement>();
    speedContainer->SetLayoutMode(LM_HORIZONTAL);
    speedContainer->SetLayoutSpacing(10);

    auto* speedLabel = speedContainer->CreateChild<Text>();
    speedLabel->SetText("Pulse:");
    speedLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 11);
    speedLabel->SetMinWidth(50);

    auto* speedSlider = speedContainer->CreateChild<Slider>();
    speedSlider->SetRange(1.0f);
    speedSlider->SetValue(0.5f);
    speedSlider->SetSize(200, 20);
    SubscribeToEvent(speedSlider, E_SLIDERCHANGED, [this](StringHash, VariantMap& eventData) {
        using namespace SliderChanged;
        pulseSpeed_ = eventData[P_VALUE].GetFloat() * 4.0f;
    });

    // Layer highlight buttons
    auto* layerLabel = content->CreateChild<Text>();
    layerLabel->SetText("Highlight Layer:");
    layerLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 11);

    auto* layerButtonContainer = content->CreateChild<UIElement>();
    layerButtonContainer->SetLayoutMode(LM_HORIZONTAL);
    layerButtonContainer->SetLayoutSpacing(5);

    const char* layerNames[] = {"Input", "Hidden 1", "Hidden 2", "Output"};
    for (int i = 0; i < 4; i++) {
        auto* layerButton = layerButtonContainer->CreateChild<Button>();
        layerButton->SetSize(80, 24);
        layerButton->SetStyleAuto();

        auto* buttonText = layerButton->CreateChild<Text>();
        buttonText->SetText(layerNames[i]);
        buttonText->SetAlignment(HA_CENTER, VA_CENTER);
        buttonText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 11);

        SubscribeToEvent(layerButton, E_RELEASED, [this, i](StringHash, VariantMap&) {
            if (highlightedLayer_ == i) {
                ClearHighlights();
            } else {
                HighlightLayer(i);
            }
        });
    }

    // Initially hidden
    uiWindow_->SetVisible(false);
}

void GenomeVisualization::CreateNetworkVisualization() {
    if (!vizRootNode_) return;

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    // Clear existing visualization
    ClearVisualization();

    // Calculate total width needed for visualization
    float totalWidth = 0.0f;
    for (int i = 0; i < topology_.Size(); i++) {
        float layerHeight = (topology_[i] - 1) * NEURON_SPACING;
        if (layerHeight > totalWidth) totalWidth = layerHeight;
    }

    // Create neurons for each layer
    for (int layer = 0; layer < topology_.Size(); layer++) {
        float layerX = layer * LAYER_SPACING - (topology_.Size() - 1) * LAYER_SPACING * 0.5f;

        for (int neuron = 0; neuron < topology_[layer]; neuron++) {
            Vector3 position = CalculateNeuronPosition(layer, neuron, topology_[layer]);
            CreateNeuron(layer, neuron, position);
        }
    }

    // Create connections between layers
    int connectionIndex = 0;
    for (int layer = 0; layer < topology_.Size() - 1; layer++) {
        for (int fromNeuron = 0; fromNeuron < topology_[layer]; fromNeuron++) {
            for (int toNeuron = 0; toNeuron < topology_[layer + 1]; toNeuron++) {
                // Create a connection with default weight
                CreateConnection(layer, fromNeuron, layer + 1, toNeuron, 0.0f);
                connectionIndex++;
            }
        }
    }

    // Add lighting for better visualization
    Node* lightNode = vizRootNode_->CreateChild("VizLight");
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetBrightness(1.0f);
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
}

void GenomeVisualization::CreateNeuron(int layer, int index, const Vector3& position) {
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    NeuronViz neuron;
    neuron.layer = layer;
    neuron.index = index;
    neuron.position = position;
    neuron.activation = 0.0f;

    // Create node
    neuron.node = vizRootNode_->CreateChild("Neuron_" + String(layer) + "_" + String(index));
    neuron.node->SetPosition(position);

    // Create visual representation
    neuron.model = neuron.node->CreateComponent<StaticModel>();
    neuron.model->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));

    // Create material with appropriate color
    SharedPtr<Material> mat = cache->GetResource<Material>("Materials/UnlitSolid.xml")->Clone();
    Color neuronColor;

    // Color code by layer type
    switch (layer) {
        case 0: // Input layer - green
            neuronColor = Color(0.2f, 0.8f, 0.2f);
            break;
        case 3: // Output layer - red
            neuronColor = Color(0.8f, 0.2f, 0.2f);
            break;
        default: // Hidden layers - blue
            neuronColor = Color(0.2f, 0.4f, 0.8f);
            break;
    }

    mat->SetShaderParameter("MatDiffColor", neuronColor);
    neuron.model->SetMaterial(mat);

    // Set scale
    neuron.node->SetScale(NEURON_SIZE);

    // Add to list
    neurons_.Push(neuron);
}

void GenomeVisualization::CreateConnection(int fromLayer, int fromNeuron, int toLayer, int toNeuron, float weight) {
    ConnectionViz conn;
    conn.fromLayer = fromLayer;
    conn.fromNeuron = fromNeuron;
    conn.toLayer = toLayer;
    conn.toNeuron = toNeuron;
    conn.weight = weight;

    // Create node for the connection
    String connName = "Conn_" + String(fromLayer) + "_" + String(fromNeuron) +
                      "_" + String(toLayer) + "_" + String(toNeuron);
    conn.node = vizRootNode_->CreateChild(connName);

    // Create custom geometry for the connection line
    conn.geometry = conn.node->CreateComponent<CustomGeometry>();
    conn.geometry->SetNumGeometries(1);

    // Find the neurons
    NeuronViz* fromNeuronViz = nullptr;
    NeuronViz* toNeuronViz = nullptr;

    for (int i = 0; i < neurons_.Size(); i++) {
        if (neurons_[i].layer == fromLayer && neurons_[i].index == fromNeuron) {
            fromNeuronViz = &neurons_[i];
        }
        if (neurons_[i].layer == toLayer && neurons_[i].index == toNeuron) {
            toNeuronViz = &neurons_[i];
        }
    }

    if (fromNeuronViz && toNeuronViz) {
        CreateConnectionGeometry(conn, fromNeuronViz->position, toNeuronViz->position);
    }

    connections_.Push(conn);
}

void GenomeVisualization::CreateConnectionGeometry(ConnectionViz& conn, const Vector3& start, const Vector3& end) {
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    conn.geometry->BeginGeometry(0, TRIANGLE_STRIP);
    conn.geometry->SetMaterial(0, cache->GetResource<Material>("Materials/UnlitVertexColor.xml"));

    // Calculate direction and perpendicular vector for line thickness
    Vector3 direction = (end - start).Normalized();
    Vector3 perpendicular = direction.CrossProduct(Vector3::UP).Normalized() * CONNECTION_WIDTH;

    // If the connection is vertical, use a different perpendicular
    if (Abs(direction.y_) > 0.99f) {
        perpendicular = direction.CrossProduct(Vector3::RIGHT).Normalized() * CONNECTION_WIDTH;
    }

    // Get color based on weight
    Color color = GetWeightColor(conn.weight);

    // Create a quad strip for the connection
    conn.geometry->DefineVertex(start - perpendicular);
    conn.geometry->DefineColor(color);

    conn.geometry->DefineVertex(start + perpendicular);
    conn.geometry->DefineColor(color);

    conn.geometry->DefineVertex(end - perpendicular);
    conn.geometry->DefineColor(color);

    conn.geometry->DefineVertex(end + perpendicular);
    conn.geometry->DefineColor(color);

    conn.geometry->Commit();
}

Vector3 GenomeVisualization::CalculateNeuronPosition(int layer, int index, int totalInLayer) {
    float layerX = layer * LAYER_SPACING - (topology_.Size() - 1) * LAYER_SPACING * 0.5f;
    float neuronY = (index - (totalInLayer - 1) * 0.5f) * NEURON_SPACING;

    return Vector3(layerX, neuronY, 0.0f);
}

Color GenomeVisualization::GetWeightColor(float weight) {
    // Normalize weight to [-1, 1] range (assuming typical neural network weights)
    float normalizedWeight = Clamp(weight / 3.0f, -1.0f, 1.0f);

    // Create color gradient: blue (negative) -> gray (zero) -> red (positive)
    if (normalizedWeight < 0) {
        float intensity = -normalizedWeight;
        return Color(0.2f, 0.2f, 0.2f + 0.6f * intensity, 0.8f);
    } else {
        float intensity = normalizedWeight;
        return Color(0.2f + 0.6f * intensity, 0.2f, 0.2f, 0.8f);
    }
}

Color GenomeVisualization::GetActivationColor(float activation) {
    // Assume activation is normalized [0, 1] for visualization
    float intensity = Clamp(activation, 0.0f, 1.0f);

    // Create gradient from dark to bright
    float brightness = 0.2f + 0.8f * intensity;
    return Color(brightness, brightness, brightness);
}

void GenomeVisualization::Update(float timeStep) {
    if (!isVisible_ || !currentAgent_) return;

    // Update animation
    animationTime_ += timeStep;

    // Periodic update for performance
    updateTimer_ += timeStep;
    if (updateTimer_ >= UPDATE_INTERVAL) {
        updateTimer_ = 0.0f;

        if (needsUpdate_) {
            UpdateNeuronColors();
            UpdateConnectionColors();
            UpdateUIInfo();
            needsUpdate_ = false;
        }
    }

    // Animate neurons with pulse effect
    float pulse = (Sin(animationTime_ * pulseSpeed_) + 1.0f) * 0.5f;

    for (int i = 0; i < neurons_.Size(); i++) {
        NeuronViz& neuron = neurons_[i];

        // Apply subtle pulsing to active neurons
        if (neuron.activation > 0.1f) {
            float scale = NEURON_SIZE * (1.0f + pulse * 0.1f * neuron.activation);
            neuron.node->SetScale(scale);
        }

        // Highlight effect
        if (highlightedLayer_ >= 0 && neuron.layer == highlightedLayer_) {
            float highlightScale = NEURON_SIZE * (1.2f + pulse * 0.2f);
            neuron.node->SetScale(highlightScale);
        }
    }
}

void GenomeVisualization::UpdateNeuronColors() {
    if (!currentAgent_ || !currentAgent_->ffn) return;

    // For visualization purposes, we'll use random activations
    // In a real implementation, you'd get actual activations from the neural network
    for (int i = 0; i < neurons_.Size(); i++) {
        NeuronViz& neuron = neurons_[i];

        // Simulate activation values (in real use, get from network)
        neuron.activation = Random(0.0f, 1.0f);

        // Update neuron color based on activation
        SharedPtr<Material> mat = neuron.model->GetMaterial()->Clone();
        Color baseColor;

        // Base color by layer
        switch (neuron.layer) {
            case 0: // Input
                baseColor = Color(0.2f, 0.8f, 0.2f);
                break;
            case 3: // Output
                baseColor = Color(0.8f, 0.2f, 0.2f);
                break;
            default: // Hidden
                baseColor = Color(0.2f, 0.4f, 0.8f);
                break;
        }

        // Modulate by activation
        float brightness = 0.3f + 0.7f * neuron.activation;
        baseColor = baseColor * brightness;

        mat->SetShaderParameter("MatDiffColor", baseColor);
        neuron.model->SetMaterial(mat);
    }
}

void GenomeVisualization::UpdateConnectionColors() {
    if (!currentAgent_ || !currentAgent_->genotype) return;

    // Get the genotype parameters
    std::vector<float> params = currentAgent_->genotype->getParameterCopy();

    // Update connection weights and colors
    int paramIndex = 0;
    for (int i = 0; i < connections_.Size() && paramIndex < params.size(); i++) {
        ConnectionViz& conn = connections_[i];

        // Update weight from genotype
        conn.weight = params[paramIndex++];

        // Recreate geometry with new color
        NeuronViz* fromNeuron = nullptr;
        NeuronViz* toNeuron = nullptr;

        for (int j = 0; j < neurons_.Size(); j++) {
            if (neurons_[j].layer == conn.fromLayer && neurons_[j].index == conn.fromNeuron) {
                fromNeuron = &neurons_[j];
            }
            if (neurons_[j].layer == conn.toLayer && neurons_[j].index == conn.toNeuron) {
                toNeuron = &neurons_[j];
            }
        }

        if (fromNeuron && toNeuron) {
            CreateConnectionGeometry(conn, fromNeuron->position, toNeuron->position);
        }
    }
}

void GenomeVisualization::UpdateUIInfo() {
    if (!currentAgent_ || !currentAgent_->genotype) return;

    // Update agent info
    agentNameText_->SetText("Agent: " + String(currentAgent_->id));
    fitnessText_->SetText("Fitness: " + String(currentAgent_->genotype->fitness));
    generationText_->SetText("Generation: " + String(EvolutionManager::getGenerationCount()));
    paramCountText_->SetText("Parameters: " + String(currentAgent_->genotype->getParameterCount()));

    // Calculate weight statistics
    std::vector<float> params = currentAgent_->genotype->getParameterCopy();
    if (!params.empty()) {
        float minWeight = params[0];
        float maxWeight = params[0];
        float avgWeight = 0.0f;

        for (float weight : params) {
            minWeight = Min(minWeight, weight);
            maxWeight = Max(maxWeight, weight);
            avgWeight += weight;
        }
        avgWeight /= params.size();

        weightStatsText_->SetText(
                "Weights: Min: " + String(minWeight, 2) +
                ", Max: " + String(maxWeight, 2) +
                ", Avg: " + String(avgWeight, 2)
        );
    }

    // Count active neurons
    int activeNeurons = 0;
    for (const NeuronViz& neuron : neurons_) {
        if (neuron.activation > 0.1f) activeNeurons++;
    }

    activationStatsText_->SetText(
            "Activations: Active neurons: " + String(activeNeurons) + "/" + String(neurons_.Size())
    );
}

void GenomeVisualization::SetVisible(bool visible) {
    isVisible_ = visible;
    if (uiWindow_) {
        uiWindow_->SetVisible(visible);
    }
    if (vizRootNode_) {
        vizRootNode_->SetEnabled(visible);
    }
}

void GenomeVisualization::SetVisualizationPosition(const Vector3& position) {
    if (vizRootNode_) {
        vizRootNode_->SetPosition(position);
    }
}

void GenomeVisualization::SetVisualizationScale(float scale) {
    vizScale_ = scale;
    if (vizRootNode_) {
        vizRootNode_->SetScale(scale);
    }
}

void GenomeVisualization::HighlightLayer(int layerIndex) {
    highlightedLayer_ = layerIndex;

    // Update layer info text
    const char* layerNames[] = {"Input Layer", "Hidden Layer 1", "Hidden Layer 2", "Output Layer"};
    if (layerIndex >= 0 && layerIndex < 4) {
        layerInfoText_->SetText(String(layerNames[layerIndex]) + " - " +
                                String(topology_[layerIndex]) + " neurons");
    }
}

void GenomeVisualization::ClearHighlights() {
    highlightedLayer_ = -1;
    layerInfoText_->SetText("Network: 9-10-8-3");

    // Reset all neuron scales
    for (NeuronViz& neuron : neurons_) {
        neuron.node->SetScale(NEURON_SIZE);
    }
}

void GenomeVisualization::ClearVisualization() {
    neurons_.Clear();
    connections_.Clear();

    if (vizRootNode_) {
        vizRootNode_->RemoveAllChildren();
    }
}