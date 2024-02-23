#include "Framework.h"

void Material::SelectShader() {
	if (ImGui::Button((name + "Shader").c_str()))
		DIALOG->OpenDialog(name + "Shader", "SelectShader", ".hlsl", ".");

	if (DIALOG->Display(name + "Shader")) {
		if (DIALOG->IsOk()) {
			string path = DIALOG->GetFilePathName();
			path = path.substr(projectPath.length() + 1, path.length());
			SetShader(ToWString(path));
		} DIALOG->Close();
	}
}

Material::Material() { mBuffer = new MaterialBuffer(); }

Material::Material(wstring shaderFile) {
	mBuffer = new MaterialBuffer();
	SetShader(shaderFile); 
}
Material::~Material() { SAFE_DELETE(mBuffer); }

void Material::Set() {
	mBuffer->SetPSBuffer(1);
	if (diffuseMap  != nullptr) diffuseMap ->PSSet(0);
	if (specularMap != nullptr) specularMap->PSSet(1);
	if (normalMap   != nullptr) normalMap  ->PSSet(2);
	vShader->Set();
	pShader->Set();
}

void Material::GUIRender() {
	if (ImGui::TreeNode((name + "_Material").c_str())) {
		ImGui::Indent(); 
		SelectShader(); ImGui::SameLine(); ImGui::LabelText("%s", ToString(vShader->GetFile()).c_str());
		char name_buf[128] = ""; 
		if (ImGui::InputText("Name", name_buf, IM_ARRAYSIZE(name_buf))) 
			if (KEY_DOWN(ImGuiKey_Enter)) name = string(name_buf);
		
		ImGui::ColorEdit4("Diffuse ", (float*)&mBuffer->data.diffuse );
		ImGui::ColorEdit4("Specular", (float*)&mBuffer->data.specular);
		ImGui::ColorEdit4("Ambient ", (float*)&mBuffer->data.ambient );
		ImGui::SliderFloat("Shiness", (float*)&mBuffer->data.shininess, 1.0f, 50.0f);

		ImGui::Checkbox("Is Diffuse   ", (bool*)&mBuffer->data.hasDiffuse);
		ImGui::SameLine();
		ImGui::Checkbox("Is Specular  ", (bool*)&mBuffer->data.hasSpecular);
		ImGui::SameLine();
		ImGui::Checkbox("Is Normal    ", (bool*)&mBuffer->data.hasNormal);

		{
			ImVec2 xbox = { 24, 24 };
			if(EditorGUI::ImageButton_Dialog_LoadTexture("LoadDiffuse" , "Diffuse" , diffuseMap))
				SetDiffuseMap(diffuseMap->GetPath());
			ImGui::SameLine();
			if (ImGui::Button("DX", xbox)) SetDiffuseMap(L"");
			ImGui::SameLine();
			if(EditorGUI::ImageButton_Dialog_LoadTexture("LoadSpecular", "Specular", specularMap))
				SetSpecularMap(specularMap->GetPath());
			ImGui::SameLine();
			if (ImGui::Button("SX", xbox)) SetSpecularMap(L"");
			ImGui::SameLine();
			if(EditorGUI::ImageButton_Dialog_LoadTexture("LoadNormal"  , "Normal"  , normalMap))
				SetNormalMap(normalMap->GetPath());
			ImGui::SameLine();
			if (ImGui::Button("NX", xbox)) SetNormalMap(L"");
		}

		if (ImGui::Button("Save Material", { 160, 25 })) SaveMaterial(this->file);
		ImGui::SameLine();
		ImGui::LabelText("%s", this->file.c_str());
		if (ImGui::Button("SaveMaterial At...", { 160, 25 }))
			DIALOG->OpenDialog("SaveMaterial", "Save Material As", ".mat", name);
		ImGui::SameLine();
		if (ImGui::Button("LoadMaterial From...", { 160, 25 }))
			DIALOG->OpenDialog("LoadMaterial", "Load Material As", ".mat", ".");

		if (DIALOG->Display("LoadMaterial")) {
			if (DIALOG->IsOk()) {
				string path = DIALOG->GetFilePathName();
				path = path.substr(projectPath.length() + 1, path.length());
				LoadMaterial(path);
			} DIALOG->Close();
		}
		if (DIALOG->Display("SaveMaterial")) {
			if (DIALOG->IsOk()) {
				string path = DIALOG->GetFilePathName();
				path = path.substr(projectPath.length() + 1, path.length());
				SaveMaterial(path);
			} DIALOG->Close();
		}

		ImGui::Unindent();
		ImGui::TreePop();
	}
} 
void Material::SaveMaterial(string path) {
	BinaryWriter* w = new BinaryWriter(path);
	if (w->IsFailed()) {
		delete w;
		assert(false);
		return;
	}
	w->String(name); // 이름

	w->WString((vShader) ? vShader->GetFile() : L"");
	w->WString((pShader) ? pShader->GetFile() : L"");

	w->Float4(mBuffer->data.diffuse );
	w->Float4(mBuffer->data.specular);
	w->Float4(mBuffer->data.ambient );

	w->Float(mBuffer->data.shininess);

	w->Int(mBuffer->data.hasDiffuse );
	w->Int(mBuffer->data.hasSpecular);
	w->Int(mBuffer->data.hasNormal  );

	w->WString((diffuseMap ) ? diffuseMap ->GetPath() : L""); // 각 텍스쳐 path
	w->WString((specularMap) ? specularMap->GetPath() : L""); 
	w->WString((normalMap  ) ? normalMap  ->GetPath() : L""); 

	delete w;

	this->file = path;
}

void Material::LoadMaterial(string path) {
	BinaryReader* r = new BinaryReader(path);
	if (r->IsFailed()) {
		delete r;
		assert(false);
		return;
	}
	name = r->String();

	vShader = Shader::AddVS(r->WString());
	pShader = Shader::AddPS(r->WString());

	mBuffer->data.diffuse  = r->Float4();
	mBuffer->data.specular = r->Float4();
	mBuffer->data.ambient  = r->Float4();

	mBuffer->data.shininess = r->Float();

	mBuffer->data.hasDiffuse  = r->Int();
	mBuffer->data.hasSpecular = r->Int();
	mBuffer->data.hasNormal   = r->Int();
	SetDiffuseMap (r->WString());
	SetSpecularMap(r->WString());
	SetNormalMap  (r->WString());
	delete r;

	this->file = path;
}

void Material::SetShader(wstring file) {
	vShader = Shader::AddVS(file);
	pShader = Shader::AddPS(file);
}

void Material::SetDiffuseMap (wstring file) {
	if (file.length() == 0) {
		diffuseMap = nullptr;
		mBuffer->data.hasDiffuse = false;
	} else {
		diffuseMap = Texture::Add(file);
		mBuffer->data.hasDiffuse = true;
	}
}
void Material::SetSpecularMap(wstring file) {
	if (file.length() == 0) {
		specularMap = nullptr;
		mBuffer->data.hasSpecular = false;
	} else {
		specularMap = Texture::Add(file);
		mBuffer->data.hasSpecular = true;
	}
}
void Material::SetNormalMap  (wstring file) {
	if (file.length() == 0) {
		normalMap = nullptr;
		mBuffer->data.hasNormal = false;
	} else {
		normalMap = Texture::Add(file);
		mBuffer->data.hasNormal = true;
	}
}



wstring Material::GetDiffusePath () {
	if (diffuseMap) return diffuseMap->GetPath();
	return wstring();
}
wstring Material::GetSpecularPath() {
	if (specularMap) return specularMap->GetPath();
	return wstring();
}
wstring Material::GetNormalPath  () {
	if (normalMap) return normalMap->GetPath();
	return wstring();
}
