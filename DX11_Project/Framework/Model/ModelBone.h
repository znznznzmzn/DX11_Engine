#pragma once

class ModelBone : public Model {
private:
    class BoneBuffer : public ConstantBuffer {
    private: struct Data {  Matrix bones[512]; } data;
    public:
        BoneBuffer() : ConstantBuffer(&data, sizeof(Data)) 
        { for (int i = 0; i < 512; i++)  data.bones[i] = XMMatrixIdentity(); }
        void SetBone(Matrix matrix, UINT index)
        { data.bones[index] = XMMatrixTranspose(matrix); }
    };

public:
    ModelBone(string name);
    ~ModelBone();

    void Render();
private:
    void SetBones();

    BoneBuffer* boneBuffer;
    Matrix* nodeTransforms;
};