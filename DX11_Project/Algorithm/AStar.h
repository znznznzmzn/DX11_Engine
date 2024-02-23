#pragma once

class AStar {
protected:
	Point Size = { 0, 0 };
	vector<Node*> nodes;
	Heap* heap;
	float2 interval = { 0.0f, 0.0f };
	bool toggle_render = false;

	void Reset();
	float GetManhattanDistance(int start, int end);
	float GetDiagonalManhattanDistance(int start, int end);

	int GetMinNode();
	void Extend(int select, int end);

	void SetEdge();

public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Update();
	void Render();
	void GUIRender();

	void CreateNode(class Terrain* t);

	int FindCloseNode(Vector3 pos);
	void GetPath(Vector3 start_pos, Vector3 end_pos, OUT vector<Vector3>& path)
	{ GetPath(FindCloseNode(start_pos), FindCloseNode(end_pos), path); }
	void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);
	Node*& GetNode(int idx) { return nodes[idx]; }
	Node*& GetNode(Vector3 pos);
	vector<Node*>& GetNodes() { return nodes; }
	void ToggleRender(bool isRender) { toggle_render = isRender; }
};