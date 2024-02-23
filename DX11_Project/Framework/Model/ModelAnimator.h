#pragma once

class ModelAnimator : public Model {
protected:
	struct Frame {
		int clip = 0;
		UINT curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};
	class FrameBuffer : public ConstantBuffer {
	public:
		struct Data {
			float takeTime = 0.2f;
			float tweenTime = 0.0f;
			float runningTime = 0.0f;
			float padding = 0.0f;
			Frame cur, next;
		} data;
		FrameBuffer() : ConstantBuffer(&data, sizeof(Data)) { data.next.clip = -1; }
	};
private:
	void Rend();
protected:
	void CreateTexture();
	void CreateClipTransform(UINT index);

	bool isPlay = false;
	vector<ModelClip*> clips;
	FrameBuffer* fBuffer = nullptr;
	vector<ClipTransform*> clipTransforms;
	vector<ClipTransform*> nodeTransforms;

	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
public:
	ModelAnimator(string name);
	~ModelAnimator();

	void UpdateFrame();
	virtual void Render() override;
	virtual void InstancingRender(const UINT& instanceCount) override;
	virtual void GUIRender() override;

	void ReadClip(string clipName, UINT clipNum = 0);
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);
	void Pause(const bool& isPause) { isPlay = !isPause; }

	ModelClip* GetClip(UINT idx) { return clips[idx]; }
	Matrix GetTransformByNode(int nodeIndex);
};