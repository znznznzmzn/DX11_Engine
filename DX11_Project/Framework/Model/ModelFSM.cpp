#include "Framework.h"

ModelFSM::ModelFSM(string name) : ModelAnimator(name) {
}

ModelFSM::~ModelFSM() {
}

void ModelFSM::Update() {
	ModelAnimator::Update();
}
