//
// Copyright (c) 2008-2016 the Urho3D project.
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

#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Log.h>
#include <SDL/SDL_joystick.h>
#include <cstdio>


#include "GameController.h"
#include "network/NetworkActor.h"

#include <Urho3D/DebugNew.h>
//=============================================================================
//=============================================================================
//#define TEST_PS4_CONTROLLER
//const float YAW_SENSITIVITY = 0.5f;
//=============================================================================
//=============================================================================
GameController::GameController(Context* context)
    : Object(context)
    , joystickID_(-1)
    , minTolerance_(0.2f)

{
    SetupControllerMaps();
    CreateController();
}

GameController::~GameController()
{
}

void GameController::SetupControllerMaps()
{
    #ifdef __ANDROID__
    // PS4 controller results in the below GUID and keymapping
    const char *ps4Mapping = "576972656C65737320436F6E74726F6C,PS4 Controller,a:b1,b:b17,back:b16,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,guide:b7,leftshoulder:b3,leftstick:b4,lefttrigger:b9,leftx:a0,lefty:a1,rightshoulder:b18,rightstick:b6,righttrigger:a10,rightx:a2,righty:a5,start:b5,x:b0,y:b2,Android";

    SDL_GameControllerAddMapping(ps4Mapping);
    #endif
}

bool GameController::CreateController()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Input* input = GetSubsystem<Input>();

    // create or detect joystick
    if (GetPlatform() == "Android" || GetPlatform() == "iOS")
    {
        // remove the default screen joystick created in the Sample::InitTouchInput() fn.
        //**note** this would not be required if you don't inherit your app from the Sample class
        RemoveScreenJoystick();

        // and create our own
        XMLFile *layout = cache->GetResource<XMLFile>("ScreenJoystick/ScreenJoystick.xml");
        joystickID_ = input->AddScreenJoystick(layout, cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
    }
    else
    {
        // get the 1st controller joystick detected
        for ( unsigned i = 0; i < input->GetNumJoysticks(); ++i )
        {
            JoystickState *joystick = input->GetJoystickByIndex(i);

            if (joystick->IsController())
            {
                joystickID_ = joystick->joystickID_;
                break;
            }
        }
    }

    //#define DUMP_JOYSTICK_INFO
    #ifdef DUMP_JOYSTICK_INFO
    if (joystickID_ != -1)
    {
        JoystickState *joystick = input->GetJoystick(joystickID_);

        if (joystick)
        {
            URHO3D_LOGINFOF("Selected joystick: id=%d, name='%s'", 
                            joystickID_,
                            !joystick->name_.Empty()?joystick->name_.CString():"null");
        }
        DumpAll();
    }
    #endif

    SubscribeToEvent(E_JOYSTICKCONNECTED, URHO3D_HANDLER(GameController, HandleJoystickConnected));
    SubscribeToEvent(E_JOYSTICKDISCONNECTED, URHO3D_HANDLER(GameController, HandleJoystickDisconnected));

    return (joystickID_ != -1);
}

void GameController::RemoveScreenJoystick()
{
    Input* input = GetSubsystem<Input>();

    for ( unsigned i = 0; i < input->GetNumJoysticks(); ++i )
    {
        JoystickState *joystick = input->GetJoystickByIndex(i);

        if (joystick->screenJoystick_)
        {
            input->RemoveScreenJoystick(joystick->joystickID_);
            break;
        }
    }
}
//
void GameController::UpdateControlInputs(Controls& controls)
{
    // Clear buttons first
    controls.buttons_ = 0;

    if (!IsValid()) return;

    Input* input = GetSubsystem<Input>();
    JoystickState* joystick = input->GetJoystickByIndex(joystickID_);

    if (!joystick) return;

    // Get raw axis values
    float leftStickX = joystick->GetAxisPosition(0);  // Left stick X
    float leftStickY = joystick->GetAxisPosition(1);  // Left stick Y
    float rightStickX = joystick->GetAxisPosition(2); // Right stick X
    float rightStickY = joystick->GetAxisPosition(3); // Right stick Y

    // Apply deadzone and clamp values
    Vector2 leftStick(leftStickX, leftStickY);
    Vector2 rightStick(rightStickX, rightStickY);

    ClampValues(leftStick, minTolerance_);
    ClampValues(rightStick, minTolerance_);

    // Set movement controls (left stick)
    if (Abs(leftStick.x_) > minTolerance_) {
        if (leftStick.x_ < 0.0f) {
            controls.buttons_ |= NTWK_CTRL_LEFT;
        } else {
            controls.buttons_ |= NTWK_CTRL_RIGHT;
        }
    }

    if (Abs(leftStick.y_) > minTolerance_) {
        if (leftStick.y_ < 0.0f) {
            controls.buttons_ |= NTWK_CTRL_FORWARD;
        } else {
            controls.buttons_ |= NTWK_CTRL_BACK;
        }
    }

    // Set rotation (right stick X for yaw) - CRITICAL: Clamp yaw properly
    controls.yaw_ = Clamp(rightStick.x_ * 90.0f, -180.0f, 180.0f); // Limit to reasonable range

    // Store analog values for smooth movement
    controls.extraData_["steerLevel"] = Clamp(leftStick.x_, -1.0f, 1.0f);
    controls.extraData_["accelLevel"] = Clamp(-leftStick.y_, -1.0f, 1.0f); // Invert Y for forward

    // Handle buttons
    if (joystick->GetButtonDown(0)) // A button
        controls.buttons_ |= NTWK_CTRL_FIRE;

    if (joystick->GetButtonDown(1)) // B button
        controls.buttons_ |= NTWK_CTRL_ENTER;
}
//
/*
void GameController::UpdateControlInputs(Controls& controls)
{
    // clear buttons
    controls.buttons_ = 0;

    JoystickState *joystick = GetSubsystem<Input>()->GetJoystick(joystickID_);

    if (joystick)
    {
        // buttons
        for ( unsigned i = 0; i < joystick->GetNumButtons() && i < 32; ++i )
        {
            controls.Set((1<<i), joystick->GetButtonDown(i));
        }

        // dpad
        if (joystick->GetNumHats())
        {
            int hatVal = joystick->GetHatPosition(0);
            controls.Set(BUTTON_DPAD_UP,    (hatVal & SDL_HAT_UP) != 0);
            controls.Set(BUTTON_DPAD_DOWN,  (hatVal & SDL_HAT_DOWN) != 0);
            controls.Set(BUTTON_DPAD_LEFT,  (hatVal & SDL_HAT_LEFT) != 0);
            controls.Set(BUTTON_DPAD_RIGHT, (hatVal & SDL_HAT_RIGHT) != 0);

        }

        // axis
        const StringHash axisHashList[SDL_CONTROLLER_AXIS_MAX/2] = { VAR_AXIS_0, VAR_AXIS_1, VAR_AXIS_2 };
        for ( unsigned i = 0; i < joystick->GetNumAxes() && i < SDL_CONTROLLER_AXIS_MAX; i += 2 )
        {
            Vector2 val(joystick->GetAxisPosition(i), joystick->GetAxisPosition(i+1));

            // clamp values except for triggers
            if (i < SDL_CONTROLLER_AXIS_TRIGGERLEFT)
            {
                ClampValues(val, minTolerance_);
            }

            controls.extraData_[axisHashList[i/2]] = val;
        }
    }
}
*/
// Fix the ClampValues method
void GameController::ClampValues(Vector2 &vec, float minVal) const
{
    // Apply deadzone
    if (Abs(vec.x_) < minVal) vec.x_ = 0.0f;
    if (Abs(vec.y_) < minVal) vec.y_ = 0.0f;

    // Normalize if magnitude > 1 (diagonal movement)
    float magnitude = vec.Length();
    if (magnitude > 1.0f) {
        vec /= magnitude;
    }

    // Clamp individual components
    vec.x_ = Clamp(vec.x_, -1.0f, 1.0f);
    vec.y_ = Clamp(vec.y_, -1.0f, 1.0f);
}

void GameController::DumpAll() const
{
    Input* input = GetSubsystem<Input>();
    URHO3D_LOGINFOF("-------- num joysticks=%u --------", input->GetNumJoysticks());

    for ( unsigned i = 0; i < input->GetNumJoysticks(); ++i )
    {
        DumpControllerInfo(i, true);
    }
    URHO3D_LOGINFO("---------------------------------");
}

void GameController::DumpControllerInfo(unsigned idx, bool useIdx) const
{
    JoystickState *joystick = useIdx?GetSubsystem<Input>()->GetJoystickByIndex(idx):GetSubsystem<Input>()->GetJoystick((int)idx);

    if (joystick)
    {
        // get GUID
        String strGUID;

        if (joystick->joystick_)
        {
            SDL_JoystickGUID jystGuid = SDL_JoystickGetGUID(joystick->joystick_);
            char buff[16*2+1];
            int idx = 0;
            for ( unsigned i = 0; i < 16; ++i )
            {
                idx += sprintf(buff + idx, "%02X", jystGuid.data[i]);
            }
            buff[idx] = '\0';
            strGUID = String(buff);
        }

        URHO3D_LOGINFOF("joystick: id=%d, name='%s', btns=%u, axes=%u, hats=%u, isctrl=%u \n\tGUID=%s",
                        joystick->joystickID_,
                        !joystick->name_.Empty()?joystick->name_.CString():"null",
                        joystick->GetNumButtons(),
                        joystick->GetNumAxes(),
                        joystick->GetNumHats(),
                        joystick->IsController()?1:0,
                        strGUID.CString());
    }
}

void GameController::HandleJoystickConnected(StringHash eventType, VariantMap& eventData)
{
    using namespace JoystickConnected;
    int joystickID = eventData[P_JOYSTICKID].GetInt();

    URHO3D_LOGINFOF("HandleJoystickConnected: id=%d", joystickID);

    // testing PS4 controller on Android
    #if defined(__ANDROID__) && defined(TEST_PS4_CONTROLLER)
    JoystickState *joystick = GetSubsystem<Input>()->GetJoystickByIndex(joystickID);
    if (joystick && !joystick->name_.Empty() && joystick->name_ == "Wireless Controller")
    {
        // remove screen joystick and assign the ps4 controller for testing
        RemoveScreenJoystick();
        joystickID_ = joystickID;

        char *controllerMapping = SDL_GameControllerMapping(joystick->controller_);
        URHO3D_LOGINFOF("controller mapping=%s", controllerMapping?controllerMapping:"null");
    }
    #endif

    // auto-assign if yet assigned
    if (joystickID_ == -1)
    {
        joystickID_ = joystickID;
    }

    DumpControllerInfo((unsigned)joystickID);
}

void GameController::HandleJoystickDisconnected(StringHash eventType, VariantMap& eventData)
{
    using namespace JoystickDisconnected;
    int joystickID = eventData[P_JOYSTICKID].GetInt();

    URHO3D_LOGINFOF("HandleJoystickDisconnected: id=%d", joystickID);
}
