#define NOMINMAX
#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include<math.h>

using cv::Mat;
using std::cout;
using std::endl;
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
class mouse_control
{
private:
	
public:
	int fire(Mat img, float* Boxes, int* ClassIndexs, int* BboxNum, int isHead);
};