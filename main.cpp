#include<config.h>


using cv::Mat;
using std::cout;
using std::endl;



int main(int argc, char** argv) {
    options option;
    //getchar();
    option.init();
    cout << "是否显示窗口" << endl;
    cin >> option.is_show_windows;
    cout << "开始游戏吧" << endl;

    std::thread main_Thread(&options::sendWrapper, &option);
    //std::thread move_Thread(&mouse_control::receiveWrapper, &option.mouse);
    std::thread move_Thread(&pid_move::receiveWrapper2, &option.mouse.pid);
    std::thread smooth_Thread(&pid_move::receiveWrapper3, &option.mouse.mouse_smooth);
    std::thread auto_fire_Thread(&auto_fire::receive1Wrapper, &option.mouse.Auto_fire);

    main_Thread.join();
    move_Thread.join();
    //move_Thread.join();
    auto_fire_Thread.join();
    smooth_Thread.join();
    return 0;
}
