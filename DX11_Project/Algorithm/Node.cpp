#include "Framework.h"

Node::Node(Vector3 pos, int idx) : idx(idx) {
	Position() = pos;
	UpdateWorld();
}

Node::~Node() {
	for (Edge*& elem : edges) { SAFE_DELETE(elem); }
}

void Node::Render() {
	switch (state) { 
	case Node::NONE: SetColor(1, 1, 1); break;
	case Node::OPEN: SetColor(0, 1, 0); break;
	case Node::CLOSE:SetColor(0, 0, 0); break;
	case Node::USING: SetColor(0, 0, 1); break;
	case Node::OBSTACLE: SetColor(1, 0, 0); break;
	default: break;
	}
	Collider::Render();
}

void Node::AddEdge(Node* node) {
	Edge* edge = new Edge();
	edge->idx = node->idx;
	edge->cost = Vector3::distanceXZ(node->Position(), Position());
	edges.push_back(edge);
}
