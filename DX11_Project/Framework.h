#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN 

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define CENTER_X SCREEN_WIDTH / 2
#define CENTER_Y SCREEN_HEIGHT / 2

#define MAX_INSTANCE 128

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define SAFE_RELEASE(ID3D) if(ID3D != nullptr) ID3D->Release(); ID3D = nullptr
#define SAFE_DELETE(PTR) if(PTR != nullptr) delete PTR; PTR = nullptr

//시간
#define DELTA ImGui::GetIO().DeltaTime

// 키
#define KEY_PRESS(key) ImGui::IsKeyDown(key)
#define KEY_DOWN(key) ImGui::IsKeyPressed(key, false)
#define KEY_DOWN_ING(key) ImGui::IsKeyPressed(key, true)
#define KEY_UP(key) ImGui::IsKeyReleased(key)

// 마우스 - 0:좌, 1:우, 2:중간
#define MOUSE_PRESS(idx) ImGui::IsMouseDown(idx)
#define MOUSE_CLICK(idx) ImGui::IsMouseClicked(idx, false)
#define MOUSE_CLICK_ING(idx) ImGui::IsMouseClicked(idx, true)
#define MOUSE_DBCLICK(idx) ImGui::IsMouseDoubleClicked(idx)
#define MOUSE_UP(idx) ImGui::IsMouseReleased(idx)

#define SCENE SceneManager::Get()

#define CAMERA Environment::Get()->GetMainCamera()
#define TWEEN Environment::Get()->GetMainTween()

#define DIALOG ImGuiFileDialog::Instance()

#define LERP(s, e, t) (s + (e - s) * t)

// Windows 헤더 파일
#include <windows.h>
#include <random>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <functional>
#include <fstream>

#include <DirectXCollision.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>
#pragma comment(lib, "ImGui.lib")

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment(lib, "assimp-vc143-mtd.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::TriangleTests;

typedef XMFLOAT4X4 float4x4;
typedef XMFLOAT4 float4;
typedef XMFLOAT3 float3;
typedef XMFLOAT2 float2;
typedef XMMATRIX Matrix;
typedef XMVECTOR Vector4;

typedef function<void()> Event;
typedef function<void(void*)> ObjectEvent;

// Framework Header
#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/Gloval.h"
#include "Framework/Utility/Utility.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"

using namespace Utility;

#include "Framework/Device/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"

#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/ConstantBuffer.h"
#include "Framework/Buffer/RawBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/VertexLayout.h"
#include "Framework/Buffer/StructuredBuffer.h"

#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/Transformation.h"

#include "Framework/Math/Calc.h"
#include "Framework/Utility/GUI.h"

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"

#include "Framework/Model/ModelType.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelReader.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelBone.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Tween.h"
#include "Framework/Environment/Environment.h"

#include "Algorithm/Node.h"
#include "Algorithm/Heap.h"
#include "Algorithm/AStar.h"
#include "Algorithm/Observer.h"

// GameObject Header
#include "Objects/GUI/GUI_Basic.h"
#include "Objects/GUI/GUI_BitmapLabel.h"
#include "Objects/GUI/GUI_ProgressBar.h"
#include "Objects/GUI/GUI_FPS_CrossHair.h"

#include "Objects/Light/Light.h"
#include "Objects/Light/PointLight.h"
#include "Objects/Light/SpotLight.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Sprite.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/Rain.h"

#include "Objects/Basic/Grid.h"
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/GameObject_Packer.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Cylinder.h"
#include "Objects/Basic/Cylinder_Reversed.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Sphere_Reversed.h"
#include "Objects/Basic/Plane.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Quad2.h"

#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/Maze.h"

#include "Objects/Character/Robot.h"
#include "Objects/Character/C_PotMan.h"
#include "Objects/Character/Controller_Souls.h"
#include "Objects/Character/C_FirstPerson.h"
#include "Objects/Character/FPS_Runner.h"

// Scene Header
#include "Scenes/Scene.h"
#include "GameManager/GameManager.h"
#include "GameManager/SceneManager.h"

extern HWND hWnd;
extern string projectPath;
extern Vector3 mousePos;
