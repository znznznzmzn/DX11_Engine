#pragma once

class Model : public Transform {
protected:
	ModelReader* reader;
	MatrixBuffer* wBuffer; 
	string name;
public:
	Model(string name);
	~Model();
	virtual void Render();
	virtual void InstancingRender(const UINT& instanceCount);
	virtual void GUIRender();
	vector<Material*>& GetMaterials() { return reader->GetMaterials(); }

	void SetShader(wstring file) { reader->SetShader(file); }
};
