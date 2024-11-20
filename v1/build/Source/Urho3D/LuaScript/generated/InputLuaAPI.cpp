/*
** Lua binding: InputLuaAPI
*/

//
// Copyright (c) 2008-2021 the Urho3D project.
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

#include <toluapp/tolua++.h>
#include "LuaScript/ToluaUtils.h"

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

/* Exported function */
TOLUA_API int tolua_InputLuaAPI_open (lua_State* tolua_S);

#include "Input/Controls.h"
#include "IO/File.h"
#include "Input/Input.h"
#include "Input/InputEvents.h"
using namespace Urho3D;
#pragma warning(disable:4800)

static bool InputSaveGestures(Input* input, File* file)
{
    return file ? input->SaveGestures(*file) : false;
}

static bool InputSaveGesture(Input* input, File* file, unsigned gestureID)
{
    return file ? input->SaveGesture(*file, gestureID) : false;
}

static unsigned InputLoadGestures(Input* input, File* file)
{
    return file ? input->LoadGestures(*file) : 0;
}

static bool InputSaveGestures(Input* input, const String& fileName)
{
    File file(input->GetContext(), fileName, FILE_WRITE);
    return file.IsOpen() ? input->SaveGestures(file) : false;
}

static bool InputSaveGesture(Input* input, const String& fileName, unsigned gestureID)
{
    File file(input->GetContext(), fileName, FILE_WRITE);
    return file.IsOpen() ? input->SaveGesture(file, gestureID) : false;
}

static unsigned InputLoadGestures(Input* input, const String& fileName)
{
    File file(input->GetContext(), fileName, FILE_READ);
    return file.IsOpen() ? input->LoadGestures(file) : 0;
}

#define TOLUA_DISABLE_tolua_InputLuaAPI_GetInput00
static int tolua_InputLuaAPI_GetInput00(lua_State* tolua_S)
{
    return ToluaGetSubsystem<Input>(tolua_S);
}

#define TOLUA_DISABLE_tolua_get_input_ptr
#define tolua_get_input_ptr tolua_InputLuaAPI_GetInput00

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Controls (lua_State* tolua_S)
{
 Controls* self = (Controls*) tolua_tousertype(tolua_S,1,0);
 Mtolua_delete(self);
 return 0;
}

static int tolua_collect_Vector2 (lua_State* tolua_S)
{
 Vector2* self = (Vector2*) tolua_tousertype(tolua_S,1,0);
 Mtolua_delete(self);
 return 0;
}

static int tolua_collect_IntVector2 (lua_State* tolua_S)
{
 IntVector2* self = (IntVector2*) tolua_tousertype(tolua_S,1,0);
 Mtolua_delete(self);
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"VariantMap");
 tolua_usertype(tolua_S,"XMLFile");
 tolua_usertype(tolua_S,"UIElement");
 tolua_usertype(tolua_S,"JoystickState");
 tolua_usertype(tolua_S,"TouchState");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"Vector2");
 tolua_usertype(tolua_S,"Input");
 tolua_usertype(tolua_S,"File");
 tolua_usertype(tolua_S,"Controls");
 tolua_usertype(tolua_S,"IntVector2");
}

/* method: new of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_new00
static int tolua_InputLuaAPI_Controls_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Controls",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Controls* tolua_ret = (Controls*)  Mtolua_new((Controls)());
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"Controls");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_new00_local
static int tolua_InputLuaAPI_Controls_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Controls",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Controls* tolua_ret = (Controls*)  Mtolua_new((Controls)());
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"Controls");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_delete00
static int tolua_InputLuaAPI_Controls_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Controls",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
 Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Reset of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_Reset00
static int tolua_InputLuaAPI_Controls_Reset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Controls",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Reset'", NULL);
#endif
 {
  self->Reset();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Reset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Set of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_Set00
static int tolua_InputLuaAPI_Controls_Set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Controls",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
  unsigned buttons = ((unsigned)  tolua_tonumber(tolua_S,2,0));
  bool down = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Set'", NULL);
#endif
 {
  self->Set(buttons,down);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsDown of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_IsDown00
static int tolua_InputLuaAPI_Controls_IsDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Controls",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Controls* self = (const Controls*)  tolua_tousertype(tolua_S,1,0);
  unsigned button = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsDown'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsDown(button);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsPressed of class  Controls */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Controls_IsPressed00
static int tolua_InputLuaAPI_Controls_IsPressed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Controls",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Controls",0,&tolua_err)) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Controls* self = (const Controls*)  tolua_tousertype(tolua_S,1,0);
  unsigned button = ((unsigned)  tolua_tonumber(tolua_S,2,0));
  const Controls* previousControls = ((const Controls*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsPressed'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsPressed(button,*previousControls);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsPressed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: buttons_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_get_Controls_buttons
static int tolua_get_Controls_buttons(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'buttons_'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->buttons_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: buttons_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_set_Controls_buttons
static int tolua_set_Controls_buttons(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'buttons_'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->buttons_ = ((unsigned)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yaw_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_get_Controls_yaw
static int tolua_get_Controls_yaw(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yaw_'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->yaw_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yaw_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_set_Controls_yaw
static int tolua_set_Controls_yaw(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yaw_'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->yaw_ = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pitch_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_get_Controls_pitch
static int tolua_get_Controls_pitch(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch_'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->pitch_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pitch_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_set_Controls_pitch
static int tolua_set_Controls_pitch(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch_'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pitch_ = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: extraData_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_get_Controls_extraData
static int tolua_get_Controls_extraData(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extraData_'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->extraData_,"VariantMap");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: extraData_ of class  Controls */
#ifndef TOLUA_DISABLE_tolua_set_Controls_extraData
static int tolua_set_Controls_extraData(lua_State* tolua_S)
{
  Controls* self = (Controls*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extraData_'",NULL);
 if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"VariantMap",0,&tolua_err)))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extraData_ = *((VariantMap*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTouchedElement of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_TouchState_GetTouchedElement00
static int tolua_InputLuaAPI_TouchState_GetTouchedElement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"TouchState",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTouchedElement'", NULL);
#endif
 {
  UIElement* tolua_ret = (UIElement*)  self->GetTouchedElement();
  ToluaPushObject(tolua_S,(void*)tolua_ret,"UIElement");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTouchedElement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: touchID_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_get_TouchState_touchID
static int tolua_get_TouchState_touchID(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'touchID_'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->touchID_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: touchID_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_set_TouchState_touchID
static int tolua_set_TouchState_touchID(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'touchID_'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->touchID_ = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_get_TouchState_position
static int tolua_get_TouchState_position(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position_'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->position_,"IntVector2");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_set_TouchState_position
static int tolua_set_TouchState_position(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position_'",NULL);
 if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"IntVector2",0,&tolua_err)))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position_ = *((IntVector2*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lastPosition_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_get_TouchState_lastPosition
static int tolua_get_TouchState_lastPosition(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastPosition_'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->lastPosition_,"IntVector2");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lastPosition_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_set_TouchState_lastPosition
static int tolua_set_TouchState_lastPosition(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lastPosition_'",NULL);
 if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"IntVector2",0,&tolua_err)))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->lastPosition_ = *((IntVector2*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: delta_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_get_TouchState_delta
static int tolua_get_TouchState_delta(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delta_'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->delta_,"IntVector2");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: delta_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_set_TouchState_delta
static int tolua_set_TouchState_delta(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delta_'",NULL);
 if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"IntVector2",0,&tolua_err)))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->delta_ = *((IntVector2*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pressure_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_get_TouchState_pressure
static int tolua_get_TouchState_pressure(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pressure_'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->pressure_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pressure_ of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_set_TouchState_pressure
static int tolua_set_TouchState_pressure(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pressure_'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pressure_ = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: touchedElement of class  TouchState */
#ifndef TOLUA_DISABLE_tolua_get_TouchState_touchedElement_ptr
static int tolua_get_TouchState_touchedElement_ptr(lua_State* tolua_S)
{
  TouchState* self = (TouchState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'touchedElement'",NULL);
#endif
  ToluaPushObject(tolua_S,(void*)self->GetTouchedElement(),"UIElement");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name_ of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_get_JoystickState_name
static int tolua_get_JoystickState_name(lua_State* tolua_S)
{
  JoystickState* self = (JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name_'",NULL);
#endif
 tolua_pushurho3dstring(tolua_S,(const char*)self->name_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: joystickID_ of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_get_JoystickState_joystickID
static int tolua_get_JoystickState_joystickID(lua_State* tolua_S)
{
  JoystickState* self = (JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'joystickID_'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->joystickID_);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsController of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_IsController00
static int tolua_InputLuaAPI_JoystickState_IsController00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsController'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsController();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsController'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumButtons of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetNumButtons00
static int tolua_InputLuaAPI_JoystickState_GetNumButtons00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumButtons'", NULL);
#endif
 {
  unsigned tolua_ret = (unsigned)  self->GetNumButtons();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumButtons'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumAxes of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetNumAxes00
static int tolua_InputLuaAPI_JoystickState_GetNumAxes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumAxes'", NULL);
#endif
 {
  unsigned tolua_ret = (unsigned)  self->GetNumAxes();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumAxes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumHats of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetNumHats00
static int tolua_InputLuaAPI_JoystickState_GetNumHats00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumHats'", NULL);
#endif
 {
  unsigned tolua_ret = (unsigned)  self->GetNumHats();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumHats'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetButtonDown of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetButtonDown00
static int tolua_InputLuaAPI_JoystickState_GetButtonDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
  unsigned index = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetButtonDown'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetButtonDown(index);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetButtonDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetButtonPress of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetButtonPress00
static int tolua_InputLuaAPI_JoystickState_GetButtonPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
  unsigned index = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetButtonPress'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetButtonPress(index);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetButtonPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAxisPosition of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetAxisPosition00
static int tolua_InputLuaAPI_JoystickState_GetAxisPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
  unsigned index = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAxisPosition'", NULL);
#endif
 {
  float tolua_ret = (float)  self->GetAxisPosition(index);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAxisPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHatPosition of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_JoystickState_GetHatPosition00
static int tolua_InputLuaAPI_JoystickState_GetHatPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const JoystickState",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const JoystickState* self = (const JoystickState*)  tolua_tousertype(tolua_S,1,0);
  unsigned index = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHatPosition'", NULL);
#endif
 {
  int tolua_ret = (int)  self->GetHatPosition(index);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHatPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: controller of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_get_JoystickState_controller
static int tolua_get_JoystickState_controller(lua_State* tolua_S)
{
  JoystickState* self = (JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'controller'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->IsController());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: numButtons of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_get_JoystickState_numButtons
static int tolua_get_JoystickState_numButtons(lua_State* tolua_S)
{
  JoystickState* self = (JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numButtons'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetNumButtons());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: numAxes of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_get_JoystickState_numAxes
static int tolua_get_JoystickState_numAxes(lua_State* tolua_S)
{
  JoystickState* self = (JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numAxes'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetNumAxes());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: numHats of class  JoystickState */
#ifndef TOLUA_DISABLE_tolua_get_JoystickState_numHats
static int tolua_get_JoystickState_numHats(lua_State* tolua_S)
{
  JoystickState* self = (JoystickState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numHats'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetNumHats());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetToggleFullscreen of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetToggleFullscreen00
static int tolua_InputLuaAPI_Input_SetToggleFullscreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetToggleFullscreen'", NULL);
#endif
 {
  self->SetToggleFullscreen(enable);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetToggleFullscreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMouseVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetMouseVisible00
static int tolua_InputLuaAPI_Input_SetMouseVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool suppressEvent = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMouseVisible'", NULL);
#endif
 {
  self->SetMouseVisible(enable,suppressEvent);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMouseVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMouseGrabbed of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetMouseGrabbed00
static int tolua_InputLuaAPI_Input_SetMouseGrabbed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  bool grab = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool suppressEvent = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMouseGrabbed'", NULL);
#endif
 {
  self->SetMouseGrabbed(grab,suppressEvent);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMouseGrabbed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMouseMode of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetMouseMode00
static int tolua_InputLuaAPI_Input_SetMouseMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  MouseMode mode = ((MouseMode) (int)  tolua_tonumber(tolua_S,2,0));
  bool suppressEvent = ((bool)  tolua_toboolean(tolua_S,3,false));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMouseMode'", NULL);
#endif
 {
  self->SetMouseMode(mode,suppressEvent);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMouseMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMouseLocked of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_IsMouseLocked00
static int tolua_InputLuaAPI_Input_IsMouseLocked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMouseLocked'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsMouseLocked();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMouseLocked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddScreenJoystick of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_AddScreenJoystick00
static int tolua_InputLuaAPI_Input_AddScreenJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"XMLFile",1,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"XMLFile",1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  XMLFile* layoutFile = ((XMLFile*)  tolua_tousertype(tolua_S,2,0));
  XMLFile* styleFile = ((XMLFile*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddScreenJoystick'", NULL);
#endif
 {
  int tolua_ret = (int)  self->AddScreenJoystick(layoutFile,styleFile);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddScreenJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveScreenJoystick of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_RemoveScreenJoystick00
static int tolua_InputLuaAPI_Input_RemoveScreenJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveScreenJoystick'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->RemoveScreenJoystick(id);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveScreenJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScreenJoystickVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetScreenJoystickVisible00
static int tolua_InputLuaAPI_Input_SetScreenJoystickVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  bool enable = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScreenJoystickVisible'", NULL);
#endif
 {
  self->SetScreenJoystickVisible(id,enable);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScreenJoystickVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetScreenKeyboardVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetScreenKeyboardVisible00
static int tolua_InputLuaAPI_Input_SetScreenKeyboardVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetScreenKeyboardVisible'", NULL);
#endif
 {
  self->SetScreenKeyboardVisible(enable);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetScreenKeyboardVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTouchEmulation of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetTouchEmulation00
static int tolua_InputLuaAPI_Input_SetTouchEmulation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTouchEmulation'", NULL);
#endif
 {
  self->SetTouchEmulation(enable);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTouchEmulation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RecordGesture of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_RecordGesture00
static int tolua_InputLuaAPI_Input_RecordGesture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RecordGesture'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->RecordGesture();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RecordGesture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InputSaveGestures of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SaveGestures00
static int tolua_InputLuaAPI_Input_SaveGestures00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"File",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  File* dest = ((File*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InputSaveGestures'", NULL);
#endif
 {
  tolua_outside bool tolua_ret = (tolua_outside bool)  InputSaveGestures(self,dest);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SaveGestures'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InputSaveGesture of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SaveGesture00
static int tolua_InputLuaAPI_Input_SaveGesture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"File",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  File* dest = ((File*)  tolua_tousertype(tolua_S,2,0));
  unsigned gestureID = ((unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InputSaveGesture'", NULL);
#endif
 {
  tolua_outside bool tolua_ret = (tolua_outside bool)  InputSaveGesture(self,dest,gestureID);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SaveGesture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InputLoadGestures of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_LoadGestures00
static int tolua_InputLuaAPI_Input_LoadGestures00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isusertype(tolua_S,2,"File",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  File* source = ((File*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InputLoadGestures'", NULL);
#endif
 {
  tolua_outside unsigned tolua_ret = (tolua_outside unsigned)  InputLoadGestures(self,source);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadGestures'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: InputSaveGestures of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SaveGestures01
static int tolua_InputLuaAPI_Input_SaveGestures01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  const String fileName = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InputSaveGestures'", NULL);
#endif
 {
  tolua_outside bool tolua_ret = (tolua_outside bool)  InputSaveGestures(self,fileName);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_InputLuaAPI_Input_SaveGestures00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: InputSaveGesture of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SaveGesture01
static int tolua_InputLuaAPI_Input_SaveGesture01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  const String fileName = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
  unsigned gestureID = ((unsigned)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InputSaveGesture'", NULL);
#endif
 {
  tolua_outside bool tolua_ret = (tolua_outside bool)  InputSaveGesture(self,fileName,gestureID);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_InputLuaAPI_Input_SaveGesture00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: InputLoadGestures of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_LoadGestures01
static int tolua_InputLuaAPI_Input_LoadGestures01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  const String fileName = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InputLoadGestures'", NULL);
#endif
 {
  tolua_outside unsigned tolua_ret = (tolua_outside unsigned)  InputLoadGestures(self,fileName);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
tolua_lerror:
 return tolua_InputLuaAPI_Input_LoadGestures00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveGesture of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_RemoveGesture00
static int tolua_InputLuaAPI_Input_RemoveGesture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  unsigned gestureID = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveGesture'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->RemoveGesture(gestureID);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveGesture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveAllGestures of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_RemoveAllGestures00
static int tolua_InputLuaAPI_Input_RemoveAllGestures00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveAllGestures'", NULL);
#endif
 {
  self->RemoveAllGestures();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveAllGestures'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMousePosition of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_SetMousePosition00
static int tolua_InputLuaAPI_Input_SetMousePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const IntVector2",0,&tolua_err)) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  const IntVector2* position = ((const IntVector2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMousePosition'", NULL);
#endif
 {
  self->SetMousePosition(*position);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMousePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CenterMousePosition of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_CenterMousePosition00
static int tolua_InputLuaAPI_Input_CenterMousePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CenterMousePosition'", NULL);
#endif
 {
  self->CenterMousePosition();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CenterMousePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKeyFromName of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetKeyFromName00
static int tolua_InputLuaAPI_Input_GetKeyFromName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  const String name = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKeyFromName'", NULL);
#endif
 {
  Key tolua_ret = (Key)  self->GetKeyFromName(name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKeyFromName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKeyFromScancode of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetKeyFromScancode00
static int tolua_InputLuaAPI_Input_GetKeyFromScancode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Scancode scancode = ((Scancode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKeyFromScancode'", NULL);
#endif
 {
  Key tolua_ret = (Key)  self->GetKeyFromScancode(scancode);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKeyFromScancode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKeyName of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetKeyName00
static int tolua_InputLuaAPI_Input_GetKeyName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Key key = ((Key) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKeyName'", NULL);
#endif
 {
  String tolua_ret = (String)  self->GetKeyName(key);
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKeyName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScancodeFromKey of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetScancodeFromKey00
static int tolua_InputLuaAPI_Input_GetScancodeFromKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Key key = ((Key) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScancodeFromKey'", NULL);
#endif
 {
  Scancode tolua_ret = (Scancode)  self->GetScancodeFromKey(key);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScancodeFromKey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScancodeFromName of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetScancodeFromName00
static int tolua_InputLuaAPI_Input_GetScancodeFromName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  const String name = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScancodeFromName'", NULL);
#endif
 {
  Scancode tolua_ret = (Scancode)  self->GetScancodeFromName(name);
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScancodeFromName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScancodeName of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetScancodeName00
static int tolua_InputLuaAPI_Input_GetScancodeName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Scancode scancode = ((Scancode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScancodeName'", NULL);
#endif
 {
  String tolua_ret = (String)  self->GetScancodeName(scancode);
 tolua_pushurho3dstring(tolua_S,(const char*)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScancodeName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKeyDown of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetKeyDown00
static int tolua_InputLuaAPI_Input_GetKeyDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Key key = ((Key) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKeyDown'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetKeyDown(key);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKeyDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetKeyPress of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetKeyPress00
static int tolua_InputLuaAPI_Input_GetKeyPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Key key = ((Key) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetKeyPress'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetKeyPress(key);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetKeyPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScancodeDown of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetScancodeDown00
static int tolua_InputLuaAPI_Input_GetScancodeDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Scancode scancode = ((Scancode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScancodeDown'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetScancodeDown(scancode);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScancodeDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScancodePress of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetScancodePress00
static int tolua_InputLuaAPI_Input_GetScancodePress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Scancode scancode = ((Scancode) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScancodePress'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetScancodePress(scancode);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScancodePress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseButtonDown of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseButtonDown00
static int tolua_InputLuaAPI_Input_GetMouseButtonDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  MouseButton button = ((MouseButton) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseButtonDown'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetMouseButtonDown(button);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseButtonDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseButtonPress of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseButtonPress00
static int tolua_InputLuaAPI_Input_GetMouseButtonPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  MouseButton button = ((MouseButton) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseButtonPress'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetMouseButtonPress(button);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseButtonPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetQualifierDown of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetQualifierDown00
static int tolua_InputLuaAPI_Input_GetQualifierDown00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Qualifier qualifier = ((Qualifier) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetQualifierDown'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetQualifierDown(qualifier);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetQualifierDown'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetQualifierPress of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetQualifierPress00
static int tolua_InputLuaAPI_Input_GetQualifierPress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  Qualifier qualifier = ((Qualifier) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetQualifierPress'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetQualifierPress(qualifier);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetQualifierPress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetQualifiers of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetQualifiers00
static int tolua_InputLuaAPI_Input_GetQualifiers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetQualifiers'", NULL);
#endif
 {
  int tolua_ret = (int)  self->GetQualifiers();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetQualifiers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMousePosition of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMousePosition00
static int tolua_InputLuaAPI_Input_GetMousePosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMousePosition'", NULL);
#endif
 {
  IntVector2 tolua_ret = (IntVector2)  self->GetMousePosition();
 {
#ifdef __cplusplus
 void* tolua_obj = Mtolua_new((IntVector2)(tolua_ret));
  tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(IntVector2));
  tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMousePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseMove of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseMove00
static int tolua_InputLuaAPI_Input_GetMouseMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseMove'", NULL);
#endif
 {
  IntVector2 tolua_ret = (IntVector2)  self->GetMouseMove();
 {
#ifdef __cplusplus
 void* tolua_obj = Mtolua_new((IntVector2)(tolua_ret));
  tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(IntVector2));
  tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseMoveX of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseMoveX00
static int tolua_InputLuaAPI_Input_GetMouseMoveX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseMoveX'", NULL);
#endif
 {
  int tolua_ret = (int)  self->GetMouseMoveX();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseMoveX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseMoveY of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseMoveY00
static int tolua_InputLuaAPI_Input_GetMouseMoveY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseMoveY'", NULL);
#endif
 {
  int tolua_ret = (int)  self->GetMouseMoveY();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseMoveY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseMoveWheel of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseMoveWheel00
static int tolua_InputLuaAPI_Input_GetMouseMoveWheel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseMoveWheel'", NULL);
#endif
 {
  int tolua_ret = (int)  self->GetMouseMoveWheel();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseMoveWheel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInputScale of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetInputScale00
static int tolua_InputLuaAPI_Input_GetInputScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInputScale'", NULL);
#endif
 {
  Vector2 tolua_ret = (Vector2)  self->GetInputScale();
 {
#ifdef __cplusplus
 void* tolua_obj = Mtolua_new((Vector2)(tolua_ret));
  tolua_pushusertype(tolua_S,tolua_obj,"Vector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector2));
  tolua_pushusertype(tolua_S,tolua_obj,"Vector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
 }
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInputScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumTouches of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetNumTouches00
static int tolua_InputLuaAPI_Input_GetNumTouches00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumTouches'", NULL);
#endif
 {
  unsigned tolua_ret = (unsigned)  self->GetNumTouches();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumTouches'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTouch of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetTouch00
static int tolua_InputLuaAPI_Input_GetTouch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  unsigned index = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTouch'", NULL);
#endif
 {
  TouchState* tolua_ret = (TouchState*)  self->GetTouch(index);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"TouchState");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTouch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetNumJoysticks of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetNumJoysticks00
static int tolua_InputLuaAPI_Input_GetNumJoysticks00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetNumJoysticks'", NULL);
#endif
 {
  unsigned tolua_ret = (unsigned)  self->GetNumJoysticks();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetNumJoysticks'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetJoystick of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetJoystick00
static int tolua_InputLuaAPI_Input_GetJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetJoystick'", NULL);
#endif
 {
  JoystickState* tolua_ret = (JoystickState*)  self->GetJoystick(id);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"JoystickState");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetJoystickByIndex of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetJoystickByIndex00
static int tolua_InputLuaAPI_Input_GetJoystickByIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  unsigned index = ((unsigned)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetJoystickByIndex'", NULL);
#endif
 {
  JoystickState* tolua_ret = (JoystickState*)  self->GetJoystickByIndex(index);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"JoystickState");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetJoystickByIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetJoystickByName of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetJoystickByName00
static int tolua_InputLuaAPI_Input_GetJoystickByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isurho3dstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
  const String name = ((const String)  tolua_tourho3dstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetJoystickByName'", NULL);
#endif
 {
  JoystickState* tolua_ret = (JoystickState*)  self->GetJoystickByName(name);
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"JoystickState");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetJoystickByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetToggleFullscreen of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetToggleFullscreen00
static int tolua_InputLuaAPI_Input_GetToggleFullscreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetToggleFullscreen'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetToggleFullscreen();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetToggleFullscreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetScreenKeyboardSupport of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetScreenKeyboardSupport00
static int tolua_InputLuaAPI_Input_GetScreenKeyboardSupport00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetScreenKeyboardSupport'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetScreenKeyboardSupport();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScreenKeyboardSupport'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsScreenJoystickVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_IsScreenJoystickVisible00
static int tolua_InputLuaAPI_Input_IsScreenJoystickVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsScreenJoystickVisible'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsScreenJoystickVisible(id);
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsScreenJoystickVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsScreenKeyboardVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_IsScreenKeyboardVisible00
static int tolua_InputLuaAPI_Input_IsScreenKeyboardVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsScreenKeyboardVisible'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsScreenKeyboardVisible();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsScreenKeyboardVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetTouchEmulation of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetTouchEmulation00
static int tolua_InputLuaAPI_Input_GetTouchEmulation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetTouchEmulation'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->GetTouchEmulation();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetTouchEmulation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMouseVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_IsMouseVisible00
static int tolua_InputLuaAPI_Input_IsMouseVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMouseVisible'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsMouseVisible();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMouseVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMouseGrabbed of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_IsMouseGrabbed00
static int tolua_InputLuaAPI_Input_IsMouseGrabbed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMouseGrabbed'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsMouseGrabbed();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMouseGrabbed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMouseMode of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_GetMouseMode00
static int tolua_InputLuaAPI_Input_GetMouseMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMouseMode'", NULL);
#endif
 {
  MouseMode tolua_ret = (MouseMode)  self->GetMouseMode();
 tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMouseMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HasFocus of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_HasFocus00
static int tolua_InputLuaAPI_Input_HasFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HasFocus'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->HasFocus();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HasFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsMinimized of class  Input */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_Input_IsMinimized00
static int tolua_InputLuaAPI_Input_IsMinimized00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"const Input",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const Input* self = (const Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsMinimized'", NULL);
#endif
 {
  bool tolua_ret = (bool)  self->IsMinimized();
 tolua_pushboolean(tolua_S,(bool)tolua_ret);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsMinimized'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: qualifiers of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_qualifiers
static int tolua_get_Input_qualifiers(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'qualifiers'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetQualifiers());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mousePosition of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mousePosition
static int tolua_get_Input_mousePosition(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mousePosition'",NULL);
#endif
 IntVector2 tolua_ret = (IntVector2)self->GetMousePosition();
 #ifdef __cplusplus
 void* tolua_obj = (void*)Mtolua_new((IntVector2)(tolua_ret));
 tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 #else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(IntVector2));
 tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 #endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mousePosition of class  Input */
#ifndef TOLUA_DISABLE_tolua_set_Input_mousePosition
static int tolua_set_Input_mousePosition(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mousePosition'",NULL);
 if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"IntVector2",0,&tolua_err)))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SetMousePosition(*((IntVector2*)  tolua_tousertype(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseMove of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseMove
static int tolua_get_Input_mouseMove(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseMove'",NULL);
#endif
 IntVector2 tolua_ret = (IntVector2)self->GetMouseMove();
 #ifdef __cplusplus
 void* tolua_obj = (void*)Mtolua_new((IntVector2)(tolua_ret));
 tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 #else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(IntVector2));
 tolua_pushusertype(tolua_S,tolua_obj,"IntVector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 #endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseMoveX of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseMoveX
static int tolua_get_Input_mouseMoveX(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseMoveX'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetMouseMoveX());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseMoveY of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseMoveY
static int tolua_get_Input_mouseMoveY(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseMoveY'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetMouseMoveY());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseMoveWheel of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseMoveWheel
static int tolua_get_Input_mouseMoveWheel(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseMoveWheel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetMouseMoveWheel());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: inputScale of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_inputScale
static int tolua_get_Input_inputScale(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'inputScale'",NULL);
#endif
 Vector2 tolua_ret = (Vector2)self->GetInputScale();
 #ifdef __cplusplus
 void* tolua_obj = (void*)Mtolua_new((Vector2)(tolua_ret));
 tolua_pushusertype(tolua_S,tolua_obj,"Vector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 #else
 void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector2));
 tolua_pushusertype(tolua_S,tolua_obj,"Vector2");
 tolua_register_gc(tolua_S,lua_gettop(tolua_S));
 #endif
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: numTouches of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_numTouches
static int tolua_get_Input_numTouches(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numTouches'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetNumTouches());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: numJoysticks of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_numJoysticks
static int tolua_get_Input_numJoysticks(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'numJoysticks'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetNumJoysticks());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: toggleFullscreen of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_toggleFullscreen
static int tolua_get_Input_toggleFullscreen(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'toggleFullscreen'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->GetToggleFullscreen());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: screenKeyboardSupport of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_screenKeyboardSupport
static int tolua_get_Input_screenKeyboardSupport(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'screenKeyboardSupport'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->GetScreenKeyboardSupport());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseMode of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseMode
static int tolua_get_Input_mouseMode(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseMode'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->GetMouseMode());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mouseMode of class  Input */
#ifndef TOLUA_DISABLE_tolua_set_Input_mouseMode
static int tolua_set_Input_mouseMode(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseMode'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SetMouseMode(((MouseMode) (int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: screenKeyboardVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_screenKeyboardVisible
static int tolua_get_Input_screenKeyboardVisible(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'screenKeyboardVisible'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->IsScreenKeyboardVisible());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: screenKeyboardVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_set_Input_screenKeyboardVisible
static int tolua_set_Input_screenKeyboardVisible(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'screenKeyboardVisible'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SetScreenKeyboardVisible(((bool)  tolua_toboolean(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: touchEmulation of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_touchEmulation
static int tolua_get_Input_touchEmulation(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'touchEmulation'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->GetTouchEmulation());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: touchEmulation of class  Input */
#ifndef TOLUA_DISABLE_tolua_set_Input_touchEmulation
static int tolua_set_Input_touchEmulation(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'touchEmulation'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SetTouchEmulation(((bool)  tolua_toboolean(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseVisible
static int tolua_get_Input_mouseVisible(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseVisible'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->IsMouseVisible());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mouseVisible of class  Input */
#ifndef TOLUA_DISABLE_tolua_set_Input_mouseVisible
static int tolua_set_Input_mouseVisible(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseVisible'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SetMouseVisible(((bool)  tolua_toboolean(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseGrabbed of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseGrabbed
static int tolua_get_Input_mouseGrabbed(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseGrabbed'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->IsMouseGrabbed());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mouseGrabbed of class  Input */
#ifndef TOLUA_DISABLE_tolua_set_Input_mouseGrabbed
static int tolua_set_Input_mouseGrabbed(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseGrabbed'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SetMouseGrabbed(((bool)  tolua_toboolean(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mouseLocked of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_mouseLocked
static int tolua_get_Input_mouseLocked(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mouseLocked'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->IsMouseLocked());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: focus of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_focus
static int tolua_get_Input_focus(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'focus'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->HasFocus());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: minimized of class  Input */
#ifndef TOLUA_DISABLE_tolua_get_Input_minimized
static int tolua_get_Input_minimized(lua_State* tolua_S)
{
  Input* self = (Input*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'minimized'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->IsMinimized());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetInput */
#ifndef TOLUA_DISABLE_tolua_InputLuaAPI_GetInput00
static int tolua_InputLuaAPI_GetInput00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Input* tolua_ret = (Input*)  GetInput();
  tolua_pushusertype(tolua_S,(void*)tolua_ret,"Input");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInput'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: input */
#ifndef TOLUA_DISABLE_tolua_get_input_ptr
static int tolua_get_input_ptr(lua_State* tolua_S)
{
  tolua_pushusertype(tolua_S,(void*)GetInput(),"Input");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_InputLuaAPI_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"Controls","Controls","",tolua_collect_Controls);
 #else
 tolua_cclass(tolua_S,"Controls","Controls","",NULL);
 #endif
 tolua_beginmodule(tolua_S,"Controls");
  tolua_function(tolua_S,"new",tolua_InputLuaAPI_Controls_new00);
  tolua_function(tolua_S,"new_local",tolua_InputLuaAPI_Controls_new00_local);
  tolua_function(tolua_S,".call",tolua_InputLuaAPI_Controls_new00_local);
  tolua_function(tolua_S,"delete",tolua_InputLuaAPI_Controls_delete00);
  tolua_function(tolua_S,"Reset",tolua_InputLuaAPI_Controls_Reset00);
  tolua_function(tolua_S,"Set",tolua_InputLuaAPI_Controls_Set00);
  tolua_function(tolua_S,"IsDown",tolua_InputLuaAPI_Controls_IsDown00);
  tolua_function(tolua_S,"IsPressed",tolua_InputLuaAPI_Controls_IsPressed00);
  tolua_variable(tolua_S,"buttons",tolua_get_Controls_buttons,tolua_set_Controls_buttons);
  tolua_variable(tolua_S,"yaw",tolua_get_Controls_yaw,tolua_set_Controls_yaw);
  tolua_variable(tolua_S,"pitch",tolua_get_Controls_pitch,tolua_set_Controls_pitch);
  tolua_variable(tolua_S,"extraData",tolua_get_Controls_extraData,tolua_set_Controls_extraData);
 tolua_endmodule(tolua_S);
 tolua_constant(tolua_S,"MM_ABSOLUTE",MM_ABSOLUTE);
 tolua_constant(tolua_S,"MM_RELATIVE",MM_RELATIVE);
 tolua_constant(tolua_S,"MM_WRAP",MM_WRAP);
 tolua_constant(tolua_S,"MM_FREE",MM_FREE);
 tolua_cclass(tolua_S,"TouchState","TouchState","",NULL);
 tolua_beginmodule(tolua_S,"TouchState");
  tolua_function(tolua_S,"GetTouchedElement",tolua_InputLuaAPI_TouchState_GetTouchedElement00);
  tolua_variable(tolua_S,"touchID",tolua_get_TouchState_touchID,tolua_set_TouchState_touchID);
  tolua_variable(tolua_S,"position",tolua_get_TouchState_position,tolua_set_TouchState_position);
  tolua_variable(tolua_S,"lastPosition",tolua_get_TouchState_lastPosition,tolua_set_TouchState_lastPosition);
  tolua_variable(tolua_S,"delta",tolua_get_TouchState_delta,tolua_set_TouchState_delta);
  tolua_variable(tolua_S,"pressure",tolua_get_TouchState_pressure,tolua_set_TouchState_pressure);
  tolua_variable(tolua_S,"touchedElement",tolua_get_TouchState_touchedElement_ptr,NULL);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"JoystickState","JoystickState","",NULL);
 tolua_beginmodule(tolua_S,"JoystickState");
  tolua_variable(tolua_S,"name",tolua_get_JoystickState_name,NULL);
  tolua_variable(tolua_S,"joystickID",tolua_get_JoystickState_joystickID,NULL);
  tolua_function(tolua_S,"IsController",tolua_InputLuaAPI_JoystickState_IsController00);
  tolua_function(tolua_S,"GetNumButtons",tolua_InputLuaAPI_JoystickState_GetNumButtons00);
  tolua_function(tolua_S,"GetNumAxes",tolua_InputLuaAPI_JoystickState_GetNumAxes00);
  tolua_function(tolua_S,"GetNumHats",tolua_InputLuaAPI_JoystickState_GetNumHats00);
  tolua_function(tolua_S,"GetButtonDown",tolua_InputLuaAPI_JoystickState_GetButtonDown00);
  tolua_function(tolua_S,"GetButtonPress",tolua_InputLuaAPI_JoystickState_GetButtonPress00);
  tolua_function(tolua_S,"GetAxisPosition",tolua_InputLuaAPI_JoystickState_GetAxisPosition00);
  tolua_function(tolua_S,"GetHatPosition",tolua_InputLuaAPI_JoystickState_GetHatPosition00);
  tolua_variable(tolua_S,"controller",tolua_get_JoystickState_controller,NULL);
  tolua_variable(tolua_S,"numButtons",tolua_get_JoystickState_numButtons,NULL);
  tolua_variable(tolua_S,"numAxes",tolua_get_JoystickState_numAxes,NULL);
  tolua_variable(tolua_S,"numHats",tolua_get_JoystickState_numHats,NULL);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Input","Input","Object",NULL);
 tolua_beginmodule(tolua_S,"Input");
  tolua_function(tolua_S,"SetToggleFullscreen",tolua_InputLuaAPI_Input_SetToggleFullscreen00);
  tolua_function(tolua_S,"SetMouseVisible",tolua_InputLuaAPI_Input_SetMouseVisible00);
  tolua_function(tolua_S,"SetMouseGrabbed",tolua_InputLuaAPI_Input_SetMouseGrabbed00);
  tolua_function(tolua_S,"SetMouseMode",tolua_InputLuaAPI_Input_SetMouseMode00);
  tolua_function(tolua_S,"IsMouseLocked",tolua_InputLuaAPI_Input_IsMouseLocked00);
  tolua_function(tolua_S,"AddScreenJoystick",tolua_InputLuaAPI_Input_AddScreenJoystick00);
  tolua_function(tolua_S,"RemoveScreenJoystick",tolua_InputLuaAPI_Input_RemoveScreenJoystick00);
  tolua_function(tolua_S,"SetScreenJoystickVisible",tolua_InputLuaAPI_Input_SetScreenJoystickVisible00);
  tolua_function(tolua_S,"SetScreenKeyboardVisible",tolua_InputLuaAPI_Input_SetScreenKeyboardVisible00);
  tolua_function(tolua_S,"SetTouchEmulation",tolua_InputLuaAPI_Input_SetTouchEmulation00);
  tolua_function(tolua_S,"RecordGesture",tolua_InputLuaAPI_Input_RecordGesture00);
  tolua_function(tolua_S,"SaveGestures",tolua_InputLuaAPI_Input_SaveGestures00);
  tolua_function(tolua_S,"SaveGesture",tolua_InputLuaAPI_Input_SaveGesture00);
  tolua_function(tolua_S,"LoadGestures",tolua_InputLuaAPI_Input_LoadGestures00);
  tolua_function(tolua_S,"SaveGestures",tolua_InputLuaAPI_Input_SaveGestures01);
  tolua_function(tolua_S,"SaveGesture",tolua_InputLuaAPI_Input_SaveGesture01);
  tolua_function(tolua_S,"LoadGestures",tolua_InputLuaAPI_Input_LoadGestures01);
  tolua_function(tolua_S,"RemoveGesture",tolua_InputLuaAPI_Input_RemoveGesture00);
  tolua_function(tolua_S,"RemoveAllGestures",tolua_InputLuaAPI_Input_RemoveAllGestures00);
  tolua_function(tolua_S,"SetMousePosition",tolua_InputLuaAPI_Input_SetMousePosition00);
  tolua_function(tolua_S,"CenterMousePosition",tolua_InputLuaAPI_Input_CenterMousePosition00);
  tolua_function(tolua_S,"GetKeyFromName",tolua_InputLuaAPI_Input_GetKeyFromName00);
  tolua_function(tolua_S,"GetKeyFromScancode",tolua_InputLuaAPI_Input_GetKeyFromScancode00);
  tolua_function(tolua_S,"GetKeyName",tolua_InputLuaAPI_Input_GetKeyName00);
  tolua_function(tolua_S,"GetScancodeFromKey",tolua_InputLuaAPI_Input_GetScancodeFromKey00);
  tolua_function(tolua_S,"GetScancodeFromName",tolua_InputLuaAPI_Input_GetScancodeFromName00);
  tolua_function(tolua_S,"GetScancodeName",tolua_InputLuaAPI_Input_GetScancodeName00);
  tolua_function(tolua_S,"GetKeyDown",tolua_InputLuaAPI_Input_GetKeyDown00);
  tolua_function(tolua_S,"GetKeyPress",tolua_InputLuaAPI_Input_GetKeyPress00);
  tolua_function(tolua_S,"GetScancodeDown",tolua_InputLuaAPI_Input_GetScancodeDown00);
  tolua_function(tolua_S,"GetScancodePress",tolua_InputLuaAPI_Input_GetScancodePress00);
  tolua_function(tolua_S,"GetMouseButtonDown",tolua_InputLuaAPI_Input_GetMouseButtonDown00);
  tolua_function(tolua_S,"GetMouseButtonPress",tolua_InputLuaAPI_Input_GetMouseButtonPress00);
  tolua_function(tolua_S,"GetQualifierDown",tolua_InputLuaAPI_Input_GetQualifierDown00);
  tolua_function(tolua_S,"GetQualifierPress",tolua_InputLuaAPI_Input_GetQualifierPress00);
  tolua_function(tolua_S,"GetQualifiers",tolua_InputLuaAPI_Input_GetQualifiers00);
  tolua_function(tolua_S,"GetMousePosition",tolua_InputLuaAPI_Input_GetMousePosition00);
  tolua_function(tolua_S,"GetMouseMove",tolua_InputLuaAPI_Input_GetMouseMove00);
  tolua_function(tolua_S,"GetMouseMoveX",tolua_InputLuaAPI_Input_GetMouseMoveX00);
  tolua_function(tolua_S,"GetMouseMoveY",tolua_InputLuaAPI_Input_GetMouseMoveY00);
  tolua_function(tolua_S,"GetMouseMoveWheel",tolua_InputLuaAPI_Input_GetMouseMoveWheel00);
  tolua_function(tolua_S,"GetInputScale",tolua_InputLuaAPI_Input_GetInputScale00);
  tolua_function(tolua_S,"GetNumTouches",tolua_InputLuaAPI_Input_GetNumTouches00);
  tolua_function(tolua_S,"GetTouch",tolua_InputLuaAPI_Input_GetTouch00);
  tolua_function(tolua_S,"GetNumJoysticks",tolua_InputLuaAPI_Input_GetNumJoysticks00);
  tolua_function(tolua_S,"GetJoystick",tolua_InputLuaAPI_Input_GetJoystick00);
  tolua_function(tolua_S,"GetJoystickByIndex",tolua_InputLuaAPI_Input_GetJoystickByIndex00);
  tolua_function(tolua_S,"GetJoystickByName",tolua_InputLuaAPI_Input_GetJoystickByName00);
  tolua_function(tolua_S,"GetToggleFullscreen",tolua_InputLuaAPI_Input_GetToggleFullscreen00);
  tolua_function(tolua_S,"GetScreenKeyboardSupport",tolua_InputLuaAPI_Input_GetScreenKeyboardSupport00);
  tolua_function(tolua_S,"IsScreenJoystickVisible",tolua_InputLuaAPI_Input_IsScreenJoystickVisible00);
  tolua_function(tolua_S,"IsScreenKeyboardVisible",tolua_InputLuaAPI_Input_IsScreenKeyboardVisible00);
  tolua_function(tolua_S,"GetTouchEmulation",tolua_InputLuaAPI_Input_GetTouchEmulation00);
  tolua_function(tolua_S,"IsMouseVisible",tolua_InputLuaAPI_Input_IsMouseVisible00);
  tolua_function(tolua_S,"IsMouseGrabbed",tolua_InputLuaAPI_Input_IsMouseGrabbed00);
  tolua_function(tolua_S,"GetMouseMode",tolua_InputLuaAPI_Input_GetMouseMode00);
  tolua_function(tolua_S,"HasFocus",tolua_InputLuaAPI_Input_HasFocus00);
  tolua_function(tolua_S,"IsMinimized",tolua_InputLuaAPI_Input_IsMinimized00);
  tolua_variable(tolua_S,"qualifiers",tolua_get_Input_qualifiers,NULL);
  tolua_variable(tolua_S,"mousePosition",tolua_get_Input_mousePosition,tolua_set_Input_mousePosition);
  tolua_variable(tolua_S,"mouseMove",tolua_get_Input_mouseMove,NULL);
  tolua_variable(tolua_S,"mouseMoveX",tolua_get_Input_mouseMoveX,NULL);
  tolua_variable(tolua_S,"mouseMoveY",tolua_get_Input_mouseMoveY,NULL);
  tolua_variable(tolua_S,"mouseMoveWheel",tolua_get_Input_mouseMoveWheel,NULL);
  tolua_variable(tolua_S,"inputScale",tolua_get_Input_inputScale,NULL);
  tolua_variable(tolua_S,"numTouches",tolua_get_Input_numTouches,NULL);
  tolua_variable(tolua_S,"numJoysticks",tolua_get_Input_numJoysticks,NULL);
  tolua_variable(tolua_S,"toggleFullscreen",tolua_get_Input_toggleFullscreen,NULL);
  tolua_variable(tolua_S,"screenKeyboardSupport",tolua_get_Input_screenKeyboardSupport,NULL);
  tolua_variable(tolua_S,"mouseMode",tolua_get_Input_mouseMode,tolua_set_Input_mouseMode);
  tolua_variable(tolua_S,"screenKeyboardVisible",tolua_get_Input_screenKeyboardVisible,tolua_set_Input_screenKeyboardVisible);
  tolua_variable(tolua_S,"touchEmulation",tolua_get_Input_touchEmulation,tolua_set_Input_touchEmulation);
  tolua_variable(tolua_S,"mouseVisible",tolua_get_Input_mouseVisible,tolua_set_Input_mouseVisible);
  tolua_variable(tolua_S,"mouseGrabbed",tolua_get_Input_mouseGrabbed,tolua_set_Input_mouseGrabbed);
  tolua_variable(tolua_S,"mouseLocked",tolua_get_Input_mouseLocked,NULL);
  tolua_variable(tolua_S,"focus",tolua_get_Input_focus,NULL);
  tolua_variable(tolua_S,"minimized",tolua_get_Input_minimized,NULL);
 tolua_endmodule(tolua_S);
 tolua_function(tolua_S,"GetInput",tolua_InputLuaAPI_GetInput00);
 tolua_variable(tolua_S,"input",tolua_get_input_ptr,NULL);
 tolua_constant(tolua_S,"MOUSEB_LEFT",MOUSEB_LEFT);
 tolua_constant(tolua_S,"MOUSEB_MIDDLE",MOUSEB_MIDDLE);
 tolua_constant(tolua_S,"MOUSEB_RIGHT",MOUSEB_RIGHT);
 tolua_constant(tolua_S,"QUAL_SHIFT",QUAL_SHIFT);
 tolua_constant(tolua_S,"QUAL_CTRL",QUAL_CTRL);
 tolua_constant(tolua_S,"QUAL_ALT",QUAL_ALT);
 tolua_constant(tolua_S,"QUAL_ANY",QUAL_ANY);
 tolua_constant(tolua_S,"KEY_UNKNOWN",KEY_UNKNOWN);
 tolua_constant(tolua_S,"KEY_A",KEY_A);
 tolua_constant(tolua_S,"KEY_B",KEY_B);
 tolua_constant(tolua_S,"KEY_C",KEY_C);
 tolua_constant(tolua_S,"KEY_D",KEY_D);
 tolua_constant(tolua_S,"KEY_E",KEY_E);
 tolua_constant(tolua_S,"KEY_F",KEY_F);
 tolua_constant(tolua_S,"KEY_G",KEY_G);
 tolua_constant(tolua_S,"KEY_H",KEY_H);
 tolua_constant(tolua_S,"KEY_I",KEY_I);
 tolua_constant(tolua_S,"KEY_J",KEY_J);
 tolua_constant(tolua_S,"KEY_K",KEY_K);
 tolua_constant(tolua_S,"KEY_L",KEY_L);
 tolua_constant(tolua_S,"KEY_M",KEY_M);
 tolua_constant(tolua_S,"KEY_N",KEY_N);
 tolua_constant(tolua_S,"KEY_O",KEY_O);
 tolua_constant(tolua_S,"KEY_P",KEY_P);
 tolua_constant(tolua_S,"KEY_Q",KEY_Q);
 tolua_constant(tolua_S,"KEY_R",KEY_R);
 tolua_constant(tolua_S,"KEY_S",KEY_S);
 tolua_constant(tolua_S,"KEY_T",KEY_T);
 tolua_constant(tolua_S,"KEY_U",KEY_U);
 tolua_constant(tolua_S,"KEY_V",KEY_V);
 tolua_constant(tolua_S,"KEY_W",KEY_W);
 tolua_constant(tolua_S,"KEY_X",KEY_X);
 tolua_constant(tolua_S,"KEY_Y",KEY_Y);
 tolua_constant(tolua_S,"KEY_Z",KEY_Z);
 tolua_constant(tolua_S,"KEY_0",KEY_0);
 tolua_constant(tolua_S,"KEY_1",KEY_1);
 tolua_constant(tolua_S,"KEY_2",KEY_2);
 tolua_constant(tolua_S,"KEY_3",KEY_3);
 tolua_constant(tolua_S,"KEY_4",KEY_4);
 tolua_constant(tolua_S,"KEY_5",KEY_5);
 tolua_constant(tolua_S,"KEY_6",KEY_6);
 tolua_constant(tolua_S,"KEY_7",KEY_7);
 tolua_constant(tolua_S,"KEY_8",KEY_8);
 tolua_constant(tolua_S,"KEY_9",KEY_9);
 tolua_constant(tolua_S,"KEY_BACKSPACE",KEY_BACKSPACE);
 tolua_constant(tolua_S,"KEY_TAB",KEY_TAB);
 tolua_constant(tolua_S,"KEY_RETURN",KEY_RETURN);
 tolua_constant(tolua_S,"KEY_RETURN2",KEY_RETURN2);
 tolua_constant(tolua_S,"KEY_KP_ENTER",KEY_KP_ENTER);
 tolua_constant(tolua_S,"KEY_SHIFT",KEY_SHIFT);
 tolua_constant(tolua_S,"KEY_CTRL",KEY_CTRL);
 tolua_constant(tolua_S,"KEY_ALT",KEY_ALT);
 tolua_constant(tolua_S,"KEY_GUI",KEY_GUI);
 tolua_constant(tolua_S,"KEY_PAUSE",KEY_PAUSE);
 tolua_constant(tolua_S,"KEY_CAPSLOCK",KEY_CAPSLOCK);
 tolua_constant(tolua_S,"KEY_ESCAPE",KEY_ESCAPE);
 tolua_constant(tolua_S,"KEY_SPACE",KEY_SPACE);
 tolua_constant(tolua_S,"KEY_PAGEUP",KEY_PAGEUP);
 tolua_constant(tolua_S,"KEY_PAGEDOWN",KEY_PAGEDOWN);
 tolua_constant(tolua_S,"KEY_END",KEY_END);
 tolua_constant(tolua_S,"KEY_HOME",KEY_HOME);
 tolua_constant(tolua_S,"KEY_LEFT",KEY_LEFT);
 tolua_constant(tolua_S,"KEY_UP",KEY_UP);
 tolua_constant(tolua_S,"KEY_RIGHT",KEY_RIGHT);
 tolua_constant(tolua_S,"KEY_DOWN",KEY_DOWN);
 tolua_constant(tolua_S,"KEY_SELECT",KEY_SELECT);
 tolua_constant(tolua_S,"KEY_PRINTSCREEN",KEY_PRINTSCREEN);
 tolua_constant(tolua_S,"KEY_INSERT",KEY_INSERT);
 tolua_constant(tolua_S,"KEY_DELETE",KEY_DELETE);
 tolua_constant(tolua_S,"KEY_LGUI",KEY_LGUI);
 tolua_constant(tolua_S,"KEY_RGUI",KEY_RGUI);
 tolua_constant(tolua_S,"KEY_APPLICATION",KEY_APPLICATION);
 tolua_constant(tolua_S,"KEY_KP_0",KEY_KP_0);
 tolua_constant(tolua_S,"KEY_KP_1",KEY_KP_1);
 tolua_constant(tolua_S,"KEY_KP_2",KEY_KP_2);
 tolua_constant(tolua_S,"KEY_KP_3",KEY_KP_3);
 tolua_constant(tolua_S,"KEY_KP_4",KEY_KP_4);
 tolua_constant(tolua_S,"KEY_KP_5",KEY_KP_5);
 tolua_constant(tolua_S,"KEY_KP_6",KEY_KP_6);
 tolua_constant(tolua_S,"KEY_KP_7",KEY_KP_7);
 tolua_constant(tolua_S,"KEY_KP_8",KEY_KP_8);
 tolua_constant(tolua_S,"KEY_KP_9",KEY_KP_9);
 tolua_constant(tolua_S,"KEY_KP_MULTIPLY",KEY_KP_MULTIPLY);
 tolua_constant(tolua_S,"KEY_KP_PLUS",KEY_KP_PLUS);
 tolua_constant(tolua_S,"KEY_KP_MINUS",KEY_KP_MINUS);
 tolua_constant(tolua_S,"KEY_KP_PERIOD",KEY_KP_PERIOD);
 tolua_constant(tolua_S,"KEY_KP_DIVIDE",KEY_KP_DIVIDE);
 tolua_constant(tolua_S,"KEY_F1",KEY_F1);
 tolua_constant(tolua_S,"KEY_F2",KEY_F2);
 tolua_constant(tolua_S,"KEY_F3",KEY_F3);
 tolua_constant(tolua_S,"KEY_F4",KEY_F4);
 tolua_constant(tolua_S,"KEY_F5",KEY_F5);
 tolua_constant(tolua_S,"KEY_F6",KEY_F6);
 tolua_constant(tolua_S,"KEY_F7",KEY_F7);
 tolua_constant(tolua_S,"KEY_F8",KEY_F8);
 tolua_constant(tolua_S,"KEY_F9",KEY_F9);
 tolua_constant(tolua_S,"KEY_F10",KEY_F10);
 tolua_constant(tolua_S,"KEY_F11",KEY_F11);
 tolua_constant(tolua_S,"KEY_F12",KEY_F12);
 tolua_constant(tolua_S,"KEY_F13",KEY_F13);
 tolua_constant(tolua_S,"KEY_F14",KEY_F14);
 tolua_constant(tolua_S,"KEY_F15",KEY_F15);
 tolua_constant(tolua_S,"KEY_F16",KEY_F16);
 tolua_constant(tolua_S,"KEY_F17",KEY_F17);
 tolua_constant(tolua_S,"KEY_F18",KEY_F18);
 tolua_constant(tolua_S,"KEY_F19",KEY_F19);
 tolua_constant(tolua_S,"KEY_F20",KEY_F20);
 tolua_constant(tolua_S,"KEY_F21",KEY_F21);
 tolua_constant(tolua_S,"KEY_F22",KEY_F22);
 tolua_constant(tolua_S,"KEY_F23",KEY_F23);
 tolua_constant(tolua_S,"KEY_F24",KEY_F24);
 tolua_constant(tolua_S,"KEY_NUMLOCKCLEAR",KEY_NUMLOCKCLEAR);
 tolua_constant(tolua_S,"KEY_SCROLLLOCK",KEY_SCROLLLOCK);
 tolua_constant(tolua_S,"KEY_LSHIFT",KEY_LSHIFT);
 tolua_constant(tolua_S,"KEY_RSHIFT",KEY_RSHIFT);
 tolua_constant(tolua_S,"KEY_LCTRL",KEY_LCTRL);
 tolua_constant(tolua_S,"KEY_RCTRL",KEY_RCTRL);
 tolua_constant(tolua_S,"KEY_LALT",KEY_LALT);
 tolua_constant(tolua_S,"KEY_RALT",KEY_RALT);
 tolua_constant(tolua_S,"KEY_AC_BACK",KEY_AC_BACK);
 tolua_constant(tolua_S,"KEY_AC_BOOKMARKS",KEY_AC_BOOKMARKS);
 tolua_constant(tolua_S,"KEY_AC_FORWARD",KEY_AC_FORWARD);
 tolua_constant(tolua_S,"KEY_AC_HOME",KEY_AC_HOME);
 tolua_constant(tolua_S,"KEY_AC_REFRESH",KEY_AC_REFRESH);
 tolua_constant(tolua_S,"KEY_AC_SEARCH",KEY_AC_SEARCH);
 tolua_constant(tolua_S,"KEY_AC_STOP",KEY_AC_STOP);
 tolua_constant(tolua_S,"KEY_AGAIN",KEY_AGAIN);
 tolua_constant(tolua_S,"KEY_ALTERASE",KEY_ALTERASE);
 tolua_constant(tolua_S,"KEY_AMPERSAND",KEY_AMPERSAND);
 tolua_constant(tolua_S,"KEY_ASTERISK",KEY_ASTERISK);
 tolua_constant(tolua_S,"KEY_AT",KEY_AT);
 tolua_constant(tolua_S,"KEY_AUDIOMUTE",KEY_AUDIOMUTE);
 tolua_constant(tolua_S,"KEY_AUDIONEXT",KEY_AUDIONEXT);
 tolua_constant(tolua_S,"KEY_AUDIOPLAY",KEY_AUDIOPLAY);
 tolua_constant(tolua_S,"KEY_AUDIOPREV",KEY_AUDIOPREV);
 tolua_constant(tolua_S,"KEY_AUDIOSTOP",KEY_AUDIOSTOP);
 tolua_constant(tolua_S,"KEY_BACKQUOTE",KEY_BACKQUOTE);
 tolua_constant(tolua_S,"KEY_BACKSLASH",KEY_BACKSLASH);
 tolua_constant(tolua_S,"KEY_BRIGHTNESSDOWN",KEY_BRIGHTNESSDOWN);
 tolua_constant(tolua_S,"KEY_BRIGHTNESSUP",KEY_BRIGHTNESSUP);
 tolua_constant(tolua_S,"KEY_CALCULATOR",KEY_CALCULATOR);
 tolua_constant(tolua_S,"KEY_CANCEL",KEY_CANCEL);
 tolua_constant(tolua_S,"KEY_CARET",KEY_CARET);
 tolua_constant(tolua_S,"KEY_CLEAR",KEY_CLEAR);
 tolua_constant(tolua_S,"KEY_CLEARAGAIN",KEY_CLEARAGAIN);
 tolua_constant(tolua_S,"KEY_COLON",KEY_COLON);
 tolua_constant(tolua_S,"KEY_COMMA",KEY_COMMA);
 tolua_constant(tolua_S,"KEY_COMPUTER",KEY_COMPUTER);
 tolua_constant(tolua_S,"KEY_COPY",KEY_COPY);
 tolua_constant(tolua_S,"KEY_CRSEL",KEY_CRSEL);
 tolua_constant(tolua_S,"KEY_CURRENCYSUBUNIT",KEY_CURRENCYSUBUNIT);
 tolua_constant(tolua_S,"KEY_CURRENCYUNIT",KEY_CURRENCYUNIT);
 tolua_constant(tolua_S,"KEY_CUT",KEY_CUT);
 tolua_constant(tolua_S,"KEY_DECIMALSEPARATOR",KEY_DECIMALSEPARATOR);
 tolua_constant(tolua_S,"KEY_DISPLAYSWITCH",KEY_DISPLAYSWITCH);
 tolua_constant(tolua_S,"KEY_DOLLAR",KEY_DOLLAR);
 tolua_constant(tolua_S,"KEY_EJECT",KEY_EJECT);
 tolua_constant(tolua_S,"KEY_EQUALS",KEY_EQUALS);
 tolua_constant(tolua_S,"KEY_EXCLAIM",KEY_EXCLAIM);
 tolua_constant(tolua_S,"KEY_EXSEL",KEY_EXSEL);
 tolua_constant(tolua_S,"KEY_FIND",KEY_FIND);
 tolua_constant(tolua_S,"KEY_GREATER",KEY_GREATER);
 tolua_constant(tolua_S,"KEY_HASH",KEY_HASH);
 tolua_constant(tolua_S,"KEY_HELP",KEY_HELP);
 tolua_constant(tolua_S,"KEY_KBDILLUMDOWN",KEY_KBDILLUMDOWN);
 tolua_constant(tolua_S,"KEY_KBDILLUMTOGGLE",KEY_KBDILLUMTOGGLE);
 tolua_constant(tolua_S,"KEY_KBDILLUMUP",KEY_KBDILLUMUP);
 tolua_constant(tolua_S,"KEY_KP_00",KEY_KP_00);
 tolua_constant(tolua_S,"KEY_KP_000",KEY_KP_000);
 tolua_constant(tolua_S,"KEY_KP_A",KEY_KP_A);
 tolua_constant(tolua_S,"KEY_KP_AMPERSAND",KEY_KP_AMPERSAND);
 tolua_constant(tolua_S,"KEY_KP_AT",KEY_KP_AT);
 tolua_constant(tolua_S,"KEY_KP_B",KEY_KP_B);
 tolua_constant(tolua_S,"KEY_KP_BACKSPACE",KEY_KP_BACKSPACE);
 tolua_constant(tolua_S,"KEY_KP_BINARY",KEY_KP_BINARY);
 tolua_constant(tolua_S,"KEY_KP_C",KEY_KP_C);
 tolua_constant(tolua_S,"KEY_KP_CLEAR",KEY_KP_CLEAR);
 tolua_constant(tolua_S,"KEY_KP_CLEARENTRY",KEY_KP_CLEARENTRY);
 tolua_constant(tolua_S,"KEY_KP_COLON",KEY_KP_COLON);
 tolua_constant(tolua_S,"KEY_KP_COMMA",KEY_KP_COMMA);
 tolua_constant(tolua_S,"KEY_KP_D",KEY_KP_D);
 tolua_constant(tolua_S,"KEY_KP_DBLAMPERSAND",KEY_KP_DBLAMPERSAND);
 tolua_constant(tolua_S,"KEY_KP_DBLVERTICALBAR",KEY_KP_DBLVERTICALBAR);
 tolua_constant(tolua_S,"KEY_KP_DECIMAL",KEY_KP_DECIMAL);
 tolua_constant(tolua_S,"KEY_KP_E",KEY_KP_E);
 tolua_constant(tolua_S,"KEY_KP_EQUALS",KEY_KP_EQUALS);
 tolua_constant(tolua_S,"KEY_KP_EQUALSAS400",KEY_KP_EQUALSAS400);
 tolua_constant(tolua_S,"KEY_KP_EXCLAM",KEY_KP_EXCLAM);
 tolua_constant(tolua_S,"KEY_KP_F",KEY_KP_F);
 tolua_constant(tolua_S,"KEY_KP_GREATER",KEY_KP_GREATER);
 tolua_constant(tolua_S,"KEY_KP_HASH",KEY_KP_HASH);
 tolua_constant(tolua_S,"KEY_KP_HEXADECIMAL",KEY_KP_HEXADECIMAL);
 tolua_constant(tolua_S,"KEY_KP_LEFTBRACE",KEY_KP_LEFTBRACE);
 tolua_constant(tolua_S,"KEY_KP_LEFTPAREN",KEY_KP_LEFTPAREN);
 tolua_constant(tolua_S,"KEY_KP_LESS",KEY_KP_LESS);
 tolua_constant(tolua_S,"KEY_KP_MEMADD",KEY_KP_MEMADD);
 tolua_constant(tolua_S,"KEY_KP_MEMCLEAR",KEY_KP_MEMCLEAR);
 tolua_constant(tolua_S,"KEY_KP_MEMDIVIDE",KEY_KP_MEMDIVIDE);
 tolua_constant(tolua_S,"KEY_KP_MEMMULTIPLY",KEY_KP_MEMMULTIPLY);
 tolua_constant(tolua_S,"KEY_KP_MEMRECALL",KEY_KP_MEMRECALL);
 tolua_constant(tolua_S,"KEY_KP_MEMSTORE",KEY_KP_MEMSTORE);
 tolua_constant(tolua_S,"KEY_KP_MEMSUBTRACT",KEY_KP_MEMSUBTRACT);
 tolua_constant(tolua_S,"KEY_KP_OCTAL",KEY_KP_OCTAL);
 tolua_constant(tolua_S,"KEY_KP_PERCENT",KEY_KP_PERCENT);
 tolua_constant(tolua_S,"KEY_KP_PLUSMINUS",KEY_KP_PLUSMINUS);
 tolua_constant(tolua_S,"KEY_KP_POWER",KEY_KP_POWER);
 tolua_constant(tolua_S,"KEY_KP_RIGHTBRACE",KEY_KP_RIGHTBRACE);
 tolua_constant(tolua_S,"KEY_KP_RIGHTPAREN",KEY_KP_RIGHTPAREN);
 tolua_constant(tolua_S,"KEY_KP_SPACE",KEY_KP_SPACE);
 tolua_constant(tolua_S,"KEY_KP_TAB",KEY_KP_TAB);
 tolua_constant(tolua_S,"KEY_KP_VERTICALBAR",KEY_KP_VERTICALBAR);
 tolua_constant(tolua_S,"KEY_KP_XOR",KEY_KP_XOR);
 tolua_constant(tolua_S,"KEY_LEFTBRACKET",KEY_LEFTBRACKET);
 tolua_constant(tolua_S,"KEY_LEFTPAREN",KEY_LEFTPAREN);
 tolua_constant(tolua_S,"KEY_LESS",KEY_LESS);
 tolua_constant(tolua_S,"KEY_MAIL",KEY_MAIL);
 tolua_constant(tolua_S,"KEY_MEDIASELECT",KEY_MEDIASELECT);
 tolua_constant(tolua_S,"KEY_MENU",KEY_MENU);
 tolua_constant(tolua_S,"KEY_MINUS",KEY_MINUS);
 tolua_constant(tolua_S,"KEY_MODE",KEY_MODE);
 tolua_constant(tolua_S,"KEY_MUTE",KEY_MUTE);
 tolua_constant(tolua_S,"KEY_OPER",KEY_OPER);
 tolua_constant(tolua_S,"KEY_OUT",KEY_OUT);
 tolua_constant(tolua_S,"KEY_PASTE",KEY_PASTE);
 tolua_constant(tolua_S,"KEY_PERCENT",KEY_PERCENT);
 tolua_constant(tolua_S,"KEY_PERIOD",KEY_PERIOD);
 tolua_constant(tolua_S,"KEY_PLUS",KEY_PLUS);
 tolua_constant(tolua_S,"KEY_POWER",KEY_POWER);
 tolua_constant(tolua_S,"KEY_PRIOR",KEY_PRIOR);
 tolua_constant(tolua_S,"KEY_QUESTION",KEY_QUESTION);
 tolua_constant(tolua_S,"KEY_QUOTE",KEY_QUOTE);
 tolua_constant(tolua_S,"KEY_QUOTEDBL",KEY_QUOTEDBL);
 tolua_constant(tolua_S,"KEY_RIGHTBRACKET",KEY_RIGHTBRACKET);
 tolua_constant(tolua_S,"KEY_RIGHTPAREN",KEY_RIGHTPAREN);
 tolua_constant(tolua_S,"KEY_SEMICOLON",KEY_SEMICOLON);
 tolua_constant(tolua_S,"KEY_SEPARATOR",KEY_SEPARATOR);
 tolua_constant(tolua_S,"KEY_SLASH",KEY_SLASH);
 tolua_constant(tolua_S,"KEY_SLEEP",KEY_SLEEP);
 tolua_constant(tolua_S,"KEY_STOP",KEY_STOP);
 tolua_constant(tolua_S,"KEY_SYSREQ",KEY_SYSREQ);
 tolua_constant(tolua_S,"KEY_THOUSANDSSEPARATOR",KEY_THOUSANDSSEPARATOR);
 tolua_constant(tolua_S,"KEY_UNDERSCORE",KEY_UNDERSCORE);
 tolua_constant(tolua_S,"KEY_UNDO",KEY_UNDO);
 tolua_constant(tolua_S,"KEY_VOLUMEDOWN",KEY_VOLUMEDOWN);
 tolua_constant(tolua_S,"KEY_VOLUMEUP",KEY_VOLUMEUP);
 tolua_constant(tolua_S,"KEY_WWW",KEY_WWW);
 tolua_constant(tolua_S,"SCANCODE_UNKNOWN",SCANCODE_UNKNOWN);
 tolua_constant(tolua_S,"SCANCODE_CTRL",SCANCODE_CTRL);
 tolua_constant(tolua_S,"SCANCODE_SHIFT",SCANCODE_SHIFT);
 tolua_constant(tolua_S,"SCANCODE_ALT",SCANCODE_ALT);
 tolua_constant(tolua_S,"SCANCODE_GUI",SCANCODE_GUI);
 tolua_constant(tolua_S,"SCANCODE_A",SCANCODE_A);
 tolua_constant(tolua_S,"SCANCODE_B",SCANCODE_B);
 tolua_constant(tolua_S,"SCANCODE_C",SCANCODE_C);
 tolua_constant(tolua_S,"SCANCODE_D",SCANCODE_D);
 tolua_constant(tolua_S,"SCANCODE_E",SCANCODE_E);
 tolua_constant(tolua_S,"SCANCODE_F",SCANCODE_F);
 tolua_constant(tolua_S,"SCANCODE_G",SCANCODE_G);
 tolua_constant(tolua_S,"SCANCODE_H",SCANCODE_H);
 tolua_constant(tolua_S,"SCANCODE_I",SCANCODE_I);
 tolua_constant(tolua_S,"SCANCODE_J",SCANCODE_J);
 tolua_constant(tolua_S,"SCANCODE_K",SCANCODE_K);
 tolua_constant(tolua_S,"SCANCODE_L",SCANCODE_L);
 tolua_constant(tolua_S,"SCANCODE_M",SCANCODE_M);
 tolua_constant(tolua_S,"SCANCODE_N",SCANCODE_N);
 tolua_constant(tolua_S,"SCANCODE_O",SCANCODE_O);
 tolua_constant(tolua_S,"SCANCODE_P",SCANCODE_P);
 tolua_constant(tolua_S,"SCANCODE_Q",SCANCODE_Q);
 tolua_constant(tolua_S,"SCANCODE_R",SCANCODE_R);
 tolua_constant(tolua_S,"SCANCODE_S",SCANCODE_S);
 tolua_constant(tolua_S,"SCANCODE_T",SCANCODE_T);
 tolua_constant(tolua_S,"SCANCODE_U",SCANCODE_U);
 tolua_constant(tolua_S,"SCANCODE_V",SCANCODE_V);
 tolua_constant(tolua_S,"SCANCODE_W",SCANCODE_W);
 tolua_constant(tolua_S,"SCANCODE_X",SCANCODE_X);
 tolua_constant(tolua_S,"SCANCODE_Y",SCANCODE_Y);
 tolua_constant(tolua_S,"SCANCODE_Z",SCANCODE_Z);
 tolua_constant(tolua_S,"SCANCODE_1",SCANCODE_1);
 tolua_constant(tolua_S,"SCANCODE_2",SCANCODE_2);
 tolua_constant(tolua_S,"SCANCODE_3",SCANCODE_3);
 tolua_constant(tolua_S,"SCANCODE_4",SCANCODE_4);
 tolua_constant(tolua_S,"SCANCODE_5",SCANCODE_5);
 tolua_constant(tolua_S,"SCANCODE_6",SCANCODE_6);
 tolua_constant(tolua_S,"SCANCODE_7",SCANCODE_7);
 tolua_constant(tolua_S,"SCANCODE_8",SCANCODE_8);
 tolua_constant(tolua_S,"SCANCODE_9",SCANCODE_9);
 tolua_constant(tolua_S,"SCANCODE_0",SCANCODE_0);
 tolua_constant(tolua_S,"SCANCODE_RETURN",SCANCODE_RETURN);
 tolua_constant(tolua_S,"SCANCODE_ESCAPE",SCANCODE_ESCAPE);
 tolua_constant(tolua_S,"SCANCODE_BACKSPACE",SCANCODE_BACKSPACE);
 tolua_constant(tolua_S,"SCANCODE_TAB",SCANCODE_TAB);
 tolua_constant(tolua_S,"SCANCODE_SPACE",SCANCODE_SPACE);
 tolua_constant(tolua_S,"SCANCODE_MINUS",SCANCODE_MINUS);
 tolua_constant(tolua_S,"SCANCODE_EQUALS",SCANCODE_EQUALS);
 tolua_constant(tolua_S,"SCANCODE_LEFTBRACKET",SCANCODE_LEFTBRACKET);
 tolua_constant(tolua_S,"SCANCODE_RIGHTBRACKET",SCANCODE_RIGHTBRACKET);
 tolua_constant(tolua_S,"SCANCODE_BACKSLASH",SCANCODE_BACKSLASH);
 tolua_constant(tolua_S,"SCANCODE_NONUSHASH",SCANCODE_NONUSHASH);
 tolua_constant(tolua_S,"SCANCODE_SEMICOLON",SCANCODE_SEMICOLON);
 tolua_constant(tolua_S,"SCANCODE_APOSTROPHE",SCANCODE_APOSTROPHE);
 tolua_constant(tolua_S,"SCANCODE_GRAVE",SCANCODE_GRAVE);
 tolua_constant(tolua_S,"SCANCODE_COMMA",SCANCODE_COMMA);
 tolua_constant(tolua_S,"SCANCODE_PERIOD",SCANCODE_PERIOD);
 tolua_constant(tolua_S,"SCANCODE_SLASH",SCANCODE_SLASH);
 tolua_constant(tolua_S,"SCANCODE_CAPSLOCK",SCANCODE_CAPSLOCK);
 tolua_constant(tolua_S,"SCANCODE_F1",SCANCODE_F1);
 tolua_constant(tolua_S,"SCANCODE_F2",SCANCODE_F2);
 tolua_constant(tolua_S,"SCANCODE_F3",SCANCODE_F3);
 tolua_constant(tolua_S,"SCANCODE_F4",SCANCODE_F4);
 tolua_constant(tolua_S,"SCANCODE_F5",SCANCODE_F5);
 tolua_constant(tolua_S,"SCANCODE_F6",SCANCODE_F6);
 tolua_constant(tolua_S,"SCANCODE_F7",SCANCODE_F7);
 tolua_constant(tolua_S,"SCANCODE_F8",SCANCODE_F8);
 tolua_constant(tolua_S,"SCANCODE_F9",SCANCODE_F9);
 tolua_constant(tolua_S,"SCANCODE_F10",SCANCODE_F10);
 tolua_constant(tolua_S,"SCANCODE_F11",SCANCODE_F11);
 tolua_constant(tolua_S,"SCANCODE_F12",SCANCODE_F12);
 tolua_constant(tolua_S,"SCANCODE_PRINTSCREEN",SCANCODE_PRINTSCREEN);
 tolua_constant(tolua_S,"SCANCODE_SCROLLLOCK",SCANCODE_SCROLLLOCK);
 tolua_constant(tolua_S,"SCANCODE_PAUSE",SCANCODE_PAUSE);
 tolua_constant(tolua_S,"SCANCODE_INSERT",SCANCODE_INSERT);
 tolua_constant(tolua_S,"SCANCODE_HOME",SCANCODE_HOME);
 tolua_constant(tolua_S,"SCANCODE_PAGEUP",SCANCODE_PAGEUP);
 tolua_constant(tolua_S,"SCANCODE_DELETE",SCANCODE_DELETE);
 tolua_constant(tolua_S,"SCANCODE_END",SCANCODE_END);
 tolua_constant(tolua_S,"SCANCODE_PAGEDOWN",SCANCODE_PAGEDOWN);
 tolua_constant(tolua_S,"SCANCODE_RIGHT",SCANCODE_RIGHT);
 tolua_constant(tolua_S,"SCANCODE_LEFT",SCANCODE_LEFT);
 tolua_constant(tolua_S,"SCANCODE_DOWN",SCANCODE_DOWN);
 tolua_constant(tolua_S,"SCANCODE_UP",SCANCODE_UP);
 tolua_constant(tolua_S,"SCANCODE_NUMLOCKCLEAR",SCANCODE_NUMLOCKCLEAR);
 tolua_constant(tolua_S,"SCANCODE_KP_DIVIDE",SCANCODE_KP_DIVIDE);
 tolua_constant(tolua_S,"SCANCODE_KP_MULTIPLY",SCANCODE_KP_MULTIPLY);
 tolua_constant(tolua_S,"SCANCODE_KP_MINUS",SCANCODE_KP_MINUS);
 tolua_constant(tolua_S,"SCANCODE_KP_PLUS",SCANCODE_KP_PLUS);
 tolua_constant(tolua_S,"SCANCODE_KP_ENTER",SCANCODE_KP_ENTER);
 tolua_constant(tolua_S,"SCANCODE_KP_1",SCANCODE_KP_1);
 tolua_constant(tolua_S,"SCANCODE_KP_2",SCANCODE_KP_2);
 tolua_constant(tolua_S,"SCANCODE_KP_3",SCANCODE_KP_3);
 tolua_constant(tolua_S,"SCANCODE_KP_4",SCANCODE_KP_4);
 tolua_constant(tolua_S,"SCANCODE_KP_5",SCANCODE_KP_5);
 tolua_constant(tolua_S,"SCANCODE_KP_6",SCANCODE_KP_6);
 tolua_constant(tolua_S,"SCANCODE_KP_7",SCANCODE_KP_7);
 tolua_constant(tolua_S,"SCANCODE_KP_8",SCANCODE_KP_8);
 tolua_constant(tolua_S,"SCANCODE_KP_9",SCANCODE_KP_9);
 tolua_constant(tolua_S,"SCANCODE_KP_0",SCANCODE_KP_0);
 tolua_constant(tolua_S,"SCANCODE_KP_PERIOD",SCANCODE_KP_PERIOD);
 tolua_constant(tolua_S,"SCANCODE_NONUSBACKSLASH",SCANCODE_NONUSBACKSLASH);
 tolua_constant(tolua_S,"SCANCODE_APPLICATION",SCANCODE_APPLICATION);
 tolua_constant(tolua_S,"SCANCODE_POWER",SCANCODE_POWER);
 tolua_constant(tolua_S,"SCANCODE_KP_EQUALS",SCANCODE_KP_EQUALS);
 tolua_constant(tolua_S,"SCANCODE_F13",SCANCODE_F13);
 tolua_constant(tolua_S,"SCANCODE_F14",SCANCODE_F14);
 tolua_constant(tolua_S,"SCANCODE_F15",SCANCODE_F15);
 tolua_constant(tolua_S,"SCANCODE_F16",SCANCODE_F16);
 tolua_constant(tolua_S,"SCANCODE_F17",SCANCODE_F17);
 tolua_constant(tolua_S,"SCANCODE_F18",SCANCODE_F18);
 tolua_constant(tolua_S,"SCANCODE_F19",SCANCODE_F19);
 tolua_constant(tolua_S,"SCANCODE_F20",SCANCODE_F20);
 tolua_constant(tolua_S,"SCANCODE_F21",SCANCODE_F21);
 tolua_constant(tolua_S,"SCANCODE_F22",SCANCODE_F22);
 tolua_constant(tolua_S,"SCANCODE_F23",SCANCODE_F23);
 tolua_constant(tolua_S,"SCANCODE_F24",SCANCODE_F24);
 tolua_constant(tolua_S,"SCANCODE_EXECUTE",SCANCODE_EXECUTE);
 tolua_constant(tolua_S,"SCANCODE_HELP",SCANCODE_HELP);
 tolua_constant(tolua_S,"SCANCODE_MENU",SCANCODE_MENU);
 tolua_constant(tolua_S,"SCANCODE_SELECT",SCANCODE_SELECT);
 tolua_constant(tolua_S,"SCANCODE_STOP",SCANCODE_STOP);
 tolua_constant(tolua_S,"SCANCODE_AGAIN",SCANCODE_AGAIN);
 tolua_constant(tolua_S,"SCANCODE_UNDO",SCANCODE_UNDO);
 tolua_constant(tolua_S,"SCANCODE_CUT",SCANCODE_CUT);
 tolua_constant(tolua_S,"SCANCODE_COPY",SCANCODE_COPY);
 tolua_constant(tolua_S,"SCANCODE_PASTE",SCANCODE_PASTE);
 tolua_constant(tolua_S,"SCANCODE_FIND",SCANCODE_FIND);
 tolua_constant(tolua_S,"SCANCODE_MUTE",SCANCODE_MUTE);
 tolua_constant(tolua_S,"SCANCODE_VOLUMEUP",SCANCODE_VOLUMEUP);
 tolua_constant(tolua_S,"SCANCODE_VOLUMEDOWN",SCANCODE_VOLUMEDOWN);
 tolua_constant(tolua_S,"SCANCODE_KP_COMMA",SCANCODE_KP_COMMA);
 tolua_constant(tolua_S,"SCANCODE_KP_EQUALSAS400",SCANCODE_KP_EQUALSAS400);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL1",SCANCODE_INTERNATIONAL1);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL2",SCANCODE_INTERNATIONAL2);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL3",SCANCODE_INTERNATIONAL3);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL4",SCANCODE_INTERNATIONAL4);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL5",SCANCODE_INTERNATIONAL5);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL6",SCANCODE_INTERNATIONAL6);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL7",SCANCODE_INTERNATIONAL7);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL8",SCANCODE_INTERNATIONAL8);
 tolua_constant(tolua_S,"SCANCODE_INTERNATIONAL9",SCANCODE_INTERNATIONAL9);
 tolua_constant(tolua_S,"SCANCODE_LANG1",SCANCODE_LANG1);
 tolua_constant(tolua_S,"SCANCODE_LANG2",SCANCODE_LANG2);
 tolua_constant(tolua_S,"SCANCODE_LANG3",SCANCODE_LANG3);
 tolua_constant(tolua_S,"SCANCODE_LANG4",SCANCODE_LANG4);
 tolua_constant(tolua_S,"SCANCODE_LANG5",SCANCODE_LANG5);
 tolua_constant(tolua_S,"SCANCODE_LANG6",SCANCODE_LANG6);
 tolua_constant(tolua_S,"SCANCODE_LANG7",SCANCODE_LANG7);
 tolua_constant(tolua_S,"SCANCODE_LANG8",SCANCODE_LANG8);
 tolua_constant(tolua_S,"SCANCODE_LANG9",SCANCODE_LANG9);
 tolua_constant(tolua_S,"SCANCODE_ALTERASE",SCANCODE_ALTERASE);
 tolua_constant(tolua_S,"SCANCODE_SYSREQ",SCANCODE_SYSREQ);
 tolua_constant(tolua_S,"SCANCODE_CANCEL",SCANCODE_CANCEL);
 tolua_constant(tolua_S,"SCANCODE_CLEAR",SCANCODE_CLEAR);
 tolua_constant(tolua_S,"SCANCODE_PRIOR",SCANCODE_PRIOR);
 tolua_constant(tolua_S,"SCANCODE_RETURN2",SCANCODE_RETURN2);
 tolua_constant(tolua_S,"SCANCODE_SEPARATOR",SCANCODE_SEPARATOR);
 tolua_constant(tolua_S,"SCANCODE_OUT",SCANCODE_OUT);
 tolua_constant(tolua_S,"SCANCODE_OPER",SCANCODE_OPER);
 tolua_constant(tolua_S,"SCANCODE_CLEARAGAIN",SCANCODE_CLEARAGAIN);
 tolua_constant(tolua_S,"SCANCODE_CRSEL",SCANCODE_CRSEL);
 tolua_constant(tolua_S,"SCANCODE_EXSEL",SCANCODE_EXSEL);
 tolua_constant(tolua_S,"SCANCODE_KP_00",SCANCODE_KP_00);
 tolua_constant(tolua_S,"SCANCODE_KP_000",SCANCODE_KP_000);
 tolua_constant(tolua_S,"SCANCODE_THOUSANDSSEPARATOR",SCANCODE_THOUSANDSSEPARATOR);
 tolua_constant(tolua_S,"SCANCODE_DECIMALSEPARATOR",SCANCODE_DECIMALSEPARATOR);
 tolua_constant(tolua_S,"SCANCODE_CURRENCYUNIT",SCANCODE_CURRENCYUNIT);
 tolua_constant(tolua_S,"SCANCODE_CURRENCYSUBUNIT",SCANCODE_CURRENCYSUBUNIT);
 tolua_constant(tolua_S,"SCANCODE_KP_LEFTPAREN",SCANCODE_KP_LEFTPAREN);
 tolua_constant(tolua_S,"SCANCODE_KP_RIGHTPAREN",SCANCODE_KP_RIGHTPAREN);
 tolua_constant(tolua_S,"SCANCODE_KP_LEFTBRACE",SCANCODE_KP_LEFTBRACE);
 tolua_constant(tolua_S,"SCANCODE_KP_RIGHTBRACE",SCANCODE_KP_RIGHTBRACE);
 tolua_constant(tolua_S,"SCANCODE_KP_TAB",SCANCODE_KP_TAB);
 tolua_constant(tolua_S,"SCANCODE_KP_BACKSPACE",SCANCODE_KP_BACKSPACE);
 tolua_constant(tolua_S,"SCANCODE_KP_A",SCANCODE_KP_A);
 tolua_constant(tolua_S,"SCANCODE_KP_B",SCANCODE_KP_B);
 tolua_constant(tolua_S,"SCANCODE_KP_C",SCANCODE_KP_C);
 tolua_constant(tolua_S,"SCANCODE_KP_D",SCANCODE_KP_D);
 tolua_constant(tolua_S,"SCANCODE_KP_E",SCANCODE_KP_E);
 tolua_constant(tolua_S,"SCANCODE_KP_F",SCANCODE_KP_F);
 tolua_constant(tolua_S,"SCANCODE_KP_XOR",SCANCODE_KP_XOR);
 tolua_constant(tolua_S,"SCANCODE_KP_POWER",SCANCODE_KP_POWER);
 tolua_constant(tolua_S,"SCANCODE_KP_PERCENT",SCANCODE_KP_PERCENT);
 tolua_constant(tolua_S,"SCANCODE_KP_LESS",SCANCODE_KP_LESS);
 tolua_constant(tolua_S,"SCANCODE_KP_GREATER",SCANCODE_KP_GREATER);
 tolua_constant(tolua_S,"SCANCODE_KP_AMPERSAND",SCANCODE_KP_AMPERSAND);
 tolua_constant(tolua_S,"SCANCODE_KP_DBLAMPERSAND",SCANCODE_KP_DBLAMPERSAND);
 tolua_constant(tolua_S,"SCANCODE_KP_VERTICALBAR",SCANCODE_KP_VERTICALBAR);
 tolua_constant(tolua_S,"SCANCODE_KP_DBLVERTICALBAR",SCANCODE_KP_DBLVERTICALBAR);
 tolua_constant(tolua_S,"SCANCODE_KP_COLON",SCANCODE_KP_COLON);
 tolua_constant(tolua_S,"SCANCODE_KP_HASH",SCANCODE_KP_HASH);
 tolua_constant(tolua_S,"SCANCODE_KP_SPACE",SCANCODE_KP_SPACE);
 tolua_constant(tolua_S,"SCANCODE_KP_AT",SCANCODE_KP_AT);
 tolua_constant(tolua_S,"SCANCODE_KP_EXCLAM",SCANCODE_KP_EXCLAM);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMSTORE",SCANCODE_KP_MEMSTORE);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMRECALL",SCANCODE_KP_MEMRECALL);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMCLEAR",SCANCODE_KP_MEMCLEAR);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMADD",SCANCODE_KP_MEMADD);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMSUBTRACT",SCANCODE_KP_MEMSUBTRACT);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMMULTIPLY",SCANCODE_KP_MEMMULTIPLY);
 tolua_constant(tolua_S,"SCANCODE_KP_MEMDIVIDE",SCANCODE_KP_MEMDIVIDE);
 tolua_constant(tolua_S,"SCANCODE_KP_PLUSMINUS",SCANCODE_KP_PLUSMINUS);
 tolua_constant(tolua_S,"SCANCODE_KP_CLEAR",SCANCODE_KP_CLEAR);
 tolua_constant(tolua_S,"SCANCODE_KP_CLEARENTRY",SCANCODE_KP_CLEARENTRY);
 tolua_constant(tolua_S,"SCANCODE_KP_BINARY",SCANCODE_KP_BINARY);
 tolua_constant(tolua_S,"SCANCODE_KP_OCTAL",SCANCODE_KP_OCTAL);
 tolua_constant(tolua_S,"SCANCODE_KP_DECIMAL",SCANCODE_KP_DECIMAL);
 tolua_constant(tolua_S,"SCANCODE_KP_HEXADECIMAL",SCANCODE_KP_HEXADECIMAL);
 tolua_constant(tolua_S,"SCANCODE_LCTRL",SCANCODE_LCTRL);
 tolua_constant(tolua_S,"SCANCODE_LSHIFT",SCANCODE_LSHIFT);
 tolua_constant(tolua_S,"SCANCODE_LALT",SCANCODE_LALT);
 tolua_constant(tolua_S,"SCANCODE_LGUI",SCANCODE_LGUI);
 tolua_constant(tolua_S,"SCANCODE_RCTRL",SCANCODE_RCTRL);
 tolua_constant(tolua_S,"SCANCODE_RSHIFT",SCANCODE_RSHIFT);
 tolua_constant(tolua_S,"SCANCODE_RALT",SCANCODE_RALT);
 tolua_constant(tolua_S,"SCANCODE_RGUI",SCANCODE_RGUI);
 tolua_constant(tolua_S,"SCANCODE_MODE",SCANCODE_MODE);
 tolua_constant(tolua_S,"SCANCODE_AUDIONEXT",SCANCODE_AUDIONEXT);
 tolua_constant(tolua_S,"SCANCODE_AUDIOPREV",SCANCODE_AUDIOPREV);
 tolua_constant(tolua_S,"SCANCODE_AUDIOSTOP",SCANCODE_AUDIOSTOP);
 tolua_constant(tolua_S,"SCANCODE_AUDIOPLAY",SCANCODE_AUDIOPLAY);
 tolua_constant(tolua_S,"SCANCODE_AUDIOMUTE",SCANCODE_AUDIOMUTE);
 tolua_constant(tolua_S,"SCANCODE_MEDIASELECT",SCANCODE_MEDIASELECT);
 tolua_constant(tolua_S,"SCANCODE_WWW",SCANCODE_WWW);
 tolua_constant(tolua_S,"SCANCODE_MAIL",SCANCODE_MAIL);
 tolua_constant(tolua_S,"SCANCODE_CALCULATOR",SCANCODE_CALCULATOR);
 tolua_constant(tolua_S,"SCANCODE_COMPUTER",SCANCODE_COMPUTER);
 tolua_constant(tolua_S,"SCANCODE_AC_SEARCH",SCANCODE_AC_SEARCH);
 tolua_constant(tolua_S,"SCANCODE_AC_HOME",SCANCODE_AC_HOME);
 tolua_constant(tolua_S,"SCANCODE_AC_BACK",SCANCODE_AC_BACK);
 tolua_constant(tolua_S,"SCANCODE_AC_FORWARD",SCANCODE_AC_FORWARD);
 tolua_constant(tolua_S,"SCANCODE_AC_STOP",SCANCODE_AC_STOP);
 tolua_constant(tolua_S,"SCANCODE_AC_REFRESH",SCANCODE_AC_REFRESH);
 tolua_constant(tolua_S,"SCANCODE_AC_BOOKMARKS",SCANCODE_AC_BOOKMARKS);
 tolua_constant(tolua_S,"SCANCODE_BRIGHTNESSDOWN",SCANCODE_BRIGHTNESSDOWN);
 tolua_constant(tolua_S,"SCANCODE_BRIGHTNESSUP",SCANCODE_BRIGHTNESSUP);
 tolua_constant(tolua_S,"SCANCODE_DISPLAYSWITCH",SCANCODE_DISPLAYSWITCH);
 tolua_constant(tolua_S,"SCANCODE_KBDILLUMTOGGLE",SCANCODE_KBDILLUMTOGGLE);
 tolua_constant(tolua_S,"SCANCODE_KBDILLUMDOWN",SCANCODE_KBDILLUMDOWN);
 tolua_constant(tolua_S,"SCANCODE_KBDILLUMUP",SCANCODE_KBDILLUMUP);
 tolua_constant(tolua_S,"SCANCODE_EJECT",SCANCODE_EJECT);
 tolua_constant(tolua_S,"SCANCODE_SLEEP",SCANCODE_SLEEP);
 tolua_constant(tolua_S,"SCANCODE_APP1",SCANCODE_APP1);
 tolua_constant(tolua_S,"SCANCODE_APP2",SCANCODE_APP2);
 tolua_constant(tolua_S,"HAT_CENTER",HAT_CENTER);
 tolua_constant(tolua_S,"HAT_UP",HAT_UP);
 tolua_constant(tolua_S,"HAT_RIGHT",HAT_RIGHT);
 tolua_constant(tolua_S,"HAT_DOWN",HAT_DOWN);
 tolua_constant(tolua_S,"HAT_LEFT",HAT_LEFT);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_A",CONTROLLER_BUTTON_A);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_B",CONTROLLER_BUTTON_B);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_X",CONTROLLER_BUTTON_X);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_Y",CONTROLLER_BUTTON_Y);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_BACK",CONTROLLER_BUTTON_BACK);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_GUIDE",CONTROLLER_BUTTON_GUIDE);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_START",CONTROLLER_BUTTON_START);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_LEFTSTICK",CONTROLLER_BUTTON_LEFTSTICK);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_RIGHTSTICK",CONTROLLER_BUTTON_RIGHTSTICK);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_LEFTSHOULDER",CONTROLLER_BUTTON_LEFTSHOULDER);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_RIGHTSHOULDER",CONTROLLER_BUTTON_RIGHTSHOULDER);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_DPAD_UP",CONTROLLER_BUTTON_DPAD_UP);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_DPAD_DOWN",CONTROLLER_BUTTON_DPAD_DOWN);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_DPAD_LEFT",CONTROLLER_BUTTON_DPAD_LEFT);
 tolua_constant(tolua_S,"CONTROLLER_BUTTON_DPAD_RIGHT",CONTROLLER_BUTTON_DPAD_RIGHT);
 tolua_constant(tolua_S,"CONTROLLER_AXIS_LEFTX",CONTROLLER_AXIS_LEFTX);
 tolua_constant(tolua_S,"CONTROLLER_AXIS_LEFTY",CONTROLLER_AXIS_LEFTY);
 tolua_constant(tolua_S,"CONTROLLER_AXIS_RIGHTX",CONTROLLER_AXIS_RIGHTX);
 tolua_constant(tolua_S,"CONTROLLER_AXIS_RIGHTY",CONTROLLER_AXIS_RIGHTY);
 tolua_constant(tolua_S,"CONTROLLER_AXIS_TRIGGERLEFT",CONTROLLER_AXIS_TRIGGERLEFT);
 tolua_constant(tolua_S,"CONTROLLER_AXIS_TRIGGERRIGHT",CONTROLLER_AXIS_TRIGGERRIGHT);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_InputLuaAPI (lua_State* tolua_S) {
 return tolua_InputLuaAPI_open(tolua_S);
}
#endif

#if __clang__
#pragma clang diagnostic pop
#endif