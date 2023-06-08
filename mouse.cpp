#include<mouse.h>

cv::Rect enlargeRect(const cv::Rect& rect, float scale)
{
	int width = static_cast<int>(rect.width * scale);
	int height = static_cast<int>(rect.height * scale);
	int x = rect.x - static_cast<int>((width - rect.width) / 2);
	int y = rect.y - static_cast<int>((height - rect.height) / 2);

	return cv::Rect(x, y, width, height);
}

int mouse_control::fire(Mat img, ObjectDetector::BoxArray box)
{
	if (box.empty())
	{
		//cout << 1 << endl;
		lost_frame++;
	}
	if (lost_frame >= 2)
	{
		is_first_frame = true;
		lost_frame = 0;
	}
	if (box.size() <= 2)
	{
		for (auto& obj : box)
		{
			if (obj.class_label != isHead)
				continue;
			cv::Rect rect(cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom));
			aim = rect;
		}
	}
	else
	{
		float iou_max = 0;
		for (auto& obj : box)
		{
			if (obj.class_label != isHead)
				continue;

			cv::Rect rect(cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom));
			//if (rect.area() > max_value)
			//{
			//	max_value = rect.area();
			//	aim = rect;
			//}
			if (rect.contains(cv::Point2f(416 * 0.5, 416 * 0.5)))
			{
				aim = rect;
				//cout << 1 << endl;
			}
			if (is_first_frame)
			{

				aim = rect;
				is_first_frame = false;
			}
			else
			{
				float iou_temp = cal_iou(aim, rect, 1.5);
				if (iou_temp == 0.f)
					continue;
				if (iou_temp > iou_max)
				{
					aim = rect;
					iou_max = iou_temp;
				}
			}
		}
		//cout << iou_max << endl;
		if (iou_max == 0.f)
			lost_frame++;
	}
	cv::rectangle(img, aim, cv::Scalar(0, 255, 255));
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
float mouse_control::cal_iou(cv::Rect rect1, cv::Rect rect2, float scale)
{
	cv::Rect rect_temp1 = enlargeRect(rect1, scale);
	cv::Rect rect_temp2 = enlargeRect(rect2, scale);
	cv::Rect intersection = rect_temp1 & rect_temp2;
	float intersectionArea = intersection.area();
	float unionArea = rect_temp1.area() + rect_temp2.area();
	return  intersectionArea / unionArea;
}