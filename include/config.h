#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include <dxgi-cap.h>
#include <builder/trt_builder.hpp>
#include <infer/trt_infer.hpp>
#include <common/ilogger.hpp>
#include "app_yolo/yolo.hpp"
#include "app_yolo/multi_gpu.hpp"

#include<mouse.h>

#define X_ 1
#define Y_ 2

class options
{
private:
	string model_file = "cf.FP16.trtmodel";
	dxgi_cap dxgi;
	dc_cap dc;
	
	int capture = 1;
public:
	mouse_control mouse;
	void init();
	static void sendWrapper(options* option)
	{
		option->main_function();
	}
	int is_show_windows = 0;
	void main_function();
	void draw_objects(Mat img, ObjectDetector::BoxArray box, int is_head);
};

