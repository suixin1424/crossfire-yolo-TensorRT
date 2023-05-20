#include<pid.h>

void pid_move::move()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(data_mutex_);
        data_cond_.wait(lock, [this] { return data_ready_; });
        //std::cout << Type << " " << target_position << " " << Kp << " " << std::endl;
            // �������
            error_x = target_position_x - position_x;
            error_y = target_position_y - position_y;

            // ���������
            integral_x += error_x;
            integral_y += error_y;

            // ����΢����
            derivative_x = error_x - last_error_x;
            derivative_y = error_y - last_error_y;

            // ������һ�ε����
            last_error_x = error_x;
            last_error_y = error_y;

            // ���������
            double control_x = Kp * error_x + Ki * integral_x + Kd * derivative_x;
            double control_y = Kp * error_y + Ki * integral_y + Kd * derivative_y;

            // ��������������Ϊ�ƶ��ľ���
            double move_distance_x = control_x;
            double move_distance_y = control_y;
             mouse_event(MOUSEEVENTF_MOVE, move_distance_x, move_distance_y, 0, 0);

            // ����λ��
            //position += move_distance;
        
        //std::cout << 1 << std::endl;
        //refresh();
        data_ready_ = false;
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
}

void pid_move::init(double kp, double ki, double kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}