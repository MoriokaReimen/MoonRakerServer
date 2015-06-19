/*!
-----------------------------------------------------------------------------
@file    Quaternion.cpp
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
@brief Quaternion manipulation class
-----------------------------------------------------------------------------
*/
#include "Quaternion.hpp"
namespace Math3D
{

/*!
 * @brief Constructor for Quaternion class
 * @param[in] w x element of vector
 * @param[in] x x element of vector
 * @param[in] y y element of vector
 * @param[in] z z element of vector
 */
Quaternion::Quaternion(const double& w, const double& x,
                       const double& y, const double& z)
    : w(w), x(x), y(y), z(z)
{
    return;
}

/*!
 * @brief Constructor from axis angle and angle
 * @param[in] angle angle of rotation
 * @param[in] axis axis of rotation
 */
Quaternion::Quaternion(const Degree& angle, const Vector3& axis)
{
    this->fromAngleAxis(angle, axis);
    return;
}

/*!
 * @brief Constructor from axis angle and angle
 * @param[in] angle angle of rotation
 * @param[in] axis axis of rotation
 */
Quaternion::Quaternion(const Vector3& a, const Vector3& b)
{
    auto angle = getAngle(a, b);
    auto axis = getNormal(a, b);
    this->fromAngleAxis(angle, axis);
    return;
}

void Quaternion::set(const double& w, const double& x,
                     const double& y, const double& z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;

    return;
}

/*!
 * @brief Convert quaternion to angle and axis
 * @param[out] angle angle of rotation
 * @param[out] axis axis of rotation
 */
void Quaternion::toAngleAxis(Degree& angle, Vector3& axis) const
{
    angle.fromRadian(2.0 * std::acos(this->w));
    double s = 1.0 - this->w * this->w;
    if(s > 0.001) {
        axis.x    = this->x / std::sqrt(1 - this->w * this->w);
        axis.y    = this->y / std::sqrt(1 - this->w * this->w);
        axis.z    = this->z / std::sqrt(1 - this->w * this->w);
        return;
    } else {
        axis.x = 1.0;
        axis.y = 0.0;
        axis.z = 0.0;
        return;
    }
}

/*!
 * @brief set quaternion from angle and axis
 * @param[in] angle angle of rotation
 * @param[in] axis axis of rotation
 */
void Quaternion::fromAngleAxis(const Degree& angle, const Vector3& axis)
{
    auto unit_axis = normalize(axis);
    double s = Math3D::sin(scale(angle, 0.5));
    this->w = Math3D::cos(scale(angle, 0.5));
    this->x = unit_axis.x * s;
    this->y = unit_axis.y * s;
    this->z = unit_axis.z * s;
    return;
}

/*!
 * @brief set quaternion from two vectors
 * @param[in] a input vector
 * @param[in] b input vector
 */
void Quaternion::fromVectors(const Vector3& a, const Vector3& b)
{
    auto angle = getAngle(a, b);
    auto axis = getNormal(a, b);
    this->fromAngleAxis(angle, axis);
    return;
}

/*!
 * @brief convert Quaternion to XYZ euler angle
 * @param[out] roll roll angle
 * @param[out] pitch pitch angle
 * @param[out] yaw yaw angle
 */
void Quaternion::toRPY(Degree& roll, Degree& pitch, Degree& yaw) const
{
    auto m13 = 2.0 * (this->w*this->y + this->x*this->z);
    auto m23 = 2.0 * (this->y*this->z-this->w*this->x);
    auto m33 = 1.0 - 2.0*(this->x*this->x+this->y*this->y);
    auto m11 = 1.0 - 2.0 * (this->y*this->y+this->z*this->z);
    auto m12 = 2.0 * (this->x*this->y-this->w*this->z);

    roll.fromRadian(std::atan2(-m23, m33));

    pitch.fromRadian(std::atan2(m13, std::sqrt(1.0-m13*m13)));

    yaw.fromRadian(std::atan2(-m12, m11));

    return;
}

/*!
 * @brief set Quaternion from XYZ euler angle
 * @param[in] roll roll angle
 * @param[in] pitch pitch angle
 * @param[in] yaw yaw angle
 */
void Quaternion::fromRPY(const Degree& roll, const Degree& pitch, const Degree& yaw)
{
    static const Vector3 x_axis(1.0,0.0,0.0), y_axis(0.0, 1.0, 0.0), z_axis(0.0, 0.0, 1.0);
    Quaternion qroll(roll, x_axis), qpitch(pitch, y_axis), qyaw(yaw, z_axis);
    *this = qyaw * qpitch * qroll;
    return;
}

/*!
 * @brief get norm of quaternion
 * @return norm of quaternion
 */
double Quaternion::getNorm() const
{
    double length = std::sqrt(
                        (this->w * this->w) +
                        (this->x * this->x) +
                        (this->y * this->y) +
                        (this->z * this->z)
                    );
    return length;
}

Quaternion Quaternion::operator+=(const Quaternion& rhs)
{
    this->w += rhs.w;
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

Quaternion Quaternion::operator-=(const Quaternion& rhs)
{
    this->w -= rhs.w;
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

Quaternion Quaternion::operator*=(const Quaternion& rhs)
{
    double x, y, z, w;

    w = (this->w * rhs.w)
        -(this->x * rhs.x)
        -(this->y * rhs.y)
        -(this->z * rhs.z);

    x = (this->w * rhs.x)
        +(this->x * rhs.w)
        -(this->y * rhs.z)
        +(this->z * rhs.y);

    y = (this->w * rhs.y)
        +(this->x * rhs.z)
        +(this->y * rhs.w)
        -(this->z * rhs.x);

    z = (this->w * rhs.z)
        -(this->x * rhs.y)
        +(this->y * rhs.x)
        +(this->z * rhs.w);

    this->set(w, x, y, z);

    return *this;
}

/*!
 * @brief scale quaternion
 * @param[in] quat input quaternion
 * @param[in] num factor for scaling
 * @return scaled quaternion
 */
Quaternion scale(const Quaternion& quat, const double& num)
{
    Quaternion buf(quat.w * num, quat.x * num,
                   quat.y * num, quat.z * num);

    return buf;
}

/*!
 * @brief normalize quaternion
 * @param[in] quat input quaternion
 * @return normalized quaternion
 */
Quaternion normalize(const Quaternion& quat)
{
    Quaternion buf;
    if(quat.getNorm() != 0.0)
        buf = scale(quat, 1.0 / quat.getNorm());
    return buf;
}

Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion buf(lhs.w + rhs.w, lhs.x + rhs.x,
                   lhs.y + rhs.y, lhs.z + rhs.z);

    return buf;
}

Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion buf(lhs.w - rhs.w, lhs.x - rhs.x,
                   lhs.y - rhs.y, lhs.z - rhs.z);

    return buf;
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion buf;

    buf.w = (lhs.w * rhs.w)
            -(lhs.x * rhs.x)
            -(lhs.y * rhs.y)
            -(lhs.z * rhs.z);

    buf.x = (lhs.w * rhs.x)
            +(lhs.x * rhs.w)
            -(lhs.y * rhs.z)
            +(lhs.z * rhs.y);

    buf.y = (lhs.w * rhs.y)
            +(lhs.x * rhs.z)
            +(lhs.y * rhs.w)
            -(lhs.z * rhs.x);

    buf.z = (lhs.w * rhs.z)
            -(lhs.x * rhs.y)
            +(lhs.y * rhs.x)
            +(lhs.z * rhs.w);

    return buf;
}

bool operator==(const Quaternion& lhs, const Quaternion& rhs)
{
    if(std::islessgreater(lhs.w, rhs.w)) return false;
    if(std::islessgreater(lhs.x, rhs.x)) return false;
    if(std::islessgreater(lhs.y, rhs.y)) return false;
    if(std::islessgreater(lhs.z, rhs.z)) return false;
    return true;
}

bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
{
    if(std::islessgreater(lhs.w, rhs.w)) return true;
    if(std::islessgreater(lhs.x, rhs.x)) return true;
    if(std::islessgreater(lhs.y, rhs.y)) return true;
    if(std::islessgreater(lhs.z, rhs.z)) return true;
    return false;
}
};
