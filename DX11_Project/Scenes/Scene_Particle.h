#pragma once

class Scene_Particle : public Scene {
private:
	BoxCollider* collider;
	Rain* particle;
public:
	Scene_Particle();
	~Scene_Particle();


	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void GUIRender() override;

};