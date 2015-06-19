#pragma once
#include "Vector3.hpp"
#include "Degree.hpp"
#include "Quaternion.hpp"
#include <cmath>

namespace Math3D
{
  Vector3 rotate(const Quaternion& quat, const Vector3& vect);
};
