#pragma once

class Node : public BoxCollider{
private:
	friend class AStar;
	enum State { NONE, OPEN, CLOSE, USING, OBSTACLE } state = NONE;
	struct Edge { 
		int idx; 
		float cost;
	};
	int idx;
	int via = -1;
	float f, g, h;
	vector<Edge*> edges;
public:
	Node(Vector3 pos, int idx);
	~Node();

	void Render();
	void AddEdge(Node* node);
	void SetState(State state) { this->state = state; }
	void SetObstacle(bool WillObstacle = true) { this->state = (WillObstacle)? OBSTACLE : NONE; }
	float GetCost() { return f; }
	bool IsObstacle() { return state == OBSTACLE; }
};