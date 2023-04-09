#include<auto-fire.h>

int auto_fire::get_num(int start, int end)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(start, end); // Ö¸¶¨·¶Î§
	return distrib(gen);
}


void auto_fire::fire()
{
	while (1)
	{
		std::unique_lock<std::mutex> lock(data_mutex_);
		data_cond_.wait(lock, [this] { return data_ready_; });
		Sleep(get_num(30, 46));
		if (x<get_num(5, 10) && x>-get_num(5, 10) && y<get_num(5, 10) && y>-get_num(5, 10))
		{
			//std::cout << 1 << std::endl;
			left_press(1);
			Sleep(get_num(10,30));
			left_press(0);
		}
		data_ready_ = false;
	}
}

int auto_fire::init_port()
{
	if (!com.openSyn("\\\\.\\COM14", CBR_115200, NOPARITY, 8, ONESTOPBIT))
	{
		std::cout << com.getSerialLastError() << std::endl;
		getchar();
		return 0;
	}
	com.setBufferSize(100, 100);
	return 1;
}

int auto_fire::close_port()
{
	com.closeComm();
	return 0;
}

void auto_fire::move_by_port(int x, int y)
{
	string data = to_string(x) + ":" + to_string(-y) + "x";
	//cout << data << endl;
	com.writeStr(data);
}

void auto_fire::refresh_buff(DWORD flags)
{
	com.flushBuff();
}

void auto_fire::left_press(int flag)
{
	string data = ":" + to_string(flag) + "x";
	//cout << data << endl;
	com.writeStr(data);
}