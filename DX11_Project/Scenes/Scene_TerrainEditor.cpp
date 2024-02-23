#include "Framework.h"
#include "Scene_TerrainEditor.h"

/* ByteAddress 예제
void Scene_TerrainEditor::ByteAddress() {
	ComputeShader* shader = Shader::AddCS(L"Shaders/Compute/ByteAddress.hlsl");
	struct Output {
		UINT groupID[3];
		UINT groupThreadID[3];
		UINT dispatchThreadID[3];
		UINT groupIndex;
	};
	UINT groupX = 2;
	UINT size = 10 * 8 * 3 * groupX;

	Output* output = new Output[size];
	RawBuffer* buffer = new RawBuffer(sizeof(Output) * size);
	DC->CSSetUnorderedAccessViews(0, 1, &buffer->GetUAV(), nullptr);
	shader->Set();

	//2 * 1 * 1
	DC->Dispatch(groupX, 1, 1);

	buffer->Copy(output, sizeof(Output)* size);
	FILE* file;
	fopen_s(&file, "TextData/RawTest.csv", "w");
	if (file != nullptr) {
		for (UINT i = 0; i < size; i++) {
			Output tmp = output[i];
			fprintf(
				file,
				"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				i,
				tmp.groupID[0], tmp.groupID[1], tmp.groupID[2],
				tmp.groupThreadID[0], tmp.groupThreadID[1], tmp.groupThreadID[2],
				tmp.dispatchThreadID[0], tmp.dispatchThreadID[1], tmp.dispatchThreadID[2],
				tmp.groupIndex
			);
		}
		fclose(file);
	}
	
}
*/

Scene_TerrainEditor::Scene_TerrainEditor() {
	te = new TerrainEditor();
	CAMERA->is_freemode = true;
	//robot = new Robot();
	//robot->Position() = pickedPos = { 10.0f, 0.0f, 10.0f };
	//ByteAddress();
}
Scene_TerrainEditor::~Scene_TerrainEditor() {
	delete te;
	//delete robot;
}

void Scene_TerrainEditor::Update() {
	te->Update();
	/*
	if (!CAMERA->is_freemode) {
		// 피킹 위치 등록
		if (MOUSE_PRESS(0) && !ImGui::GetIO().WantCaptureMouse) pickedPos = te->PickingPos;
		
		// 카메라 추적
		CAMERA->Position() = Vector3::lerp(CAMERA->Position(), robot->Position() + cameraOffset, DELTA / 0.1f);
		CAMERA->Rotation() = {
			atan2(robot->Position().y - CAMERA->Position().y, robot->Position().z - CAMERA->Position().z) + 1.8f,
			atan2(robot->Position().x - CAMERA->Position().x, robot->Position().z - CAMERA->Position().z),
			atan2(robot->Position().y - CAMERA->Position().y, robot->Position().x - CAMERA->Position().x) + Calc::PI / 2.0f
		};
	}
	// 피킹 장소로 로봇 추적
	if (Vector3::distance(robot->Position(), pickedPos) > 3.0f) {
		robot->animState = 1;
		float destination_angle = atan2(pickedPos.x - robot->Position().x, pickedPos.z - robot->Position().z);
		if (robot->Rotation().y < 0) {
			if (destination_angle - robot->Rotation().y > Calc::PI) destination_angle -= Calc::PI_2;
		}
		else {
			if (robot->Rotation().y - destination_angle > Calc::PI) destination_angle += Calc::PI_2;
		}
		robot->Rotation().y = Calc::Lerp(robot->Rotation().y, destination_angle, DELTA / 0.1f);
		robot->Rotation().y = Calc::Repeat(robot->Rotation().y, -Calc::PI, Calc::PI);
		robot->Position() += robot->Forward() * DELTA * 5.0f;
	}
	else robot->animState = 0;
	robot->Position().y = te->getHeight(robot->Position().x, robot->Position().z) + 2.0f;

	robot->Anim();
	robot->UpdateWorld();*/
}

void Scene_TerrainEditor::PreRender() {
}

void Scene_TerrainEditor::Render() {
	//robot->Render();
	te->Render();
}

void Scene_TerrainEditor::PostRender() {
}

void Scene_TerrainEditor::GUIRender() {
	te->GUIRender();

	//ImGui::Text("Robot Rotation : %.3f", 180.0f + robot->Rotation().y * Calc::PI_to_Degree);
}
