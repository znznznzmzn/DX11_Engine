#include "Framework.h"


ModelAnimatorInstancing::ModelAnimatorInstancing(string name, UINT max_count)
	: ModelAnimator(name) {
	instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
	frameInstancingBuffer = new FrameInstancingBuffer(max_count);
	reader->SetShader(L"Shaders/Instancing/ModelAnimatorInstancing.hlsl");
}
ModelAnimatorInstancing::~ModelAnimatorInstancing() {
	SAFE_DELETE(instanceBuffer);
	SAFE_DELETE(frameInstancingBuffer);

	for (Transform*& elem : transforms) { SAFE_DELETE(elem); }
}

void ModelAnimatorInstancing::Update() {
	UpdateFrames();
	UpdateTransforms();
}

void ModelAnimatorInstancing::Render() {
	if (texture == nullptr) CreateTexture();
	frameInstancingBuffer->SetVSBuffer(4);
	DC->VSSetShaderResources(0, 1, &srv);
	instanceBuffer->IASet(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);
	reader->InstancingRender(drawCount);
}

void ModelAnimatorInstancing::GUIRender() {
	ImGui::Text("DrawCount : %d", drawCount);
	for (Transform*& elem : transforms) elem->GUIRender();
}

Transform* ModelAnimatorInstancing::Add() {
	Transform* newElem = new Transform();
	newElem->Tag() = name + "_" + to_string(transforms.size());
	transforms.push_back(newElem);
	return newElem;
}
void ModelAnimatorInstancing::UpdateFrames() {
	for (UINT i = 0; i < transforms.size(); i++)
		UpdateFrame(frameInstancingBuffer->data.frames[i], i);
}

void ModelAnimatorInstancing::UpdateFrame(FrameInstancingBuffer::FrameInstancing& motion, UINT instance) {
	{ // Cur Anim
		ModelClip* clip = clips[motion.cur.clip];
		motion.runningTime += motion.cur.speed * DELTA;
		motion.cur.time += clip->tickPerSecond * motion.cur.speed * DELTA;
		if (motion.cur.time >= 1.0f) {
			motion.cur.curFrame
				= Calc::RepeatIdx(motion.cur.curFrame + 1, clip->frameCount - 1);
			motion.cur.time = Calc::Repeat(motion.cur.time, 0.0f, 1.0f);
		}
	}
	{ // Next Anim
		if (motion.next.clip < 0) return;
		ModelClip* clip = clips[motion.next.clip];
		motion.tweenTime += DELTA / motion.takeTime;

		if (motion.tweenTime >= 1.0f) { // EndBlending
			motion.cur = motion.next;
			motion.tweenTime = 0.0f;

			motion.next.clip = -1;
			motion.next.curFrame = 0;
			motion.next.time = 0.0f;
		}
		else {
			motion.next.time += clip->tickPerSecond * motion.next.speed * DELTA;
			if (motion.next.time >= 1.0f) {
				motion.next.curFrame = Calc::RepeatIdx(motion.next.curFrame + 1, clip->frameCount - 1);
				motion.next.time = Calc::Repeat(motion.next.time, 0.0f, 1.0f);
			}
		}
	}
}

void ModelAnimatorInstancing::UpdateTransforms() {
	drawCount = 0;
	for (UINT i = 0; i < transforms.size(); i++) {
		if (transforms[i]->Active()) {
			transforms[i]->UpdateWorld();
			instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
			instanceDatas[drawCount].index = i;
			drawCount++;
		}
	}
	instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelAnimatorInstancing::PlayClip(UINT instance, UINT clip, float speed, float takeTime) {
	frameInstancingBuffer->data.frames[instance].next.clip = clip;
	frameInstancingBuffer->data.frames[instance].next.speed = speed;
	frameInstancingBuffer->data.frames[instance].takeTime = takeTime;
	frameInstancingBuffer->data.frames[instance].runningTime = 0.0f;
}

void ModelAnimatorInstancing::EndEvent(void* obj) { PlayClip(((InstanceData*)obj)->index, 0); }