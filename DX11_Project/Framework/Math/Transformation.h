#pragma once

class Transformation { // Transform °£ÆíÈ­
public:
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
	Transformation(
		Vector3 pos = Vector3::zero(),
		Vector3 rot = Vector3::zero(),
		Vector3 scl = Vector3::one ()) {}
	~Transformation() = default;
	void Set(Transform*& value){
		Position = value->Position();
		Rotation = value->Position();
		Scale    = value->Scale   ();
	}
	void GUIRender() {
		if (ImGui::TreeNode("offset_Transform")) {
			ImGui::DragFloat3("offset_Pos", (float*)&Position, 0.1f);
			ImGui::DragFloat3("offset_Rot", (float*)&Rotation, 0.01f);
			ImGui::DragFloat3("offset_Scl", (float*)&Scale, 0.1f);
		}
	}
};