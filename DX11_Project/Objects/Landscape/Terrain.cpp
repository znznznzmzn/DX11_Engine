#include "Framework.h"

Terrain::Terrain(UINT width, UINT height) : Size(width, height) {
	init(L"Shaders/Landscape/Terrain.hlsl");
	CreateMesh();
	UpdateMesh();
	Tag() = "terrain";
}
Terrain::Terrain(wstring path) {
	init(L"Shaders/Landscape/Terrain.hlsl");
	LoadTerrain(ToString(path));
	UpdateMesh();
}
Terrain::~Terrain() {
	for (Model*& elem : objects) { SAFE_DELETE(elem); }
	objects.clear();
}

void Terrain::CreateMesh() {
	vector<float4> pixels(Size.x * Size.y, float4(0, 0, 0, 0));
	if (heightMap) {
		Size.x = heightMap->Width();
		Size.y = heightMap->Height();
		heightMap->ReadPixels(pixels);
	}
	vertices.clear();
	indices.clear();

	// Vertices
	for (int z = 0; z < Size.y; z++) {
		for (int x = 0; x < Size.x; x++) {
			VertexType elem;
			elem.pos = { (float)x, 0.0f, (float)z };
			int index = Size.x * z + x;
			elem.pos.y = pixels[index].x * MAX_HEIGHT;

			elem.uv.x = 1.0f - (x / (float)(Size.x - 1));
			elem.uv.y = 1.0f - (z / (float)(Size.y - 1));
			
			elem.normal = { 0.0f, 0.0f, 0.0f };

			vertices.emplace_back(elem);
		}
	}
	// Indices
	for (int z = 0; z < Size.y - 1; z++) {
		for (int x = 0; x < Size.x - 1; x++) {
			indices.emplace_back(x +     ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x +     ( z      * Size.x));

			indices.emplace_back(x +     ( z      * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + ( z      * Size.x));
		}
	}

	Calc::Vertex::Normal(vertices, indices);
	Calc::Vertex::Tangent(vertices, indices);
}
void Terrain::UpdateMesh() {
	SAFE_DELETE(mesh);
	mesh = new Mesh(
		vertices.data(), sizeof(VertexType), static_cast<UINT>(vertices.size()),
		indices.data(), static_cast<UINT>(indices.size()));
	UpdateWorld();
}

void Terrain::Render() {
	if (!Active()) return;

	if (model_switch) for (Model*& elem : objects) elem->Render();

	if (alphaMap) alphaMap ->PSSet(10);

	if (secondDiffuseMap) secondDiffuseMap->PSSet(11);
	if (thirdDiffuseMap ) thirdDiffuseMap ->PSSet(12);

	if (secondSpecularMap) secondSpecularMap->PSSet(13);
	if (thirdSpecularMap ) thirdSpecularMap ->PSSet(14);

	if (secondNormalMap) secondNormalMap->PSSet(15);
	if (thirdNormalMap ) thirdNormalMap ->PSSet(16);

	GameObject::Render();
}

void Terrain::GUIRender() {
	if (ImGui::TreeNode(("Terrain" + Tag()).c_str())) {
		ImGui::Indent();

		ImGui::Checkbox("ModelRender", &model_switch);
		LoadHeightDialog();
		LoadAlphaDialog();
		material->GUIRender();
		LoadTerrainDialog();

		ImGui::Unindent();
		ImGui::TreePop();
	}
}

float Terrain::getHeight(float x, float z) {
	if (!Calc::isInIndex(Point(static_cast<int>(floor(x)), static_cast<int>(floor(z))), Size)) return 0.0f;
	int idx[4] = {
		static_cast<int>(floor(floor(x) +     (floor(z    ) * Size.x))), // 2 3
		static_cast<int>(floor(floor(x) + 1 + (floor(z    ) * Size.x))), // 0 1
		static_cast<int>(floor(floor(x) +     (floor(z + 1) * Size.x))), // 요 범위안에 있음
		static_cast<int>(floor(floor(x) + 1 + (floor(z + 1) * Size.x)))
	};
	float prog_x = x - vertices[idx[0]].pos.x;
	float prog_z = z - vertices[idx[0]].pos.z;
	float inc_x = (prog_x > prog_z) ? 
		(vertices[idx[1]].pos.y - vertices[idx[0]].pos.y) :
		(vertices[idx[3]].pos.y - vertices[idx[2]].pos.y);
	float inc_z = (prog_x > prog_z) ?
		(vertices[idx[3]].pos.y - vertices[idx[1]].pos.y) :
		(vertices[idx[2]].pos.y - vertices[idx[0]].pos.y);
	return vertices[idx[0]].pos.y + (inc_x * prog_x + inc_z * prog_z);
}

// HeightMap Lode
void Terrain::LoadHeight(string file) {
	heightMap = Texture::Add(ToWString(file));
	CreateMesh();
	UpdateMesh();
}
void Terrain::LoadHeightDialog() {
	if (EditorGUI::ImageButton("Load Height", heightMap))
		DIALOG->OpenDialog("LoadHeight", "Load HeightMap", EditorGUI::filter_image.c_str(), ".");

	if (DIALOG->Display("LoadHeight")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			LoadHeight(file);
		}
		DIALOG->Close();
	}
}

// AlphaMap Lode
void Terrain::LoadAlpha(string file) {
	alphaMap = Texture::Add(ToWString(file));
	mesh->UpdateVertex(vertices.data(), static_cast<UINT>(vertices.size()));
}
void Terrain::LoadAlphaDialog() {
	if (EditorGUI::ImageButton("Load Alpha", alphaMap))
		DIALOG->OpenDialog("LoadAlpha", "Load AlphaMap", EditorGUI::filter_image.c_str(), ".");

	if (DIALOG->Display("LoadAlpha")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			LoadAlpha(file);
		}
		DIALOG->Close();
	}
}

// Terrain Lode
void Terrain::LoadTerrain(string file) {
	Tag() = file;
	BinaryReader* r = new BinaryReader(file);

	LoadHeight(r->String());
	LoadAlpha(r->String());
	material->LoadMaterial(r->String());
	material->SetShader(L"Shaders/Landscape/Terrain.hlsl");

	secondDiffuseMap  = Texture::Add(r->WString());  // Second
	secondSpecularMap = Texture::Add(r->WString());
	secondNormalMap   = Texture::Add(r->WString());

	thirdDiffuseMap  = Texture::Add(r->WString()); // Third
	thirdSpecularMap = Texture::Add(r->WString());
	thirdNormalMap   = Texture::Add(r->WString());

	// objects;
	int n_obj = r->UInt();
	for (int i = 0; i < n_obj; i++) {
		string name = r->String();
		int n_list = r->UInt();
		for (int j = 0; j < n_list; j++) {
			Model * newItme = new Model(name);
			newItme->Position() = r->Vector();
			newItme->Rotation() = r->Vector();
			newItme->Scale   () = r->Vector();
			newItme->UpdateWorld();
			objects.push_back(newItme);
		}
	}

	delete r;
}
void Terrain::LoadTerrainDialog() {
	if (ImGui::Button("Load Terrain", { 160, 30 }))
		DIALOG->OpenDialog("LoadTerrain", "Load Terrain", ".map", ".");
	if (DIALOG->Display("LoadTerrain")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			LoadTerrain(file);
			UpdateMesh();
		}
		DIALOG->Close();
	}
}

Vector3 Terrain::Picking() {
	Ray ray = CAMERA->ScreenPointToRay(mousePos);
#pragma region 포인트 검사
	Point prev_origin = Point(PickingPos.x, PickingPos.z);
	while (!searchList.empty()) searchList.pop();
	searchList.push(prev_origin);
	for (int i = 1; !searchList.empty(); i++) {
		// 탐색 
		for (Point elem; !searchList.empty(); searchList.pop()) {
			elem = searchList.top();
			UINT idx[4];
			idx[0] = elem.x + Size.x * elem.y;
			idx[1] = elem.x + 1 + Size.x * elem.y;
			idx[2] = elem.x + Size.x * (elem.y + 1);
			idx[3] = elem.x + 1 + Size.x * (elem.y + 1);
			Vector3 p[4];
			for (UINT i = 0; i < 4; i++) p[i] = vertices[idx[i]].pos;

			float distance = 0.0f;
			if (Intersects(ray.position, ray.direction, p[0], p[1], p[2], distance))
				return PickingPos = ray.position + ray.direction * distance;
			if (Intersects(ray.position, ray.direction, p[3], p[1], p[2], distance))
				return PickingPos = ray.position + ray.direction * distance;
		}

		// searchList 재삽입
		int odd = (i * 2 + 1);
		if (Calc::isInIndex(prev_origin.y - i, Size.y - 1)) {
			int line_y = prev_origin.y - i;
			for (int x = 0; x < odd; x++) { // 아랫쪽 x
				Point elem = Point(prev_origin.x - i + x, line_y);
				if (Calc::isInIndex(elem.x, Size.x - 1)) searchList.push(elem);
			}
		}
		if (Calc::isInIndex(prev_origin.y + i, Size.y - 1)) {
			int line_y = prev_origin.y + i;
			for (int x = 0; x < odd; x++) { // 윗쪽 x
				Point elem = Point(prev_origin.x - i + x, line_y);
				if (Calc::isInIndex(elem.x, Size.x - 1)) searchList.push(elem);
			}
		}

		if (Calc::isInIndex(prev_origin.x - i, Size.x - 1)) {
			int line_x = prev_origin.x - i;
			for (int y = 0; y < odd - 2; y++) { // 왼쪽 y
				Point elem = Point(line_x, prev_origin.y - (i - 1) + y);
				if (Calc::isInIndex(elem.y, Size.y - 1)) searchList.push(elem);
			}
		}
		if (Calc::isInIndex(prev_origin.x + i, Size.x - 1)) {
			int line_x = prev_origin.x + i;
			for (int y = 0; y < odd - 2; y++) { // 오른쪽 y
				Point elem = Point(line_x, prev_origin.y - (i - 1) + y);
				if (Calc::isInIndex(elem.y, Size.y - 1)) searchList.push(elem);
			}
		}
	}
#pragma endregion
	return Vector3(0, 0, 0);
}