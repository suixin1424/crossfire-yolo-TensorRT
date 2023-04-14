#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include <dxgi-cap.h>
#include<yolo-trt.h>
#include<mouse.h>

class options
{
private:
	char* model_path = "D:/2/cf/cpp/end2end/build/Release/cf.trt";
	dxgi_cap dxgi;

	int n = 0;
	Yolo yolo = Yolo(model_path);

public:
	mouse_control mouse;
	void init();
	static void sendWrapper(options* option) 
	{
		option->main_function();
	}
	int is_show_windows = 0;
	void main_function();
};
