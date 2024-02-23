#pragma once

class Heap {
private:
	vector<Node*> heap; // tree
public:
	Heap();
	~Heap();

	void Insert(Node* node);
	void UpdateUpper(int idx);

	Node* DeleteRoot();
	void UpdateLower(int idx);

	void Swap(int n1, int n2);
	void Clear();

	bool Empty() { return heap.empty(); }
};