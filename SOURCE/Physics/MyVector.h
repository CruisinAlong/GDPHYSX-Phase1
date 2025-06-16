#pragma once
#include <glm/vec3.hpp>
#include <cmath>

namespace Physics {
	class MyVector {
	public:
		//3D Engine
		float x, y, z;

		MyVector() : x(0), y(0), z(0) {}

		MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }

		MyVector& operator+=(const MyVector& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		float Magnitude() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		MyVector Normalize() const {
			float mag = Magnitude();
			if (mag > 0.0f)
				return MyVector(x / mag, y / mag, z / mag);
			else
				return MyVector(0, 0, 0);
		}
	};
	MyVector operator*(const MyVector& v, float scalar);
	MyVector operator*(float scalar, const MyVector& v);
	MyVector operator+(const MyVector& lhs, const MyVector& rhs);
	MyVector operator-(const MyVector& lhs, const MyVector& rhs);
}