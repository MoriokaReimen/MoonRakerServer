/*!
-----------------------------------------------------------------------------
@file    IMU.hpp
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
 ###    @@@@@               @date June. 29. 2014
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
@brief IMU manipulation class
-----------------------------------------------------------------------------
*/
#pragma once
#include <phidget21.h>
#include <atomic>
#include <string>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "Math3D/Math3D.hpp"

class IMU
{
    CPhidgetSpatialHandle spatial_ {0};
    std::atomic<double> roll_ {0};
    std::atomic<double> pitch_ {0};
    std::atomic<double> yaw_ {0};
    Math3D::Vector3 z_{0., 0., -1};

    /* Basic Handlers */
    static int CCONV AttachHandler(CPhidgetHandle spatial, void *ptr);
    static int CCONV DetachHandler(CPhidgetHandle spatial, void *ptr);
    static int CCONV ErrorHandler(CPhidgetHandle spatial, void *ptr, int ErrorCode, const char *unknown);
    /* Spacial specific handlers */
    static int CCONV SpatialDataHandler(CPhidgetSpatialHandle spatial, void *ptr, CPhidgetSpatial_SpatialEventDataHandle *data, int count);

public:
    IMU();
    ~IMU();
    void calibrate();
    void run();
    Math3D::Degree getRoll();
    Math3D::Degree getPitch();
    Math3D::Degree getYaw();
    Math3D::Quaternion getQuat();
};
