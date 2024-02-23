#pragma once

class Scene_Collision : public Scene {
private:
    vector<Collider*> colliders;
public:
    Scene_Collision();
    ~Scene_Collision();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};