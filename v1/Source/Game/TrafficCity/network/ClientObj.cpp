//
// Copyright (c) 2008-2018 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Input/Controls.h>

#include "ClientObj.h"

#include <Urho3D/DebugNew.h>
//=============================================================================
//=============================================================================
ClientObj::ClientObj(Context *context)
    : LogicComponent(context)
    , userName_("DefaultClient")
    , colorIdx_(0)
{

}

ClientObj::~ClientObj()
{

}

void ClientObj::SetScene(Scene *scene) {
    scene_ = scene;
}

void ClientObj::SetRootNode(Node *node) {
    node_ = node;
}

void ClientObj::RegisterObject(Context* context)
{
    context->RegisterFactory<ClientObj>();

    URHO3D_ATTRIBUTE("Name", String, userName_, String::EMPTY, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("Color Index", int, colorIdx_, 0, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("Position", Vector3, position_, Vector3(0,0,0), AM_DEFAULT | AM_NET);

}

void ClientObj::SetClientInfo(const String &usrName, int colorIdx, Vector3 position)
{
    userName_ = usrName;
    colorIdx_ = colorIdx;
    position_ = position;

    String name = "client-SEQUENCER-" + userName_;
    // Create a new sequencer for client object
    SharedPtr<Node> seqNode(node_->CreateChild(name));
    sequencer_ = seqNode->CreateComponent<Sequencer>();
    sequencer_->SetId(name);

    // register
    //SetUpdateEventMask(USE_UPDATE | USE_FIXEDUPDATE);
    // AB: ClientObj doesn't get fixed update, everywhere else does - odd
}

void ClientObj::UpdateClientInfo(Vector3 position)
{
    position_ = position;
}


void ClientObj::ClearControls()
{
    controls_.buttons_ = 0;
}

void ClientObj::SetControls(const Controls &controls)
{
    controls_ = controls;
}

String ClientObj::GetUserName() {
    return userName_;
}

Vector3 ClientObj::GetPosition() {
    return position_;
}

SharedPtr<Sequencer> ClientObj::GetSequencer() {

    if (!sequencer_) {
        if (node_) {
            // Create a new sequencer for client object
            SharedPtr<Node> seqNode(node_->CreateChild("client-SEQUENCER-" + userName_));
            sequencer_ = seqNode->CreateComponent<Sequencer>();
        } else {
            return nullptr;
        }
    }

    return sequencer_;
}
/*
void ClientObj::FixedUpdate(float timeStep) {
    // Sequencer: Update play source position to client object node position
    sequencer_->GetPlaySource()->GetNode()->SetPosition(position_);
}*/



