#include "Framework.h"

AStar::AStar(UINT width, UINT height) : Size(width, height) {
	heap = new Heap();
}
AStar::~AStar() {
	for (Node*& elem : nodes) { SAFE_DELETE(elem); }
	SAFE_DELETE(heap);
}

void AStar::Update() {
	if (MOUSE_CLICK(0)) {
		Ray ray = CAMERA->ScreenPointToRay(mousePos);
		for (Node*& elem : nodes) {
			if (elem->RayCollision(ray)) {
				elem->SetState(Node::OBSTACLE);
				break;
			}
		}
	}
}
void AStar::Render() {
	if(toggle_render)
		for (Node*& elem : nodes) elem->Render();
}
void AStar::GUIRender() { ImGui::Checkbox("Toggle Astar Visible", &toggle_render); }


void AStar::Reset() {
	for (Node*& elem : nodes) {
		if (elem->state != Node::OBSTACLE) elem->SetState(Node::NONE);
	}
	heap->Clear();
}
float AStar::GetManhattanDistance(int start, int end) {
	Vector3 startPos = nodes[start]->Position();
	Vector3 endPos = nodes[end]->Position();
	Vector3 tmp = startPos - endPos;
	float x = abs(tmp.x);
	float z = abs(tmp.z);

	float minSize = min(x, z);
	float maxSize = max(x, z);

	return (maxSize - minSize) + minSize * sqrt(2);
}
float AStar::GetDiagonalManhattanDistance(int start, int end) {
	Vector3 startPos = nodes[start]->Position();
	Vector3 endPos = nodes[end]->Position();
	Vector3 tmp = startPos - endPos;
	return abs(tmp.x) + abs(tmp.z);
}
int AStar::GetMinNode() {
	return heap->DeleteRoot()->idx;
}
void AStar::Extend(int select, int end) {
	vector<Node::Edge*> edges = nodes[select]->edges;
	for (Node::Edge* edge : edges) {
		int idx = edge->idx;
		if (nodes[idx]->state == Node::CLOSE ||
			nodes[idx]->state == Node::OBSTACLE) continue;
		float G = nodes[select]->g + edge->cost;
		float H = GetDiagonalManhattanDistance(idx, end);
		float F = G + H;
		if (nodes[idx]->state == Node::OPEN) {
			if (F < nodes[idx]->GetCost()) {
				nodes[idx]->g = G;
				nodes[idx]->f = F;
				nodes[idx]->via = select;
			}
		}
		else if (nodes[idx]->state == Node::NONE) {
			nodes[idx]->g = G;
			nodes[idx]->h = H;
			nodes[idx]->f = F;
			nodes[idx]->via = select;
			nodes[idx]->state = Node::OPEN;
			heap->Insert(nodes[idx]);
		}
	}
}
void AStar::SetEdge() {
	for (UINT i = 0; i < nodes.size(); i++) {
		if (i % Size.x != Size.x - 1) {
			nodes[i]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i]);
		}
		if (i < nodes.size() - Size.x) {
			nodes[i]->AddEdge(nodes[i + Size.x]);
			nodes[i + Size.x]->AddEdge(nodes[i]);
		}
		//우상단
		if ((i % Size.x != Size.x - 1) && (i < nodes.size() - Size.x)) {
			nodes[i]->AddEdge(nodes[i + Size.x + 1]);
			nodes[i + Size.x + 1]->AddEdge(nodes[i]);
		}
		//좌상단
		if ((i % Size.x != 0) && (i < nodes.size() - Size.x)) {
			nodes[i]->AddEdge(nodes[i + Size.x - 1]);
			nodes[i + Size.x - 1]->AddEdge(nodes[i]);
		}
	}
}

void AStar::CreateNode(Terrain* t) {
	Point size = t->getSize();

	interval.x = size.x / this->Size.x;
	interval.y = size.y / this->Size.y;

	for (UINT z = 0; z < Size.y; z++) {
		for (UINT x = 0; x < Size.x; x++) {
			Vector3 pos = { x * interval.x, 0.0f, z * interval.y };
			pos.y = t->getHeight(pos);
			Node* elem = new Node(pos, nodes.size());
			if (pos.y > 3.0f) //- 하드코딩부분
				elem->SetState(Node::OBSTACLE);
			nodes.push_back(elem);
		}
	}
	SetEdge();
}
int AStar::FindCloseNode(Vector3 pos) {
	float minDist = FLT_MAX;
	int idx = -1;
	for (UINT i = 0; i < nodes.size(); i++) {
		if (nodes[i]->state == Node::OBSTACLE) continue;
		float dist = Vector3::distanceXZ(pos, nodes[i]->Position());
		if (minDist > dist) {
			minDist = dist;
			idx = i;
		}
	}
	return idx;
}
void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path) {
	Reset();

	// 1. 시작노드 초기화
	float G = 0;
	float H = GetDiagonalManhattanDistance(start, end);
	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->f = G + H;
	nodes[start]->via = start;
	nodes[start]->state = Node::OPEN;
	heap->Insert(nodes[start]);

	while (nodes[end]->state != Node::CLOSE) {
		if (heap->Empty()) return;
		int selectedIdx = GetMinNode(); // 2. 오픈노드 중 효율이 가장 좋은 노드 찾기
		Extend(selectedIdx, end); // 3. 찾은 노드의 연결된 노드를 오픈노드로 추가하고 정보갱신
		nodes[selectedIdx]->SetState(Node::CLOSE); // 4. 확장이 끝난노드 닫기
	}
	// 5. 백트래킹
	int curIdx = end;
	while (curIdx != start) {
		nodes[curIdx]->state = Node::USING;
		path.push_back(nodes[curIdx]->Position());
		curIdx = nodes[curIdx]->via;
	}
}
Node*& AStar::GetNode(Vector3 pos) {
	float minDist = FLT_MAX;
	int idx = -1;
	for (UINT i = 0; i < nodes.size(); i++) {
		float dist = Vector3::distanceXZ(pos, nodes[i]->Position());
		if (minDist > dist) {
			minDist = dist;
			idx = i;
		}
	}
	return nodes[idx];
}
