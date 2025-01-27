#pragma once

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/IO/Log.h>

using namespace Urho3D;

struct StateSnapshot {
    Scene* scene_;
    Vector<Node *> nodes_;
    VectorBuffer* state_;
    MemoryBuffer* buf_;

    void writeState(VectorBuffer* state, Scene *scene) {
        state_ = state;
        scene_ = scene;
    }

    void readState(MemoryBuffer* buf, Scene *scene) {
        buf_ = buf;
        scene_ = scene;

        // TOOD: Do something with CSP snapshot?
        //URHO3D_LOGDEBUGF("StateSnapshot::readState(%s, %d)", buf->GetData(), scene->GetID());
    }

    void addNode(Node* node) {
        nodes_.Push(node);
    }

    void removeNode(Node* node) {
        nodes_.Remove(node);
    }
};
