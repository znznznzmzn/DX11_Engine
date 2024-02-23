#pragma once

class Rain : public Particle {
private:
    class RainBuffer : public ConstantBuffer{
    public:
        struct Data {
            float3 velocity = float3(0.0f, -1.0f, 0.0f);
            float distance = 1000.0f;

            float4 rainColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

            float3 origin = float3(0.0f, 0.0f, 0.0f);
            float time = 0.0f;

            float3 size = float3(50, 50, 50);
            float padding = 0.0f;
        } data;
        RainBuffer() : ConstantBuffer(&data, sizeof(Data))  { }
    } * rainBuffer;
    VertexSize* vertices;

    float minSize = 0.1f;
    float maxSize = 1.0f;

    void Create();
public:
    Rain();
    ~Rain();

    virtual void Update() override;
    virtual void Render() override;
    virtual void GUIRender() override;
};