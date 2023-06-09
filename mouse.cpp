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
	std::vector<cv::Rect> aims;
	for (auto& obj : box)
	{
		if (obj.class_label != isHead)
			continue;
		cv::Rect rect(cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom));
		aims.push_back(rect);
	}
	if (aims.empty())
		return 0;
	if (aims.size() == 1)
	{
		aim = aims[0];
	}
	else
	{
		if (is_first_frame)
		{
			aim = aims[0];
			is_first_frame = false;
		}
		else
		{
			float iou_max = 0;
			for (auto& obj : aims)
			{
				int iou_temp = cal_iou(aim, obj, 2);
				if (iou_temp > iou_max)
				{
					iou_max = iou_temp;
					aim = obj;
				}
			}
			if (iou_max == 0)
				is_first_frame = true;
		}
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
	return 0;
}


float mouse_control::cal_iou(cv::Rect rect1, cv::Rect rect2, float scale)
{
	cv::Rect rect_temp1 = enlargeRect(rect1, scale);
	cv::Rect rect_temp2 = enlargeRect(rect2, scale);
	cv::Rect intersection = rect_temp1 & rect_temp2;
	float intersectionArea = intersection.area()- intersectionArea;
	float unionArea = rect_temp1.area() + rect_temp2.area();
	return  intersectionArea / unionArea;
}