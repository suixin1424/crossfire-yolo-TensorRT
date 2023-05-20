/*不通用所有分辨率
恰好我們愛好一樣
桌面分辨率為1920
然後是窗口游戲*/
#pragma once
#include<windows.h>
#include<math.h>
float pi = 3.1416;
float fov_x(float delta_x) {
	float FOV = 106.260205;
	int width = 1920;
	int pixel_x = 4800;   //實戰夠用且不抖動
	float per_pixel_rad = pixel_x / (2 * pi);
	float delta_abs_x = abs((int)delta_x);
	float sup_distance = (width / 2) / tan((FOV * pi / 180) / 2);
	float target_angle_rad = atan(delta_abs_x / sup_distance);
	float target_move = target_angle_rad * per_pixel_rad;
	float flag = delta_x;
	if (flag < 0) {
		return (-1) * target_move;
	}
	else {
		return target_move;
	}
}
//不讓Y軸參與算法
//親測適用我所玩的游戲
//實例：    代入值 = fov_x(X相對移動值);





