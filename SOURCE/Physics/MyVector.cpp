#include "MyVector.h"
#include <cmath>

namespace Physics {

float Magnitude(const MyVector& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

MyVector Direction(const MyVector& v) {
    float mag = Magnitude(v);
    if (mag == 0.0f) return MyVector(0, 0, 0);
    return MyVector(v.x / mag, v.y / mag, v.z / mag);
}

MyVector operator+(const MyVector& lhs, const MyVector& rhs) {
    return MyVector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

MyVector operator-(const MyVector& lhs, const MyVector& rhs) {
    return MyVector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

MyVector operator*(const MyVector& v, float scalar) {
    return MyVector(v.x * scalar, v.y * scalar, v.z * scalar);
}

MyVector operator*(float scalar, const MyVector& v) {
    return v * scalar;
}


// Component-wise product
MyVector ComponentProduct(const MyVector& a, const MyVector& b) {
    return MyVector(a.x * b.x, a.y * b.y, a.z * b.z);
}

// Scalar (dot) product
float ScalarProduct(const MyVector& a, const MyVector& b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

// Vector (cross) product
MyVector VectorProduct(const MyVector& a, const MyVector& b) {
    return MyVector(
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    );
}

} // namespace Physics
