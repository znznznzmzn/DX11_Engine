#pragma once
class Terrain : public GameObject {
private:
	const float MAX_HEIGHT = 50.0f;

	bool model_switch = true;
	Point Size;

	Texture* heightMap = nullptr;
	Texture* alphaMap = nullptr;

	Texture* secondDiffuseMap = nullptr;
	Texture* thirdDiffuseMap = nullptr;

	Texture* secondSpecularMap = nullptr;
	Texture* thirdSpecularMap = nullptr;

	Texture* secondNormalMap = nullptr;
	Texture* thirdNormalMap = nullptr;

	vector<Model*> objects;
	stack<Point> searchList;

	void CreateMesh();
	void UpdateMesh();

	void LoadHeight(string file);
	void LoadHeightDialog();

	void LoadAlpha(string file);
	void LoadAlphaDialog();

	void LoadTerrain(string file);
	void LoadTerrainDialog();

public:
	Terrain(UINT width = 100, UINT height = 100);
	Terrain(wstring path);
	~Terrain();
	void Render() override;
	void GUIRender();

	void setModels(bool const& value) { model_switch = value; }

	float getHeight(Vector3 position) { return getHeight(position.x, position.z); }
	float getHeight(float x, float z);
	Point getSize() { return Size; }

	Vector3 Picking();
	Vector3 PickingPos = Vector3(0.0f, 0.0f, 0.0f);
};