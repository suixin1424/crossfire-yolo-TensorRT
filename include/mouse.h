#define NOMINMAX
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>
#include<serial.h>
#include<auto-fire.h>
#include<pid.h>

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

public:
	auto_fire Auto_fire;
	pid_move pid_x;
	pid_move pid_y;
	//static void receiveWrapper(mouse_control* mouse) {
	//	mouse->move_mouse();
	//}
	int isHead = 0;//Ä¬ÈÏÎªÉí
	int is_use_hardware = 0;
	int is_auto_fire = 0;
	int fire(Mat img, float* Boxes, int* ClassIndexs, int* BboxNum);
	//void move_mouse();
};