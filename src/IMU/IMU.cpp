/*!
-----------------------------------------------------------------------------
@file    IMU.cpp
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
@brief IMU manipulation class
-----------------------------------------------------------------------------
*/
#include "IMU/IMU.hpp"

/*!
 * @brief Attach handler do nothing
 * @param[in] spatial spatial pointer
 * @param[in] ptr user pointer you can use freely
 * @return always return 0
 */
int CCONV IMU::AttachHandler(CPhidgetHandle spatial, void *ptr)
{
    return 0;
}

/*!
 * @brief Detach handler do nothing
 * @param[in] spatial spatial pointer
 * @param[in] ptr user pointer you can use freely
 * @return always return 0
 */
int CCONV IMU::DetachHandler(CPhidgetHandle spatial, void *ptr)
{
    return 0;
}

/*!
 * @brief Error handler throw exception
 * @param[in] spatial spatial pointer
 * @param[in] ptr user pointer you can use freely
 * @param[in] ErrorCode error number
 * @param[in] unknown error message
 * @return always return 0
 * @exception std::runtime_error with error message
 */
int CCONV IMU::ErrorHandler(CPhidgetHandle spatial, void *ptr, int ErrorCode, const char *unknown)
{
    std::string message(unknown);
    throw std::runtime_error(message);
    return 0;
}

/*!
 * @brief Data handler executed every 10[msec]
 * @param[in] spatial spatial pointer
 * @param[in] ptr hand this pointer of IMU class
 * @param[in] data pointer to data packets
 * @param[in] count number of data in the packet
 * @return always return 0
 */
int CCONV IMU::SpatialDataHandler(CPhidgetSpatialHandle spatial, void *ptr, CPhidgetSpatial_SpatialEventDataHandle *data, int count)
{
    IMU* imu = reinterpret_cast<IMU*>(ptr);
    static long long time {0};

    /* load atomic variables */
    double roll, pitch, yaw;
    roll = imu->roll_.load(std::memory_order_acquire);
    pitch = imu->pitch_.load(std::memory_order_acquire);
    yaw = imu->yaw_.load(std::memory_order_acquire);

    for(int i = 0; i < count; i++) {
        /* calc dt */
        if(time == 0) { //! skip first loop
            time = data[i]->timestamp.seconds * 1000 + data[i]->timestamp.microseconds / 1000;
            continue;
        }
        long long  now = data[i]->timestamp.seconds * 1000 + data[i]->timestamp.microseconds / 1000;
        long long dt = now - time;
        time = now;

        /* get gravity vector */
        auto g_x = -1.0 * data[i]->acceleration[0];
        auto g_y = -1.0 * data[i]->acceleration[2];
        auto g_z = -1.0 * data[i]->acceleration[1];
        Math3D::Degree g_roll, g_pitch, g_yaw;
        Math3D::Vector3 gravity(g_x, g_y, g_z);
        Math3D::Quaternion quat;
        quat.fromVectors(gravity, imu->z_);
        quat.toRPY(g_roll, g_pitch, g_yaw);

        /* get angular rate*/
        auto omega_x = -1.0 * data[i]->angularRate[0];
        auto omega_y = -1.0 * data[i]->angularRate[2];
        auto omega_z = -1.0 * data[i]->angularRate[1];

        /* integrate two sensors with complementaly filter */
        roll = 0.92 *(roll + dt * omega_x /1000.0) + 0.08 * g_roll;
        pitch = 0.92 *(pitch + dt * omega_y /1000.0) + 0.08 * g_pitch;
        yaw = 0.92 *(yaw + dt * omega_z /1000.0) + 0.08 * g_yaw;
    }

    /* store roll pitch yaw atomically */
    imu->roll_.store(roll, std::memory_order_release);
    imu->pitch_.store(pitch, std::memory_order_release);
    imu->yaw_.store(yaw, std::memory_order_release);
    return 0;
}

/*!
 * @brief constructor for IMU class. set handler.
 */
IMU::IMU()
{
    CPhidgetSpatial_create(&(this->spatial_));

    /* set Basic handlers */
    CPhidget_set_OnAttach_Handler(reinterpret_cast<CPhidgetHandle>(this->spatial_),
                                  this->AttachHandler, this);
    CPhidget_set_OnDetach_Handler(reinterpret_cast<CPhidgetHandle>(this->spatial_),
                                  this->DetachHandler, this);
    CPhidget_set_OnError_Handler(reinterpret_cast<CPhidgetHandle>(this->spatial_),
                                 this->ErrorHandler, this);

    /* set Spatial specific handlers */
    CPhidgetSpatial_set_OnSpatialData_Handler(this->spatial_, SpatialDataHandler, this);

    CPhidget_open(reinterpret_cast<CPhidgetHandle>(this->spatial_), -1);
    CPhidget_waitForAttachment(reinterpret_cast<CPhidgetHandle>(this->spatial_), 10000);

    return;
}

/*!
 * @brief destructor for IMU class close phidget and free the pointer
 */
IMU::~IMU()
{
    CPhidget_close(reinterpret_cast<CPhidgetHandle>(this->spatial_));
    CPhidget_delete(reinterpret_cast<CPhidgetHandle>(this->spatial_));
}

/*!
 * @brief calibration function. set the standard gravity vector
 * @brief and zero gyro
 */
void IMU::calibrate()
{
  /* initialize gyro */
  CPhidgetSpatial_zeroGyro(this->spatial_);

  /* initialize accelerarometer */
  double x{0.0}, y{0.0}, z{0.0};
  double bx, by, bz;
  const auto ten_millsec = std::chrono::microseconds(10000);
  for(int i=0; i < 300; ++i)
  {
    std::this_thread::sleep_for(ten_millsec);
    CPhidgetSpatial_getAcceleration(this->spatial_, 0, &bx);
    CPhidgetSpatial_getAcceleration(this->spatial_, 2, &by);
    CPhidgetSpatial_getAcceleration(this->spatial_, 1, &bz);
    x -= bx;
    y -= by;
    z -= bz;
  }
  this->z_.set(x/300.0, y/300.0, z/300.0);
  return;
}

/*!
 * @brief run IMU and update roll,pitch,yaw every 10[msec]
 */
void IMU::run()
{
    CPhidgetSpatial_setDataRate(this->spatial_, 10);
    return;
}

/*!
 * @brief get current roll
 * @return roll current roll
 */
Math3D::Degree IMU::getRoll()
{
    Math3D::Degree roll;
    roll.val = this->roll_.load(std::memory_order_acquire);
    return roll;
}

/*!
 * @brief get current pitch
 * @return pitch current pitch
 */
Math3D::Degree IMU::getPitch()
{
    Math3D::Degree pitch;
    pitch.val = this->pitch_.load(std::memory_order_acquire);
    return pitch;
}

/*!
 * @brief get current Yaw
 * @return yaw current yaw
 */
Math3D::Degree IMU::getYaw()
{
    Math3D::Degree yaw;
    yaw.val = this->yaw_.load(std::memory_order_acquire);
    return yaw;
}

/*!
 * @brief get current Quaternion 
 * @return quat current quaternion
 */
Math3D::Quaternion IMU::getQuat()
{
  Math3D::Quaternion quat;
  Math3D::Degree roll, pitch, yaw;

  roll.val = this->roll_.load(std::memory_order_acquire);
  pitch.val = this->pitch_.load(std::memory_order_acquire);
  yaw.val = this->yaw_.load(std::memory_order_acquire);

  quat.fromRPY(roll, pitch, yaw);

  return quat;
}
