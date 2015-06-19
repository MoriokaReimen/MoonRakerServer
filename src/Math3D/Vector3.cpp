/*!
-----------------------------------------------------------------------------
@file    Vector3.cpp
----------------------------------------------------------------------------
         @@
       @@@@@@
      @```@@@@
     @`  `@@@@@@
   @@`   `@@@@@@@@
  @@`    `@@@@@@@@@           Tohoku University
  @` `   `@@@@@@@@@       SPACE ROBOTICS LABORATORY
  @`` ## `@@@@@@@@@    http://www.astro.mech.tohoku.ac.jp/
  @` #..#`@@@@@@@@@        Planetary Robotics Group
  @` #..#`@@@@@@@@@
  @` ### `@@@@@@@@@          Professor Kazuya Yoshida
  @` ###``@@@@@@@@@      Associate Professor Keiji Nagatani
   @### ``@@@@@@@@
   ###  ` @@@@@@@
  ###  @  @@@@@                 Creation Date:
 ###    @@@@@               @date Dec. 29. 2014
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%             @author Kei Nakata
   #### %%%                  menschenjager.mark.neun@gmail.com
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------
@brief Vector manipulation class
-----------------------------------------------------------------------------
*/
#include "Vector3.hpp"

namespace Math3D
{

/*!
 * @brief Constructor for Vector3 class
 * @param[in] a x element of vector
 * @param[in] b y element of vector
 * @param[in] c z element of vector
 */
Vector3::Vector3(const double& a, const double& b, const double& c)
    : x(a), y(b), z(c)
{
    return;
}

void Vector3::set(const double& a, const double& b, const double& c)
{
  this->x = a;
  this->y = b;
  this->z = c;
  return;
}
/*!
 * @brief get norm of Vector
 * @return norm of Vector
 */
double Vector3::getNorm() const
{
    double len =
        std::sqrt(
            (this->x * this->x) +
            (this->y * this->y) +
            (this->z * this->z)
        );
    return len;
}

Vector3 Vector3::operator+=(const Vector3& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

Vector3 Vector3::operator-=(const Vector3& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

Vector3 Vector3::operator*=(const double& rhs)
{
    this->x *= rhs;
    this->y *= rhs;
    this->z *= rhs;
    return *this;
}

Vector3 Vector3::operator/=(const double& rhs)
{
    this->x /= rhs;
    this->y /= rhs;
    this->z /= rhs;
    return *this;
}

Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 buf(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    return buf;
}

Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 buf(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    return buf;
}

Vector3 operator*(const Vector3& lhs, const double& rhs)
{
    Vector3 buf(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    return buf;
}

Vector3 operator/(const Vector3& lhs, const double& rhs)
{
    Vector3 buf(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
    return buf;
}

bool operator==(const Vector3& lhs, const Vector3& rhs)
{
    if(std::islessgreater(lhs.x, rhs.x)) return false;
    if(std::islessgreater(lhs.y, rhs.y)) return false;
    if(std::islessgreater(lhs.z, rhs.z)) return false;
    return true;
}

bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
    if(std::islessgreater(lhs.x, rhs.x)) return true;
    if(std::islessgreater(lhs.y, rhs.y)) return true;
    if(std::islessgreater(lhs.z, rhs.z)) return true;
    return false;
}

/*!
 * @brief get normalized Vector
 * @param[in] lhs input vector
 * @return normalized vector
 */
Vector3 normalize(const Vector3& lhs)
{
    Vector3 buf;
    if(lhs.getNorm() != 0.0)
        buf = lhs / lhs.getNorm();
    return buf;
}

/*!
 * @brief dot product
 * @param[in] lhs input vector
 * @param[in] rhs input vector
 * @return dot product of vector
 */
double dot(const Vector3& lhs, const Vector3& rhs)
{
    double buf(
        (lhs.x * rhs.x) +
        (lhs.y * rhs.y) +
        (lhs.z * rhs.z));
    return buf;
}

/*!
 * @brief cross product
 * @param[in] lhs input vector
 * @param[in] rhs input vector
 * @return cross product of vector
 */
Vector3 cross(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 buf(
        (lhs.y * rhs.z) - (lhs.z * rhs.y),
        (lhs.z * rhs.x) - (lhs.x * rhs.z),
        (lhs.x * rhs.y) - (lhs.y * rhs.x)
    );
    return buf;
}

/*!
 * @brief get angle between two vectors
 * @param[in] lhs input vector
 * @param[in] rhs input vector
 * @return angle between two vector
 */
Degree getAngle(const Vector3& lhs, const Vector3& rhs)
{
    Degree angle;
    auto c = cross(lhs, rhs);
    auto d = dot(lhs, rhs);
    angle.fromRadian(std::atan(c.getNorm() / d));
    return angle;
}

/*!
 * @brief get normal vector of two vectors
 * @param[in] lhs input vector
 * @param[in] rhs input vector
 * @return vector normal to two vectors
 */
Vector3 getNormal(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 normal = cross(lhs, rhs);
    normal = normalize(normal);
    return normal;
}
};
