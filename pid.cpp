#include<pid.h>

void pid_move::move()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(data_mutex_);
        data_cond_.wait(lock, [this] { return data_ready_; });
        is_moving = true;
        //std::cout << Type << " " << target_position << " " << Kp << " " << std::endl;
            // 计算误差
            error_x = target_position_x - position_x;
            error_y = target_position_y - position_y;

            // 计算积分项
            integral_x += error_x;
            integral_y += error_y;

            // 计算微分项
            derivative_x = error_x - last_error_x;
            derivative_y = error_y - last_error_y;

            // 更新上一次的误差
            last_error_x = error_x;
            last_error_y = error_y;

            // 计算控制量
            double control_x = Kp * error_x + Ki * integral_x + Kd * derivative_x;
            double control_y = Kp * error_y + Ki * integral_y + Kd * derivative_y;

            // 假设控制器的输出为移动的距离
            //std::cout << move_distance_x << std::endl;
            move_distance_x = control_x;
            move_distance_y = control_y;
             mouse_event(MOUSEEVENTF_MOVE, move_distance_x, move_distance_y, 0, 0);

            // 更新位置
            //position += move_distance;
        
        //std::cout << 1 << std::endl;
        //refresh();
        data_ready_ = false;
        is_moving = false;
    }
}

void pid_move::refresh()
{
    position_x = 0.0;
    integral_x = 0.0;
    last_error_x = 0.0;
    position_y = 0.0;
    integral_y = 0.0;
    last_error_y = 0.0;
    move_distance_x = 0;
    move_distance_y = 0;
}

void pid_move::init(double kp, double ki, double kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

void pid_move::smooth()
{
    while (1)
    {
        if (is_moving)
            continue;
        mouse_event(MOUSEEVENTF_MOVE, move_distance_x/sqrt((pow(move_distance_x,2)+pow(move_distance_y,2))), move_distance_y / sqrt((pow(move_distance_x, 2) + pow(move_distance_y, 2))), 0, 0);
        //std::cout << move_distance_x<< " " <<move_distance_y << std::endl;
    }
}
