#pragma once

class ModelFSM : public ModelAnimator { // ����...
private:
	struct State {
		string name;
		UINT clip_idx; //���� state�� play��ų clip
		vector<UINT> connections; // ����� ���
		float transition_speed = 0; // ��ȯ�ӵ�
	};
	unordered_map<UINT, State>::iterator* state; // ���� state
	unordered_map<UINT, State> states;
	// �׸��� ������ ���, ���� state�� �ִϸ��̼��� ���
	// Ư�� ���ǿ� �����ϴ� state�� Play �Ǳ⸦ ���� �� ������, 
	// Ư�������� �����ڰ� ��ũ��Ʈ�� ¥�鼭 �޶��� �� ����
	
	State state_Start; // ���� State�� IDLE �� �����ϱ� ���� �ʱ� state
	State state_Interrupt; // �ƹ� ��Ȳ������ ��� �ٸ� state�� ���Խ�Ű�� ���� state
protected: // ���ϴ� ĳ���Ϳ� ��ӽ��Ѽ� ����ϴ°��� ����
	const string Start = "Start";
	const string Interrupt = "Interrupt";

	void AddState(string stateName, string clipName);
	void ConnectState(string nameFrom, string nameTo);
	void ConnectState(UINT fromIDX, UINT toIDX);
	UINT indexByName(string nameFrom);

	// Delete�� ����� ��, �̴ϼȶ��������� FSM�� ������ �����ִ°��� ����
public:
	ModelFSM(string name);
	~ModelFSM();

	void Update();
};