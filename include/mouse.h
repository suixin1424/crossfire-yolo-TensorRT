#define NOMINMAX
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>
#include<serial.h>

using cv::Mat;
using std::cout;
using std::endl;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
class mouse_control
{
private:
	SerialInterface com;
public:
	int fire(Mat img, float* Boxes, int* ClassIndexs, int* BboxNum, int isHead, int is_use_hardware);
	int init_port();
	int close_port();
	void move_by_port(int x, int y);
};