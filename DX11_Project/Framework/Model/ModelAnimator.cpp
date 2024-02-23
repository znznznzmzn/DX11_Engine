#include "Framework.h"

void ModelAnimator::CreateTexture() {
	UINT clipCount = clips.size();

	clipTransforms.resize(clipCount);
	nodeTransforms.resize(clipCount);

	UINT max_bone  = reader->GetNodes().size();
	UINT max_frame = 0;
	for (UINT i = 0; i < clipCount; i++) {
		clipTransforms[i] = new ClipTransform();
		clipTransforms[i]->transform.resize(clips[i]->frameCount);
		nodeTransforms[i] = new ClipTransform();
		nodeTransforms[i]->transform.resize(clips[i]->frameCount);
		for (vector<Matrix>& elem : clipTransforms[i]->transform) elem.resize(max_bone);
		for (vector<Matrix>& elem : nodeTransforms[i]->transform) elem.resize(max_bone);
		max_frame = max(max_frame, clips[i]->frameCount);
	}
	for (UINT i = 0; i < clipCount; i++) CreateClipTransform(i);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width  = max_bone * 4;
	desc.Height = max_frame;
	desc.MipLevels = 1;
	desc.ArraySize = clipCount;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	UINT pitchSize = max_bone * sizeof(Matrix);
	UINT pageSize  = pitchSize * max_frame;

	void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);
	for (UINT c = 0; c < clipCount; c++) {
		UINT start = c * pageSize;
		for (UINT y = 0; y < max_frame; y++) {
			void* tmp = (BYTE*)p + pitchSize * y + start;
			VirtualAlloc(tmp, pitchSize, MEM_COMMIT, PAGE_READWRITE);
			if(y >= clips[c]->frameCount) memcpy(tmp, new Matrix[max_bone], pitchSize);
			else memcpy(tmp, clipTransforms[c]->transform[y].data(), pitchSize);
		} 
	}
	D3D11_SUBRESOURCE_DATA* subResources = new D3D11_SUBRESOURCE_DATA[clipCount];

	for (UINT c = 0; c < clipCount; c++) {
		void* tmp = (BYTE*)p + c * pageSize;
		subResources[c].pSysMem = tmp;
		subResources[c].SysMemPitch = pitchSize;
		subResources[c].SysMemSlicePitch = pageSize;
	}
	DEVICE->CreateTexture2D(&desc, subResources, &texture);

	VirtualFree(p, 0, MEM_RELEASE);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = clipCount;
	DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}
void ModelAnimator::CreateClipTransform(UINT index) {
	ModelClip* clip = clips[index];
	for (UINT f = 0; f < clip->frameCount; f++) {
		vector<NodeData> nodes = reader->GetNodes();
		UINT nodeIndex = 0;
		for (NodeData elem : nodes) {
			Matrix animation;
			KeyFrame* frame = clip->GetKeyFrame(elem.name);
			if (frame != nullptr) {
				KeyTransform& transform = frame->transforms[f];
				animation = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(),
					Vector3(transform.scale), XMVectorZero(), XMLoadFloat4(&transform.rotation),
					Vector3(transform.position));
			}
			else animation = XMMatrixIdentity();

			Matrix parent;
			int parentIndex = elem.parent;

			parent = (parentIndex < 0) ? XMMatrixIdentity() : 
				nodeTransforms[index]->transform[f][parentIndex];

			nodeTransforms[index]->transform[f][nodeIndex] = animation * parent;

			int boneIndex = reader->GetBone(elem.name);
			if (boneIndex >= 0) {
				clipTransforms[index]->transform[f][boneIndex] =
					reader->GetBones()[boneIndex].offset * 
					nodeTransforms[index]->transform[f][nodeIndex];
			}
			nodeIndex++;
		}
	}
}

ModelAnimator::ModelAnimator(string name) : Model(name) {
	reader->SetShader(L"Shaders/Model/ModelAnimator.hlsl");
	fBuffer = new FrameBuffer();
}
ModelAnimator::~ModelAnimator() {
	for (ModelClip*& elem : clips) { SAFE_DELETE(elem); }
	clips.clear();

	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);

	SAFE_DELETE(fBuffer);

	for (ClipTransform*& elem : clipTransforms) { SAFE_DELETE(elem); }
	for (ClipTransform*& elem : nodeTransforms) { SAFE_DELETE(elem); }
	clipTransforms.clear();
	nodeTransforms.clear();
}


void ModelAnimator::UpdateFrame() {
	if (!isPlay) return;
	FrameBuffer::Data& frameData = fBuffer->data;
	{ // Cur Anim
		ModelClip* clip = clips[frameData.cur.clip];
		clip->playTime += frameData.cur.speed * DELTA;
		frameData.cur.time += clip->tickPerSecond * frameData.cur.speed * DELTA;
		if (frameData.cur.time >= 1.0f) {
			frameData.cur.curFrame 
				= Calc::RepeatIdx(frameData.cur.curFrame + 1, clip->frameCount - 1);
			frameData.cur.time = Calc::Repeat(frameData.cur.time, 0.0f, 1.0f);
		}
		clip->Excute();
	} 
	{ // Next Anim
		if (frameData.next.clip < 0) return;
		ModelClip* clip = clips[frameData.next.clip];
		frameData.tweenTime += DELTA / frameData.takeTime;

		if (frameData.tweenTime >= 1.0f) { // EndBlending
			frameData.cur = frameData.next;
			frameData.tweenTime = 0.0f;

			frameData.next.clip = -1.0f;
			frameData.next.curFrame = 0;
			frameData.next.time =  0.0f;
		}
		else {
			frameData.next.time += clip->tickPerSecond * frameData.next.speed * DELTA;
			if (frameData.next.time >= 1.0f) {
				frameData.next.curFrame = Calc::RepeatIdx(frameData.next.curFrame + 1, clip->frameCount - 1);
				frameData.next.time = Calc::Repeat(frameData.next.time, 0.0f, 1.0f);
			}
		}
	}
}


void ModelAnimator::Rend() {
	if (texture == nullptr) CreateTexture();
	fBuffer->SetVSBuffer(3);
	DC->VSSetShaderResources(0, 1, &srv);
}
void ModelAnimator::Render() {
	if (!Active()) return;
	Rend();
	Model::Render();
}
void ModelAnimator::InstancingRender(const UINT& instanceCount) {
	if (!Active()) return;
	Rend();
	Model::InstancingRender(instanceCount);
}

void ModelAnimator::GUIRender() {
	Transform::GUIRender();
	int clip = fBuffer->data.cur.clip;
	if(ImGui::SliderInt("Clip", &clip, 0, clips.size() - 1)) PlayClip(clip);
	ImGui::SliderInt("Frame", (int*)&fBuffer->data.cur.curFrame, 0, clips[clip]->frameCount - 1);
	reader->GUIRender();
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum) {
	string path = "Models/Clips/" + name + "/" + clipName + to_string(clipNum) + ".clip";

	BinaryReader* r = new BinaryReader(path);

	if(r->IsFailed()) assert(false);

	ModelClip* clip = new ModelClip();
	clip->name = r->String();
	clip->frameCount = r->UInt();
	clip->tickPerSecond = r->Float();

	UINT boneCount = r->UInt();
	for (UINT i = 0; i < boneCount; i++) {
		KeyFrame* keyFrame = new KeyFrame();

		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		keyFrame->transforms.resize(size);
		void* ptr = (void*)keyFrame->transforms.data();
		r->Byte(&ptr, sizeof(KeyTransform) * size);
		clip->keyFrames[keyFrame->boneName] = keyFrame;
	}
	clips.push_back(clip);
}
void ModelAnimator::PlayClip(UINT clip, float speed, float takeTime) {
	isPlay = true;
	fBuffer->data.next.clip = clip;
	fBuffer->data.next.speed = speed;
	fBuffer->data.takeTime = takeTime;
	clips[clip]->duration /= speed;
	clips[clip]->Init();
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex) {
	if (texture == nullptr) return XMMatrixIdentity();

	nodeIndex = Calc::Clamp(nodeIndex, 0, reader->GetNodes().size() - 1);

	Matrix curAnim;

	{//CurAnim
		Frame& curFrame = fBuffer->data.cur;

		Matrix cur  = nodeTransforms[curFrame.clip]->transform[curFrame.curFrame    ][nodeIndex];
		Matrix next = nodeTransforms[curFrame.clip]->transform[curFrame.curFrame + 1][nodeIndex];

		curAnim = LERP(cur, next, curFrame.time);
	}

	{//NextAnim
		Frame& nextFrame = fBuffer->data.next;

		if (nextFrame.clip == -1) return curAnim;

		Matrix cur  = nodeTransforms[nextFrame.clip]->transform[nextFrame.curFrame    ][nodeIndex];
		Matrix next = nodeTransforms[nextFrame.clip]->transform[nextFrame.curFrame + 1][nodeIndex];

		Matrix nextAnim = LERP(cur, next, nextFrame.time);
		return LERP(curAnim, nextAnim, fBuffer->data.tweenTime);
	}
}
