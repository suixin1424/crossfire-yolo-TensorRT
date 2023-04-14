#include<config.h>

void options::init()
{
    dxgi.init();
    mouse.Auto_fire.init_port();
    mouse.pid_x.init(0.4, 0.2, 0.31, 1);
    mouse.pid_y.init(0.4, 0.2, 0.2, 2);
}

void options::main_function()
{
    while (1)
    {

        auto start = std::chrono::system_clock::now();
        cv::Mat frame = dxgi.get_img(do_not_show_windows);
        if (frame.empty())
        {
            break;
        }
        float* Boxes = new float[4000];
        int* BboxNum = new int[1];
        int* ClassIndexs = new int[1000];
        //run inference
        yolo.Infer(frame.cols, frame.rows, frame.channels(), frame.data, Boxes, ClassIndexs, BboxNum);
        auto end = std::chrono::system_clock::now();
        //cout << "FPS: " << 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;
        if (KEY_DOWN(VK_UP) && mouse.isHead == 0) {
            mouse.isHead = 1;
            std::cout << "头" << std::endl;
        }
        else if (KEY_DOWN(VK_DOWN) && mouse.isHead == 1) {
            mouse.isHead = 0;
            std::cout << "身" << std::endl;
        }
        if (KEY_DOWN(VK_LEFT) && mouse.is_auto_fire == 0) {
            mouse.is_auto_fire = 1;
            std::cout << "自动开火" << std::endl;
        }
        else if (KEY_DOWN(VK_RIGHT) && mouse.is_auto_fire == 1) {
            mouse.is_auto_fire = 0;
            std::cout << "关闭自动开火" << std::endl;
        }
        if (BboxNum > 0 && KEY_DOWN(VK_MBUTTON))
        {
            mouse.fire(frame, Boxes, ClassIndexs, BboxNum);
        }
        if (is_show_windows)
        {
            yolo.draw_objects(frame, Boxes, ClassIndexs, BboxNum, mouse.isHead);
            putText(frame, "fps:" + std::to_string(1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()), Point(10, 50), FONT_HERSHEY_PLAIN, 1.6, Scalar(0, 0, 255), 2);
            imshow("img", frame);
            HWND hWnd = (HWND)cvGetWindowHandle("img");
            HWND hRawWnd = ::GetParent(hWnd);

            if (NULL != hRawWnd)
            {
                BOOL bRet = ::SetWindowPos(hRawWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                assert(bRet);
            }
            waitKey(1);
        }
        if (KEY_DOWN(VK_HOME) && mouse.is_use_hardware == 0)
        {
            mouse.is_use_hardware = 1;
            cout << "使用硬件鼠标" << endl;
        }
        if (KEY_DOWN(VK_END) && mouse.is_use_hardware == 1)
        {
            mouse.is_use_hardware = 0;
            cout << "使用系统函数" << endl;
        }
        if (mouse.is_use_hardware == 1)
        {
            if (n >= 3000)
            {
                //cout << 1 << endl;
                n = 0;
                mouse.Auto_fire.close_port();
                mouse.Auto_fire.init_port();
                //pid.Auto_fire.refresh_buff(PURGE_TXABORT);
            }
            n++;
        }
        delete[]Boxes;
        delete[]BboxNum;
        delete[]ClassIndexs;

    }

    dxgi.release();
    mouse.Auto_fire.close_port();
}