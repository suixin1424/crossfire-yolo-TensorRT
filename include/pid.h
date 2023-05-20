#pragma once
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>


class pid_move
{
public:
    std::mutex data_mutex_;
    std::condition_variable data_cond_;
    bool data_ready_ = false;
    double target_position_x;
    double target_position_y;
    void move();
    static void receiveWrapper2(pid_move* pid) {
        pid->move();
    }
    void init(double kp, double ki, double kd);
    void refresh();
private:
    double Kp = 0.5;
    double Ki = 0.1;
    double Kd = 0.2;
    double position_x = 0.0;
    double position_y = 0.0;
    double error_x, integral_x = 0.0, derivative_x, last_error_x = 0.0;
    double error_y, integral_y = 0.0, derivative_y, last_error_y = 0.0;
};