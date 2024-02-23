#include "Framework.h"

void Sprite::Create() {
    vertices = new VertexSize();
    vBuffer = new VertexBuffer(vertices, sizeof(VertexSize), particleCount);
}

Sprite::Sprite(wstring file, float2 maxFrame, float2 size, bool isAdditive) : size(size) {
    particleCount = 1;

    material->SetShader(L"Shaders/Geometry/Sprite.hlsl");
    material->SetDiffuseMap(file);

    gShader = Shader::AddGS(L"Shaders/Geometry/Sprite.hlsl");

    spriteBuffer = new SpriteBuffer();
    spriteBuffer->data.maxFrame = maxFrame;
    maxFrameCount = (UINT)(maxFrame.x * maxFrame.y);

    SetAdditive(isAdditive);

    Create();
}
Sprite::~Sprite() {
    SAFE_DELETE(spriteBuffer);
    delete [] vertices;
}

void Sprite::Update() {
    if (!Active()) return;

    time += speed * DELTA;

    if (time > 0.1f) {
        curFrameCount++;
        spriteBuffer->data.curFrame.x = curFrameCount % (UINT)spriteBuffer->data.maxFrame.x;
        spriteBuffer->data.curFrame.y = curFrameCount / (UINT)spriteBuffer->data.maxFrame.x;
        time = Calc::Repeat(time, 0.0f, 0.1f);
    }

    if (curFrameCount >= maxFrameCount) Stop();
}

void Sprite::Render() {
    spriteBuffer->SetPSBuffer(10);
    Particle::Render();
}
void Sprite::GUIRender() {
    ImGui::DragFloat("Speed", &speed, 0.1f);
    ImGui::DragFloat2("Size", (float*)&size, 0.1f);
}

void Sprite::Play(Vector3 position) {
    SetActive(true);

    time = 0.0f;
    curFrameCount = 0;

    vertices->pos = position;
    vertices->size = size;

    vBuffer->Update(vertices, particleCount);
}

void Sprite::SetAdditive(bool isAdditive) {
    if (isAdditive) blendState[1]->Additive();
    else blendState[1]->Alpha(true);
}
