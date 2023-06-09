#define NOMINMAX
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>
#include<pid.h>
#include <BYTETracker.h>

using cv::Mat;
using std::cout;
using std::endl;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)



class mouse_control
{
private:

	float offset_x = 0;
	float offset_y = 0;
	std::mutex data_mutex_;
	std::condition_variable data_cond_;
	bool data_ready_ = false;
	float pi = 3.1416;
public:
	float cal_iou(cv::Rect rect1, cv::Rect rect2, float scale);
	bool is_first_frame = true;
	cv::Rect aim;
	pid_move pid;
	int isHead = 0;//Ä¬ÈÏÎªÉí
	int is_use_hardware = 0;
	int is_auto_fire = 0;
	int fire(Mat img, ObjectDetector::BoxArray box);
};
