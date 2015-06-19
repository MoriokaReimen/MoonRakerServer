/*!
-----------------------------------------------------------------------------
@file    RotateFunc.cpp
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
@brief Rotation manipulation function
-----------------------------------------------------------------------------
*/
#include "RotateFunc.hpp"

namespace Math3D
{

/*!
 * @brief Constructor for Quaternion class
 * @param[in] quat rotation quaternion
 * @param[in] vect original vector
 * @return rotated vector
 */
  Vector3 rotate(const Quaternion& quat, const Vector3& vect)
  {
    Math3D::Vector3 axis, ans;
    Degree angle;
    quat.toAngleAxis(angle, axis);
    ans = vect * Math3D::cos(angle) + cross(axis, vect) * Math3D::sin(angle)
      + axis * (1.0 - Math3D::cos(angle)) * dot(axis,vect);
    return ans;
  }
};
