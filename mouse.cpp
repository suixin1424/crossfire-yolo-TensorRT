#include<mouse.h>

int mouse_control::fire(Mat img, ObjectDetector::BoxArray box)
{
	int min_value = 9999;
	cv::Rect aim;
	for (auto& obj : box)
	{
		if (obj.class_label != isHead)
			continue;

		cv::Rect rect(cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom));
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
	pid.target_position_x = offset_x;
	pid.target_position_y = offset_y;
	{
		pid.data_ready_ = true;
	}
	pid.data_cond_.notify_one();
}

