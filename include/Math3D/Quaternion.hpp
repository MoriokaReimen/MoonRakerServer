#pragma once
#include "Vector3.hpp"
#include "Degree.hpp"
#include <cmath>

namespace Math3D
{
class Quaternion
{
public:
    double w {1.0};
    double x {0.0};
    double y {0.0};
    double z {0.0};

    Quaternion() = default;
    Quaternion(const double& w, const double& x,
               const double& y, const double& z);
    Quaternion(const Degree& angle, const Vector3& axis);
    Quaternion(const Vector3& a, const Vector3& b);
    virtual ~Quaternion() = default;
    void set(const double& w, const double& x,
             const double& y, const double& z);
    void toAngleAxis(Degree& angle, Vector3& axis) const;
    void fromAngleAxis(const Degree& angle, const Vector3& axis);
    void fromVectors(const Vector3& a, const Vector3& b);
    void toRPY(Degree& roll, Degree& pitch, Degree& yaw) const;
    void fromRPY(const Degree& roll, const Degree& pitch, const Degree& yaw);
    double getNorm() const;
    Quaternion operator+=(const Quaternion& rhs);
    Quaternion operator-=(const Quaternion& rhs);
    Quaternion operator*=(const Quaternion& rhs);
};
Quaternion scale(const Quaternion& quat, const double& num);
Quaternion normalize(const Quaternion& quat);
Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

bool operator==(const Quaternion& lhs, const Quaternion& rhs);
bool operator!=(const Quaternion& lhs, const Quaternion& rhs);
};
