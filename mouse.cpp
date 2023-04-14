#include<mouse.h>



class KalmanFilter
{
public:
    KalmanFilter()
    {
        x_ = 0;
        P_ = 1;
        Q_ = 0.3;
        R_ = 2;
    }

    double update(double measurement)
    {
        // Ô¤²â
        x_ = x_;
        P_ = P_ + Q_;

        // ¸üÐÂ
        K_ = P_ / (P_ + R_);
        x_ = x_ + K_ * (measurement - x_);
        P_ = (1 - K_) * P_;

        return x_;
    }

private:
    double x_;
    double P_;
    double Q_;
    double R_;
    double K_;
};
int mouse_control::fire(Mat img, float* Boxes, int* ClassIndexs, int* BboxNum)
{
	int min_value = 9999;
	cv::Rect aim;
	if (*BboxNum > 100)
	{
		cout << "too much aim" << endl;
		return 0;
	}
	for (int i = 0; i < BboxNum[0]; i++)
	{
		if (ClassIndexs[i] != isHead)
			continue;

		cv::Rect rect(Boxes[i * 4], Boxes[i * 4 + 1], Boxes[i * 4 + 2], Boxes[i * 4 + 3]);
		if (sqrt(pow((rect.x - 208), 2) + pow((rect.y - 208), 2)) < min_value)
		{
			min_value = sqrt(pow((rect.x - 208), 2) + pow((rect.y - 208), 2));
			aim = rect;
		}
	}
	if (aim.x == 0 && aim.y == 0)
	{
		//cout << "no aim" << endl;
		return 0;
	}
    //rectangle(img, aim, cv::Scalar(0x27, 0xC1, 0x36), 2);
	float pos_x = ((aim.x + aim.width / 2.0f - 208) / 416);
	float pos_y = ((aim.y + aim.height / 2.0f - 208) / 416);
	float w = aim.width / 416.0f;
	float h = aim.height / 416.0f;
    
    offset_x = pos_x * 416;
    offset_y = pos_y * 416;
    if (isHead == 1)
    {
    offset_y = ((pos_y) * 416);
    }
    else
    {
    offset_y = ((pos_y - w * 0.3) * 416);
    }
    pid_x.target_position = offset_x;
    pid_y.target_position = offset_y;
    //cout << offset_x << " " << pid_x.target_position << endl;
    {
        //std::lock_guard<std::mutex> lock(data_mutex_);
        pid_x.data_ready_ = true;
        pid_y.data_ready_ = true;
    }
    pid_x.data_cond_.notify_one();
    pid_y.data_cond_.notify_one();
    //cout << pid_x.data_ready_ << endl;
}



//void mouse_control::move_mouse()
//{
//    while (1)
//    {
//        std::unique_lock<std::mutex> lock(data_mutex_);
//        data_cond_.wait(lock, [this] { return data_ready_; });
//        //KalmanFilter kf;
//        //double filteredX = kf.update(offset_x);
//        //double filteredY = kf.update(offset_y);
//        //if (abs(filteredX) < 250 && abs(filteredX) < 250 && abs(filteredY) > 0 && abs(filteredY) > 0)
//        //{
//        //    if (is_use_hardware)
//        //    {
//        //        //mouse_event(MOUSEEVENTF_MOVE, filteredX, filteredY, 0, 0);
//        //        Auto_fire.move_by_port(filteredX, filteredY);
//        //        if (is_auto_fire == 1)
//        //        {
//        //            Auto_fire.x = filteredX;
//        //            Auto_fire.y = filteredY;
//        //            /*{
//        //                std::lock_guard<std::mutex> lock(Auto_fire.data_mutex_);
//        //                Auto_fire.data_ready_ = true;
//        //            }*/
//        //            Auto_fire.data_cond_.notify_one();
//        //        }
//        //        else
//        //        {
//        //            Auto_fire.data_ready_ = false;
//        //        }
//        //    }
//        //    else
//        //    {
//        //        //cout << filteredX << "  " << filteredY << endl;
//        //        mouse_event(MOUSEEVENTF_MOVE, filteredX, filteredY, 0, 0);
//        //        if (is_auto_fire == 1)
//        //        {
//        //            Auto_fire.x = filteredX;
//        //            Auto_fire.y = filteredY;
//        //            {
//        //                //std::lock_guard<std::mutex> lock(Auto_fire.data_mutex_);
//        //                Auto_fire.data_ready_ = true;
//        //            }
//        //            Auto_fire.data_cond_.notify_one();
//        //        }
//        //        else
//        //        {
//        //            Auto_fire.data_ready_ = false;
//        //        }
//        //    }
//        //}
//
//
//
//        data_ready_ = false;
//    }
//}
