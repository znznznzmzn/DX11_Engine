#pragma once

class Transform {
private:
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;
	Vector3 globalPosition;
	Vector3 globalRotation;
	Vector3 globalScale;
protected:
	Vector3 pivot;
	Matrix  world;
	Transform* parentTransform = nullptr;
	Matrix* parent = nullptr;

	string tag = "Unknown";
	bool is_active = true;
public:
	Transform();
	~Transform();

	virtual void UpdateWorld();

	void NullParent() {
		parentTransform = nullptr;
		parent = nullptr;
	}
	void SetParent(Transform* transform) {
		parentTransform = transform;
		parent = &transform->GetWorld();
	}
	void SetParent(Matrix* matrix) { parent = matrix; }
	Transform* GetParent() { return parentTransform; }

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	//Transform& LookAt(Vector3& target);

	Vector3 GlobalPosition() { return globalPosition; }
	Vector3 GlobalRotation() { return globalRotation; }
	Vector3 GlobalScale()    { return globalScale; }

	Matrix& GetWorld() { return world; }

	Vector3& Position() { return localPosition; }
	Vector3& Rotation() { return localRotation; }
	Vector3& Scale()	{ return localScale; }

	string& Tag() { return tag; }

	void SetActive(bool isActive) { this->is_active = isActive; }
	bool ActiveSelf() { return is_active; }
	bool Active();

	virtual void GUIRender();

	void Save();
	void Load();

	void SetTransformation(class Transformation const& value);
	void Lerp(Transform*& to, float const& speed);
	void Lerp(Transformation to, float const& speed);
};