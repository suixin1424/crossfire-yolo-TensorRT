#include<iostream>
#include<windows.h>
#include<opencv2/opencv.hpp>
#include <random>
#include<serial.h>

class auto_fire
{
private:
	int get_num(int start, int end);
	SerialInterface com;
public:
	std::condition_variable data_cond_;
	std::mutex data_mutex_;
	bool data_ready_ = false;
	int x, y;
	void fire();
	int init_port();
	static void receive1Wrapper(auto_fire* Auto_fire) {
		Auto_fire->fire();
	}

	int close_port();
	void move_by_port(int x, int y);
	void refresh_buff(DWORD flags);
	void left_press(int flag);
};
