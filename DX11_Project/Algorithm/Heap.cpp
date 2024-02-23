#include "Framework.h"

Heap::Heap() {
}
Heap::~Heap() {
}

void Heap::Insert(Node* node) {
    heap.push_back(node);
    UpdateUpper(heap.size() - 1);
}

void Heap::UpdateUpper(int idx) {
    int curIdx = idx;
    int parent = static_cast<int>(floor((curIdx - 1) / 2.0f));
    while (curIdx != 0) {
        if (heap[parent]->GetCost() <= heap[curIdx]->GetCost()) break;
        Swap(curIdx, parent);
        curIdx = parent;
        parent = (curIdx - 1);
    }
}

Node* Heap::DeleteRoot() {
    Node* root = heap.front();
    Swap(0, heap.size() - 1);
    heap.pop_back();
    UpdateLower(0);
    return root;
}

void Heap::UpdateLower(int idx) {
    int curIdx = idx;
    int lChild = curIdx * 2 + 1;
    int rChild = lChild + 1;
    int minNode = curIdx;
    while (true) {
        if (lChild < heap.size() && heap[lChild]->GetCost() < heap[minNode]->GetCost()) minNode = lChild;
        if (rChild < heap.size() && heap[rChild]->GetCost() < heap[minNode]->GetCost()) minNode = rChild;

        if (minNode == curIdx) break;

        Swap(curIdx, minNode);
        curIdx = minNode;
        lChild = curIdx * 2 + 1; 
        rChild = lChild + 1;
    }
}

void Heap::Swap(int n1, int n2) {
    swap(heap[n1], heap[n2]);
}

void Heap::Clear() {
    heap.clear();
}
