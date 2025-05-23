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

#pragma once

#include <Urho3D/Urho3D.h>

#if URHO3D_ANGELSCRIPT
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/AngelScript/Addons.h>
#include <Urho3D/AngelScript/GeneratedIncludes.h>
#include <Urho3D/AngelScript/Generated_Includes.h>
#include <Urho3D/AngelScript/Generated_Members.h>
#include <Urho3D/AngelScript/Generated_Templates.h>
#include <Urho3D/AngelScript/Manual.h>
#include <Urho3D/AngelScript/Manual_Container.h>
#include <Urho3D/AngelScript/Manual_Core.h>
#include <Urho3D/AngelScript/Manual_Graphics.h>
#include <Urho3D/AngelScript/Manual_IO.h>
#include <Urho3D/AngelScript/Manual_Input.h>
#include <Urho3D/AngelScript/Manual_Math.h>
#include <Urho3D/AngelScript/Manual_Navigation.h>
#include <Urho3D/AngelScript/Manual_Network.h>
#include <Urho3D/AngelScript/Manual_Physics.h>
#include <Urho3D/AngelScript/Manual_Resource.h>
#include <Urho3D/AngelScript/Manual_Scene.h>
#include <Urho3D/AngelScript/Manual_UI.h>
#include <Urho3D/AngelScript/Manual_Urho2D.h>
#include <Urho3D/AngelScript/RegistrationMacros.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptAPI.h>
#include <Urho3D/AngelScript/ScriptEventListener.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/ScriptInstance.h>
#endif
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/AudioDefs.h>
#include <Urho3D/Audio/AudioEvents.h>
#include <Urho3D/Audio/BufferedSoundStream.h>
#include <Urho3D/Audio/OggVorbisSoundStream.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/SoundStream.h>
#include <Urho3D/Base/Algorithm.h>
#include <Urho3D/Base/Iter.h>
#include <Urho3D/Container/Allocator.h>
#include <Urho3D/Container/ArrayPtr.h>
#include <Urho3D/Container/FlagSet.h>
#include <Urho3D/Container/ForEach.h>
#include <Urho3D/Container/Hash.h>
#include <Urho3D/Container/HashBase.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Container/HashSet.h>
#include <Urho3D/Container/LinkedList.h>
#include <Urho3D/Container/List.h>
#include <Urho3D/Container/ListBase.h>
#include <Urho3D/Container/Pair.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/RefCounted.h>
#include <Urho3D/Container/Sort.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Swap.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Container/VectorBase.h>
#include <Urho3D/Core/Attribute.h>
#include <Urho3D/Core/Condition.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/EventProfiler.h>
#include <Urho3D/Core/Main.h>
#include <Urho3D/Core/MiniDump.h>
#include <Urho3D/Core/Mutex.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Core/Profiler.h>
#include <Urho3D/Core/Spline.h>
#include <Urho3D/Core/StringHashRegister.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Core/Thread.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Core/WorkQueue.h>
#if URHO3D_DATABASE
#include <Urho3D/Database/Database.h>
#include <Urho3D/Database/DatabaseEvents.h>
#include <Urho3D/Database/DbConnection.h>
#include <Urho3D/Database/DbResult.h>
#endif
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Engine/EngineEvents.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Batch.h>
#include <Urho3D/Graphics/BillboardSet.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/ConstantBuffer.h>
#include <Urho3D/Graphics/CustomGeometry.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/DecalSet.h>
#include <Urho3D/Graphics/Drawable.h>
#include <Urho3D/Graphics/DrawableEvents.h>
#include <Urho3D/Graphics/GPUObject.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/GraphicsEvents.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/OcclusionBuffer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/OctreeQuery.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/RenderSurface.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RibbonTrail.h>
#include <Urho3D/Graphics/Shader.h>
#include <Urho3D/Graphics/ShaderPrecache.h>
#include <Urho3D/Graphics/ShaderProgram.h>
#include <Urho3D/Graphics/ShaderVariation.h>
#include <Urho3D/Graphics/Skeleton.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/StaticModelGroup.h>
#include <Urho3D/Graphics/Tangent.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/TerrainPatch.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Texture2DArray.h>
#include <Urho3D/Graphics/Texture3D.h>
#include <Urho3D/Graphics/TextureCube.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/VertexDeclaration.h>
#include <Urho3D/Graphics/View.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#if URHO3D_IK
#include <Urho3D/IK/IK.h>
#include <Urho3D/IK/IKConstraint.h>
#include <Urho3D/IK/IKEffector.h>
#include <Urho3D/IK/IKEvents.h>
#include <Urho3D/IK/IKSolver.h>
#endif
#include <Urho3D/IO/AbstractFile.h>
#include <Urho3D/IO/Compression.h>
#include <Urho3D/IO/Deserializer.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/FileWatcher.h>
#include <Urho3D/IO/IOEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/MacFileWatcher.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/IO/NamedPipe.h>
#include <Urho3D/IO/PackageFile.h>
#include <Urho3D/IO/RWOpsWrapper.h>
#include <Urho3D/IO/Serializer.h>
#include <Urho3D/IO/VectorBuffer.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputConstants.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/LibraryInfo.h>
#if URHO3D_LUA
#include <Urho3D/LuaScript/LuaFile.h>
#include <Urho3D/LuaScript/LuaFunction.h>
#include <Urho3D/LuaScript/LuaScript.h>
#include <Urho3D/LuaScript/LuaScriptEventInvoker.h>
#include <Urho3D/LuaScript/LuaScriptEventListener.h>
#include <Urho3D/LuaScript/LuaScriptInstance.h>
#endif
#include <Urho3D/Math/AreaAllocator.h>
#include <Urho3D/Math/BoundingBox.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Frustum.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Matrix2.h>
#include <Urho3D/Math/Matrix3.h>
#include <Urho3D/Math/Matrix3x4.h>
#include <Urho3D/Math/Matrix4.h>
#include <Urho3D/Math/Plane.h>
#include <Urho3D/Math/Polyhedron.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/Math/Rect.h>
#include <Urho3D/Math/Sphere.h>
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Vector4.h>
#if URHO3D_NAVIGATION
#include <Urho3D/Navigation/CrowdAgent.h>
#include <Urho3D/Navigation/CrowdManager.h>
#include <Urho3D/Navigation/DynamicNavigationMesh.h>
#include <Urho3D/Navigation/NavArea.h>
#include <Urho3D/Navigation/NavBuildData.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Navigation/NavigationEvents.h>
#include <Urho3D/Navigation/NavigationMesh.h>
#include <Urho3D/Navigation/Obstacle.h>
#include <Urho3D/Navigation/OffMeshConnection.h>
#endif
#if URHO3D_NETWORK
#include <Urho3D/Network/Connection.h>
#include <Urho3D/Network/HttpRequest.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Network/NetworkPriority.h>
#include <Urho3D/Network/Protocol.h>
#endif
#if URHO3D_PHYSICS
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/KinematicCharacterController.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsUtils.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RaycastVehicle.h>
#include <Urho3D/Physics/RaycastVehicleBase.h>
#include <Urho3D/Physics/RigidBody.h>
#endif
#include <Urho3D/Resource/BackgroundLoader.h>
#include <Urho3D/Resource/Decompress.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/JSONValue.h>
#include <Urho3D/Resource/Localization.h>
#include <Urho3D/Resource/PListFile.h>
#include <Urho3D/Resource/Resource.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Scene/Animatable.h>
#include <Urho3D/Scene/AnimationDefs.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/ObjectAnimation.h>
#include <Urho3D/Scene/ReplicationState.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/SceneResolver.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/Scene/SmoothedTransform.h>
#include <Urho3D/Scene/SplinePath.h>
#include <Urho3D/Scene/UnknownComponent.h>
#include <Urho3D/Scene/ValueAnimation.h>
#include <Urho3D/Scene/ValueAnimationInfo.h>
#include <Urho3D/UI/BorderImage.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/Cursor.h>
#include <Urho3D/UI/DropDownList.h>
#include <Urho3D/UI/FileSelector.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/FontFace.h>
#include <Urho3D/UI/FontFaceBitmap.h>
#include <Urho3D/UI/FontFaceFreeType.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/ListView.h>
#include <Urho3D/UI/Menu.h>
#include <Urho3D/UI/MessageBox.h>
#include <Urho3D/UI/ProgressBar.h>
#include <Urho3D/UI/ScrollBar.h>
#include <Urho3D/UI/ScrollView.h>
#include <Urho3D/UI/Slider.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/UI/ToolTip.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIBatch.h>
#include <Urho3D/UI/UIComponent.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/UISelectable.h>
#include <Urho3D/UI/View3D.h>
#include <Urho3D/UI/Window.h>
#if URHO3D_URHO2D
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/CollisionChain2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>
#include <Urho3D/Urho2D/CollisionEdge2D.h>
#include <Urho3D/Urho2D/CollisionPolygon2D.h>
#include <Urho3D/Urho2D/CollisionShape2D.h>
#include <Urho3D/Urho2D/Constraint2D.h>
#include <Urho3D/Urho2D/ConstraintDistance2D.h>
#include <Urho3D/Urho2D/ConstraintFriction2D.h>
#include <Urho3D/Urho2D/ConstraintGear2D.h>
#include <Urho3D/Urho2D/ConstraintMotor2D.h>
#include <Urho3D/Urho2D/ConstraintMouse2D.h>
#include <Urho3D/Urho2D/ConstraintPrismatic2D.h>
#include <Urho3D/Urho2D/ConstraintPulley2D.h>
#include <Urho3D/Urho2D/ConstraintRevolute2D.h>
#include <Urho3D/Urho2D/ConstraintRope2D.h>
#include <Urho3D/Urho2D/ConstraintWeld2D.h>
#include <Urho3D/Urho2D/ConstraintWheel2D.h>
#include <Urho3D/Urho2D/Drawable2D.h>
#include <Urho3D/Urho2D/ParticleEffect2D.h>
#include <Urho3D/Urho2D/ParticleEmitter2D.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <Urho3D/Urho2D/PhysicsUtils2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Urho2D/Renderer2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>
#include <Urho3D/Urho2D/SpriterData2D.h>
#include <Urho3D/Urho2D/SpriterInstance2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/StretchableSprite2D.h>
#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Urho2D/TileMapDefs2D.h>
#include <Urho3D/Urho2D/TileMapLayer2D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/Urho2D/Urho2D.h>
#include <Urho3D/Urho2D/Urho2DEvents.h>
#endif

#include <Urho3D/DebugNew.h>

using namespace Urho3D;
