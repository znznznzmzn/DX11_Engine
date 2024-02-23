#pragma once

class ModelFSM : public ModelAnimator { // 뻘짓...
private:
	struct State {
		string name;
		UINT clip_idx; //현재 state가 play시킬 clip
		vector<UINT> connections; // 연결된 노드
		float transition_speed = 0; // 변환속도
	};
	unordered_map<UINT, State>::iterator* state; // 현재 state
	unordered_map<UINT, State> states;
	// 그리드 형식을 기반, 현재 state의 애니메이션을 재생
	// 특정 조건에 만족하는 state가 Play 되기를 원할 수 있지만, 
	// 특정조건은 제작자가 스크립트를 짜면서 달라질 수 있음
	
	State state_Start; // 시작 State로 IDLE 에 진입하기 전의 초기 state
	State state_Interrupt; // 아무 상황에서든 즉시 다른 state로 진입시키기 위한 state
protected: // 원하는 캐릭터에 상속시켜서 사용하는것을 지향
	const string Start = "Start";
	const string Interrupt = "Interrupt";

	void AddState(string stateName, string clipName);
	void ConnectState(string nameFrom, string nameTo);
	void ConnectState(UINT fromIDX, UINT toIDX);
	UINT indexByName(string nameFrom);

	// Delete가 없어야 함, 이니셜라이저에서 FSM의 구조가 잡혀있는것을 지향
public:
	ModelFSM(string name);
	~ModelFSM();

	void Update();
};