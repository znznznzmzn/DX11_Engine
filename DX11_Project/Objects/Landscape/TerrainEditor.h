#pragma once
class TerrainEditor : public Transform {
private:
#pragma region ObjectData
	Material* material;
	Mesh* mesh;
	MatrixBuffer* wBuffer;
#pragma endregion

#pragma region TerrainData
	Texture* heightMap = nullptr; 
	Texture* alphaMap  = nullptr;

	Texture* secondDiffuseMap = nullptr;
	Texture* thirdDiffuseMap  = nullptr;

	Texture* secondSpecularMap = nullptr;
	Texture* thirdSpecularMap  = nullptr;

	Texture* secondNormalMap = nullptr;
	Texture* thirdNormalMap  = nullptr;

	struct VertexAlpha {
		float3 pos;
		float2 uv;
		float3 normal;
		float3 tangent;
		float alpha[4];
		VertexAlpha() : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0), alpha{} { }
	};
	vector<VertexAlpha> vertices;
	vector<UINT> indices;
	void CalcNormal();
	void CalcTangent();
	void ResetNormalTangent();

	Point Size;
	float MAX_HEIGHT = 50.0f;
#pragma endregion

#pragma region EditorController
	Ray ray;
	stack<Point> searchList;
	enum class EditType { NONE, HEIGHT, EVERAGE, ALPHA, OBJECT };
	EditType editType = EditType::NONE;
	float AdjustStrength = 10.0f;
	int selectMap = 0;
	void Brush();
	void Brushing(VertexAlpha& target);

	void CreateMesh();
	void CreateComputeData();
	void UpdateInput();

	struct InputDesc { float3 v0, v1, v2; };
	struct OutputDesc {
		int picked;
		float distance;
	};
	class BrushBuffer : public ConstantBuffer {
	public:
		struct Data {
			int type = 0;
			float3 pickingPos = { 0, 0, 0 };
			float range = 5.0f;
			float3 color = { 0, 0.5f, 0 };
		} data;
		BrushBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	};
	class RayBuffer : public ConstantBuffer {
	public:
		struct Data {
			float3 position = { 0.0f, 0.0f, 0.0f };
			float triangleSize = 0.0f;
			float3 direction = { 0.0f, 0.0f, 0.0f };
			float padding = 0.0f;
		} data; 
		RayBuffer() : ConstantBuffer(&data, sizeof(Data)) { }
	};

	ComputeShader* computeShader;
	StructuredBuffer* structuredBuffer;

	BrushBuffer* brushBuffer;
	RayBuffer* rayBuffer;

	InputDesc* input;
	OutputDesc* output;

	UINT triangleSize;

	Vector3 Picking();
	Vector3 ComputePicking();
	
	class ObjectList {
	public:
		ObjectList(string name);
		~ObjectList();
		Vector3 RotationOverride;
		Vector3 ScaleOverride;
		vector<Model*> list;
	};
	int selectedObject_idx = 0;
	string selectedObject = "";
	vector<string> objectList;
	map<string, ObjectList*> objects;
#pragma endregion

	void SaveHeight(string file);
	void LoadHeight(string file);
	void SaveHeightDialog();
	void LoadHeightDialog();

	void SaveAlpha(string file);
	void LoadAlpha(string file);
	void SaveAlphaDialog();
	void LoadAlphaDialog();

	void SaveTerrain(string file);
	void LoadTerrain(string file);
	void SaveTerrainDialog();
	void LoadTerrainDialog();

public:
	TerrainEditor(UINT width = 100, UINT height = 100);
	~TerrainEditor();

	void Update();
	void Render();
	void GUIRender();

	float getHeight(float const& x, float const& z);

	Vector3 PickingPos = Vector3(0.0f, 0.0f, 0.0f);
};