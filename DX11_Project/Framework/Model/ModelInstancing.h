#pragma once

class ModelInstancing {
private:
	string name;
	ModelReader* reader;
	vector<Transform*> transforms;
	vector<InstanceData> instanceDatas;
	VertexBuffer* instanceBuffer;
	UINT drawCount = 0;
public:
	ModelInstancing(string name, UINT max_count = 100);
	~ModelInstancing();

	void Update();
	void Render();
	void GUIRender();

	Transform* Add();
};