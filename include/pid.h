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
    double target_position;
    void move();
    static void receiveWrapper2(pid_move* pid_x) {
        pid_x->move();
    }
    static void receiveWrapper3(pid_move* pid_y) {
        pid_y->move();
    }
    void init(double kp, double ki, double kd, int type);
    void refresh();
private:
    int Type;
    double Kp = 0.5;
    double Ki = 0.1;
    double Kd = 0.2;
    double position = 0.0;
    double error, integral = 0.0, derivative, last_error = 0.0;
};