#include<config.h>

void options::init()
{
    dxgi.init();
    mouse.pid.init(0.25, 0.05, 0.1);
}

void options::main_function()
{
    while (1)
    {

        auto start = std::chrono::system_clock::now();
        cv::Mat frame = dxgi.get_img(do_not_show_windows);
        if (frame.empty())
        {
            continue;
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
            std::cout << "Í·" << std::endl;
        }
        else if (KEY_DOWN(VK_DOWN) && mouse.isHead == 1) {
            mouse.isHead = 0;
            std::cout << "Éí" << std::endl;
        }
        if (BboxNum > 0 && KEY_DOWN(VK_MBUTTON))
        {
            mouse.fire(frame, Boxes, ClassIndexs, BboxNum);
        }
        else
        {
            mouse.pid.refresh();
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
            dxgi.release();
            dxgi.init();
        }
        delete[]Boxes;
        delete[]BboxNum;
        delete[]ClassIndexs;

    }

    dxgi.release();
}