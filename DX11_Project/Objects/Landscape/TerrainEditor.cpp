#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height) : Size(width, height) {
	material = new Material(L"Shaders/Landscape/TerrainEditor.hlsl");
	wBuffer = new MatrixBuffer();
	world = XMMatrixIdentity();
	CreateMesh();
	CreateComputeData();
	brushBuffer = new BrushBuffer();
}
TerrainEditor::~TerrainEditor() {
	SAFE_DELETE(material);
	SAFE_DELETE(mesh    );
	SAFE_DELETE(wBuffer );

	SAFE_DELETE(structuredBuffer);
	SAFE_DELETE(brushBuffer		);
	SAFE_DELETE(rayBuffer		);
	delete[] input;
	delete[] output;

	for (auto& pair : objects) { SAFE_DELETE(pair.second); }
}

void TerrainEditor::CreateMesh() {
	vector<float4> pixels(Size.x * Size.y, float4(0, 0, 0, 0));
	if (heightMap) {
		Size.x = heightMap->Width();
		Size.y = heightMap->Height();
		heightMap->ReadPixels(pixels);
	}
	vertices.clear();
	indices .clear();

	// Vertices
	for (UINT z = 0; z < Size.y; z++) {
		for (UINT x = 0; x < Size.x; x++) {
			VertexAlpha elem;
			elem.pos = { float(x), 0.0f, (float)z };
			UINT index = Size.x * z + x;
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
			indices.emplace_back(x + ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x + (z * Size.x));

			indices.emplace_back(x + (z * Size.x));
			indices.emplace_back(x + 1 + ((z + 1) * Size.x));
			indices.emplace_back(x + 1 + (z * Size.x));
		}
	}

	CalcNormal();
	CalcTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexAlpha), (UINT)vertices.size(), indices.data(), indices.size());
}
void TerrainEditor::CreateComputeData() {
	computeShader = Shader::AddCS(L"Shaders/Compute/Picking.hlsl");
	triangleSize = indices.size() / 3;

	input = new InputDesc[triangleSize];
	output = new OutputDesc[triangleSize];

	for (UINT i = 0; i < triangleSize; i++) {
		input[i].v0 = vertices[indices[i * 3 + 0]].pos;
		input[i].v1 = vertices[indices[i * 3 + 1]].pos;
		input[i].v2 = vertices[indices[i * 3 + 2]].pos;
	}

	structuredBuffer = new StructuredBuffer(
		input, sizeof(InputDesc), triangleSize,
		sizeof(OutputDesc), triangleSize);

	rayBuffer = new RayBuffer();
}
void TerrainEditor::UpdateInput() {
	for (UINT i = 0; i < triangleSize; i++) {
		input[i].v0 = vertices[indices[i * 3 + 0]].pos;
		input[i].v1 = vertices[indices[i * 3 + 1]].pos;
		input[i].v2 = vertices[indices[i * 3 + 2]].pos;
	}
	structuredBuffer->UpdateInput(input);
}

void TerrainEditor::Update() { 
	if(!ImGui::GetIO().WantCaptureMouse) PickingPos = brushBuffer->data.pickingPos = ComputePicking();
	if (CAMERA->is_freemode) {
		if (MOUSE_PRESS(0) && !ImGui::GetIO().WantCaptureMouse) Brush();
		if (MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse) {
			if (editType == EditType::OBJECT && objects.count(selectedObject) != 0) {
				Model* newItem = new Model(selectedObject);
				ObjectList*& Bag = objects[selectedObject];
				newItem->Position() = PickingPos;
				newItem->Position().y = getHeight(PickingPos.x, PickingPos.z);
				newItem->Rotation() = Bag->RotationOverride;
				newItem->Scale   () = Bag->ScaleOverride;
				newItem->UpdateWorld();
				Bag->list.emplace_back(newItem);
			}
		}
		if (MOUSE_UP(0)) {
			ResetNormalTangent();
			CalcNormal ();
			CalcTangent();

			mesh->UpdateVertex(vertices.data(), vertices.size());

			for (auto& pair : objects)
				for (auto& elem : pair.second->list) {
					elem->Position().y = getHeight(elem->Position().x, elem->Position().z);
					elem->UpdateWorld();
				}
		}
	}
}
void TerrainEditor::Render() {
	for (auto& pair : objects) {
		for (auto& elem : pair.second->list) elem->Render();
	}

	brushBuffer->SetPSBuffer(10);

	if (secondDiffuseMap) secondDiffuseMap->PSSet(11);
	if (thirdDiffuseMap ) thirdDiffuseMap ->PSSet(12);

	if (secondSpecularMap) secondSpecularMap->PSSet(13);
	if (thirdSpecularMap ) thirdSpecularMap ->PSSet(14);

	if (secondNormalMap) secondNormalMap->PSSet(15);
	if (thirdNormalMap ) thirdNormalMap ->PSSet(16);

	wBuffer->Set(world);
	wBuffer->SetVSBuffer(0);
	mesh->IASet();
	material->Set();
	DC->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}

void TerrainEditor::CalcNormal() {
	UINT idx1 = 0;
	UINT idx2 = 0;
	UINT idx3 = 0;
	for (int i = 0; i < indices.size() / 3; i++) {
		idx1 = indices[i * 3 + 0];
		idx2 = indices[i * 3 + 1];
		idx3 = indices[i * 3 + 2];

		Vector3 normal = Vector3::cross(
			Vector3(vertices[idx2].pos) - vertices[idx1].pos,
			Vector3(vertices[idx3].pos) - vertices[idx1].pos).normalize();

		vertices[idx1].normal = normal + vertices[idx1].normal;
		vertices[idx2].normal = normal + vertices[idx2].normal;
		vertices[idx3].normal = normal + vertices[idx3].normal;
	}
}
void TerrainEditor::CalcTangent() {
	UINT idx1 = 0;
	UINT idx2 = 0;
	UINT idx3 = 0;
	for (UINT i = 0; i < indices.size() / 3; i++) {
		idx1 = indices[i * 3 + 0];
		idx2 = indices[i * 3 + 1];
		idx3 = indices[i * 3 + 2];

		Vector3 e0 = Vector3(vertices[idx2].pos) - vertices[idx1].pos;
		Vector3 e1 = Vector3(vertices[idx3].pos) - vertices[idx1].pos;

		float u0 = vertices[idx2].uv.x - vertices[idx1].uv.x;
		float v0 = vertices[idx2].uv.y - vertices[idx1].uv.y;
		float u1 = vertices[idx3].uv.x - vertices[idx1].uv.x;
		float v1 = vertices[idx3].uv.y - vertices[idx1].uv.y;

		Vector3 tangent = (1.0f / (u0 * v1 - v0 * u1)) * (e0 * v1 - e1 * v0);

		vertices[idx1].tangent += tangent;
		vertices[idx2].tangent += tangent;
		vertices[idx3].tangent += tangent;
	}
	for (VertexAlpha& elem : vertices) {
		Vector3 t = elem.tangent;
		Vector3 n = elem.normal;
		elem.tangent = (t - n * Vector3::dot(n, t)).normalize();
	}
}
void TerrainEditor::ResetNormalTangent() 
{ for (VertexAlpha& elem : vertices) elem.tangent = elem.normal = float3(0, 0, 0); }

void TerrainEditor::Brush() {
	Point origin(PickingPos.x, PickingPos.z);
	if (!Calc::isInIndex(origin, Size) || editType == EditType::NONE) return;

	// 브러시 범위 구하기
	Point brush_min = { origin.x - floor(brushBuffer->data.range), origin.y - floor(brushBuffer->data.range) };
	Point brush_max = { origin.x + ceil(brushBuffer->data.range), origin.y + ceil(brushBuffer->data.range) };
	brush_min = Calc::ClampIdx(brush_min, Size - 1);
	brush_max = Calc::ClampIdx(brush_max, Size - 1);

	switch (brushBuffer->data.type) { // 브러시 타입별로 목표로하는 버택스 세부 선별
	case 0: { // 원
		int idx = 0;
		Vector3 target;
		float distance;
		for (int z = brush_min.y; z <= brush_max.y; z++) {
			for (int x = brush_min.x; x <= brush_max.x; x++) {
				idx = x + (z * Size.x);
				distance = Vector3::distance(
					Vector3(vertices[idx].pos.x, 0, vertices[idx].pos.z), 
					Vector3(PickingPos.x       , 0, PickingPos.z       ));
				if (distance < brushBuffer->data.range) 
					Brushing(vertices[idx]);
			}
		}
		break;
	}
	case 1: { // 사각형
		for (int z = brush_min.y; z <= brush_max.y; z++)
			for (int x = brush_min.x; x <= brush_max.x; x++)
				Brushing(vertices[x + (z * Size.x)]);
		break;
	}
	default: break;
	}
	UpdateInput();
	mesh->UpdateVertex(vertices.data(), static_cast<UINT>(vertices.size()));
}
void TerrainEditor::Brushing(VertexAlpha& target) {
	switch (editType) {
	case EditType::HEIGHT: {
		target.pos.y = 
			Calc::Clamp(target.pos.y + AdjustStrength * DELTA, 0.0f, MAX_HEIGHT);
		return;
	}
	case EditType::EVERAGE: {
		target.pos.y = 
			Calc::Lerp(target.pos.y, AdjustStrength, DELTA);
		return;
	}
	case EditType::ALPHA: {
		target.alpha[selectMap] = 
			Calc::Clamp(target.alpha[selectMap] + AdjustStrength * DELTA, 0.0f, 1.0f);
		return;
	}
	default: return;
	}
}

// GPU 피킹
Vector3 TerrainEditor::ComputePicking() { 
	ray = CAMERA->ScreenPointToRay(mousePos);
	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.triangleSize = static_cast<float>(triangleSize);
	rayBuffer->SetCSBuffer(0);
	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	computeShader->Set();

	UINT x = static_cast<UINT>(ceil((float)triangleSize / 64.0f));

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputDesc) * triangleSize);

	float minDistance = FLT_MAX;
	int minIndex = -1;
	for (UINT i = 0; i < triangleSize; i++) {
		OutputDesc tmp = output[i];
		if (tmp.picked) {
			if (minDistance > tmp.distance) {
				minDistance = tmp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0) return ray.position + ray.direction * minDistance;
	return Vector3(0, 0, 0);
}
// CPU 피킹
Vector3 TerrainEditor::Picking() {
	ray = CAMERA->ScreenPointToRay(mousePos);

#pragma region 포인트 검사
	Point prev_origin = Point(PickingPos.x, PickingPos.z);
	while (!searchList.empty()) searchList.pop();
	searchList.push(prev_origin);
	for (int i = 1; !searchList.empty(); i++) {
		// 탐색 
		for (Point elem; !searchList.empty(); searchList.pop()) {
			elem = searchList.top();
			UINT idx[4];
			idx[0] = elem.x +     Size.x *  elem.y;
			idx[1] = elem.x + 1 + Size.x *  elem.y;
			idx[2] = elem.x +     Size.x * (elem.y + 1);
			idx[3] = elem.x + 1 + Size.x * (elem.y + 1);
			Vector3 p[4];
			for (UINT i = 0; i < 4; i++) p[i] = vertices[idx[i]].pos;

			float distance = 0.0f;
			if (Intersects(ray.position, ray.direction, p[0], p[1], p[2], distance))
				return ray.position + ray.direction * distance;
			if (Intersects(ray.position, ray.direction, p[3], p[1], p[2], distance))
				return ray.position + ray.direction * distance;
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

#pragma region 전체 검사
	/* 
	for (UINT z = 0; z < Size.y - 1; z++) {
		for (UINT x = 0; x < Size.x - 1; x++) {
			UINT idx[4];
			idx[0] = Size.x *  z +      x;
			idx[1] = Size.x *  z +      x + 1;
			idx[2] = Size.x * (z + 1) + x;
			idx[3] = Size.x * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++) p[i] = vertices[idx[i]].pos;

			float distance = 0.0f;
			if (Intersects(ray.position, ray.direction, p[0], p[1], p[2], distance))
				return ray.position + ray.direction * distance;
			if (Intersects(ray.position, ray.direction, p[3], p[1], p[2], distance))
				return ray.position + ray.direction * distance;
		}
	}*/
#pragma endregion

	return Vector3(0, 0, 0);
}

// 특정 위치의 높이 구하기
float TerrainEditor::getHeight(float const& x, float const& z) {
	if (!Calc::isInIndex(Point(floor(x), floor(z)), Size)) return 0.0f;
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

void TerrainEditor::GUIRender() {
	ImGui::Text("Picking Pos : %.2f, %.2f, %.2f", PickingPos.x, PickingPos.y, PickingPos.z);

	ImGui::SliderInt("BrushType", &brushBuffer->data.type, 0, 2);
	ImGui::SliderFloat("Range", &brushBuffer->data.range, 0.5f, 100.0f);
	ImGui::SliderFloat("Adjust Strength", &AdjustStrength, -30.0f, 30.0f);
	ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->data.color);

	const char* editList[] = { "None", "Height", "Average", "Alpha", "Object"};
	ImGui::Combo("EditType", (int*)&editType, editList, 5);

	switch (editType) {
	case EditType::NONE: {
		LoadHeightDialog(); ImGui::SameLine(); LoadAlphaDialog();
		SaveHeightDialog(); ImGui::SameLine(); SaveAlphaDialog();
		break;
	}
	case EditType::HEIGHT: case EditType::EVERAGE: {
		LoadHeightDialog();
		SaveHeightDialog();
		break;
	}
	case EditType::ALPHA: {
		ImGui::SliderInt("SelectMap", &selectMap, 0, 1);

		if (ImGui::Button("Second", { 80, 20 })) DIALOG->OpenDialog("Second", "Second Material", ".mat", ".");
		if (DIALOG->Display("Second")) {
			if (DIALOG->IsOk()) {
				string file = DIALOG->GetFilePathName();
				file = file.substr(projectPath.length() + 1, file.length());
				Material* data = new Material();
				data->LoadMaterial(file);
				secondDiffuseMap  = Texture::Add(data->GetDiffusePath ());
				secondSpecularMap = Texture::Add(data->GetSpecularPath());
				secondNormalMap   = Texture::Add(data->GetNormalPath  ());
				delete data;
			}
			DIALOG->Close();
		}
		ImGui::SameLine();
		if (ImGui::Button("Third", { 80, 20 })) DIALOG->OpenDialog("Third", "Third Material", ".mat", ".");
		if (DIALOG->Display("Third")) {
			if (DIALOG->IsOk()) {
				string file = DIALOG->GetFilePathName();
				file = file.substr(projectPath.length() + 1, file.length());
				Material* data = new Material();
				data->LoadMaterial(file);
				thirdDiffuseMap  = Texture::Add(data->GetDiffusePath());
				thirdSpecularMap = Texture::Add(data->GetSpecularPath());
				thirdNormalMap   = Texture::Add(data->GetNormalPath());
				delete data;
			}
			DIALOG->Close();
		}
		ImGui::SameLine();
		SaveAlphaDialog();

		EditorGUI::ImageButton_Dialog_LoadTexture("SecondDiffuse", "Diffuse.2", secondDiffuseMap);
		ImGui::SameLine();
		EditorGUI::ImageButton_Dialog_LoadTexture("ThirdDiffuse", "Diffuse.3", thirdDiffuseMap);
		ImGui::SameLine();
		LoadAlphaDialog();

		EditorGUI::ImageButton_Dialog_LoadTexture("SecondSpecular", "Specular.2", secondSpecularMap);
		ImGui::SameLine();
		EditorGUI::ImageButton_Dialog_LoadTexture("ThirdSpecular", "Specular.3", thirdSpecularMap);

		EditorGUI::ImageButton_Dialog_LoadTexture("SecondNormal", "Normal.2", secondNormalMap);
		ImGui::SameLine();
		EditorGUI::ImageButton_Dialog_LoadTexture("ThirdNormal", "Normal.3", thirdNormalMap);
		break;
	}
	case EditType::OBJECT: {
		if (ImGui::BeginCombo("Objects", selectedObject.c_str()))
		{
			for (int n = 0; n < objectList.size(); n++) {
				bool is_selected = (selectedObject.compare(objectList[n]) == 0);
				if (ImGui::Selectable(objectList[n].c_str(), is_selected))
					selectedObject = objectList[n];
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("+", { 20, 20 }))
			DIALOG->OpenDialog("LoadObject", "Load Object", ".fbx", ".");

		if (DIALOG->Display("LoadObject")) {
			if (DIALOG->IsOk()) {
				string name = DIALOG->GetFilePathName();
				size_t const x = name.find_last_of('\\') + sizeof(char);
				size_t const y = name.find_last_of('.');
				name = name.substr(x, y - x);

				if (objects.count(name) == 0) { // 해당 키값이 없을 경우
					objects[name] = new ObjectList(name);
					selectedObject = name;
					objectList.emplace_back(name);
					selectedObject_idx = static_cast<int>(objectList.size()) - 1;
				}
			}
			DIALOG->Close();
		}
		if (objects.count(selectedObject) != 0) {
			ImGui::DragFloat3("Rotation_Override", 
				(float*)&objects[selectedObject]->RotationOverride, 0.01f);
			ImGui::DragFloat3("Scale_Override", 
				(float*)&objects[selectedObject]->ScaleOverride, 0.01f);
		}

		break;
	}
	default: break;
	}

	material->GUIRender();

	LoadTerrainDialog(); ImGui::SameLine(); SaveTerrainDialog();
}

// HeightMap Save/Lode
void TerrainEditor::LoadHeight(string file) {
	heightMap = Texture::Add(ToWString(file));
	SAFE_DELETE(mesh);

	CreateMesh();

	SAFE_DELETE(structuredBuffer);
	SAFE_DELETE(rayBuffer		);
	delete[] input;
	delete[] output;

	CreateComputeData();
}
void TerrainEditor::SaveHeight(string file) {
	UINT size = Size.x * Size.y * 4;
	uint8_t* pixels = new uint8_t[size];

	for (UINT i = 0; i < size / 4; i++) {
		float y = vertices[i].pos.y;
		uint8_t height = static_cast<uint8_t>(y / MAX_HEIGHT * 255);

		pixels[i * 4 + 0] = height;
		pixels[i * 4 + 1] = height;
		pixels[i * 4 + 2] = height;
		pixels[i * 4 + 3] = 255;
	}

	Image img;
	img.width = Size.x;
	img.height = Size.y;
	img.pixels = pixels;
	img.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	img.rowPitch = Size.x * 4;
	img.slicePitch = size;

	SaveToWICFile(img, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());
	heightMap = Texture::Add(ToWString(file));
}
void TerrainEditor::LoadHeightDialog() {
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
void TerrainEditor::SaveHeightDialog() {
	if (ImGui::Button("Save Height", { 80, 20 })) 
		DIALOG->OpenDialog("SaveHeight", "Save Height Map", ".png", ".");
	
	if (DIALOG->Display("SaveHeight")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			SaveHeight(file);
		}
		DIALOG->Close();
	}
}

// AlphaMap Save/Lode
void TerrainEditor::LoadAlpha(string file) {
	alphaMap = Texture::Add(ToWString(file));
	vector<float4> pixels;
	alphaMap->ReadPixels(pixels);
	
	for (UINT i = 0; i < vertices.size(); i++) {
		vertices[i].alpha[0] = pixels[i].z;
		vertices[i].alpha[1] = pixels[i].y;
		vertices[i].alpha[2] = pixels[i].x;
		vertices[i].alpha[3] = 1.0f;
	}
	mesh->UpdateVertex(vertices.data(), static_cast<UINT>(vertices.size()));
}
void TerrainEditor::SaveAlpha(string file) {
	size_t size = static_cast<size_t>(Size.x * Size.y * 4);
	uint8_t* pixels = new uint8_t[size];
	
	for (size_t i = 0, len = size / 4; i < len; i++) {
		pixels[i * 4 + 0] = static_cast<uint8_t>(vertices[i].alpha[0] * 255);
		pixels[i * 4 + 1] = static_cast<uint8_t>(vertices[i].alpha[1] * 255);
		pixels[i * 4 + 2] = static_cast<uint8_t>(vertices[i].alpha[2] * 255);
		pixels[i * 4 + 3] = 255;
	}
	Image img;
	img.width  = static_cast<size_t>(Size.x);
	img.height = static_cast<size_t>(Size.y);
	img.pixels = pixels;
	img.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	img.rowPitch = static_cast<size_t>(Size.x * 4);
	img.slicePitch = size;

	SaveToWICFile(img, WIC_FLAGS_FORCE_RGB, GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str()); 
	alphaMap = Texture::Add(ToWString(file));
}
void TerrainEditor::LoadAlphaDialog() {
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
void TerrainEditor::SaveAlphaDialog() {
	if (ImGui::Button("Save Alpha", { 80, 20 })) 
		DIALOG->OpenDialog("SaveAlpha", "Save Alpha Map", ".png", ".");
	
	if (DIALOG->Display("SaveAlpha")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			SaveAlpha(file);
		}
		DIALOG->Close();
	}
}

// Terrain Save/Lode
void TerrainEditor::LoadTerrain(string file) {
	BinaryReader* r = new BinaryReader(file);

	LoadHeight(r->String());
	LoadAlpha(r->String());
	material->LoadMaterial(r->String());
	material->SetShader(L"Shaders/Landscape/TerrainEditor.hlsl");

	secondDiffuseMap  = Texture::Add(r->WString());  // Second
	secondSpecularMap = Texture::Add(r->WString());
	secondNormalMap   = Texture::Add(r->WString());
	thirdDiffuseMap  = Texture::Add(r->WString()); // Third
	thirdSpecularMap = Texture::Add(r->WString());
	thirdNormalMap   = Texture::Add(r->WString());

	// objects
	objectList.resize(r->UInt()); // 오브젝트 종류 갯수 받기
	for (int i = 0; i < objectList.size(); i++) {
		string name = r->String();
		objectList[i] = name;
		objects[name] = new ObjectList(name);

		vector<Model*>& data = objects[name]->list;
		data.resize(r->UInt());
		for (int j = 0; j < data.size(); j++) {
			data[j] = new Model(name);
			data[j]->Position() = r->Vector();
			data[j]->Rotation() = r->Vector();
			data[j]->Scale   () = r->Vector();
		}
	}
	delete r;
}
void TerrainEditor::SaveTerrain(string file) {
	BinaryWriter* w = new BinaryWriter(file);
	{ // height
		string height_path = file.substr(0, file.find_last_of('.')) + "_height.png";
		SaveHeight(height_path);
		w->String(height_path);
	}
	
	{ // alpha
		string alpha_path = file.substr(0, file.find_last_of('.')) + "_alpha.png";
		SaveAlpha(alpha_path);
		w->String(alpha_path);
	}
	
	{ // material
		string material_path = file.substr(0, file.find_last_of('.')) + "_material.mat";
		material->SaveMaterial(material_path);
		w->String(material_path);
	}
	
	w->WString((secondDiffuseMap ) ? secondDiffuseMap ->GetPath() : L""); // 각 텍스쳐 path
	w->WString((secondSpecularMap) ? secondSpecularMap->GetPath() : L""); 
	w->WString((secondNormalMap  ) ? secondNormalMap  ->GetPath() : L""); 
	w->WString((thirdDiffuseMap  ) ? thirdDiffuseMap  ->GetPath() : L"");  // Third
	w->WString((thirdSpecularMap ) ? thirdSpecularMap ->GetPath() : L"");
	w->WString((thirdNormalMap   ) ? thirdNormalMap   ->GetPath() : L"");

	// objects
	w->UInt(static_cast<UINT>(objects.size())); // 오브젝트 종류 갯수
	for (int i = 0; i < objectList.size(); i++) {
		w->String(objectList[i]);

		vector<Model*>& data = objects[objectList[i]]->list;
		w->UInt(static_cast<UINT>(data.size()));
		for (int j = 0; j < data.size(); j++) {
			w->Vector(data[j]->Position());
			w->Vector(data[j]->Rotation());
			w->Vector(data[j]->Scale   ());
		}
	}

	delete w;
}
void TerrainEditor::LoadTerrainDialog() {
	if (ImGui::Button("Load Terrain", { 160, 30 }))
		DIALOG->OpenDialog("LoadTerrain", "Load Terrain", ".map", ".");
	if (DIALOG->Display("LoadTerrain")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			LoadTerrain(file);
		}
		DIALOG->Close();
	}
}
void TerrainEditor::SaveTerrainDialog() {
	if (ImGui::Button("Save Terrain", { 160, 30 }))
		DIALOG->OpenDialog("SaveTerrain", "Save Terrain", ".map", ".");
	if (DIALOG->Display("SaveTerrain")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			file = file.substr(projectPath.length() + 1, file.length());
			SaveTerrain(file);
		}
		DIALOG->Close();
	}
}

TerrainEditor::ObjectList::ObjectList(string name)
	: RotationOverride (0, 0, 0), ScaleOverride(1, 1, 1) { 
}
TerrainEditor::ObjectList::~ObjectList() { 
	for (Model*& elem : list) { SAFE_DELETE(elem); }
	list.clear();
}
