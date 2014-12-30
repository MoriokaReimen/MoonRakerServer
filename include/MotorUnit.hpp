/*
-----------------------------------------------------------------------------
Filename:    MotorUnit.hpp
-----------------------------------------------------------------------------
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
 ###    @@@@@                    Spring 2013
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%               Nathan John Britton
   #### %%%                 nathan@astro.mech.tohoku.ac.jp
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

#ifndef __MotorUnit_hpp_
#define __MotorUnit_hpp_

#include "Subsystem.hpp"

class MotorUnit : public Subsystem
{
public:
    MotorUnit(zmq::context_t& context_arg, std::string robot_address,
           std::string data_address, std::string inproc_address);
    ~MotorUnit();
    void join();
protected:
    friend class Rover;
private:
    virtual void mainExecutionLoop();

    //Function to handle a message coming
    //in from the robot's camera interface
    void handleMotorMessage();
    void handleMotorData();
    void handleUICommand();


    //Flag for terminating the mainExecutionLoop
    bool loop;


    zmq::socket_t motor_publisher;



};


#endif
