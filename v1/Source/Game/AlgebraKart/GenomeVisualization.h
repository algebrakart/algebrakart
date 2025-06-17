#ifndef GENOME_VISUALIZATION_H
#define GENOME_VISUALIZATION_H

#include "Urho3D/Scene/Component.h"
#include "Urho3D/UI/UIElement.h"
#include "Urho3D/Graphics/StaticModel.h"
#include "Urho3D/Graphics/BillboardSet.h"
#include "Urho3D/UI/Window.h"
#include "Urho3D/UI/Text.h"
#include "Urho3D/Graphics/CustomGeometry.h"
#include "Urho3D/Math/Color.h"
#include "ai/agent.h"
#include "ai/neural_network.h"

using namespace Urho3D;

// Constants for visualization
const float LAYER_SPACING = 3.0f;
const float NEURON_SPACING = 1.5f;
const float NEURON_SIZE = 0.3f;
const float CONNECTION_WIDTH = 0.05f;

// Neuron visualization data
struct NeuronViz {
    SharedPtr<Node> node;
    SharedPtr<StaticModel> model;
    Vector3 position;
    int layer;
    int index;
    float activation;
};

// Connection visualization data
struct ConnectionViz {
    SharedPtr<Node> node;
    SharedPtr<CustomGeometry> geometry;
    float weight;
    int fromLayer;
    int fromNeuron;
    int toLayer;
    int toNeuron;
};

class GenomeVisualization : public Component {
URHO3D_OBJECT(GenomeVisualization, Component);

public:
    /// Construct
    GenomeVisualization(Context* context);
    /// Destruct
    virtual ~GenomeVisualization();

    /// Register object factory
    static void RegisterObject(Context* context);

    /// Initialize the visualization
    void Initialize();

    /// Set the agent to visualize
    void SetAgent(std::shared_ptr<Agent> agent);

    /// Update visualization (called per frame)
    void Update(float timeStep);

    /// Show/hide the visualization
    void SetVisible(bool visible);

    /// Create UI panel
    void CreateUIPanel();

    /// Update UI information
    void UpdateUIInfo();

    /// Get the UI window
    Window* GetUIWindow() { return uiWindow_; }

    /// Set position of the 3D visualization
    void SetVisualizationPosition(const Vector3& position);

    /// Set scale of the visualization
    void SetVisualizationScale(float scale);

    /// Highlight specific layer
    void HighlightLayer(int layerIndex);

    /// Clear highlights
    void ClearHighlights();

protected:
    /// Handle node being assigned
    virtual void OnNodeSet(Node* node);

private:
    /// Create 3D neural network visualization
    void CreateNetworkVisualization();

    /// Update neuron colors based on activation
    void UpdateNeuronColors();

    /// Update connection colors based on weights
    void UpdateConnectionColors();

    /// Create a neuron node
    void CreateNeuron(int layer, int index, const Vector3& position);

    /// Create a connection between neurons
    void CreateConnection(int fromLayer, int fromNeuron, int toLayer, int toNeuron, float weight);

    /// Get color for weight value
    Color GetWeightColor(float weight);

    /// Get color for activation value
    Color GetActivationColor(float activation);

    /// Clear all visualization elements
    void ClearVisualization();

    /// Calculate neuron position
    Vector3 CalculateNeuronPosition(int layer, int index, int totalInLayer);

    /// Create connection geometry
    void CreateConnectionGeometry(ConnectionViz& conn, const Vector3& start, const Vector3& end);

    /// Update connection geometry
    void UpdateConnectionGeometry(ConnectionViz& conn);

    // Member variables
    std::shared_ptr<Agent> currentAgent_;
    SharedPtr<Node> vizRootNode_;
    SharedPtr<Window> uiWindow_;
    SharedPtr<Text> agentNameText_;
    SharedPtr<Text> fitnessText_;
    SharedPtr<Text> generationText_;
    SharedPtr<Text> layerInfoText_;
    SharedPtr<Text> paramCountText_;

    // Visualization elements
    Vector<NeuronViz> neurons_;
    Vector<ConnectionViz> connections_;

    // Network topology (from the agent)
    Vector<int> topology_;

    // Visualization parameters
    float vizScale_;
    bool isVisible_;
    int highlightedLayer_;

    // Animation
    float animationTime_;
    float pulseSpeed_;

    // UI Elements for detailed info
    SharedPtr<UIElement> detailPanel_;
    SharedPtr<Text> weightStatsText_;
    SharedPtr<Text> activationStatsText_;

    // Performance optimization
    bool needsUpdate_;
    float updateTimer_;
    const float UPDATE_INTERVAL = 0.1f; // Update every 100ms
};

#endif // GENOME_VISUALIZATION_H