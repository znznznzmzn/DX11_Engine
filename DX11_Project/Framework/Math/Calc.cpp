#include "Framework.h"

namespace Calc {
	const double s5 = sqrt(5);
	int Fibo(int cnt)
	{ return static_cast<int>((1 / s5) * (pow((1 + s5)/ 2, cnt) - pow((1 - s5) / 2, cnt))); }
	
	namespace Easing {
		const float n1 = 7.5625f;
		const float d1 = 2.75f;
		float OutBounce(float value) {
			if (value < 1.0f / d1) return n1 * value * value;
			else if (value < 2.0f / d1) return n1 * (value -= 1.5f / d1) * value + 0.75f;
			else if (value < 2.5f / d1) return n1 * (value -= 2.25f / d1) * value + 0.9375f;
			else return n1 * (value -= 2.625f / d1) * value + 0.984375f;
		}
		float InBounce(float value) { return 1 - OutBounce(1 - value); }
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		float OutBack(float value)
		{ return 1.0f + c3 * pow(value - 1.0f, 3.0f) + c1 * pow(value - 1.0f, 2.0f); }

		float InExpo(float value)
		{ return value = (value == 0.0f) ? 0.0f : pow(2.0f, 10.0f * value - 10.0f); }
		float OutExpo(float value) 
		{ return 1 - InExpo(1 - value); }
	}

	namespace Vertex {
		UINT idx1 = 0;
		UINT idx2 = 0;
		UINT idx3 = 0;
		void Normal(vector<VertexUVNormal>& vertices, vector<UINT> const& indices) {
			for (int i = 0; i < indices.size() / 3; i++) {
				idx1 = indices[i * 3 + 0];
				idx2 = indices[i * 3 + 1];
				idx3 = indices[i * 3 + 2];

				Vector3 normal = Vector3::cross(
					Vector3(vertices[idx2].pos) - vertices[idx1].pos,
					Vector3(vertices[idx3].pos) - vertices[idx1].pos).normalize();

				vertices[idx1].normal = normal + vertices[idx1].normal;
				vertices[idx2].normal = normal + vertices[idx2].normal;
				vertices[idx3].normal = normal + vertices[idx3].normal;
			}
		}
		void Normal(vector<VertexUVNormalTangent>& vertices, vector<UINT> const& indices) {
			for (int i = 0; i < indices.size() / 3; i++) {
				idx1 = indices[i * 3 + 0];
				idx2 = indices[i * 3 + 1];
				idx3 = indices[i * 3 + 2];

				Vector3 normal = Vector3::cross(
					Vector3(vertices[idx2].pos) - vertices[idx1].pos,
					Vector3(vertices[idx3].pos) - vertices[idx1].pos).normalize();

				vertices[idx1].normal = normal + vertices[idx1].normal;
				vertices[idx2].normal = normal + vertices[idx2].normal;
				vertices[idx3].normal = normal + vertices[idx3].normal;
			}
		}
		void Tangent(vector<VertexUVNormalTangent>& vertices, vector<UINT> const& indices) {
			for (UINT i = 0; i < indices.size() / 3; i++) {
				idx1 = indices[i * 3 + 0];
				idx2 = indices[i * 3 + 1];
				idx3 = indices[i * 3 + 2];

				Vector3 e0 = Vector3(vertices[idx2].pos) - vertices[idx1].pos;
				Vector3 e1 = Vector3(vertices[idx3].pos) - vertices[idx1].pos;

				float u0 = vertices[idx2].uv.x - vertices[idx1].uv.x;
				float v0 = vertices[idx2].uv.y - vertices[idx1].uv.y;
				float u1 = vertices[idx3].uv.x - vertices[idx1].uv.x;
				float v1 = vertices[idx3].uv.y - vertices[idx1].uv.y;

				float d = 1.0f / (u0 * v1 - v0 * u1);

				Vector3 tangent = d * (e0 * v1 - e1 * v0);

				vertices[idx1].tangent += tangent;
				vertices[idx2].tangent += tangent;
				vertices[idx3].tangent += tangent;
			}

			for (VertexUVNormalTangent& vertex : vertices) {
				Vector3 t = vertex.tangent;
				Vector3 n = vertex.normal;
				vertex.tangent = (t - n * Vector3::dot(n, t)).normalize();
			}
		}

		void Clear_Normal(vector<VertexUVNormal>& vertices)
		{ for (VertexUVNormal& elem : vertices) elem.normal = float3(0, 0, 0); }
		void Clear_Normal(vector<VertexUVNormalTangent>& vertices)
		{ for (VertexUVNormalTangent& elem : vertices) elem.normal = float3(0, 0, 0); }
		void Clear_Tangent(vector<VertexUVNormalTangent>& vertices)
		{ for (VertexUVNormalTangent& elem : vertices) elem.tangent = float3(0, 0, 0); }
		void Clear_NormalTangent(vector<VertexUVNormalTangent>& vertices)
		{ for (VertexUVNormalTangent& elem : vertices) elem.tangent = elem.normal = float3(0, 0, 0); }
	}

	Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point) {
		Vector3 line = end - start;
		return start + line * Clamp(
			Vector3::dot(line, point - start) / Vector3::dot(line, line), 0.0f, 1.0f);
	}
}

namespace Random {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(-2147483640, 2147483640);
	int Number() { return dist(gen); }
	int NumberTo(unsigned int to) { return abs(Number()) % to; }
	float RealNumber() { return static_cast<float>(Number() % 10000) / Calc::PI; }
	int   Range (int   min, int   max) { return Calc::Repeat(Number(), min, max); } // min이상 max미만
	float Range (float min, float max) { return Calc::Repeat(RealNumber(), min, max); }
	float Length(float lenght) { return Range(-lenght / 2.0f, lenght / 2.0f); }
	float PI() { return Calc::Repeat(RealNumber(), -XM_PI, XM_PI); }
}