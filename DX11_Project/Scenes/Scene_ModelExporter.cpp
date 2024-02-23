#include "Framework.h"
#include "Scene_ModelExporter.h"

Scene_ModelExporter::Scene_ModelExporter() {
	string name = "PotGirl";
	string file = "";
	ModelExpoter* exporter;
	// Model
	file = "Models/FBX/" + name + ".fbx";
	exporter = new ModelExpoter(name, file);
	exporter->ExportMaterial();
	exporter->ExportMesh();
	delete exporter;
	
	// Models
	//string names[1] = {
	//	"DefenceTower"
	//};
	//for (int i = 0; i < 1; i++) {
	//	file = "Models/FBX/" + names[i] + ".fbx";
	//	exporter = new ModelExpoter(names[i], file);
	//	exporter->ExportMaterial();
	//	exporter->ExportMesh();
	//	delete exporter;
	//}
	
	// Anims
	//name = "PotMan";
	//string Clips[1] = {
	//	"StandingWalkRight"
	//};
	////name = "PotMan";
	//for (int i = 0; i < 1; i++) {
	//	file = "Models/Animations/" + name + "/" + Clips[i] + ".fbx";
	//	exporter = new ModelExpoter(name, file);
	//	exporter->ExportClip(Clips[i]);
	//	delete exporter;
	//}
}
Scene_ModelExporter::~Scene_ModelExporter() {
}

void Scene_ModelExporter::Update() {
}
void Scene_ModelExporter::PreRender() {
}
void Scene_ModelExporter::Render() {
}
void Scene_ModelExporter::PostRender() {
}
void Scene_ModelExporter::GUIRender() {
}
