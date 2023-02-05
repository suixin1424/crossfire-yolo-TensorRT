#define NOMINMAX
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>

using cv::Mat;
using std::cout;
using std::endl;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
class pid_control
{
private:
	float last_offset_x = 0;  // 上一次截图时的观测距离 
	float last_offset_y = 0;
	float pid_x_I = 0;        // x方向I的累计值
	float pid_y_I = 0;        // y方向I的累计值
	float kx_p = 0.2;    // X方向上P的参数
	float kx_i = 0.01;        // X方向上I的参数
	float kx_d = 0.1;        //X方向上的D参数
	float ky_p = 0.2;      // Y方向上P的参数
	float ky_i = 0.01;      // Y方向上I的参数
	float ky_d = 0.1;       // Y方向上D的参数
	int first_move = 1;     // 是否为第一次移动
	int max_i = 50;
public:
	int fire(float* Boxes, int* ClassIndexs, int* BboxNum, int isHead);
	void rec();
};