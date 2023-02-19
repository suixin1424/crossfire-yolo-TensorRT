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
int mouse_control::fire(Mat img, float* Boxes, int* ClassIndexs, int* BboxNum, int isHead)
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
    
    float offset_x = pos_x * 416;
    float offset_y = pos_y * 416;
    if (isHead == 1)
    {
    offset_y = ((pos_y) * 416);
    }
    else
    {
    offset_y = ((pos_y - w * 0.3) * 416);
    }
    KalmanFilter kf;
    double filteredX = kf.update(offset_x);
    double filteredY = kf.update(offset_y);
    if (abs(filteredX) < 250 && abs(filteredX) < 250 && abs(filteredY) > 0 && abs(filteredY) > 0)
    {
        mouse_event(MOUSEEVENTF_MOVE, filteredX, filteredY, 0, 0);
    }
}
