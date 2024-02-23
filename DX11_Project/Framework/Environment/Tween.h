#pragma once


class Tween {
private:
	float playTime = 0.0f;
	map<float, Event> events;
	map<float, Event>::iterator eventIter;

	bool isPaused = false;
public:
	void registerMethod();
	void registerProperty();

	Tween();
	~Tween();

	void Update();

	void Start();
	void Pause();
	void Resume();
	void Stop();
	void Finish();
};