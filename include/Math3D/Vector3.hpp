#pragma once
#include <cmath>
#include "Degree.hpp"

namespace Math3D
{
class Vector3
{
public:
    double x {0.0};
    double y {0.0};
    double z {0.0};
    Vector3() = default;
    Vector3(const double& a, const double& b, const double& c);
    void set(const double& a, const double& b, const double& c);
    double getNorm() const;
    Vector3 operator+=(const Vector3& rhs);
    Vector3 operator-=(const Vector3& rhs);
    Vector3 operator*=(const double& rhs);
    Vector3 operator/=(const double& rhs);
};

Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
Vector3 operator*(const Vector3& lhs, const double& rhs);
Vector3 operator/(const Vector3& lhs, const double& rhs);

bool operator==(const Vector3& lhs, const Vector3& rhs);
bool operator!=(const Vector3& lhs, const Vector3& rhs);

Vector3 normalize(const Vector3& lhs);
double dot(const Vector3& lhs, const Vector3& rhs);
Vector3 cross(const Vector3& lhs, const Vector3& rhs);
Degree getAngle(const Vector3& lhs, const Vector3& rhs);
Vector3 getNormal(const Vector3& lhs, const Vector3& rhs);
};
