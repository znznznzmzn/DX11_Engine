#pragma once
class Cylinder_Reversed : public GameObject {
protected:
	void setVertex(int slice);
public:
	Cylinder_Reversed(int slice = 21);
};