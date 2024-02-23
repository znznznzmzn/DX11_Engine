#pragma once

class ModelAnimatorInstancing : public ModelAnimator {
private:
    class FrameInstancingBuffer : public ConstantBuffer {
    public:
        struct FrameInstancing {
            float takeTime = 0.2f;
            float tweenTime = 0.0f;
            float runningTime = 0.0f;
            float padding = 0.0f;
            Frame cur, next;
            FrameInstancing() { next.clip = -1; }
        };
        struct Data { FrameInstancing frames[MAX_INSTANCE];}data;
		FrameInstancingBuffer(UINT max_size) : ConstantBuffer(&data, sizeof(Data)) { }
    };

	vector<Transform*> transforms;
	InstanceData instanceDatas[MAX_INSTANCE];
	VertexBuffer* instanceBuffer;
	FrameInstancingBuffer* frameInstancingBuffer;
	UINT drawCount = 0;

	void UpdateFrames();
	void UpdateFrame(FrameInstancingBuffer::FrameInstancing& motion, UINT instance);
	void UpdateTransforms();

	void EndEvent(void* obj);

public:
	ModelAnimatorInstancing(string name, UINT max_count);
	~ModelAnimatorInstancing();

	void Update();
	void Render();
	void GUIRender();

	Transform* Add();
	int instanceCount() { return transforms.size(); }

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 0.2f);
};