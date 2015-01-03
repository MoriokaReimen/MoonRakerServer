#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <unistd.h>
#include "rs232.hpp"


//Declare functions
void serialProcess();
void dataProcess();


//Global  constants
float gear_ratio = 690.0f;      //motor:gearhead ratio
float torque_const = 0.00902f; //mNm/mA
float gearhead_efficiency = 0.49; //%


std::string command_address("inproc://motor_commands");
std::string data_address("inproc://motor_telemetry");

zmq::context_t context(1);



struct timeval epoch_time;

int main ()
{
    zmq::socket_t motor_socket(context, ZMQ_PAIR);
    motor_socket.bind(command_address.c_str());

    std::cout << "Hello World " << sizeof(signed short) << std::endl;


    std::thread serial_reader(serialProcess);

    sleep(5);
    std::thread data_writer(dataProcess);

    bool loop = true;

    signed short speed = 0;

    while(loop) {
        zmq::message_t motor_command(sizeof(signed short));
        std::cin >> speed;

        unsigned char *data_packet = (unsigned char *)motor_command.data();
        data_packet[0] = (speed >> 8) & 0xFF;
        data_packet[1] = speed & 0xFF;
        data_packet[2] = (speed >> 8) & 0xFF;
        data_packet[3] = speed & 0xFF;

        motor_socket.send(motor_command);

        std::cout << "All motors set to "<< speed << "rpm" << std::endl;
    }

    return 1;
}



void serialProcess()
{

    zmq::socket_t publisher(context, ZMQ_PUB);
    zmq::socket_t commander(context, ZMQ_PAIR);

    commander.connect(command_address.c_str());
    publisher.bind(data_address.c_str());


//	std::cout << "Opening COM port..." << std::endl;
    //OpenComport(0, 38400);
    OpenComport(0, 115200);
//	std::cout << "SUCCESS!!" << std::endl;

    //Set up an outgoing motor message container
    unsigned char motor_message[11];
    motor_message[0] = 0x75; //Header/Footer base byte
    motor_message[1] = 0xAA; //Header byte
    motor_message[2] = 0x13; //Source device 0x13 = PCBoard
    //(0x11 = left, 0x12 = right)
    motor_message[3] = 0x00; //Boost Flag
    motor_message[4] = 0x00; //Boost Flag
    //motor_message[5];        //Left Motor RPM Upper Byte
    //motor_message[6];        //Left Motor RPM Lower Byte
    //motor_message[7];        //Right Motor RPM Upper Byte
    //motor_message[8];        //Right Motor RPM Lower Byte
    motor_message[9] = 0x75; //Header/Footer base byte
    motor_message[10] = 0xFF; //Footer byte

    unsigned char incoming[19] = {0};
    /** Incoming Data format
    incoming[0] = 0x75; //Header/Footer base byte
    incoming[1] = 0xAA; //Header byte
    incoming[2] = 0x1?; //Source device (0x11 = left, 0x12 = right)
    incoming[3];    //Back Current Upper Byte
    incoming[4];    //Back Current Lower Byte
    incoming[5];    //Front Current Upper Byte
    incoming[6];    //Front Current Lower Byte
    incoming[7];      //Back Motor RPM Upper Byte
    incoming[8];      //Back Motor RPM Lower Byte
    incoming[9];      //Front Motor RPM Upper Byte
    incoming[10];      //Front Motor RPM Lower Byte
    incoming[11];        //Battery Level Volts
    incoming[12];       //Battery Level millivolts
    incoming[13];         //Time stamp Top Byte
    incoming[14];         //Time stamp Byte
    incoming[15];         //Time stamp Byte
    incoming[16];         //Time stamp Bottom Byte
    incoming[17] = 0x75; //Header/Footer base byte
    incoming[18] = 0xFF; //Footer byte
    **/
    unsigned char buffer[40];
    int message_counter = 0;
    int no_reply_counter = 0;
    int bytes_read = 0;
    int message_bookmark = 0;

    //Incoming motor command message
    unsigned char * rpm_cmd;
    rpm_cmd = new unsigned char[4];
    rpm_cmd[0] = 0;
    rpm_cmd[1] = 0;
    rpm_cmd[2] = 0;
    rpm_cmd[3] = 0;


    std::string right_tag("R");
    std::string left_tag("L");
    unsigned char motor_data[32]; //28 chars are 14 for the left and 14 for the right (with the back and front for the current it becomes 32(+upper and lower))
    signed short left_target_speed, right_target_speed;
    signed short left_front_rpm, left_back_rpm;
    signed short right_front_rpm, right_back_rpm;

    float left_back_torque, left_front_torque, right_back_torque, right_front_torque;
    float left_back_power, left_front_power, right_back_power, right_front_power;

    unsigned long left_controller_clock, right_controller_clock;

    /* changes being made for front and back current
        signed short left_current, right_current;
        float left_voltage, right_voltage;
        float current_average_left, current_average_right;
        int current_average_counter_left, current_average_counter_right;
        float current_calibration_left = 0;
        float current_calibration_right = 0;
        float battery_level;
        */
    unsigned short left_back_current, left_front_current, right_back_current, right_front_current;
    //float left_voltage, right_voltage;
    //current_average_counter gets the reading for the components to work (when the wheels are stoped) //coment all this out for now
    float battery_level;


    zmq::message_t command;

    //Prepare messages to be filled with motor data
    zmq::message_t tag(right_tag.size());
    zmq::message_t timestamp(sizeof(struct timeval));
    zmq::message_t controller_clock(sizeof(unsigned long));
    zmq::message_t target(sizeof(signed short));
    zmq::message_t back_rpm(sizeof(signed short));
    zmq::message_t front_rpm(sizeof(signed short));
    zmq::message_t back_torque(sizeof(float));
    zmq::message_t front_torque(sizeof(float));
    zmq::message_t back_power(sizeof(float));
    zmq::message_t front_power(sizeof(float));

    //Battery variables
    int battery_delay_counter = 0;
    std::string battery_tag("B");
    zmq::message_t battery(sizeof(float));
    unsigned char volts;
    unsigned char millivolts;

    unsigned int temp0;
    unsigned int temp1;
    signed short temp2;
    signed short temp3;
    unsigned long timestamp_check;

    std::cout << "Entering Motor Loop..." << std::endl;
    bool loop = true;
    while(loop) {
        //std::cout <<"Checking for command..." <<std::endl;
        command.rebuild();
        //If there's a command from the operator in the pipe
        if (commander.recv(&command, ZMQ_NOBLOCK) == true) {
            //std::cout << "<";
            //Record the data
            rpm_cmd = (unsigned char *)command.data();


            //std::cout << "<" << rpm_cmd[1];
        }
        if ( (message_bookmark == 0) && (message_counter == 0) ) {
            //Assign the 4 bytes to the data packet array
            motor_message[5] = rpm_cmd[0]; //Left Upper Byte
            motor_message[6] = rpm_cmd[1]; //Left Lower Byte
            motor_message[7] = rpm_cmd[2]; //Right Upper Byte
            motor_message[8] = rpm_cmd[3]; //Right Lower Byte

            //Send it out on the RS485 line
            SendBuf(0, motor_message, 11);
            message_counter ++;
            //Debug output

            temp2 = ((motor_message[5] << 8) | motor_message[6]);
            temp3 = ((motor_message[7] << 8) | motor_message[8]);
            //std::cout << " ";//\n" << temp2 << " =>L ; " << temp3 << " =>R" << "\n";
        }
        //std::cout <<"|" <<std::endl;

        //Cross-platform sleep function defined in SubsystemInterface
        //xsleep(15);
        sleep(5);

        //std::cout << ".";

        //std::cout <<"Reading serial port..." <<std::endl;
        //Try to read up to 40 bytes from the serial line
        //This does not block
        bytes_read = PollComport(0, buffer, 40);
        //If there's nothing on the line
        if (bytes_read == 0) {
            //And we were in the middle of a message
            if(message_bookmark != 0) {
                //Then give up on the message
                message_bookmark = 0;
                std::cout << "!";  //Each . on the output indicates a skipped message
            }

            //And/or if we're waiting for a reply
            if(message_counter > 0) {
                //And have been 3 times in a row
                if (++no_reply_counter > 2) {
                    std::cout <<"\nMC UNRESPONSIVE\n";

                    //Give up on the reply
                    message_counter = 0;
                    no_reply_counter = 0;
                }
            }
        } else {
            no_reply_counter = 0;    //
        }

        //For each new byte read
        for (int i = 0; i < bytes_read; i++) {
            //Record the data and increment the bookmark
            incoming[message_bookmark] = buffer[i];
            message_bookmark++;

            //If at any time we complete a message
            if (message_bookmark == 19) {
                //Reset the bookmark for the next message
                message_bookmark = 0;
                //And process the message
                //If the message is formatted correctly with proper
                //Headers and footers
                if ((incoming[0] == 0x75) && (incoming[1] == 0xAA) &&
                    (incoming[17] == 0x75) && (incoming[18] == 0xFF) ) {


                    //record the time
                    gettimeofday(&epoch_time,NULL);


                    //Then check if it's left or right
                    //For a message from the left motor
                    if(incoming[2] == 0x11) {
                        //Increment the message counter to indicate
                        //that the left response has been received
                        message_counter++;
                        ///message_counter = 0;




                        //Record the left motor data to the front of the
                        //motor data array
                        motor_data[0] = incoming[3];//Back Current Upper Byte
                        motor_data[1] = incoming[4];//Back Current Lower Byte
                        motor_data[2] = incoming[5];//Front Current Upper Byte
                        motor_data[3] = incoming[6];//Front Current Lower Byte
                        motor_data[4] = incoming[7];//Back upper
                        motor_data[5] = incoming[8];//Back lower
                        motor_data[6] = incoming[9];//Front upper
                        motor_data[7] = incoming[10];//Front lower
                        motor_data[8] = incoming[11];//Battery upper
                        motor_data[9] = incoming[12];//Battery lower
                        motor_data[10] = incoming[13];//TS upper
                        motor_data[11] = incoming[14];//TS
                        motor_data[12]= incoming[15];//TS
                        motor_data[13]= incoming[16];//TS lower

                        left_target_speed = ((rpm_cmd[0] << 8) | rpm_cmd[1]);
                        left_back_current = ((incoming[3] << 8) | incoming[4]);
                        left_front_current = ((incoming[5] << 8) | incoming[6]);
                        left_back_rpm = ((incoming[7] << 8) | incoming[8]);
                        left_front_rpm = ((incoming[9] << 8) | incoming[10]);


                        //comenting out the calibration for the stoped wheels
                        //it still needs to be adjusted for dealing with the system who gets the front and back current from each motor controller
                        /* if((left_target_speed == 0) && ((left_front_rpm==0) && (left_back_rpm==0)))
                         {
                             current_average_left += left_current;
                             if(++current_average_counter_left == 10)
                             {
                                 current_calibration_left = current_average_left / 10;
                                 current_average_left = 0;
                                 current_average_counter_left = 0;
                             }
                         }
                         else
                         {
                             current_average_counter_left = 0;
                             current_average_left = 0;
                             left_current -= current_calibration_left;
                         }*/


                        //float left_back_current_amps = left_back_current / 1000.0f; //units in this operation: A=mA/1000
                        //float left_front_current_amps = left_front_current / 1000.0f; //so just transforming mA to A
                        //left_voltage = 8;
                        float left_back_current_mA = (float)left_back_current; //units in this operation: A=mA/1000
                        float left_front_current_mA = (float)left_front_current; //so just transforming mA to A

                        //left_back_torque = (torque_const * left_back_current_amps)/1000.0f; //units in this operation Nm = (mNm/A *A)/1000
                        left_back_torque = (left_back_current_mA * torque_const) * (gear_ratio * gearhead_efficiency);
                        //left_front_torque = (torque_const * left_front_current_amps)/1000.0f; //so the torque unit obtained is Newton meters(Nm)
                        left_front_torque = (left_front_current_mA * torque_const) * (gear_ratio * gearhead_efficiency);
                        left_back_power = (left_back_torque * (left_back_rpm/gear_ratio)) / 9.55f; //the power unit is in Watts
                        left_front_power = (left_front_torque * (left_front_rpm/gear_ratio)) / 9.55f;

                        left_controller_clock = ((((incoming[13] << 24) | (incoming[14] << 16)) | (incoming[15]) << 8) | incoming[16]);

                        volts = incoming[11];
                        millivolts = incoming[12];
                        battery_level = (float)volts + (float)(millivolts / 1000.0f);


                        //Reset the messages so they can be refilled
                        tag.rebuild(left_tag.size());
                        timestamp.rebuild(sizeof(struct timeval));
                        controller_clock.rebuild(sizeof(unsigned long));
                        target.rebuild(sizeof(signed short));
                        back_rpm.rebuild(sizeof(signed short));
                        front_rpm.rebuild(sizeof(signed short));
                        back_torque.rebuild(sizeof(float));
                        front_torque.rebuild(sizeof(float));
                        back_power.rebuild(sizeof(float));
                        front_power.rebuild(sizeof(float));
                        battery.rebuild(sizeof(float));

                        //Fill messages with left motor data
                        memcpy((void *)tag.data(), left_tag.data(), left_tag.size());
                        memcpy((void *)timestamp.data(), &epoch_time, sizeof(struct timeval));
                        memcpy((void *)controller_clock.data(), &left_controller_clock, sizeof(unsigned long));
                        memcpy((void *)target.data(), &left_target_speed, sizeof(signed short));
                        memcpy((void *)back_rpm.data(), &left_back_rpm, sizeof(signed short));
                        memcpy((void *)front_rpm.data(), &left_front_rpm, sizeof(signed short));
                        memcpy((void *)back_torque.data(), &left_back_torque, sizeof(float));
                        memcpy((void *)front_torque.data(), &left_front_torque, sizeof(float));
                        memcpy((void *)back_power.data(), &left_back_power, sizeof(float));
                        memcpy((void *)front_power.data(), &left_front_power, sizeof(float));
                        memcpy((void *)battery.data(), &battery_level, sizeof(float));



                        //Send the left motor data out
                        publisher.send(tag, ZMQ_SNDMORE);
                        publisher.send(timestamp, ZMQ_SNDMORE);
                        publisher.send(controller_clock, ZMQ_SNDMORE);
                        publisher.send(target, ZMQ_SNDMORE);
                        publisher.send(back_rpm, ZMQ_SNDMORE);
                        publisher.send(front_rpm, ZMQ_SNDMORE);
                        publisher.send(back_torque, ZMQ_SNDMORE);
                        publisher.send(front_torque, ZMQ_SNDMORE);
                        publisher.send(back_power, ZMQ_SNDMORE);
                        publisher.send(front_power, ZMQ_SNDMORE);
                        publisher.send(battery);


#ifdef DEBUG
                        //Then merge the bytes to signed shorts (big endian)
                        temp0 = ((incoming[3] << 8) | incoming[4]);
                        temp1 = ((incoming[5] << 8) | incoming[6]);
                        temp2 = ((incoming[7] << 8) | incoming[8]);
                        temp3 = ((incoming[9] << 8) | incoming[10]);
                        volts = incoming[11];
                        millivolts = incoming[12];
                        timestamp_check = ((((incoming[13] << 24) | (incoming[14] << 16)) | (incoming[15]) << 8) | incoming[16]);
                        std::cout << "\tBack current: " << temp0 << "mA, Front current: " << temp1 << "mA."<<std::endl
                                  << "\tBack motor RPM: " << temp2 << "\tFront motor RPM: "<< temp3<<std::endl
                                  << "\tL: TS: " <<timestamp_check << "ms, " << (int)volts << "." << (int)millivolts <<"V from battery, " <<std::endl;
#endif
                    }
                    //Otherwise if it's from the right motor
                    else if(incoming[2] == 0x12) {
                        //Reset the message counter to indicate that
                        //a complete cycle has been completed
                        message_counter = 0;
                        //Record the right motor data to the back
                        //of the motor data array
                        motor_data[14] = incoming[3];//Back Current Upper Byte
                        motor_data[15] = incoming[4];//Back Current Lower Byte
                        motor_data[16] = incoming[5];//Front Current Upper Byte
                        motor_data[17] = incoming[6];//Front Current Lower Byte
                        motor_data[18] = incoming[7];//Back upper
                        motor_data[19] = incoming[8];//Back lower
                        motor_data[20] = incoming[9];//Front upper
                        motor_data[21] = incoming[10];//Front lower
                        motor_data[22] = incoming[11];//Battery upper
                        motor_data[23] = incoming[12];//Battery lower
                        motor_data[24] = incoming[13];//TS upper
                        motor_data[25] = incoming[14];//TS
                        motor_data[26] = incoming[15];//TS
                        motor_data[27] = incoming[16];//TS lower

                        motor_data[28] = rpm_cmd[0];
                        motor_data[29] = rpm_cmd[1];
                        motor_data[30] = rpm_cmd[2];
                        motor_data[31] = rpm_cmd[3];









                        right_target_speed = ((rpm_cmd[2] << 8) | rpm_cmd[3]);
                        right_back_current = ((incoming[3] << 8) | incoming[4]);
                        right_front_current = ((incoming[5] << 8) | incoming[6]);
                        right_back_rpm = ((incoming[7] << 8) | incoming[8]);
                        right_front_rpm = ((incoming[9] << 8) | incoming[10]);

                        //comenting out more calibration
                        //this also need to be adjusted
                        /*if((right_target_speed == 0) && ((right_front_rpm==0) && (right_back_rpm==0)))
                        {
                            current_average_right += right_current;
                            if(++current_average_counter_right == 10)
                            {
                                current_calibration_right = current_average_right / 10;
                                current_average_right = 0;
                                current_average_counter_right = 0;
                            }
                        }
                        else
                        {
                            current_average_counter_right = 0;
                            current_average_right = 0;
                            right_current -= current_calibration_right;
                        }*/

                        //float right_back_current_amp = right_back_current / 1000.0f;
                        //float right_front_current_amp = right_front_current / 1000.0f;
                        //right_voltage = 15;

                        float right_back_current_mA = (float)right_back_current; //units in this operation: A=mA/1000
                        float right_front_current_mA = (float)right_front_current; //so just transforming mA to A

                        right_back_torque = (right_back_current_mA * torque_const) * (gear_ratio * gearhead_efficiency);
                        right_front_torque = (right_front_current_mA * torque_const) * (gear_ratio * gearhead_efficiency);
                        //right_back_torque = (torque_const * right_back_current_amp)/1000.0f;
                        //right_front_torque = (torque_const * right_front_current_amp)/1000.0f;
                        right_back_power = (right_back_torque * (right_back_rpm/gear_ratio)) / 9.55f;
                        right_front_power = (right_front_torque * (right_front_rpm/gear_ratio)) / 9.55f;

                        right_controller_clock = ((((incoming[13] << 24) | (incoming[14] << 16)) | (incoming[15]) << 8) | incoming[16]);

                        volts = incoming[11];
                        millivolts = incoming[12];
                        battery_level = (float)volts + (float)(millivolts / 1000.0f);


                        //Reset the messages so they can be refilled with the right motor data
                        tag.rebuild(right_tag.size());
                        timestamp.rebuild(sizeof(struct timeval));
                        controller_clock.rebuild(sizeof(unsigned long));
                        target.rebuild(sizeof(signed short));
                        back_rpm.rebuild(sizeof(signed short));
                        front_rpm.rebuild(sizeof(signed short));
                        back_torque.rebuild(sizeof(float));
                        front_torque.rebuild(sizeof(float));
                        back_power.rebuild(sizeof(float));
                        front_power.rebuild(sizeof(float));
                        battery.rebuild(sizeof(float));


                        //Fill up the messages with right motor data
                        memcpy((void *)tag.data(), right_tag.data(), right_tag.size());
                        memcpy((void *)timestamp.data(), &epoch_time, sizeof(struct timeval));
                        memcpy((void *)controller_clock.data(), &right_controller_clock, sizeof(unsigned long));
                        memcpy((void *)target.data(), &right_target_speed, sizeof(signed short));
                        memcpy((void *)back_rpm.data(), &right_back_rpm, sizeof(signed short));
                        memcpy((void *)front_rpm.data(), &right_front_rpm, sizeof(signed short));
                        memcpy((void *)back_torque.data(), &right_back_torque, sizeof(float));
                        memcpy((void *)front_torque.data(), &right_front_torque, sizeof(float));
                        memcpy((void *)back_power.data(), &right_back_power, sizeof(float));
                        memcpy((void *)front_power.data(), &right_front_power, sizeof(float));
                        memcpy((void *)battery.data(), &battery_level, sizeof(float));

                        //Send the left motor data out
                        publisher.send(tag, ZMQ_SNDMORE);
                        publisher.send(timestamp, ZMQ_SNDMORE);
                        publisher.send(controller_clock, ZMQ_SNDMORE);
                        publisher.send(target, ZMQ_SNDMORE);
                        publisher.send(back_rpm, ZMQ_SNDMORE);
                        publisher.send(front_rpm, ZMQ_SNDMORE);
                        publisher.send(back_torque, ZMQ_SNDMORE);
                        publisher.send(front_torque, ZMQ_SNDMORE);
                        publisher.send(back_power, ZMQ_SNDMORE);
                        publisher.send(front_power, ZMQ_SNDMORE);
                        publisher.send(battery);

                        /*
                        if(++battery_delay_counter > 10)
                        {
                            battery_delay_counter = 0;
                            volts = incoming[9];
                            millivolts = incoming[10];
                            battery_level = (float)volts + (float)(millivolts / 1000.0f);

                            tag.rebuild(battery_tag.size());
                            battery.rebuild(sizeof(float));

                            memcpy((void *)tag.data(), battery_tag.data(), battery_tag.size());
                            memcpy((void *)battery.data(), &battery_level, sizeof(float));

                            publisher.send(tag, ZMQ_SNDMORE);
                            publisher.send(battery);

                        }*/


                        //Prepare the outgoing message
                        ///broadcast.rebuild(28);
                        //Fill it with motor data
                        ///memcpy((void *)broadcast.data(), motor_data, 28);
                        //Send it
                        ///publisher.send(broadcast);

#ifdef DEBUG
                        //Then merge the bytes to signed shorts (big endian)
                        temp0 = ((incoming[3] << 8) | incoming[4]);
                        temp1 = ((incoming[5] << 8) | incoming[6]);
                        temp2 = ((incoming[7] << 8) | incoming[8]);
                        temp3 = ((incoming[9] << 8) | incoming[10]);
                        volts = incoming[11];
                        millivolts = incoming[12];
                        timestamp_check = ((((incoming[13] << 24) | (incoming[14] << 16)) | (incoming[15]) << 8) | incoming[16]);
                        std::cout << "\tBack current :" << temp0 << "mA, Front current: " << temp1 << "mA"<<std::endl
                                  << "\tBack motor RPM: " << temp2 << "\tFront motor RPM: "<< temp3<<std::endl
                                  << "\tR: TS: " <<timestamp_check << "ms, " << (int)volts << "." << (int)millivolts <<"V from battery, " <<std::endl;
#endif
                    } else {
                        std::cout << "?";
                    }
                } else {
                    std::cout << (int)incoming[0] << "," << (int)incoming[1] << ";"
                              << (int)incoming[2] << ";"
                              << (int)incoming[3] << "," << (int)incoming[4] << ";"
                              << (int)incoming[5] << "," << (int)incoming[6] << ";"
                              << (int)incoming[7] << "," << (int)incoming[8] << ";"
                              << (int)incoming[9] << "," << (int)incoming[10] <<";"
                              << (int)incoming[11] << "," << (int)incoming[12] <<";"
                              << (int)incoming[13] << "," << (int)incoming[14] <<","
                              << (int)incoming[15] << "," << (int)incoming[16] << ";"
                              << (int)incoming[17] << "," << (int)incoming[18] << std::endl;
                }
                incoming[0] = 0;
                incoming[1] = 0;
                incoming[2] = 0;
                incoming[3] = 0;
                incoming[4] = 0;
                incoming[5] = 0;
                incoming[6] = 0;
                incoming[7] = 0;
                incoming[8] = 0;
                incoming[9] = 0;
                incoming[10] = 0;
                incoming[11] = 0;
                incoming[12] = 0;
                incoming[13] = 0;
                incoming[14] = 0;
                incoming[15] = 0;
                incoming[16] = 0;
                incoming[17] = 0;
                incoming[18] = 0;

            }
        }





        //if(*(char*)command.data() == '\xFF')
        //{
        //    loop = false;
        //}

    }
    CloseComport(0);
    std::cout << "Motor joined" << std::endl;
}

void dataProcess()
{
    std::cout <<"Data writer..." <<std::endl;

    std::ofstream data_file;
    std::string filename = "motor.txt";
    data_file.open(filename.c_str(), ((std::ios::out | std::ios::app)));

    timeval recv_TS;

    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(data_address.c_str());
    subscriber.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);


    int number_of_sockets = 1;
    int wait_time = -1;
    //0: don't wait
    //-1: wait forever
    zmq::pollitem_t incoming_list[] = { { subscriber, 0, ZMQ_POLLIN, 0 }};

    bool record = true;
    bool loop = true;
    while(loop) {
        //std::cout <<"Waiting for data..." <<std::endl;
        //Wait for new motor data
        zmq::poll(incoming_list, number_of_sockets, wait_time);

        //record the time
        gettimeofday(&recv_TS, NULL);



        //Prepare messages to be filled with motor data
        zmq::message_t tag;
        zmq::message_t timestamp;
        zmq::message_t controller_clock;
        zmq::message_t target;
        zmq::message_t back_rpm;
        zmq::message_t front_rpm;
        zmq::message_t back_torque;
        zmq::message_t front_torque;
        zmq::message_t back_power;
        zmq::message_t front_power;
        zmq::message_t battery;


        subscriber.recv(&tag);
        subscriber.recv(&timestamp);
        subscriber.recv(&controller_clock);
        subscriber.recv(&target);
        subscriber.recv(&back_rpm);
        subscriber.recv(&front_rpm);
        subscriber.recv(&back_torque);
        subscriber.recv(&front_torque);
        subscriber.recv(&back_power);
        subscriber.recv(&front_power);
        subscriber.recv(&battery);

        struct timeval rover_TS;
        unsigned long motor_clock;
        signed short target_speed, back_speed, front_speed;
        float back_motor_torque, front_motor_torque, back_motor_power, front_motor_power, battery_voltage;

        std::string motor_tag(static_cast<char*>(tag.data()), tag.size());
        rover_TS = *static_cast<struct timeval*>(timestamp.data());
        motor_clock = *static_cast<unsigned long*>(controller_clock.data());
        target_speed = *static_cast<signed short*>(target.data());
        back_speed = *static_cast<signed short*>(back_rpm.data());
        front_speed = *static_cast<signed short*>(front_rpm.data());
        back_motor_torque = *static_cast<float*>(back_torque.data());
        front_motor_torque = *static_cast<float*>(front_torque.data());
        back_motor_power = *static_cast<float*>(back_power.data());
        front_motor_power = *static_cast<float*>(front_power.data());
        battery_voltage = *static_cast<float*>(battery.data());

        /*
            motor_publisher.send(tag, ZMQ_SNDMORE);
            motor_publisher.send(back_torque, ZMQ_SNDMORE);
            motor_publisher.send(front_torque, ZMQ_SNDMORE);
            motor_publisher.send(back_power, ZMQ_SNDMORE);
            motor_publisher.send(front_power);
        */


        if(record && data_file.is_open()) {
            data_file       << motor_tag << ":"
                            << recv_TS.tv_sec << "," << recv_TS.tv_usec << ";"
                            << rover_TS.tv_sec << "," << rover_TS.tv_usec << ";"
                            << motor_clock << ";"
                            << target_speed << "," << front_speed << "," << back_speed << ";"
                            << back_motor_torque << ","<< front_motor_torque << ";"
                            << back_motor_power << ","<< front_motor_power << ";"
                            << battery_voltage << std::endl;
        }
    }
}
