#pragma once

class Point {
public:
	int x, y;
	Point() : x(0), y(0) { }
	Point(int x, int y) : x(x), y(y) { }
	Point(UINT x, UINT y) : x(x), y(y) { }
	Point(float x, float y) : x(static_cast<int>(floor(x))), y(static_cast<int>(floor(y))) { }
	Point operator-(const float& value) const { return Point(x - value, y - value); }
	Point operator+(const float& value) const { return Point(x + value, y + value); }
};

class UPoint {
public:
	int x, y;
	UPoint() : x(0), y(0) { }
	UPoint(int x, int y) : x(x), y(y) { }
	UPoint(UINT x, UINT y) : x(x), y(y) { }
	UPoint(float x, float y) : x(static_cast<UINT>(floor(x))), y(static_cast<UINT>(floor(y))) { }
	UPoint operator-(const float& value) const { return UPoint(x - value, y - value); }
	UPoint operator+(const float& value) const { return UPoint(x + value, y + value); }
};

namespace Calc {
	float const PI = 3.141592f;
	float const PI_2 = PI * 2.0f;

	float const PI_to_Degree = 180.0f / PI;
	float const Degree_to_PI = PI / 180.0f;

	inline int   Clamp (int   const& value, int   const& min, int   const& max) // min이상 max이하 자르기
	{ return (value < min) ? min : ((value > max) ? max : value); }
	inline float Clamp (float const& value, float const& min, float const& max) 
	{ return (value < min) ? min : ((value > max) ? max : value); }

	inline Point Clamp (Point const& value, Point const& min, Point const& max)
	{ return Point(Clamp(value.x, min.x, max.x), Clamp(value.y, min.y, max.y)); }
	inline Point ClampIdx (Point const& value, Point const& max)
	{ return Point(Clamp(value.x, 0, max.x), Clamp(value.y, 0, max.y)); }

	inline int   Repeat(int   const& value, int   const& min, int   const& max) // 정수형 : min이상 max미만 반복
	{ return min + ((value - min) % (max - min) + (max - min)) % (max - min); } // [(5,0,3) -> 2, (3,0,3) -> 0]
	inline int   Repeat(UINT  const& value, UINT  const& min, UINT  const& max) // 
	{ return min + ((value - min) % (max - min) + (max - min)) % (max - min); }
	inline float Repeat(float const& value, float const& min, float const& max)
	{ return min + fmodf(fmodf(value - min, max - min) + (max - min), max - min); }

	inline float RepeatPI(float const& value) { return Repeat(value, -PI, PI); }
	inline Vector3 RepeatPI(Vector3 value)
	{ return Vector3(RepeatPI(value.x), RepeatPI(value.y), RepeatPI(value.z)); }

	inline int RepeatIdx(int   const& value, int   const& len) // Index : 0이상 len미만 반복
	{ return (value % len + len) % len; } 
	inline int RepeatIdx(UINT  const& value, UINT  const& len)  
	{ return (value % len + len) % len; }

	// 부드러운 이동, (from에 서 to까지 speed만큼만 이동)
	inline float const Lerp(float const& from, float const& to, float speed) { return from + (to - from) * speed; }

	// min이상 max이하 범위안에 있는가
	inline bool const isInLine(int const& point, int const& min, int const& max) { return point >= min && point <= max; }
	inline bool const isInBox(Point const& point, Point const& min, Point const& max) 
	{ return isInLine(point.x, min.x, max.x) && isInLine(point.y, min.y, max.y); }

	// 인덱스 간편화
	inline bool const isInIndex(int const& point, int const& N) { return 0 <= point && point < N; }
	inline bool const isInIndex(Point const& point, Point const& N) { return isInIndex(point.x, N.x) && isInIndex(point.y, N.y); }

	int Fibo(int cnt); // 피보나치

	namespace Easing{ // value 0 ~ 1 사이의 진척도
		float OutBounce(float value);
		float InBounce (float value);
		float OutBack  (float value);
		float InExpo   (float value);
		float OutExpo  (float value);
		
		// 0 ~ 1 ~ 0
		inline float Pow2(float value) { return -((value - 0.5f) * (value - 0.5f)) * 4.0f + 1.0f; }
	}

	namespace Vertex {
		void Normal(vector<VertexUVNormal>& vertices, vector<UINT> const& indices);
		void Normal(vector<VertexUVNormalTangent>& vertices, vector<UINT> const& indices);
		void Tangent(vector<VertexUVNormalTangent>& vertices, vector<UINT> const& indices);

		void Clear_Normal (vector<VertexUVNormal>& vertices);
		void Clear_Normal (vector<VertexUVNormalTangent>& vertices);
		void Clear_Tangent(vector<VertexUVNormalTangent>& vertices); 
		void Clear_NormalTangent(vector<VertexUVNormalTangent>& vertices);
	}

	Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);
}

namespace Random {
	int   Number();
	int   NumberTo(unsigned int to);
	float RealNumber();
	int   Range(int min, int max);
	float Range(float f_min, float f_max);
	float Length(float lenght);
	float PI();
}
