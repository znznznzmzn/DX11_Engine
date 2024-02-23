#include "Framework.h"

void Tween::registerMethod() {
}

void Tween::registerProperty() {
}

Tween::Tween() { }
Tween::~Tween() { }

void Tween::Update() {
	if (isPaused) return;
	if (events.empty() || eventIter == events.end()) return;
	playTime += DELTA;
	if (eventIter->first >= playTime) return;
	eventIter->second();
	eventIter++;
	if (eventIter == events.end()) events.clear();
}

void Tween::Start() {
	eventIter = events.begin();
	playTime = 0.0f;
}

void Tween::Pause()  { isPaused = true; }
void Tween::Resume() { isPaused = false; }
void Tween::Stop()   { events.clear(); }
void Tween::Finish() {
	playTime = FLT_MAX;
	for (auto elem : events) eventIter->second();
	events.clear();
}
