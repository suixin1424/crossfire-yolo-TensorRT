#pragma once
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>
#include <mutex>

#pragma comment  (lib,"User32.lib")
#pragma comment  (lib,"Gdi32.lib")

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
    static void receiveWrapper3(pid_move* mouse_smooth) {
        mouse_smooth->smooth();
    }
    void init(double kp, double ki, double kd);
    void refresh();
    void smooth();
    double move_distance_x = 0;
    double move_distance_y = 0;
private:
    double Kp = 0.5;
    double Ki = 0.1;
    double Kd = 0.2;
    double error_x, integral_x = 0.0, derivative_x, last_error_x = 0.0;
    double error_y, integral_y = 0.0, derivative_y, last_error_y = 0.0;

    bool is_moving = false;
};
