#pragma once
#include <cmath>
#define _USE_MATH_DEFINES

namespace Math3D
{

class Degree
{
public:
    double val {0.0};
    Degree() = default;
    Degree(const double& num);
    double toRadian() const;
    void fromRadian(const double& radian);
    Degree& operator+=(const Degree& lhs);
    Degree& operator-=(const Degree& lhs);
    operator double()
    {
        return double(this->val);
    }
};

Degree operator+(const Degree& lhs, const Degree& rhs);
Degree operator-(const Degree& lhs, const Degree& rhs);

bool operator==(const Degree& lhs, const Degree& rhs);
bool operator!=(const Degree& lhs, const Degree& rhs);
bool operator>(const Degree& lhs, const Degree& rhs);
bool operator<(const Degree& lhs, const Degree& rhs);
bool operator<=(const Degree& lhs, const Degree& rhs);
bool operator>=(const Degree& lhs, const Degree& rhs);

Degree scale(const Degree& degree, const double& num);
double sin(const Degree& degree);
double cos(const Degree& degree);
double tan(const Degree& degree);
}
