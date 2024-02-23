#pragma once

class Vector3 {
private: Vector4 data;
public:
	Vector3(float3 value) { data = XMLoadFloat3(&value); }
	Vector3(float4 value) { data = XMLoadFloat4(&value); }
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) { data = XMVectorSet(x, y, z, 0); }
	Vector3(Vector4 value) : data(value) { }
	operator Vector4() { return data; }
	operator float3() {
		float3 result;
		XMStoreFloat3(&result, data);
		return result;
	}
	void SetX(const float& value) { data = XMVectorSetX(data, value); }
	void SetY(const float& value) { data = XMVectorSetY(data, value); }
	void SetZ(const float& value) { data = XMVectorSetZ(data, value); }
	float GetX() { return XMVectorGetX(data); }
	float GetY() { return XMVectorGetY(data); }
	float GetZ() { return XMVectorGetZ(data); }

	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	Vector3 operator+(const Vector3& value) const { return data + value.data; }
	Vector3 operator-(const Vector3& value) const { return data - value.data; }

	void operator+=(const Vector3& value) { data += value.data; }
	void operator-=(const Vector3& value) { data -= value.data; }

	void operator*=(const float& value) { data *= value; }
	void operator/=(const float& value) { data /= value; }


	Vector3 operator*(const float& value) { return data * value; }
	Vector3 operator/(const float& value) { return data / value; }

	Vector3 operator* (const Matrix& value) const { return XMVector3TransformNormal(data, value); }
	void    operator*=(const Matrix& value) { data = XMVector3TransformNormal(data, value); }

	bool operator==(const Vector3& value) { return XMVector3Equal(data, value.data); }

	friend Vector3 operator+(const float3& value1, const Vector3& value2) { return value2 + value1; }
	friend Vector3 operator-(const float3& value1, const Vector3& value2) { return value2 - value1; }

	friend Vector3 operator*(const float& value1, const Vector3& value2) { return value2.data * value1; }
	friend Vector3 operator/(const float& value1, const Vector3& value2) { return value2.data / value1; }

	friend void operator+=(float3& value1, const Vector3& value2) { XMStoreFloat3(&value1, XMLoadFloat3(&value1) + value2.data); }
	friend void operator-=(float3& value1, const Vector3& value2) { XMStoreFloat3(&value1, XMLoadFloat3(&value1) - value2.data); }

	float operator[](const UINT& idx) const {
		switch (idx) {
		case 0: return XMVectorGetX(data);
		case 1: return XMVectorGetY(data);
		case 2: return XMVectorGetZ(data);
		default: assert(false);
		}
	}

	float length() { return XMVectorGetX(XMVector3Length(data)); }
	float magnitude() { return sqrt(x * x + y * y + z * z); }
	Vector3 normalize() { return XMVector3Normalize(data); }
	Vector4* GetData() { return &data; }

	static Vector3 lerp(const Vector3& from, const Vector3& to, const float& speed) { return from + (to - from) * speed; }
	static Vector3 cross(const Vector3& vec1, const Vector3& vec2) { return XMVector3Cross(vec1.data, vec2.data); }
	static float dot(const Vector3& vec1, const Vector3 & vec2) { return XMVectorGetX(XMVector3Dot(vec1.data, vec2.data)); }
	static float distance(const Vector3& from, const Vector3& to) { return (from - to).magnitude(); }
	static float distanceXZ(Vector3 from, Vector3 to) { from.y = 0; to.y = 0; return (from - to).magnitude(); }

	static Vector3 zero   () { return Vector3(0, 0, 0); }
	static Vector3 one    () { return Vector3(1, 1, 1); }
	static Vector3 left   () { return Vector3(-1,  0,  0); }
	static Vector3 right  () { return Vector3(+1,  0,  0); }
	static Vector3 up     () { return Vector3( 0, +1,  0); }
	static Vector3 down   () { return Vector3( 0, -1,  0); }
	static Vector3 forward() { return Vector3( 0,  0, +1); }
	static Vector3 back   () { return Vector3( 0,  0, -1); }
};