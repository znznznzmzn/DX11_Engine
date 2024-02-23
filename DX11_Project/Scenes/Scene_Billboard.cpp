#include "Framework.h"
#include "Scene_Billboard.h"

Scene_Billboard::Scene_Billboard() {
	t = new Terrain();
	t->UpdateWorld();
	t->setModels(false);
	//trees.resize(COUNT);
	//for (Quad*& elem : trees) {
	//	elem = new Quad();
	//	elem->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Tree.png");
	//	elem->GetMaterial()->SetShader(L"Shaders/Basic/Texture.hlsl");
	//	elem->Position().x = Random::Range(0.0f, (float)t->getSize().x - 1);
	//	elem->Position().z = Random::Range(0.0f, (float)t->getSize().y - 1);
	//	elem->Scale().x = Random::Range(1.0f, 3.0f);
	//	elem->Scale().y = Random::Range(0.5f, 4.0f);
	//	elem->Position().y = t->getHeight(elem->Position()) + elem->Scale().y / 2.0f;
	//	elem->UpdateWorld();
	//}
	vector<VertexSize> vertices;
	vertices.resize(COUNT);
	for (VertexSize& vt : vertices) {
		Vector3 pos;
		pos.x = Random::Range(0.0f, (float)t->getSize().x - 1);
		pos.z = Random::Range(0.0f, (float)t->getSize().y - 1);
		float2 scl;
		scl.x = Random::Range(1.0f, 3.0f);
		scl.y = Random::Range(1.0f, 5.0f);
		pos.y = t->getHeight(pos) + scl.y / 2.0f;

		vt.pos = pos;
		vt.size = scl;
	}
	vBuffer = new VertexBuffer(vertices.data(), sizeof(VertexSize), COUNT);
	vertices.clear();

	mat = new Material(L"Shaders/Geometry/BillboardShader.hlsl");
	mat->SetDiffuseMap(L"Textures/Landscape/Tree.png");

	gShader = new GeometryShader(L"Shaders/Geometry/BillboardShader.hlsl");

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}
Scene_Billboard::~Scene_Billboard() {
	SAFE_DELETE(t);
	SAFE_DELETE(depthState[0]);
	SAFE_DELETE(depthState[1]);
	SAFE_DELETE(vBuffer);
	SAFE_DELETE(gShader);
	SAFE_DELETE(mat);
	//for (Quad*& elem : trees) { SAFE_DELETE(elem); }
}

void Scene_Billboard::Update() {
	//for (Quad*& elem : trees) {
	//	elem->Rotation().y = CAMERA->Rotation().y;
	//	elem->Rotation().x = CAMERA->Rotation().x;
	//	//Vector3 dir = elem->Position() - CAMERA->Position();
	//	//elem->Rotation().y = atan2f(dir.x, dir.y);
	//	elem->UpdateWorld();
	//}
}

void Scene_Billboard::PreRender() {
}
void Scene_Billboard::Render() {
	t->Render();

	depthState[1]->SetState();
	//for (Quad*& elem : trees) elem->Render();
	vBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	mat->Set();
	gShader->Set();
	DC->Draw(COUNT, 0);
	DC->GSSetShader(nullptr, nullptr, 0);
	depthState[0]->SetState();
}
void Scene_Billboard::PostRender() {
}

void Scene_Billboard::GUIRender() {
}
